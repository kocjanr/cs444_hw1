#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <semaphore.h>

struct Item {
    int ID;
    int SleepTime;
};

std::vector<Item> buffer;
sem_t producerSem;
sem_t consumerSem;
std::mutex mtx;

int num_prod;
int num_cons;
int buf_size;
int num_items;
int bufferCount;
int leftOver;

void producer(int args){
    int threadId = args;
    int itemsPerThread = num_items/num_prod;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread);

    // printf("Producer threadID: %i start: %i end: %i \n", threadId,start,end);
    
    srand (time(NULL));
    unsigned int sleep = (rand() % 400) + 300;
    usleep(sleep);

    for (int i =start; i < end; i++){
        Item temp;
        temp.ID = i;
        temp.SleepTime = (rand() % 700) + 200;
        mtx.lock();
        buffer.push_back(temp);
        // printf("PRODUCER:%i\n",temp.ID);
        mtx.unlock();
    }
}

void consumer(int args){
    int threadId = args;
    int itemsPerThread = num_items/num_cons;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread);
    printf("Consumer threadID: %i start: %i end: %i \n", threadId,start,end);

    for(int i=start;i<end;i++){
        Item temp;
        mtx.lock();
        temp = buffer.back();
        buffer.pop_back();
        printf("CONSUMER:%i\n",temp.ID);
        mtx.unlock();
    }
        
}

int main(int argc, char**argv){
    num_prod = atoi(argv[1]);
    num_cons = atoi(argv[2]);
    buf_size = atoi(argv[3]);
    num_items = atoi(argv[4]);
    
    if (argc < 4){
        fprintf(stderr,"usage: ./hw1 <num of producers> <num of consumers> <buffer size> <number of items created> \n");
    }

    if(num_items%num_prod == 0){
        std::vector<std::thread> threads;
        std::vector<std::thread> consumerThreads;

        for(int i=0;i<num_prod;i++){
            threads.push_back(std::thread(producer,i));
        }
        
        for(int j=0;j<num_prod;j++){
            threads[j].join();
        }

        for(int a=0;a<num_cons;a++){
            consumerThreads.push_back(std::thread(consumer,a));
        }

         for(int b=0;b<num_cons;b++){
            consumerThreads[b].join();
        }

    } 

    return 0;
}