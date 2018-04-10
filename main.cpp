#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <semaphore.h>
#include <mutex>

struct Item {
    int ID;
    int SleepTime;
};

std::vector<Item> buffer;
sem_t numberOfSlots;
sem_t itemsToConsume;
std::mutex mtx;

int num_prod;
int num_cons;
int buf_size;
int num_items;

void producer(int args){
    int threadId = args;
    int itemsPerThread = num_items/num_prod;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread);

    // printf("Producer threadID: %i start: %i end: %i \n", threadId,start,end);
    
    srand (time(NULL));
    unsigned int sleep = (rand() % 400) + 300;
    usleep(sleep);

    sem_wait(&numberOfSlots);

    for(int i=start; i < end; i++){
        Item temp;
        temp.ID = i;
        temp.SleepTime = (rand() % 700) + 200;
        mtx.lock();
        buffer.push_back(temp);
        // printf("PRODUCER:%i\n",temp.ID);
        mtx.unlock();
    }
    sem_post(&itemsToConsume);
}

void consumer(int args){
    int threadId = args;
    int itemsPerThread = num_items/num_cons;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread);
    // printf("Consumer threadID: %i start: %i end: %i \n", threadId,start,end);

    //sem_wait(&itemsToConsume);
    // while(1){
        
    //     Item temp;
    //     mtx.lock();
    //     temp = buffer.back();
    //     buffer.pop_back();
    //     mtx.unlock();
    //     printf("%i: consuming %i\n",args, temp.ID);
    //     usleep(temp.SleepTime);
        
    // }
    //sem_post(&numberOfSlots);
    sem_wait(&itemsToConsume);
    
    for(int i=start;i<end;i++){
        Item temp;
        mtx.lock();
        temp = buffer.back();
        buffer.pop_back();
        mtx.unlock();
        printf("%i: consuming %i\n",args, temp.ID);
        usleep(temp.SleepTime);
    }   
    sem_post(&numberOfSlots);
}

int main(int argc, char**argv){
    num_prod = atoi(argv[1]);
    num_cons = atoi(argv[2]);
    buf_size = atoi(argv[3]);
    num_items = atoi(argv[4]);

    sem_init(&numberOfSlots,0,buf_size);
    sem_init(&itemsToConsume,0,0);
    
    if (argc < 4){
        fprintf(stderr,"usage: ./hw1 <num of producers> <num of consumers> <buffer size> <number of items created> \n");
    }

        std::vector<std::thread> producerThreads;
        std::vector<std::thread> consumerThreads;

        for(int a=0;a<num_cons;a++){
            consumerThreads.push_back(std::thread(consumer,a));
        }

        for(int i=0;i<num_prod;i++){
            producerThreads.push_back(std::thread(producer,i));
        }
        
        for(int j=0;j<num_cons;j++){
            consumerThreads[j].join();
        }

        for(int j=0;j<num_prod;j++){
            producerThreads[j].join();
        }

        printf("DONE PRODUCING!!\n");
    
    return 0;
}