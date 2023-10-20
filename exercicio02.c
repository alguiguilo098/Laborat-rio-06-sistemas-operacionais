#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>
#include<pthread.h>
int soma_matriz=0;
int**matriz;
pthread_mutex_t mut;
pthread_t thread[3];
typedef struct chunck{
    int*vetor;
    int quantidade;
}chunck;
int** criar_matriz_aleatoria(int linhas, int colunas) {
    int** matriz = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = rand() % 11;  // Gera números aleatórios de 0 a 10
        }
    }
    return matriz;
}

void embaralhar_vetor(int vetor[], int tamanho) {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com o tempo atual
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Troca o elemento atual com o elemento na posição j
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}
chunck*criar_intervalo(int*vetor,int tam){
    chunck*a=malloc(sizeof(chunck));
    a->vetor=vetor;
    a->quantidade=tam;
    return a;
}
int somar_vetor(int*v, int qtd){
    int sum=0;
    for (int i = 0; i < qtd; i++){
        sum+=v[i];
    }
    return sum;
}
void* somar_linha(void*arg){
    chunck*values=(chunck*)arg;
    for (int i = 0; i<values->quantidade; i++){
        pthread_mutex_lock(&mut);
        int sum=somar_vetor(matriz[values->vetor[i]],10);
        printf("soma da linha %d: %d\n\n",values->vetor[i],sum);
        soma_matriz+=sum;
        pthread_mutex_unlock(&mut);
    }
    pthread_exit(0);
}
int*dividir_linhas_da_thread(){
    int* v=(int*)malloc(3*sizeof(int));
    int num=10;
    srand(time(NULL));
    for (int i = 0; i <2; i++){
        v[i]=rand()%num;
        num-=v[i];
        if (num<0){
            printf("Impossível implementar");
        }
    }
    v[2]=num;
    return v;
}
void imprimir_matriz(int** matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}
void imprimir_vetor(int vetor[], int tamanho) {
    for(int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}
int main(int argc, char const *argv[]){
    int v[]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    printf("Vetor de indices de linhas de cada matriz:\n");
    embaralhar_vetor(v,10);
    imprimir_vetor(v,10);
    int*qtd=dividir_linhas_da_thread();
    int* intervalo_thread_1=(int*)malloc(qtd[0]*sizeof(int));
    memcpy(intervalo_thread_1,v,qtd[0]*sizeof(int));
    int* intervalo_thread_2=(int*)malloc(qtd[1]*sizeof(int));
    int* vetor_2=&v[qtd[0]];
    memcpy(intervalo_thread_2,vetor_2,qtd[1]*sizeof(int));
    int* intervalo_thread_3=(int*)malloc(qtd[2]*sizeof(int));
    int* vetor_3=&vetor_2[qtd[1]];
    memcpy(intervalo_thread_3,vetor_3,qtd[2]*sizeof(int));
    printf("thread 1: indices das linhas\n");
    imprimir_vetor(intervalo_thread_1,qtd[0]);
    chunck* chunck_1=criar_intervalo(intervalo_thread_1,qtd[0]);
    printf("thread 2: indices das linhas\n");
    imprimir_vetor(intervalo_thread_2,qtd[1]);
    chunck* chunck_2=criar_intervalo(intervalo_thread_2,qtd[1]);
    printf("thread 3: indices das linhas\n");
    imprimir_vetor(intervalo_thread_3,qtd[2]);
    chunck* chunck_3=criar_intervalo(intervalo_thread_3,qtd[2]);
    matriz=criar_matriz_aleatoria(10,10);
    printf("matriz aleatória:\n");
    imprimir_matriz(matriz,10,10);
    pthread_mutex_init(&mut,NULL);
    pthread_create(&thread[0],NULL,somar_linha,(void*)chunck_1);
    pthread_create(&thread[1],NULL,somar_linha,(void*)chunck_2);
    pthread_create(&thread[2],NULL,somar_linha,(void*)chunck_3);
    pthread_join(thread[0],NULL); 
    pthread_join(thread[1],NULL); 
    pthread_join(thread[2],NULL); 
    printf("A soma dos valores é: %d",soma_matriz);
    pthread_exit(&thread);
    pthread_mutex_destroy(&mut);
    return 0;
}


