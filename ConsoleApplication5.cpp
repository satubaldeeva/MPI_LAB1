#include <stdio.h>
#include "mpi.h"
#include <iostream>
using namespace std;
const int M = 5;

void start(const int* i, char*** n,int message) {

	int ProcNum, ProcRank, RecvRank, ProcMain;
	int Send = message;
		MPI_Status Status;
		
		MPI_Init(i, n);
		MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
		MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
		for (int j = 0; j < M; j++) {
			if (ProcRank == 0)
			{
				// Действия, выполняемые только процессом с рангом 0

				for (int i = 1; i < ProcNum; i++)
				{

					MPI_Send(&Send, 1, MPI_INT, i,
						100, MPI_COMM_WORLD);
					printf("\nI(0) send the message to %d process: ", i);
					MPI_Recv(&RecvRank, 1, MPI_INT, i,
						MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
					printf("\nI got an answer from process %d", RecvRank);
				}

			}
			else { // Сообщение, отправляемое всеми процессами,
				// кроме процесса с рангом 0
				MPI_Recv(&ProcMain, 1, MPI_INT, 0,
					MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				printf("\nI(%d) got %3d from 0 process!", ProcRank, ProcMain);
				MPI_Send(&ProcRank, 1, MPI_INT, 0, 100, MPI_COMM_WORLD);

			}
			cout << "\n"<< j <<" ends"<<endl;
		}

		MPI_Finalize();
	

}

int main(int *argc, char** argv)

{
	start(argc , &argv,12345);
	
	return 0;
}