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
#include <string.h>
#include <ctype.h>

#define COUNT_PARAM 5

#define DEPART_NAME "/xbarto96_depart"
#define ARR_BUS_NAME "/xbarto96_arr_bus"
#define WRITER_NAME "/xbarto96_writer"
#define FULL_BUS_NAME "/xbarto96_writer"
#define END_RID_NAME "/xbarto96_end_rid"
#define COUNTER_NAME "/xbarto96_counter"
#define COMPLETED_NAME "/xbarto96_completed"


#define FILE_NAME "proj2.out"
#define LOCKED 1


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
        *S_completed=NULL;

int busThereID=0,
        capStationID=0,
        ridIDid=0,
        boardedID=0,
        stillRidID=0,
        lineIDid=0,
        counterRidID=0;

int *busThere=NULL,
        *capStation=NULL,
        *ridID=NULL,
        *boarded=NULL,
        *stillRid=NULL,
        *lineID=NULL,
        *counterRid=NULL;

pid_t mainPid;

FILE *file;


//---------------Prototype--functions--------//
void error(char* text);
void initArgs(int argc,char** args,int count);
void createRider(Params param);
void createBus(Params param);
void freeSources();
void initSem();
void initSHM();
void killAll();
void incLineID();
int isNumber(char* string);

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
    lineIDid=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    counterRidID=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

    busThere=shmat(busThereID, NULL, 0);
    capStation=shmat(capStationID, NULL, 0);
    ridID=shmat(ridIDid, NULL, 0);
    boarded=shmat(boardedID, NULL, 0);
    stillRid=shmat(stillRidID, NULL, 0);
    lineID=shmat(lineIDid, NULL, 0);
    counterRid=shmat(counterRidID, NULL, 0);

    *busThere=0;
    *capStation=0;
    *ridID=1;
    *boarded=0;
    *stillRid=Tparam.R;
    *lineID=0;
    *counterRid=0;
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

    mainPid=fork();

    if(mainPid==0) {
        for (unsigned int i = 1; i <= Tparam.R; i++) {
            pid_t ridersPid = fork();
            if (ridersPid == 0) {
                srandom((unsigned int) time(NULL));
                long delay= (random() % (Tparam.ART + 1));
                usleep((__useconds_t) (delay * 1000));
                createRider(Tparam);
                freeSources();
                exit(0);
            } else if (ridersPid > 0) {
                SRiders[i] = ridersPid;
            } else {
                killAll();
                error("Error - Create riders!!");
            }
        }
        for(unsigned int i=1;i<=Tparam.R;i++){
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
            killAll();
            error("Error - Create bus!!");
        }
    } else{
        killAll();
        error("Error - Create subs");
    }

    freeSources();
    return 0;
}

void freeSources(){
    if(sem_destroy(S_depart)    == -1  ||
       sem_destroy(S_arrBus)    == -1  ||
       sem_destroy(S_completed) == -1  ||
       sem_destroy(S_counter)   == -1  ||
       sem_destroy(S_endRid)    == -1  ||
       sem_destroy(S_fullBus)   == -1  ||
       sem_destroy(S_writer)    == -1
            ) {
        error("Error - Removing semaphores!!");
    }

    if(shmctl(busThereID, IPC_RMID, NULL)   == -1 ||
       shmctl(capStationID, IPC_RMID, NULL) == -1 ||
       shmctl(ridIDid, IPC_RMID, NULL)      == -1 ||
       shmctl(boardedID, IPC_RMID, NULL)    == -1 ||
       shmctl(stillRidID, IPC_RMID, NULL)   == -1 ||
       shmdt(busThere)      == -1 ||
       shmdt(capStation)    == -1 ||
       shmdt(ridID)         == -1 ||
       shmdt(boarded)       == -1 ||
       shmdt(stillRid)      == -1
            ) {
        error("Error - Removing shared memory!!");
    }

    sem_unlink(DEPART_NAME);
    sem_unlink(ARR_BUS_NAME);
    sem_unlink(WRITER_NAME);
    sem_unlink(FULL_BUS_NAME);
    sem_unlink(END_RID_NAME);
    sem_unlink(COUNTER_NAME);
    sem_unlink(COMPLETED_NAME);

}
void incLineID(){
    sem_wait(S_counter);
    (*lineID)++;
    sem_post(S_counter);
}

