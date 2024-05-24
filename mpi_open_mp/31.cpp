#include <mpi.h>
#include <omp.h>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank_mpi, size_mpi;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);
	MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);

	omp_set_num_threads(4);
#pragma omp parallel
	{
		int rank_omp = omp_get_thread_num(); 
		int size_omp = omp_get_num_threads();
		printf("I am %d thread from %d process. Number of hybrid threads = %d.\n", rank_omp, rank_mpi, size_omp * size_mpi);
	}

	MPI_Finalize();
	return 0;
}