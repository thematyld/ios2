#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>


#define COUNT_PARAM 5
#define SHM_NAME "/xbarto96_shm_name"
#define SEM1_NAME "/xbarto96_sem1"
#define SEM2_NAME "/xbarto96_sem2"


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
sem_t *bus=NULL;
sem_t *tmpRider=NULL;
sem_t **rider;
int sharedId=0;
int *counter=NULL;


//---------------Prototype--functions--------//
void error(char* text);
void initArgs(int argc,char** args,int count);


//----------Main-----------------------//
int main(int argc,char** args) {
    setbuf(stdout,NULL);
    initArgs(argc,args,COUNT_PARAM);
    if((bus=sem_open(SEM1_NAME,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED){

    }
    if((tmpRider=sem_open(SEM2_NAME,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED){

    }
    sharedId=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    counter=shmat(sharedId, NULL, 0);

    pid_t* riders= malloc(Tparam.R* sizeof(pid_t));
    pid_t processPid;
    if(riders==NULL)
        error("Error - Create array of pid_t riders!!");

    for(unsigned int i=1;i<=Tparam.R;i++){
        processPid=fork();

        if(processPid==0){

        } else if(processPid>0){
            riders[i]=processPid;
        } else{
            error("Error - Create riders!!");
        }

    }

    for(int i=1;i<=Tparam.R;i++){
        printf("%d\n",riders[i]);
    }


    waitpid(processPid,NULL,0);
    printf("End paren \n");

    sem_close(bus);
    sem_close(tmpRider);

    sem_unlink(SEM1_NAME);
    sem_unlink(SEM2_NAME);

    return 0;
}

void createRider(){

}

void createBus(){

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