#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <mutex>

struct Item {
    int ID;
    int SleepTime;
};

std::vector <Item*> buffer;
std::mutex mu;
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

    // for(int i=0; i < count; i++){
    //     Item *temp = (Item*)malloc(sizeof(Item));
    //     if(temp == NULL){
    //         fprintf(stderr,"Can not create Item object.");
    //     }
    //     temp->ID = i;
    //     temp->SleepTime = (rand() % 700) + 200;
    //     mu.lock();
    //     buffer.push_back(temp);
    //     mu.unlock();
    //     free(temp);
    // }
}

void *consumer(void *args){

}

int main(int argc, char**argv){
    num_prod = atoi(argv[1]);
    num_cons = atoi(argv[2]);
    buf_size = atoi(argv[3]);
    num_items = atoi(argv[4]);
    

    if (argc < 4){
        fprintf(stderr,"usage: ./hw1 <num of producers> <num of consumers> <buffer size> <number of items created> \n");
    }

    //create threads
    pthread_t thread;
    for(int i=0; i < num_prod; i++){
        int create = pthread_create(&thread,NULL,producer,(void*)i);
    }

    //join threads
    for(int j=0; j< num_prod; j++){
        int join = pthread_join(thread,NULL);
    }

    return 0;
}