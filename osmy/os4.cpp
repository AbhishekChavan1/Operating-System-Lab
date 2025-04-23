#include<iostream>
#include<unistd.h>
#include<pthread.h>
using namespace std;

int balance = 10000;
pthread_mutex_t balance_mutex;

void* reader(void *arg){
    int id = *((int*)arg);
    cout<<"[A] Reader: "<<id<<" reads balance: $"<<balance<<endl;
    sleep(1);
    return nullptr;
}

void* sync_reader(void* arg){
    int id = *((int*)arg);
    pthread_mutex_lock(&balance_mutex);
    cout<<"[S] Reader: "<<id<<" reads balance: $"<<balance<<endl;
    pthread_mutex_unlock(&balance_mutex);
    sleep(1);
    return nullptr;
}

void* writer(void *arg){
    int *params = (int*)arg;
    int id = params[0];
    int amount = params[1];
    bool deposit = params[2];

    if(deposit){
        balance+=amount;
        cout<<"[A] Writer: "<<id<<" deposits $"<<amount<<endl;
    }else{
        if(balance>=amount){
            balance-=amount;
            cout<<"[A] Writer: "<<id<<" withdraws $"<<amount<<endl;
        }else{
            cout<<"[A] Writer: "<<id<<" withdraws $"<<amount<<" failed, not enough balance"<<endl;
        }
    }
    sleep(1);
    return nullptr;
}

void* sync_writer(void* arg){
    int *params = (int*)arg;
    int id = params[0];
    int amount = params[1];
    bool deposit = params[2];
    pthread_mutex_lock(&balance_mutex);
    if(deposit){
        balance+=amount;
        cout<<"[S] Writer: "<<id<<" deposits $"<<amount<<endl;
    }else{
        if(balance>=amount){
            balance-=amount;
            cout<<"[S] Writer: "<<id<<" withdraws $"<<amount<<endl;
        }else{
            cout<<"[S] Writer: "<<id<<" withdraws $"<<amount<<" failed, not enough balance"<<endl;
        }
    }
    pthread_mutex_unlock(&balance_mutex);
    sleep(1);
    return nullptr;
}

void runsim(bool usesync){
    pthread_t readers[5], writers[4];
    int readers_id[5]={1,2,3,4,5};
    int writers_id[4][3]={{1,1000,1},{2,2000,0},{3,3000,1},{4,4000,0}};

    for(int i=0;i<5;i++){
        if(usesync){
            pthread_create(&readers[i], nullptr, sync_reader, (void*)&readers_id[i]);
        }else{
            pthread_create(&readers[i], nullptr, reader, (void*)&readers_id[i]);
        }
    }
    for(int i=0;i<4;i++){
        if(usesync){
            pthread_create(&writers[i], nullptr, sync_writer, (void*)writers_id[i]);
        }else{
            pthread_create(&writers[i], nullptr, writer, (void*)writers_id[i]);
        }
    }

    for(int i=0;i<5;i++)   pthread_join(readers[i], nullptr);    
    for(int i=0;i<4;i++)   pthread_join(writers[i], nullptr);
}

int main(){
    int choice;
    cout<<"Choose the simulation type:\n1. Asynchronous\n2. Synchronous\n";
    cin>>choice;
    if(choice==1){
        cout<<"Running Asynchronous Simulation...\n";
        runsim(false);
    }else if(choice==2){
        cout<<"Running Synchronous Simulation...\n";
        pthread_mutex_init(&balance_mutex, nullptr);
        runsim(true);
        pthread_mutex_destroy(&balance_mutex);
    }else{
        cout<<"Invalid choice. Exiting...\n";
        return 1;
    }
    cout<<"Final balance: $"<<balance<<endl;
    cout<<"Simulation completed.\n";
    return 0;
}