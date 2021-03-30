/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Incrementa em usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NUM_THREADS  2 //total de threads a serem criadas
#define NUM_ELEMENTOS 10000

int vetor[NUM_ELEMENTOS];

//funcao executada pelas threads
void * incrementa (void* arg) {
	int idThread = * (int*) arg;
	
	if(idThread == (NUM_ELEMENTOS/NUM_THREADS)){
		for(int i = 0; i<idThread; i++)
			vetor[i]=1;
	}for(int i =idThread; i<NUM_ELEMENTOS; i++)
		vetor[i]=1;
	pthread_exit(NULL);
}

//funcao principal do programa
int main() {

	//inicializando o vetor com elemento em 0
	for(int i = 0; i<NUM_ELEMENTOS; i++)
		vetor[i]=0;

		
	pthread_t tid_sistema[NUM_THREADS]; //identificadores das threads no sistema
	int thread; //variavel auxiliar
	int tid_local=NUM_ELEMENTOS/NUM_THREADS;

	for(thread=0; thread<NUM_THREADS; thread++) {
	
		if (pthread_create(&tid_sistema[thread], NULL, incrementa, (void*) &tid_local)) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
		
	}

	//--espera todas as threads terminarem
	for (thread=0; thread<NUM_THREADS; thread++) {
		if (pthread_join(tid_sistema[thread], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	}
	
		
	for(int i = 0; i<NUM_ELEMENTOS; i++){
		if(!(vetor[i])) break;
		if (i == NUM_ELEMENTOS-1)
			puts("Vetor incrementado passou no teste.");
	}
	printf("--Thread principal terminou\n");
	
	pthread_exit(NULL);
}
