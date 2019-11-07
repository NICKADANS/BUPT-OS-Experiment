#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
    pid_t fpid = fork();
    int num = atoi(argv[1]);
    if(fpid < 0 || num == 0){
        printf("Error!\n");
        return 0;   
    }
    if(fpid == 0){ //子进程
        printf("I'm child!\n");
        while(num != 1){
            printf("%d ", num);
            if(num % 2 == 0){
                num/=2;
            }
            else{
                num = 3 * num + 1;
            }
        }
        printf("%d\n", num);
        printf("child is over!");
    }
    else{ //父进程
        printf("Father is waiting!\n");
        wait(NULL);
        printf("Bye!\n");
    }
    return 0;
}
