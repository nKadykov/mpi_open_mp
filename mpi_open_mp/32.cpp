#include <mpi.h>
#include <omp.h>
#include <iostream>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank_mpi, size_mpi;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);
	MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);

	int k = 4;
	int N;
	double pi = 0;
	double sum = 0;
	double start, end;

	if (rank_mpi == 0) {
		std::cout << "Enter N: ";
		std::cin >> N;
		MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} else {
		MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	
	start = MPI_Wtime();
	
#pragma omp parallel reduction(+:pi) num_threads(k)
	{
#pragma omp for
		for (int i = rank_mpi; i < N; i = i + size_mpi) 
		{
			double x = (i + 0.5) / N;
			pi += 4 / (1 + x * x) / N;
		}
	}
	end = MPI_Wtime();
	MPI_Reduce(&pi, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank_mpi == 0) {
		std::cout.precision(9);
		std::cout << "pi: " << sum << "\n";
		std::cout << "Time: " << end - start << "\n";
	}

	MPI_Finalize();

	return 0;
}