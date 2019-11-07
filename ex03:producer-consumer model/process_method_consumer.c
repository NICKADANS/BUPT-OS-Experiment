/* I get this code from my friend in SYSU */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>

#define DATA_SIZE 20

struct shared_memory
{
	int written; // 0 for false, 1 for true
	int read;
	int data[DATA_SIZE];
	int data_num;
};
struct shared_memory *shared;

pthread_mutex_t lock;
pthread_cond_t wait;

int remove_item(void* param) {
	pthread_mutex_lock(&lock);

	if(shared -> data_num == 0) {
		pthread_mutex_unlock(&lock);
		return -1;
	}
	else {
		int item =shared -> data[shared -> data_num - 1];
		printf("[Consumer] Thread id : %lu, data : %d\n", pthread_self(), item);
		shared -> data[shared -> data_num - 1] = 0;
		shared -> data_num --;
		pthread_mutex_unlock(&lock);
		return 0;
	}
}

void *consumer(void *param) {
	remove_item(param);
	pthread_exit(0);
}

int main(int argc, char *argv[]) {
	int* timestep = (int*) argv[1];
	int num;
	scanf("%d",&num);
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&wait, NULL);

	void *shm = NULL;
	int shmid;

	shmid = shmget((key_t)1234, sizeof(struct shared_memory), 0666|IPC_CREAT);

	shm = shmat(shmid, 0, 0);

	shared = (struct shared_memory*) shm;

	//shared -> written = 1;
	//shared -> read = 0;

	int permit;
	while(1) {
		permit = shared -> read;
		if(permit) {
			shared -> written = 0;

			pthread_t consumer_1;
			pthread_t consumer_2;
			pthread_t consumer_3;

			pthread_create(&consumer_1, NULL, consumer, NULL);
			pthread_create(&consumer_2, NULL, consumer, NULL);
			pthread_create(&consumer_3, NULL, consumer, NULL);

			pthread_join(consumer_1, NULL);
			pthread_join(consumer_2, NULL);
			pthread_join(consumer_3, NULL);
			
			shared -> written = 1;
		}
		sleep(num);
	}
	return 0;
}

