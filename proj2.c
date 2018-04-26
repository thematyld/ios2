#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COUNT_PARAM 5

typedef enum{false=0,true=1} boolean;

typedef struct{
    unsigned int R;
    unsigned int C;
    unsigned int ART;
    unsigned int ABT;
} Params;
Params param;

void errorArgs();
void initArgs(int,char**,int);

int main(int argc,char** args) {
    initArgs(argc,args,COUNT_PARAM);

    return 0;
}

void errorArgs(){
    fprintf(stderr,"Syntax Error!!\n");
    exit(1);
}

void initArgs(int argc,char** args,int count){
    if(argc == count) {
        for(int i=1;i<count;i++){
            unsigned int tmp=(unsigned int)strtol(args[i],NULL,10);
            if(tmp<=0)
                errorArgs();
            switch(i){
                case 1:
                    tmp>0 ? param.R=tmp : errorArgs();
                    break;
                case 2:
                    tmp>0 ? param.C=tmp : errorArgs();
                    break;
                case 3:
                    tmp>0 && tmp<=1000 ? param.ART=tmp : errorArgs();
                    break;
                case 4:
                    tmp>0 && tmp<=1000 ? param.ABT=tmp : errorArgs();
                    break;
            }
        }
    }
    else
        errorArgs();
}