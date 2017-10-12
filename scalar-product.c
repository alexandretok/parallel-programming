#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

void Get_input(int my_rank, int comm_sz, int* a, int* b, int n, int escalar);
void Generate_list(int my_rank, int comm_sz, int* a, int* b, int n, int escalar);

int main(void) {
   int my_rank, comm_sz, n;
   int *a, *b, escalar, local_total = 0, global_total = 0;
   double tempo, tempo_total;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   if(!my_rank){
      printf("Qual a dimensão dos vetores:\n");
      scanf("%d", &n);
      if(n % comm_sz != 0){
         printf("\n\nATENÇÃO -----------> Utilize um valor multiplo da quantidade de processos!\n\n");
         // exit(1);
         MPI_Abort(MPI_COMM_WORLD, 0);
      }
   }

   if(!my_rank){
   	srand(time(NULL));
   	escalar = rand() % 5 + 1;
   	printf("Escalar gerado: %i\n", escalar);
   }

   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&escalar, 1, MPI_INT, 0, MPI_COMM_WORLD);

   a = (int*) malloc(n*sizeof(int));
   b = (int*) malloc(n*sizeof(int));

   Get_input(my_rank, comm_sz, a, b, n, escalar);
   
   tempo = MPI_Wtime();

   for(int i = 0; i < n/comm_sz; i++){
      local_total += a[i] * b[i];
   }

   tempo = MPI_Wtime() - tempo;

   MPI_Reduce(&tempo, &tempo_total, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
   MPI_Reduce(&local_total, &global_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   if (!my_rank) {
      printf("%.2f milisegundos para produto interno\n", tempo_total * 1000);
      printf("Resposta: %i\n", global_total);
   }

   MPI_Finalize();

   return 0;
} /*  main  */

void Get_input(int my_rank, int comm_sz, int* a, int* b, int n, int escalar) {
   if (my_rank == 0) {
      printf("Digite os elementos do vetor 1:\n");
      for (int i = 0; i < n; i++){
         scanf("%d", &a[i]);
         a[i] *= escalar;
      }
      printf("Multiplicado por escalar: ");
      for (int i = 0; i < n; i++)
      	printf("%d ", a[i]);
      printf("\n");
      printf("Digite os elementos do vetor 2:\n");
      for (int i = 0; i < n; i++){
         scanf("%d", &b[i]);
         b[i] *= escalar;
      }
      printf("Multiplicado por escalar: ");
      for (int i = 0; i < n; i++)
      	printf("%d ", b[i]);
      printf("\n");
   }

   MPI_Scatter(a, n/comm_sz, MPI_INT, a, n/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Scatter(b, n/comm_sz, MPI_INT, b, n/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
}  /* Get_input */

void Generate_list(int my_rank, int comm_sz, int* a, int* b, int n, int escalar) {
      srand(time(NULL) * (my_rank + 1));
      for (int i = 0; i < n / comm_sz; i++){
         a[i] = rand() % 5 * escalar;
         b[i] = rand() % 5 * escalar;
      }
}  /* Generate_list */
