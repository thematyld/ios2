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

Params param;
sem_t *sem1=NULL;
sem_t *sem2=NULL;

//---------------Prototype--functions--------//
void errorArgs();
void initArgs(int,char**,int);


//----------Main-----------------------//
int main(int argc,char** args) {
    setbuf(stdout,NULL);
    initArgs(argc,args,COUNT_PARAM);

    if((sem1=sem_open(SEM1_NAME, O_CREAT | O_EXCL,0666,LOCKED))==SEM_FAILED){

    }

    if((sem2=sem_open(SEM2_NAME, O_CREAT | O_EXCL,0666,LOCKED))==SEM_FAILED){

    }

    pid_t processPid=fork();

    if(processPid==0) {
        for(int i=0;i<5;i++) {
            sem_wait(sem1);
            printf("CHILD %d\n",i);
            sem_post(sem2);
        }
        //sem_wait(sem1);
    }
    else if(processPid>0) {
        for(int i=0;i<5;i++){
            sem_post(sem1);
            printf("Parents %d\n",i);
            sem_wait(sem2);
        }
    }

    printf("End child \n");
    sem_post(sem1);

    waitpid(processPid,NULL,0);
    printf("End paren \n");

    sem_close(sem1);
    sem_close(sem2);

    sem_unlink(SEM1_NAME);
    sem_unlink(SEM2_NAME);
    return 0;
}

void errorArgs(){
    fprintf(stderr,"Syntax Error!!\n");
    exit(1);
}

void initArgs(int argc,char** args,int count){
    if(argc == count) {
        for(int i=1;i<count;i++){
            int tmp=(int)strtol(args[i],NULL,10);
            if(tmp<0)
                errorArgs();
            switch(i){
                case 1:
                    if(tmp>0)
                        param.R=(unsigned int)tmp;
                    else
                        errorArgs();
                    break;
                case 2:
                    if(tmp>0)
                        param.C=(unsigned int)tmp;
                    else
                        errorArgs();
                    break;
                case 3:
                    if(tmp>=0 && tmp<=1000)
                        param.ART=(unsigned int)tmp;
                    else
                        errorArgs();
                    break;
                case 4:
                    if(tmp>=0 && tmp<=1000)
                        param.ABT=(unsigned int)tmp;
                    else
                        errorArgs();
                    break;
            }
        }
    }
    else
        errorArgs();
}