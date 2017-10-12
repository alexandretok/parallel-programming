#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

void Get_input(int my_rank, int comm_sz, int* b, int n);
void Generate_list(int my_rank, int comm_sz, int* a, int n);

int main(void) {
   int my_rank, comm_sz, n;
   int *a, *b, result = 0;
   double tempo, tempo_total = 0;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   n = comm_sz;

   a = (int*) malloc(sizeof(int));

   // Generate_list(my_rank, comm_sz, a, n);
   // MPI_Scan(a, b, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   *a = my_rank;

   tempo = MPI_Wtime();

   // MPI_Allreduce(a, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   result = *a;
   for (int i = 1; i < n; i++) {
      MPI_Sendrecv_replace(a, 1, MPI_INT,
         my_rank != comm_sz - 1 ? my_rank+1:0,
         0, 
         !my_rank ? comm_sz-1:my_rank-1,
         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      result += *a;
   }

   tempo = MPI_Wtime() - tempo;
   
   MPI_Reduce(&tempo, &tempo_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   printf("Result: %i\n", result);

   MPI_Barrier(MPI_COMM_WORLD);

   if(!my_rank) printf("Tempo: %.0fms\n", tempo_total * 1000);

   MPI_Finalize();

   return 0;
} /*  main  */

void Get_input(int my_rank, int comm_sz, int* a, int n) {
   if (my_rank == 0) {
      printf("Digite os elementos do vetor:\n");
      for (int i = 0; i < n; i++){
         scanf("%d", &a[i]);
      }
      printf("\n");
   }
}  /* Get_input */

void Generate_list(int my_rank, int comm_sz, int* a, int n) {
   if (my_rank == 0) {
      srand(time(NULL) * (my_rank+1));
      printf("Array do processo %i: ", my_rank);
      for (int i = 0; i < n; i++){
         a[i] = rand() % 5;
         printf("%i ", a[i]);
      } printf("\n");
   }
}  /* Generate_list */
