// CS444 Homework1
// Ryan Kocjan
// 4/10/2018
//

#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <signal.h>

// Struct for Item that 
// simulates work being
// done on a process
struct Item {
    int ID;
    int SleepTime;
};

//Global variables
std::vector<Item> buffer;
sem_t numberOfSlots;
sem_t itemsToConsume;
std::mutex mtx;

int num_prod;
int num_cons;
int buf_size;
int num_items;

// void producer(int args)
//
// All the Item objects are created
// here and stored into the global buffer. 
// Takes in the argument args that is the thread id being executed
void producer(int args){
    int threadId = args;
    int itemsPerThread = num_items/num_prod;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread);

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
        mtx.unlock();
    }

    sem_post(&itemsToConsume);
}

//void consumer(int args)
//
// Execites the Item objects from global
// buffer and pops them off after being executed.
// Take in one arugments args that is the 
// thread id
void consumer(int args){
    int threadId = args;
    int itemsPerThread = num_items/num_cons;
    int start = threadId * itemsPerThread;
    int end = ((threadId+1)*itemsPerThread);

    
    // while(1){
    //     sem_wait(&itemsToConsume);

    //     Item temp;
    //     mtx.lock();
    //     temp = buffer.back();
    //     buffer.pop_back();
    //     mtx.unlock();
    //     printf("%i: consuming %i\n",args, temp.ID);
    //     fflush(stdout);
    //     usleep(temp.SleepTime);
    // }
    // sem_post(&numberOfSlots);
    
    // sem_wait(&itemsToConsume);
    
    sem_wait(&itemsToConsume);
    for(int i=start;i<end;i++){
        Item temp;
        mtx.lock();
        temp = buffer.back();
        buffer.pop_back();
        mtx.unlock();
        printf("%i: consuming %i\n",args, temp.ID);
        fflush(stdout);
        usleep(temp.SleepTime);
        sem_post(&numberOfSlots);
    }    
}

// void signalHandler(int param)
//
// When a user executes the SIGINT
// signal this function will exit
// the program
void signalHandler(int param){
    printf("PROGRAM EXIT\n");
    exit(1);
}

int main(int argc, char**argv){
    num_prod = atoi(argv[1]);
    num_cons = atoi(argv[2]);
    buf_size = atoi(argv[3]);
    num_items = atoi(argv[4]);

    //Create semaphores
    sem_init(&numberOfSlots,0,buf_size);
    sem_init(&itemsToConsume,0,0);
    
    //error check for user input
    if (argc < 4){
        fprintf(stderr,"usage: ./hw1 <num of producers> <num of consumers> <buffer size> <number of items created> \n");
    }

    std::vector<std::thread> producerThreads;
    std::vector<std::thread> consumerThreads;

    //Consumer threads
    for(int a=0;a<num_cons;a++){
        consumerThreads.push_back(std::thread(consumer,a));
    }

    //Producer threads
    for(int i=0;i<num_prod;i++){
        producerThreads.push_back(std::thread(producer,i));
    }

    fprintf(stderr,"Done Producing!!\n");
    
    //Join consumer threads
    for(int j=0;j<num_cons;j++){
        consumerThreads[j].join();
    }

    //Join producer threads
    for(int j=0;j<num_prod;j++){
        producerThreads[j].join();
    }

    signal(SIGINT, signalHandler);
    
    return 0;
}