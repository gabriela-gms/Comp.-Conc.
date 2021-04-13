//Soma todos os elementos de um vetor e verifica se a soma multiplicadas por 4 dá o valor aproximado a pi.
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include <math.h>

long long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
double *vetor; //vetor de entrada com dimensao dim 

//fluxo das threads
void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   double *somaLocal; //variavel local da soma de elementos
   somaLocal = (double*) malloc(sizeof(double));
   if(somaLocal==NULL) {
   	exit(1);
   }
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   //soma os elementos do bloco da thread
   for(long int i=ini; i<fim; i++)
      *somaLocal += vetor[i];
   //retorna o resultado da soma local
   pthread_exit((void *) somaLocal); 
}

//fluxo principal
int main(int argc, char *argv[]) {
   double somaSeq= 0; //soma sequencial
   double somaConc= 0; //soma concorrente
   double ini, fim; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   double *retorno; //valor de retorno das threads

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
       return 1; 
   }
   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   //aloca o vetor de entrada
   vetor = (double*) malloc(sizeof(double)*dim);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   
   //preenche o vetor de entrada
   float aux = 1; 
   for(long int i= 0; i < dim ; i++){
	if(dim ==0); //lançar excessão break
	else if (i % 2) vetor[i] = - 1/aux;
	else vetor[i] = 1/aux;
	aux+=2;
   }
  
   //soma sequencial dos elementos
   GET_TIME(ini);
   for(long int i=0; i<dim; i++)
      somaSeq += vetor[i];   
   GET_TIME(fim);
   printf("Tempo sequencial:  %lf\n", fim-ini);

   //soma concorrente dos elementos
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //soma global
      somaConc += *retorno;
   }
   GET_TIME(fim);
   printf("Tempo concorrente:  %lf\n", fim-ini);

   //exibir os resultados
   printf("Soma seq:  %.15lf\n", 4*somaSeq);
   printf("Soma conc: %.15lf\n", 4*somaConc);   
   printf("   π   =   %.15lf\n", M_PI);

   //libera as areas de memoria alocadas
   free(vetor);
   free(tid);

   return 0;
}

// Quanto maior o valor da dimensão mais o valor resultante se aproxima de pi. O valor resultante para o código sequencial e o concorrente é o memso, pois o vetor é igual e a base do cálculo é a mesma. O que diferencia das duas formas é o tempo, quanto maior a dimensão do vetor mais eficiente o código multithreads, em relação ao código sequencial, se demonstra.
// Quando inicia com dimensão baixa o tempo sequencial chega a ser melhor, pois no trecho de código ele só realiza o cálculo, enquanto o código multithreads realiza alocação, verificação, for para criação de threads e para esperar essas threads terminarem. Então não se tem muita vantagem em usar o multithreads quando a dimensão é baixa.