int isNumber(char* string){
    char* tmp=string;
    size_t len=strlen(tmp);
    for(int i=0;i<len;i++){
        if(!isdigit(tmp[i]))
            return(-1);
    }
    return 0;
}

void createRider(Params param) {
    boolean isTransp = false;

    sem_wait(S_counter);
    (*counterRid)++;
    sem_post(S_counter);

    incLineID();

    sem_wait(S_writer);
    fprintf(file, "%d\t: RID %d\t: start \n", *lineID, *counterRid);
    sem_post(S_writer);

    if ((unsigned int)(*capStation) < param.C && (*busThere)!=1) {

        sem_wait(S_counter);
        (*capStation)++;
        sem_post(S_counter);
        incLineID();
        sem_wait(S_writer);
        fprintf(file, "%d\t: RID %d\t: enter: %d\n",
                *lineID,
                *counterRid,
                *capStation);
        sem_post(S_writer);
    } else {
        while (!isTransp) {
            sem_wait(S_depart);
            if ((unsigned int)*capStation < param.C) {
                isTransp = true;
                incLineID();
                sem_wait(S_counter);
                (*capStation)++;
                sem_post(S_counter);
                sem_wait(S_writer);
                fprintf(file, "%d\t: RID %d\t: enters: %d\n",
                        *lineID,
                        *counterRid,
                        *capStation);
                sem_post(S_writer);
            }
        }
    }
    sem_wait(S_arrBus);
    incLineID();

    sem_wait(S_writer);
    fprintf(file, "%d\t: RID %d\t: boarding\n",
            *lineID,
            *counterRid);
    sem_post(S_writer);

    sem_wait(S_counter);
    (*capStation)--;
    (*boarded)++;
    sem_post(S_counter);

    if(*capStation==0){
        sem_post(S_fullBus);
    }

    sem_wait(S_endRid);
    incLineID();
    sem_wait(S_writer);
    fprintf(file,"%d\t: RID %d\t: finish\n",
            *lineID,
            *counterRid);
    sem_post(S_writer);

}

void createBus(Params param){
    srandom((unsigned int) time(NULL));


    sem_wait(S_counter);
    (*busThere)=1;
    sem_post(S_counter);

    incLineID();
    sem_wait(S_writer);
    fprintf(file,"%d\t: BUS \t: start \n",*lineID);
    sem_post(S_writer);

    while((*stillRid) != 0){
        long delay= (random() % (param.ABT + 1));
        sem_wait(S_counter);
        (*busThere)=1;
        sem_post(S_counter);

        incLineID();
        sem_wait(S_writer);
        fprintf(file,"%d\t: BUS \t: arrival \n",*lineID);
        sem_post(S_writer);

        if((*capStation)>0) {
            incLineID();
            sem_wait(S_writer);
            fprintf(file, "%d\t: BUS \t: start boarding: %d\n",
                    *lineID,
                    *capStation);
            sem_post(S_writer);

            sem_post(S_depart);

            sem_post(S_arrBus);

            incLineID();

            sem_wait(S_fullBus);


            sem_wait(S_counter);
            (*stillRid)-=(*boarded);
            (*boarded)=0;
            sem_post(S_counter);

            sem_post(S_endRid);

        }
        incLineID();
        sem_wait(S_writer);
        fprintf(file, "%d\t: BUS \t: depart\n",
                *lineID);
        sem_post(S_writer);

        sem_wait(S_counter);
        (*busThere = 0);
        sem_post(S_counter);


        usleep((__useconds_t) (delay * 1000));

    }
    sem_wait(S_writer);
    incLineID();
    fprintf(file,"%d\t: BUS \t: finish\n",*lineID);
    sem_post(S_writer);

}
void killAll(){
    freeSources();
    kill(mainPid,SIGTERM);
    kill(getpid(),SIGTERM);
}

void error(char* text){
    fprintf(stderr,"%s\n",text);
    exit(1);
}

void initArgs(int argc,char** args,int count){
    if(argc == count) {
        for(int i=1;i<count;i++){
            if(isNumber(args[i])==-1)
                error("Wrong arguments!!");
            int tmp=(int)strtol(args[i],NULL,10);
            if(tmp<0)
                error("Wrong arguments!!");
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