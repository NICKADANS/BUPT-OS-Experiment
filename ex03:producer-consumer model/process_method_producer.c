/* I get this code from my friend in SYSU. */
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

int insert_item(void* param) {
	pthread_mutex_lock(&lock);

	if(shared -> data_num == 20) {
		pthread_mutex_unlock(&lock);
		return -1;
	}
	else {
		int item = rand() % 5; // random create data
		printf("[Producer] Thread id : %lu, data : %d\n", pthread_self(), item);
		shared -> data[shared -> data_num - 1] = item;
		shared -> data_num ++;
		pthread_mutex_unlock(&lock);
		return 0;
	}
}

void *producer(void *param) {
	insert_item(param);
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

	shared -> written = 1;
	shared -> read = 0;

	int permit;
	while(1) {
		permit = shared -> written;
		if(permit) {
			shared -> read = 0;

			pthread_t producer_1;
			pthread_t producer_2;
			pthread_t producer_3;

			pthread_create(&producer_1, NULL, producer, NULL);
			pthread_create(&producer_2, NULL, producer, NULL);
			pthread_create(&producer_3, NULL, producer, NULL);

			pthread_join(producer_1, NULL);
			pthread_join(producer_2, NULL);
			pthread_join(producer_3, NULL);
			
			shared -> read = 1;
		}
		sleep(num);
	}
	return 0;
}
