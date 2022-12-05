#include <algorithm>
#include <iostream>
#include "mpi.h"
#include <vector>
#include <time.h>
using namespace std;


int main(int argc, char* argv[])
{
	const int N = 10;
	const int sizeGroup = 1000;
	int* answer = new int[N];
	
	int RingCount, myRank, NewRingRank, MasterSlaveRank, MasterSlaveCount, RingGridRank, RingGridCount;
	int* array = new int[N];
	int count = 0;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &RingCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	if (RingCount == 5) {srand(time(NULL));
		//Access to default group
		MPI_Group base_grp, ringGroup;
		//	masterSlaveGroup;
		MPI_Comm ringComm, masterSlaveComm, RingGridComm;// MasterSlaveGridComm;
		//основная группа
		MPI_Comm_group(MPI_COMM_WORLD, &base_grp);
		cout << "MyRank :" << myRank << endl;
		//массивы для передачи значений рангов
		int ringRanks[5] = { 0, 1, 2, 3, 4 };
		//int masterSlaveRanks[5] = { 5, 6, 7, 8, 9 };

		//каждый процесс инициализирует свой массив на 10 элементов
		for (int i = 0; i < N; i++) {
			array[i] = myRank+rand() % 100;
			
		}
		cout << endl;
		//create 5 groups of proscces
		MPI_Group_incl(base_grp, 5, ringRanks, &ringGroup);
		//MPI_Group_excl(base_grp, 5, ringRanks, &masterSlaveGroup);
		//cоздание нового коммуникаторв с одной группе
		MPI_Comm_create(MPI_COMM_WORLD, ringGroup, &ringComm);
		//MPI_Comm_create(MPI_COMM_WORLD, masterSlaveGroup, &masterSlaveComm);

		if (MPI_COMM_NULL != ringComm) {
			MPI_Comm_rank(ringComm, &NewRingRank);
			MPI_Comm_size(ringComm, &RingCount);
		}
		//if (MPI_COMM_NULL != masterSlaveComm) {
		//	MPI_Comm_rank(masterSlaveComm, &MasterSlaveRank);
		//	MPI_Comm_size(masterSlaveComm, &MasterSlaveCount);

		//}


		//wait

		if (MPI_COMM_NULL != ringComm) {
			int source, dest;
			MPI_Comm_rank(ringComm, &NewRingRank);
			MPI_Request req;
			int dims[1], periods[1], reorder = 1;
			dims[0] = 0;
			periods[0] = 1;
			MPI_Dims_create(5, 1, dims);
			MPI_Cart_create(ringComm, 1, dims, periods, reorder, &RingGridComm);
			/*cout << "I here " << NewRingRank << endl;

			MPI_Comm_rank(RingGridComm, &RingGridRank);
			MPI_Comm_size(RingGridComm, &RingGridCount);
			cout << "I here " << RingGridRank << endl;
			cout << "process: " << RingGridRank << " sent array: ";
			for (int i = 0; i < N; i++) {
				cout << array[i] << " ";
			}
			MPI_Sendrecv(&array[0], N, MPI_INT, (RingGridRank + 1) % 5, 0, &answer[0], N, MPI_INT, RingGridRank, 0, RingGridComm, &Status);
			cout << "process: " << RingGridRank << " GET array: ";
			for (int i = 0; i < N; i++) {
				cout << answer[i] << " ";
			}
			*/
			cout << "Process: " << NewRingRank << " send  array: ";
			for (int i = 0; i < N; i++) {
				cout << array[i] << " ";
			}
			cout << endl;
			MPI_Cart_shift(RingGridComm, 0, 1, &source, &dest);
			MPI_Sendrecv_replace(array, N, MPI_INT, dest, 12, source, 12, RingGridComm, &Status);
			cout << "Process: " << NewRingRank << " got array: ";
			for (int i = 0; i < N; i++) {
				cout << array[i] << " ";
			}
		
			MPI_Comm_free(&RingGridComm);
		}







	
	delete[] array, answer;
	MPI_Group_free(&base_grp);
	MPI_Group_free(&ringGroup);
	//MPI_Group_free(&masterSlaveGroup);
	MPI_Comm_free(&ringComm);

	//MPI_Comm_free(&RingGridComm);
	//MPI_Comm_free(&masterSlaveComm);
	//MPI_Comm_free(&MasterSlaveGridComm);
	MPI_Finalize();

}
	return 0;
}

