#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Read_vector(double local_a[], MPI_Datatype type, int n, char vec_name[], int my_rank, MPI_Comm comm) {
	double* a = NULL;
	int i;

	if (my_rank == 0) {
		a = malloc(n*sizeof(double));
		printf("Enter the vector %s\n", vec_name);
		for (i = 0; i < n; i++)
			scanf("%lf", &a[i]);
		MPI_Scatter(a, 1, type, local_a, 1, type, 0, comm);
		free(a);
	} else
		MPI_Scatter(a, 1, type, local_a, 1, type, 0, comm);
} /* Read vector */

void Print_vector(double local_b[], MPI_Datatype type, int n, char title[], int my_rank, MPI_Comm comm) {
	double* b = NULL;
	int i;

	if (my_rank == 0) {
		b = malloc(n*sizeof(double));
		MPI_Gather(local_b, 1, type, b, 1, type, 0, comm);
		printf("%s\n", title);
		for (i = 0; i < n; i++)
			printf("%lf ", b[i]);
		printf("\n");
		free(b);
	} else
		MPI_Gather(local_b, 1, type, b, 1, type, 0, comm);
} /* Print vector */

int main(void){
	int local_n, n = 4, my_rank, comm_sz;
	double local_a[n];

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	local_n = n/comm_sz;

	MPI_Datatype myType;
	MPI_Type_contiguous(local_n, MPI_DOUBLE, &myType);
	MPI_Type_commit(&myType);


	Read_vector(local_a, myType, n, "Vetor A", my_rank, MPI_COMM_WORLD);
	Print_vector(local_a, myType, n, "Vetor A", my_rank, MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}