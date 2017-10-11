#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

void Get_input(int my_rank, int comm_sz, int* a, int* b, int n);
void Generate_list(int my_rank, int comm_sz, int* a, int* b, int n);

int main(void) {
   int my_rank, comm_sz, n;
   int *a, *b, escalar, local_total = 0, global_total = 0;
   double time;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   char option;

   if(!my_rank){
      printf("Digite um escalar:\n");
      scanf(" %d", &escalar);

      printf("Qual a dimensão dos vetores:\n");
      scanf("%d", &n);
      if(n % comm_sz != 0){
         printf("\n\nATENÇÃO -----------> Utilize um valor multiplo da quantidade de processos!\n\n");
         // exit(1);
         MPI_Abort(MPI_COMM_WORLD, 0);
      }

      printf("Digitar (d) ou gerar (g) vetores?\n");
      scanf(" %c", &option);
   }

   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&option, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
   MPI_Bcast(&escalar, 1, MPI_INT, 0, MPI_COMM_WORLD);

   a = (int*) malloc(n*sizeof(int));
   b = (int*) malloc(n*sizeof(int));

   if(!my_rank)
      time = MPI_Wtime();

   if(option == 'd')
      Get_input(my_rank, comm_sz, a, b, n);
   else Generate_list(my_rank, comm_sz, a, b, n);

   if(!my_rank){
      printf("Arrays definidos em %.2f segundos!\n", MPI_Wtime() - time);
      time = MPI_Wtime();
   }

   for(int i = 0; i < n/comm_sz; i++){
      local_total += a[i] * b[i];
   }

   printf("%.2f segundos para produto interno\n", MPI_Wtime() - time);

   // printf("Resposta do processo %i: %i\n", my_rank, local_total);

   MPI_Reduce(&local_total, &global_total, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   if (!my_rank) {
      printf("Resposta: %i\n", global_total);
   }

   MPI_Finalize();

   return 0;
} /*  main  */

void Get_input(int my_rank, int comm_sz, int* a, int* b, int n) {
   if (my_rank == 0) {
      printf("Digite os elementos do vetor 1:\n");
      for (int i = 0; i < n; i++)
         scanf("%d", &a[i]);
      printf("Digite os elementos do vetor 2:\n");
      for (int i = 0; i < n; i++)
         scanf("%d", &b[i]);
   }

   MPI_Scatter(a, n/comm_sz, MPI_INT, a, n/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Scatter(b, n/comm_sz, MPI_INT, b, n/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
}  /* Get_input */

void Generate_list(int my_rank, int comm_sz, int* a, int* b, int n) {
      srand(time(NULL) * (my_rank + 1));
      for (int i = 0; i < n / comm_sz; i++){
         a[i] = rand() % 10;
         b[i] = rand() % 10;
      }

      // printf("A: ");
      // for (int i = 0; i < n; i++){
      //    printf(" %i", a[i]);
      //    // a[i] *= escalar;
      // }

      // printf("\nB: ");
      // for (int i = 0; i < n; i++){
      //    printf(" %i", b[i]);
      // } printf("\n");
}  /* Generate_list */