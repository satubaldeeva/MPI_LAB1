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
	
	int RingCount, myRank, NewRingRank, MasterSlaveRank, MasterSlaveCount;
	int* array = new int[N];
	int count = 0;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &RingCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	if (RingCount == 10) {
		srand(time(NULL));
		//Access to default group
		MPI_Group base_grp, ringGroup, masterSlaveGroup;
		MPI_Comm ringComm, masterSlaveComm;
		//основная группа
		MPI_Comm_group(MPI_COMM_WORLD, &base_grp);

		//массивы для передачи значений рангов
		int ringRanks[5] = { 0, 1, 2, 3, 4 };
		//int masterSlaveRanks[5] = { 5, 6, 7, 8, 9 };

		//каждый процесс инициализирует свой массив на 10 элементов
		for (int i = 0; i < N; i++) {
			array[i] = myRank + rand() % 100;

		}

		//create 5 groups of proscces
		MPI_Group_incl(base_grp, 5, ringRanks, &ringGroup);
		MPI_Group_excl(base_grp, 5, ringRanks, &masterSlaveGroup);
		//cоздание нового коммуникаторв с одной группе
		MPI_Comm_create(MPI_COMM_WORLD, ringGroup, &ringComm);
		MPI_Comm_create(MPI_COMM_WORLD, masterSlaveGroup, &masterSlaveComm);


		if (MPI_COMM_NULL != ringComm) {
			int source, dest;
			MPI_Comm_rank(ringComm, &NewRingRank);
			MPI_Comm RingGridComm;
			int dims[1], periods[1], reorder = 1;
			dims[0] = 0;
			periods[0] = 1;
			MPI_Dims_create(5, 1, dims);
			MPI_Cart_create(ringComm, 1, dims, periods, reorder, &RingGridComm);
			cout << "Process: " << NewRingRank << " send  array: ";
			for (int i = 0; i < N; i++) {
				cout << array[i] << " ";
			}
			cout << endl;
			MPI_Cart_shift(RingGridComm, 0, 1, &source, &dest);

			MPI_Barrier(RingGridComm);
			MPI_Sendrecv_replace(array, N, MPI_INT, dest, 12, source, 12, RingGridComm, &Status);
			cout << "Process: " << NewRingRank << " got array: ";
			for (int i = 0; i < N; i++) {
				cout << array[i] << " ";
			}
			cout << endl;


			MPI_Comm_free(&RingGridComm);

		}

		if (MPI_COMM_NULL != masterSlaveComm) {
			MPI_Comm_rank(masterSlaveComm, &MasterSlaveRank);
			MPI_Comm_size(masterSlaveComm, &MasterSlaveCount);
			MPI_Comm MasterSlaveGridComm;
			int index[] = { 4,5,6,7,8 };
			int edges[] = { 1,2,3,4,0,0,0,0 };
			MPI_Graph_create(masterSlaveComm, 5, index, edges, 1, &MasterSlaveGridComm);
			int n, neigboars[4], neigboarsForMaster[4];
			MPI_Graph_neighbors_count(MasterSlaveGridComm, MasterSlaveRank, &n);

			MPI_Graph_neighbors(MasterSlaveGridComm, MasterSlaveRank, n, &neigboars[0]);
			//cout << MasterSlaveRank << "->" << n<<":"<<neigboars[0]<<" "<<neigboars[1]<<endl;
			const int size = N;
			int result[size];
			//MPI_Gather(&array[0], N, MPI_INT, result, N, MPI_INT, 0, MasterSlaveGridComm);
			//MPI_Sendrecv(&array[0], N, MPI_INT, neighboars[i], 1,&rank1, 1, MPI_INT, neighbors[i], 1, comm_graph,&status);
			if (MasterSlaveRank == 0) {
				MPI_Graph_neighbors(MasterSlaveGridComm, MasterSlaveRank, n, &neigboarsForMaster[0]);

				for (int i = 0; i < n; i++) {
					//cout << MasterSlaveRank << "neig" << neigboars[i] << endl;
					MPI_Send(&array[0], N, MPI_INT, neigboarsForMaster[i], 1, MasterSlaveGridComm);
					
				}
			}
			cout << endl;
			if (MasterSlaveRank != 0) {
				MPI_Recv(&result[0], N, MPI_INT, neigboars[0], 1, MasterSlaveGridComm, &Status);
				cout << "Slaves(" << MasterSlaveRank << ")got such values from master :" << endl;
				for (int i = 0; i < size; i++) {
					cout << result[i] << " ";
				}
				cout << endl;
			}
				cout << endl;
			MPI_Comm_free(&MasterSlaveGridComm);
		}
		
	MPI_Group_free(&masterSlaveGroup);
	MPI_Comm_free(&masterSlaveComm);
	MPI_Comm_free(&ringComm);
	MPI_Group_free(&ringGroup);
	MPI_Group_free(&base_grp);
	
	//MPI_Finalize();
	}
	delete[] array, answer;
	MPI_Finalize();
	return 0;
}

