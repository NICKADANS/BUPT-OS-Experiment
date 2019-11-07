#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

int main(int argc, char *argv[]) {
	int num = atoi(argv[1]);
	pid_t fpid;	
	int fd = shm_open("share", O_EXCL | O_CREAT | O_TRUNC | O_RDWR, 0666);
	int size = 1024;
	ftruncate(fd,size);
	fpid = fork();
	void *ptr = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(fpid < 0){
		printf("ERROR!\n");
		return 0;
	}
	else if(fpid == 0){
		char ans[1024];
		int index = 0;
		int N = num;
		while(N != 1){
			char temp[16];
			sprintf(temp, "%d", N);
			for(int i = 0; i<strlen(temp); i++){
				ans[index] = temp[i];
				index++;	
			}
			ans[index] = ' ';
			index++;
			if(N % 2 == 0){
				N /= 2;
			}
			else{
				N = 3 * N + 1;
			}
		}
		ans[index] = '1';
		index++;
		ans[index] = '\0';
		printf("child print ans :\n\t%s\n", ans);
		strcpy((char*)ptr,ans);
	}
	else{
		wait(NULL);
		printf("come back\n");
		char res[2048];
		strcpy(res,(char*)ptr);
		printf("father print result:\n\t%s\n", res);
		printf("Bye!\n");
		shm_unlink("share");
		close(fd);
	}
	return 0;
}
