#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>          
#include <time.h>

#define BUFFER_SIZE 10
typedef int buffer_item;

/* the buffer */
buffer_item buffer[BUFFER_SIZE];
int front;
int rear;

//declear semphores and mutex
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

/* insert item into buffer */
int insert_item(buffer_item item){
	//error
	if(buffer == NULL)
		return -1;
	//buffer is full
	else if(front == (rear + 1) % BUFFER_SIZE)
		return 1;	
	//successful
	else{			
		rear = (rear + 1) % BUFFER_SIZE;
		buffer[rear] = item;
	}
	return 0;
}

/* remove an item object from buffer */
int remove_item(buffer_item *item){
	//error
	if(buffer == NULL)
		return -1;
	//empty
	else if(front == rear)
		return 1;
	//successful
	else{
		front = (front + 1) % BUFFER_SIZE;
		*item = buffer[front];
	}
	return 0;
}


void* producer(){
	buffer_item item;
	while(1){
		/* sleep for a random period of time */
		int time = rand() % 10 + 3;        
		usleep(time * 100000);

		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		
		/* generate a random number */
		item = rand();
		//insert item
		int add = insert_item(item);
		if(add != 0){
			printf("report error condition: %d\n",add);
		}
		else{
			printf("producer produced %d\n", item);
		}	
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consumer(){
	buffer_item item;
	while(1){
		/* sleep for a random period of time */
		int time = rand() % 10 + 3;
		usleep(time * 100000);

		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		
		//remove item
		int sub = remove_item(&item);
		if(sub != 0){
			printf("report error condition: %d\n", sub);
		}
		else{
			printf("consumer consumed %d\n", item);
		}
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}

}


int main(int argc, char *argv[]) {
	srand((unsigned int)time(NULL));

	/* 1. Get command line arguments argv[1], argv[2], argv[3] */
	int sleeptime  = atoi(argv[1]);    //main process's sleep time
	int producers_num = atoi(argv[2]); //producer threads' num
	int consumers_num = atoi(argv[3]); //consumer threads' num	

	/* 2. Initialize buffer */
	rear = 0;
	front = 0;

	//initialize mutex and semphores
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty,0,BUFFER_SIZE);
	sem_init(&full,0,0);

	/* 3. Create producer thread(s) */
	pthread_t t_producer[producers_num];
	for(int i = 0; i < producers_num; i++){
		pthread_create(&t_producer[i], NULL, producer, NULL);
	}

	/* 4. Create consumer thread(s) */
	pthread_t t_consumer[consumers_num];
	for(int i = 0; i < consumers_num; i++){
		pthread_create(&t_consumer[i], NULL, consumer, NULL);
	}

	/* 5. Sleep */
	//pthread_join(producer, NULL);
	sleep(sleeptime);

	/* 6. Exit */
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_mutex_destroy(&mutex);
	return 0;
}
