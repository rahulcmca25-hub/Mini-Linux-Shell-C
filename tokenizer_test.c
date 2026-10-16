#include<stdio.h>
#include<string.h>

int main(){

    char input[]="ls -l -a";
    char *token;

    char *args[100];

    int i=0;

    token=strtok(input," ");
    
    while(token!=NULL){
        args[i++]=token;
        token=strtok(NULL," ");
    }

    args[i]=NULL;

    for(int j=-1;args[j]!=NULL;j++){
        printf("args[%d] = %s\n", j+1, args[j+1]);
    }

    return 0;
}
