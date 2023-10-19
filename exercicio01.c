#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

pthread_mutex_t mut;
#define QUANTIDADE_THREAD_PONG 10
#define QUANTIDADE_THREAD_PING 1
pthread_t thread_ping[QUANTIDADE_THREAD_PONG];
pthread_t thread_pong[QUANTIDADE_THREAD_PING];
char palavra[200];

void* ping(void*arg){
    pthread_mutex_lock(&mut);
    strcat(palavra,"ping...\0");
    printf("%s",palavra);
    strcpy(palavra,"\0"); 
    pthread_mutex_unlock(&mut);
    pthread_exit(0);
}

void* pong(void*arg){
    pthread_mutex_lock(&mut);
    strcat(palavra,"pong\n\0");
    printf("%s",palavra);
    strcpy(palavra,"\0");
    pthread_mutex_unlock(&mut);
    pthread_exit(0);
}
int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mut,NULL);
    for (int i = 0; i <QUANTIDADE_THREAD_PING;i++){
        pthread_create(&thread_ping[i],NULL,ping,NULL);
        pthread_join(thread_ping[i],NULL);
        pthread_create(&thread_pong[i],NULL,pong,NULL);
        pthread_join(thread_pong[i],NULL);
    }
    pthread_mutex_destroy(&mut);
    exit(-1);
    return 0;
}
