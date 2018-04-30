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

#define DEPART_NAME "/xbarto96_depart"
#define ARR_BUS_NAME "/xbarto96_arr_bus"
#define WRITER_NAME "/xbarto96_writer"
#define WRITER_NAME "/xbarto96_writer"
#define FULL_BUS_NAME "/xbarto96_writer"
#define END_RID_NAME "/xbarto96_end_rid"
#define COUNTER_NAME "/xbarto96_counter"
#define COMPLETED_NAME "/xbarto96_completed"


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

sem_t   *S_depart=NULL,
        *S_arrBus=NULL,
        *S_writer=NULL,
        *S_fullBus=NULL,
        *S_endRid=NULL,
        *S_counter=NULL,
        *S_completed=NULL,
        **Srider=NULL;

int busThereID=0;
int capStationID=0;
int ridIDid=0;
int boardedID=0;
int stillRidID=0;

int *busThere=NULL;
int *capStation=NULL;
int *ridID=NULL;
int *boarded=NULL;
int *stillRid=NULL;

FILE *file;


//---------------Prototype--functions--------//
void error(char* text);
void initArgs(int argc,char** args,int count);
void createRider(Params param,int i);
void createBus(Params param);

void initSem(){         //TODO

    S_depart=sem_open(DEPART_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    S_arrBus=sem_open(ARR_BUS_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    S_writer=sem_open(WRITER_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    S_fullBus=sem_open(FULL_BUS_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    S_endRid=sem_open(END_RID_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    S_counter=sem_open(COUNTER_NAME,O_CREAT|O_EXCL,0666,LOCKED);
    S_completed=sem_open(COMPLETED_NAME,O_CREAT|O_EXCL,0666,LOCKED);

}

void initSHM(){
    busThereID=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    capStationID=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    ridIDid=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    boardedID=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    stillRidID=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

    busThere=shmat(busThereID, NULL, 0);
    capStation=shmat(capStationID, NULL, 0);
    ridID=shmat(ridIDid, NULL, 0);
    boarded=shmat(boardedID, NULL, 0);
    stillRid=shmat(stillRidID, NULL, 0);

    *busThere=0;
    *capStation=Tparam.C;
    *ridID=1;
    *boarded=0;
    *stillRid=Tparam.R;
}

//----------Main-----------------------//
int main(int argc,char** args) {


    if((file = fopen(FILE_NAME,"w"))==NULL) {
        error("Error - Opening file");
    }
    setbuf(file,NULL);

    //-----------Initializations---------
    initArgs(argc,args,COUNT_PARAM);
    initSem();
    initSHM();


    pid_t* SRiders= malloc(Tparam.R* sizeof(pid_t));

    if(SRiders==NULL)
        error("Error - Create array of pid_t riders!!");

    pid_t mainPid=fork();
    if(mainPid==0) {
        for (unsigned int i = 1; i <= Tparam.R; i++) {
            pid_t ridersPid = fork();
            if (ridersPid == 0) {
                createRider(Tparam, i);
                exit(0);                //TODO
            } else if (ridersPid > 0) {
                SRiders[i] = ridersPid;
            } else {
                error("Error - Create riders!!");
            }
        }
        for(int i=1;i<=Tparam.R;i++){
            if(SRiders[i]!=0)
                waitpid(SRiders[i],NULL,0);
        }
    } else if(mainPid>0){
        pid_t busPid=fork();
        if(busPid==0){
            createBus(Tparam);
            exit(0);            //TODO
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

    sem_destroy(Sbus);
    sem_destroy(StmpRider);
    sem_destroy(Swriter);
    sem_unlink(BUS_NAME);
    sem_unlink(TMP_RIDER_NAME);
    sem_unlink(WRITER_NAME);

    return 0;
}

void freeSources(){
    if(sem_destroy(S_depart)    == -1  ||
       sem_destroy(S_arrBus)    == -1  ||
       sem_destroy(S_completed) == -1  ||
       sem_destroy(S_counter)   == -1  ||
       sem_destroy(S_endRid)    == -1  ||
       sem_destroy(S_fullBus)   == -1  ||
       sem_destroy(S_writer)    == -1) {
        fprintf(stderr,"Error - Removing semaphores!!");
    }

    if(shmctl(shm_H_id, IPC_RMID, NULL) == -1 ||
       shmctl(shm_O_id, IPC_RMID, NULL) == -1 ||
       shmctl(shm_p_id, IPC_RMID, NULL) == -1 ||
       shmctl(shm_c_id, IPC_RMID, NULL) == -1 ||
       shmctl(shm_d_id, IPC_RMID, NULL) == -1 ||
       shmdt(count) == -1 ||
       shmdt(n_done) == -1 ||
       shmdt(process_id) == -1 ||
       shmdt(H_id) == -1 ||
       shmdt(O_id) == -1 ){
        err = EXIT_SHM;
    }

}

void createRider(Params param,int i){
    int counter=i;

    srandom((unsigned int) time(NULL));
    int delay= (int) (random() % (param.ART + 1));
    usleep((__useconds_t) (delay * 1000));
    sem_wait(Swriter);
    fprintf(file,"%d\t: RID %d\t: start ",*shm_countID,counter);  //TODO
    (*shm_countID)++;
    sem_post(Swriter);
}

void createBus(Params param){
    srandom((unsigned int) time(NULL));
    long delay= (random() % (param.ABT + 1));

    usleep((__useconds_t) (delay * 1000));

    sem_wait(Swriter);
    fprintf(file,"%d\t: BUS \t: start ",*shm_countID);
    (*shm_countID)++; //TODO
    sem_post(Swriter);

    while(param.R!=0){  //TODO



    }
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
                default:
                    break;
            }
        }
    }
    else
        error("Bad arguments!!");
}