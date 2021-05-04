#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Variaveis globais */
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int *vetor;
int nthreads;

//funcao barreira
void barreira(int nthreads) {
	pthread_mutex_lock(&x_mutex); //inicio secao critica
	if (bloqueadas == (nthreads-1)) { 
	//ultima thread a chegar na barreira
		pthread_cond_broadcast(&x_cond);
		bloqueadas=0;
	} else {
		bloqueadas++;
		pthread_cond_wait(&x_cond, &x_mutex);
	}
	pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa (void *arg) {
	int id = *(int*)arg;
	for (int passo=0; passo <nthreads; passo++) {
		int aux;
		int salto = pow(2,passo);
		if((id - salto)<0)
			// printf("Thread %d: passo=%d\n", id, passo);
			aux=0;

		else{
		//	printf("Thread %d: passo=%d\n", id, passo);
			aux = *(vetor+(id-salto));}
			
		//sincronizacao condicional
		barreira(nthreads);
		if( (id-salto) < 0) 
			*(vetor+id)=*(vetor+id);
		else {
			*(vetor+id) += aux;
		}
		barreira(nthreads);	
	}
	pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {

/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x_cond, NULL);
	
	int i;


	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
	if(argc < 2) {
		fprintf(stderr, "Digite: %s <dimensao do vetor> \n", argv[0]);
		return 1; 
	}
	nthreads = atoi(argv[1]);
	//aloca o vetor de entrada
	vetor = (int*) malloc(sizeof(int)*nthreads);
	if(vetor == NULL) {
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	
	//preenche o vetor de entrada
	puts("Vetor Inicial:");
	for(i=0; i<nthreads; i++){
		vetor[i]=1+i;
		printf("%d ", vetor[i]);
	}
	puts("");	
	puts("");	
	/* sequencial */
	for(i=0; i<nthreads; i++)
		vetor[i] = vetor[i] + vetor[i-1];
	puts("Resultado sequencial");
	for(i=0; i<nthreads; i++)
		printf("%d ", vetor[i]);		
	puts("");
	puts("");
	
	
	/* Cria as threads */
	for(i=0; i<nthreads; i++)
		vetor[i]=1+i;

	pthread_t threads[nthreads];
	int id[nthreads];
	
	for(i=0;i<nthreads;i++) {
		id[i]=i;
		pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
	}

	/* Espera todas as threads completarem */
	for (i = 0; i < nthreads; i++) 
		pthread_join(threads[i], NULL);

	puts("Resultado sincronização coletiva");
	for(i=0; i<nthreads; i++)
		printf("%d ", vetor[i]);		
	puts("");
	puts("");
	
	
	printf ("FIM.\n");

	/* Desaloca variaveis e termina */
	pthread_mutex_destroy(&x_mutex);
	pthread_cond_destroy(&x_cond);
	return 0;
}
