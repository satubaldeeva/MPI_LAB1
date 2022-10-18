#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <cassert>
using namespace std;
const int M = 2;
//C:\Users\NITRO 5\source\repos\ConsoleApplication5\Debug
//mpiexec -n 5 MPI_LAB.exe


int init_count() {
	return rand()*10;
}

int* init_array(int* ar,int n) {
	for (int i = 0; i < n; i++) {
		ar[i] = rand();
	}
	return ar;
}
void start(const int* i, char*** n,int message) {

	int ProcNum, ProcRank;
	double t1, t2, dt;
	MPI_Init(i, n);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	int const size = ProcNum;
	int* array= new int[size];
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	t1 = MPI_Wtime();
	int* ar_for_recv=new int[1];
	for (int j = 0; j < M; j++) {
		if (ProcRank == 0) {
			array = init_array(array,size);
		}
				
		MPI_Scatter(array, 1, MPI_INT,ar_for_recv,1,MPI_INT, 0, MPI_COMM_WORLD);
		if (ProcRank != 0) {
			printf("Procces(%d) got this value from the main procces : %d", ProcRank, ar_for_recv[0]);
		}
		int* sub_avgs = NULL;
		if (ProcRank == 0) {
			sub_avgs = new int[size];
			assert(sub_avgs != NULL);
		}
		MPI_Gather(ar_for_recv, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
		if (ProcRank == 0) {
			for(int i = 0; i < size; i++) {
				cout << sub_avgs[i]<<" ";
			}
		}
		cout << "\n" << j << " ends" << endl;
	}
	t2 = MPI_Wtime();
	dt = t2 - t1;
	MPI_Finalize();
	cout << dt << endl;
	

}


int main(int *argc, char** argv)

{
	start(argc , &argv,12345);
	return 0;
}