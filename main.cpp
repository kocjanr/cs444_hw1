#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <mutex>

struct Item
{
    int ID;
    int SleepTime;
};

std::vector <Item*> buffer;
std::mutex mu;

void *addToBuffer(void* args){
    mu.lock();

    mu.unlock();

}

void *removeFromBuffer(void *args){

}

int main(int argc, char**argv){
    int num_prod;
    int num_cons;
    int buf_size;
    int num_items;

    if (argc < 4){
        fprintf(stderr,"Not enough args.");
    }

    for(int i=0;i< num_prod;i++){
        unsigned int microseconds = (rand() % 700) + 300;
        usleep(microseconds);
        Item *temp = (Item*)malloc(sizeof(Item));
        if(temp == NULL){

        }
        temp->ID = i;
        temp->SleepTime = (rand() % 900) + 200;
        buffer.push_back(temp);

    }

    return 0;
}