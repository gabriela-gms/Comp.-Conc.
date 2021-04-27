#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x1_cond, x2_cond;

/* Thread 4 */
void * Thread4 (void *t){
	printf("Seja bem-vindo!\n");
	pthread_mutex_lock(&x_mutex);
	x++;
	if (x==1)
		pthread_cond_broadcast(&x1_cond);
	pthread_mutex_unlock(&x_mutex);
	
	pthread_exit(NULL);
}

/* Thread 2 */
void * Thread2 (void *t){	
	pthread_mutex_lock(&x_mutex);
	if (x<1)
		pthread_cond_wait(&x1_cond,&x_mutex);
	x++;		
	printf("Fique a vontade.\n");
	pthread_cond_signal(&x2_cond);
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

/* Thread 3 */
void * Thread3 (void *t){
	pthread_mutex_lock(&x_mutex);
	if (x<1)
		pthread_cond_wait(&x1_cond,&x_mutex);
	x++;		
	printf("Sente-se por favor.\n");
	pthread_cond_signal(&x2_cond);
	pthread_mutex_unlock(&x_mutex);
	
	pthread_exit(NULL);
}

/* Thread 1 */
void * Thread1 (void *t){
	pthread_mutex_lock(&x_mutex);
	while(x < 3) 
		pthread_cond_wait(&x2_cond, &x_mutex);
	printf("Volte sempre!\n");
	pthread_mutex_unlock(&x_mutex);   
	
	pthread_exit(NULL);
}

/*		Função principal		*/
int main(int argc, char *argv[]){
	int i;
	pthread_t threads[NTHREADS];
	
	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x1_cond, NULL);
	pthread_cond_init (&x2_cond, NULL);
	
	/* Cria as threads */
	pthread_create(&threads[3], NULL, Thread4, NULL);
	pthread_create(&threads[2], NULL, Thread2, NULL);
	pthread_create(&threads[1], NULL, Thread3, NULL);
	pthread_create(&threads[0], NULL, Thread1, NULL);	
	
	/* Espera todas as threads completarem */
	for (i = 0; i < NTHREADS; i++) 
		pthread_join(threads[i], NULL);	
	
	/* Desaloca variaveis e termina */
	pthread_mutex_destroy(&x_mutex);
	pthread_cond_destroy(&x1_cond);
	pthread_cond_destroy(&x2_cond);
}
