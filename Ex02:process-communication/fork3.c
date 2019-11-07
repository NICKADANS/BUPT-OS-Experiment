/* use pipe to communicate with child process */
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int main(){
	pid_t fpid;
	int pipefd[2];
	char c_buf[50];
	char input[50] = "\0";
	pipe(pipefd);
	fpid = fork();
	if(fpid < 0){
		printf("ERROR,PROGRAM ABORT\n");
		return 0;
	}
	else if(fpid > 0){
		fgets(input,50,stdin);
		write(pipefd[1],input,50);
		printf("Process1 write:\n\t%s\n",input);
		wait(NULL);
		read(pipefd[0],c_buf,50);
		printf("Read from process2 :\n\t%s\n",c_buf);
		printf("Bye!\n");
	}
	else if(fpid == 0){
		read(pipefd[0],c_buf,50);
		for(int i = 0; i<50; i++){
			if(c_buf[i] <= 'Z' && c_buf[i] >= 'A')
				c_buf[i] = tolower(c_buf[i]);
			else if(c_buf[i] <= 'z' && c_buf[i] >= 'a')
				c_buf[i] = toupper(c_buf[i]);
		}
		write(pipefd[1],c_buf,50);
	}
	return 0;
}
