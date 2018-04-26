#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>


#define COUNT_PARAM 5
#define SHM_NAME "/xbarto96_shm_name"
#define BUS_NAME "/xbarto96_bus"
#define TMP_RIDER_NAME "/xbarto96_tmpRider"
#define WRITER_NAME "/xbarto96_writer"
#define FILE_NAME "proj2.out"

#define LOCKED 0


//--------------TypeDef-----------------//
typedef enum{false=0,true=1} boolean;
typedef struct{
    unsigned int R;
    unsigned int C;
    unsigned int ART;
    unsigned int ABT;
} Params;

//---------------Global--Variables----------//

Params Tparam;

sem_t   *Sbus=NULL,
        *StmpRider=NULL,
        *Swriter=NULL,
        **Srider;
int sharedId=0;
int *counter=NULL;
FILE *file;


//---------------Prototype--functions--------//
void error(char* text);
void initArgs(int argc,char** args,int count);
void createRider(Params param,int i);
void createBus(Params param);
//----------Main-----------------------//
int main(int argc,char** args) {
    setbuf(stdout,NULL);

    if((file = fopen(FILE_NAME,"w"))==NULL)
        error("Error - Opening file");

    initArgs(argc,args,COUNT_PARAM);
    Sbus=sem_open(BUS_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    StmpRider=sem_open(TMP_RIDER_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    Swriter=sem_open(WRITER_NAME,O_CREAT|O_EXCL,0666,LOCKED);



    sharedId=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    counter=shmat(sharedId, NULL, 0);

    pid_t* SRiders= malloc(Tparam.R* sizeof(pid_t));


    if(SRiders==NULL)
        error("Error - Create array of pid_t riders!!");

    pid_t mainPid=fork();
    if(mainPid==0) {
        for (unsigned int i = 1; i <= Tparam.R; i++) {
            pid_t ridersPid = fork();
            if (ridersPid == 0) {
                createRider(Tparam, i);
            } else if (ridersPid > 0) {
                SRiders[i] = ridersPid;
            } else {
                error("Error - Create riders!!");
            }
        }
        for(int i=1;i<=Tparam.R;i++){
            waitpid(SRiders[i],NULL,0);
        }
    } else if(mainPid>0){
        pid_t busPid=fork();
        if(busPid==0){
            createBus(Tparam);
        }else if(busPid>0){
            waitpid(busPid,NULL,0);
        } else{
            error("Error - Create bus!!");
        }
    } else{
        error("Error - Create subs");
    }


    sem_close(Sbus);
    sem_close(StmpRider);
    sem_close(Swriter);
    sem_unlink(BUS_NAME);
    sem_unlink(TMP_RIDER_NAME);
    sem_unlink(WRITER_NAME);

    return 0;
}

void createRider(Params param,int i){
    int counter=i;
    srandom((unsigned int) time(NULL));
    int delay= (int) (random() % (param.ART + 1));
    sem_wait(Swriter);
    fprintf(file,"%d\t: RID %d\t: start ",NULL,counter);  //TODO
    sem_post(Swriter);
}

void createBus(Params param){
    srandom((unsigned int) time(NULL));
    int delay= (int) (random() % (param.ART + 1));
    sem_wait(Swriter);
    fprintf(file,"%d\t: BUS \t: start ",counter);
    sem_post(Swriter);
}


void error(char* text){
    fprintf(stderr,"%s\n",text);
    exit(1);
}

void initArgs(int argc,char** args,int count){
    if(argc == count) {
        for(int i=1;i<count;i++){
            int tmp=(int)strtol(args[i],NULL,10);
            if(tmp<0)
                error("Bad arguments!!");
            switch(i){
                case 1:
                    if(tmp>0)
                        Tparam.R=(unsigned int)tmp;
                    else
                        error("Bad arguments!!");
                    break;
                case 2:
                    if(tmp>0)
                        Tparam.C=(unsigned int)tmp;
                    else
                        error("Bad arguments!!");
                    break;
                case 3:
                    if(tmp>=0 && tmp<=1000)
                        Tparam.ART=(unsigned int)tmp;
                    else
                        error("Bad arguments!!");
                    break;
                case 4:
                    if(tmp>=0 && tmp<=1000)
                        Tparam.ABT=(unsigned int)tmp;
                    else
                        error("Bad arguments!!");
                    break;
            }
        }
    }
    else
        error("Bad arguments!!");}