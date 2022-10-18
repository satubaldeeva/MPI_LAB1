#include <stdio.h>
#include "mpi.h"
#include <iostream>
using namespace std;
const int M = 2;
//C:\Users\NITRO 5\source\repos\ConsoleApplication5\Debug
//mpiexec -n 5 MPI_LAB.exe


int init_count() {
	return rand()*10;
}
void start(const int* i, char*** n,int message) {

	int ProcNum, ProcRank, RecvRank, ProcMain;
	int Send = message;
		MPI_Status Status;
		int secret_key;
		MPI_Init(i, n);
		MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
		MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
		
		int flag = 1;
		
			for (int j = 0; j < M; j++) {
				if (ProcRank == 0) {
					secret_key = init_count();
				}
				MPI_Bcast(&secret_key, 1, MPI_INT, 0, MPI_COMM_WORLD);
				
				if (ProcRank == 0)
				{
					// Действия, выполняемые только процессом с рангом 0
					printf("\nI(0) send the message to all process!");
					for (int i = 1; i < ProcNum; i++)
					{
						MPI_Recv(&RecvRank, 1, MPI_INT, i,
							MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
						printf("\nI got an answer from %d process %d", i,RecvRank);
					}

				}
				else { // Сообщение, отправляемое всеми процессами,
					// кроме процесса с рангом 0

					MPI_Send(&secret_key, 1, MPI_INT, 0, 100, MPI_COMM_WORLD);

				}
				cout << "\n" << j << " ends" << endl;
			}
		
		MPI_Finalize();
	

}

int main(int *argc, char** argv)

{
	start(argc , &argv,12345);
	
	return 0;
}