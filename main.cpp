#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>

struct Item {
    int ID;
    int SleepTime;
};

std::vector<Item*> buffer;
sem_t producerSem;
sem_t consumerSem;
std::mutex mtx;

int num_prod;
int num_cons;
int buf_size;
int num_items;
int bufferCount;
int leftOver;

void *producer(void* args){
    int threadId = *((int*)(&args));
    int itemsPerThread = num_items/num_prod;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread)-1;

    printf("Producer threadID: %i start: %i end: %i \n", threadId,start,end);
    
    srand (time(NULL));
    unsigned int sleep = (rand() % 400) + 300;
    usleep(sleep);

    for (int i =start; i < end; start++){
        Item *temp = (Item*)malloc(sizeof(Item));
        temp->ID = i;
        temp->SleepTime = (rand() % 700) + 200;

        mtx.lock();
        buffer.push_back(temp);
        mtx.unlock();

        sem_post(&consumerSem);

        free(temp);
    }
}

void *consumer(void *args){
    while(1){
        sem_wait(&producerSem);

        Item *temp;
        mtx.lock();
        temp = buffer[num_items-1];
        mtx.unlock();
        
        int id = temp->ID;
        int sleepTime = temp->SleepTime;

        buffer.pop_back();

        usleep(sleepTime);

    }

}

int main(int argc, char**argv){
    num_prod = atoi(argv[1]);
    num_cons = atoi(argv[2]);
    buf_size = atoi(argv[3]);
    num_items = atoi(argv[4]);

    sem_open("pSem", O_CREAT | O_EXCL, 0644, 0);
    
    if (argc < 4){
        fprintf(stderr,"usage: ./hw1 <num of producers> <num of consumers> <buffer size> <number of items created> \n");
    }

    pthread_t threads[num_prod];
    if(num_items%num_prod == 0){
        //create threads
        pthread_t producerThread;
         pthread_t consumerThread;

        for(int i=0; i < num_prod; i++){
            printf("producer thread create i: %i \n", i);
            pthread_create(&producerThread,NULL,producer,(void*)i);
        }

        for(int i=0; i < num_cons; i++){
            printf("consumer thread create i: %i \n", i);
            pthread_create(&consumerThread,NULL,consumer,(void*)i);
        }
    } 

    return 0;
}