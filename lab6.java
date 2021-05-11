/*
Objetivo:
    Implementar um programa concorrente em Java para incrementar de 1 todos os elementos de um vetor.
    Considere que o numero de threads é menor ou igual ao numero de elementos do vetor.
    Divida a tarefa entre as threads de forma balanceada.
Roteiro:
    1. Na thread main, preencha e imprima o vetor com valores iniciais, crie e dispare as threads,
    aguarde todas as threads terminarem e imprima os valores finais do vetor.
    2. Teste seu programa.
*/

//classe da estrutura de dados (recurso) compartilhado entre as threads
class Inc {
    //recurso compartilhado
    private int tamanho;
    private int nThreads;
    private int[] vet;

    //construtor
    public Inc(int tamanho, int nThreads, int[] vet) {
        this.tamanho = tamanho;
        this.nThreads = nThreads;
        this.vet = vet;
    }

    //operacao de escrita sobre o recurso compartilhado
    public synchronized void inc(int id) {
        int tamBloco = this.tamanho / this.nThreads; //tamanho do bloco de cada thread
        int ini = id * tamBloco; //elemento inicial do bloco da thread
        int fim; //elemento final(nao processado) do bloco da thread
        if(id == nThreads-1)
            fim = tamanho;
        else
            fim = ini + tamBloco; //trata o resto se houver

        for(int i = ini; i < fim; i++) {
            this.vet[i]++;
        }
    }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa
class T extends Thread {
    //identificador da thread
    private int id;
    //objeto compartilhado com outras threads
    private Inc soma;

    //construtor
    public T(int tid, Inc soma) {
        this.id = tid;
        this.soma = soma;
    }

    //metodo main da thread
    public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
            this.soma.inc(id);
        System.out.println("Thread " + this.id + " terminou!");
    }
}

//classe da aplicacao
class TIncrementaDeUm {
    static final int N = 10;    //dimensão vector
    static  int NT = 11;    //numero de thread
    public static void main (String[] args) {
        // inicializando o vetor
        int[] vet = new int[N];
        // preenchendo de zero
        for(int i = 0; i < N; i++)
            vet[i]=0;

        // imprimindo o vetor inicializado
        System.out.println("Vetor inicial: ");
        for(int i=0; i<N; i++)
            System.out.printf("%d ", vet[i]);

        System.out.println();
        System.out.println();

        // verificando se numero de thread é superior ao numero de elementos do vetor(dim)
        if (NT > N)
          NT = N;

        // reserva espaço para um vetor de threads
        Thread[] threads = new Thread[NT];

        //cria uma instancia do recurso compartilhado entre as threads
        Inc soma = new Inc(N,NT,vet);

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++)
            threads[i] = new T(i, soma);

        //inicia as threads
        for (int i=0; i<threads.length; i++)
            threads[i].start();

        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        System.out.println();
        System.out.println("Vetor incrementado de 1: ");
        for(int i=0; i<N; i++)
            System.out.printf("%d ", vet[i]);
        System.out.println();
    }
}

