#include <algorithm>
#include <iostream>
#include "mpi.h"
#include <vector>


using namespace std;

int* insertionSort(int *list, int listLength)
{
	for (int i = 1; i < listLength; i++)
	{
		int j = i - 1;
		while (j >= 0 && list[j] > list[j + 1])
		{
			swap(list[j], list[j + 1]);
			j--;
		}
	}
	return  list;
}
int* compareVector(int * a, int * b,int size1,int size2) {
	/*vector<int> res;
	int size = a.size() + b.size();
	cout << size << endl;
	while (res.size() !=size ) {
		if (!a.empty() && !b.empty()) {
			if (a[0] < b[0]) {
				res.push_back(a[0]);
				a.erase(a.begin());
			}
			else {
				res.push_back(b[0]);
				b.erase(b.begin());
			}
		}
		else if(a.empty()) {
			res.push_back(b[0]);
			;
			if (b.size() != 1) {
				b.erase(b.begin());
			}
			

		}
		else if (b.empty()) {
			res.push_back(a[0]);
			if (a.size() != 1) {
				a.erase(a.begin());
			}
			
		}
		cout << res.size() <<  endl;
		
	}
	return res;
	*/

	int* result = new int[size1 + size2];
	int finallSize;int j = 0;
	for (int i = 0; i < size1; i++) {
		result[i] = a[i];
	}
	for (int i = size1; i < size1 + size2; i++) {
		
		result[i] = b[j];
		j++;

	}
	
	/*
	if (size1 <= size2) finallSize = size1;
	else  finallSize = size2;
	cout << 121 << endl;
		int j = 0;
		int forB = 0;
	for (int i = 0; i < finallSize; i++) {
		
		if (a[i] < b[forB]) {
			//cout << a[i]<< " ";
			result[j] = a[i];
			j++;
			
		}
		else {
			result[j] = b[forB];
			//cout << b[forB]<<" ";
			j++;
			forB++;
			i--;
			
		}

	}
	//cout << 4545 << endl;
	if (size1 < size2) {
		for (int i = size1; i < size2; i++) {
			result[i] = b[i];
		}
	}
	else if (size2 < size1) {
		for (int i = size2; i < size1; i++) {
			result[i] = a[i];
		}

	}*/
	cout << endl;
	result = insertionSort(result, size1+size2);
	
	return result;
}
vector<int> createVector(int* list,int size) {
	vector<int>v;
	for (int i = 0; i < size; i++) {
		v.push_back(list[i]);
	}
	return v;
}
int main(int argc, char* argv[])
{
	const int N = 15000;
	const int sizeGroup = 1000;
	int* answer, * result1, * result2= new int[N];
	int* DataThirdSend = new int[4 * sizeGroup];
	int* temp = new int[sizeGroup];
	int* DataFirstSend = new int[sizeGroup];
    int *  ResSecondSend= new int[4 * sizeGroup];
	int* ResSecondSend7000 = new int[7* sizeGroup];
	int* ResSecondSendFor14 = new int[3 * sizeGroup];
	int *ResThirdSend = new int[8 * sizeGroup];
	int* ResultSorting15000 = new int[N];
	int* DataFourthSend = new int[8 * sizeGroup];
	int* ResThirdSend7000 = new int[7 * sizeGroup];
	int * ResFirstSend = new int[2 * sizeGroup];
	int * DataSecondSend = new int[2*sizeGroup];
	for (int i = 0; i < 2 * sizeGroup; i++) {
		ResFirstSend[i] = 0;
	}
	for (int i = 0; i < 2 * sizeGroup; i++) {
		DataSecondSend[i] = 0;
	}
	int ProcCount, myRank,NewProcRank;
	int* array = new int[N];
	int count = 0;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	if (ProcCount == 15) {
		//Access to default group
		MPI_Group base_grp, newgroup;
		MPI_Comm newcomm1;

		//default_group
		MPI_Comm_group(MPI_COMM_WORLD, &base_grp);
		cout << "MyRank :" << myRank << endl;
		//массивы для передачи значений рангов
		int ranks[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12 , 13, 14 };
		int* ranks_excl = new int[1];
		ranks_excl[0] = 0;

		//init vector

		if (myRank == 0) {
			for (int i = 0; i < N; i++) {
				array[i]=rand() % 100;
				
			}

		}
		//create 5 groups of proscces
		MPI_Group_incl(base_grp, 15, ranks, &newgroup);

		//cоздание ноыого коммуникаторв с одной группе
		MPI_Comm_create(MPI_COMM_WORLD, newgroup, &newcomm1);

		if (MPI_COMM_NULL != newcomm1) {
			MPI_Comm_rank(newcomm1, &NewProcRank);
			MPI_Comm_size(newcomm1, &ProcCount);
		}
		cout << "\nNewProcRank:" << NewProcRank << endl;
		cout << "\nNewProcCount:" << ProcCount << endl;

		//wait
		MPI_Barrier(newcomm1);
		//1000
		MPI_Datatype TYPE1000;
		MPI_Type_contiguous(sizeGroup, MPI_INT, &TYPE1000);
		MPI_Type_commit(&TYPE1000);
		//3000
		MPI_Datatype TYPE2000;
		MPI_Type_contiguous(sizeGroup * 2, MPI_INT, &TYPE2000);
		MPI_Type_commit(&TYPE2000);
		//6000
		MPI_Datatype TYPE4000;
		MPI_Type_contiguous(sizeGroup * 4, MPI_INT, &TYPE4000);
		MPI_Type_commit(&TYPE4000);
		//7000
		MPI_Datatype TYPE7000;
		MPI_Type_contiguous(sizeGroup * 7, MPI_INT, &TYPE7000);
		MPI_Type_commit(&TYPE7000);
		//8000
		MPI_Datatype TYPE8000;
		MPI_Type_contiguous(sizeGroup * 8, MPI_INT, &TYPE8000);
		MPI_Type_commit(&TYPE8000);
		MPI_Scatter(&array[0], 1, TYPE1000, &temp[0], 1, TYPE1000, 0, newcomm1);
		temp = insertionSort(temp, sizeGroup);
		if (NewProcRank == 1) cout << "First sending: \n";
		//1
		if (NewProcRank % 2 == 0 && NewProcRank != 14) {
			MPI_Send(&temp[0], 1, TYPE1000, NewProcRank + 1, 0, newcomm1);
			cout <<"1." << NewProcRank << " send " << NewProcRank +1 << endl;		
		}
		else if (NewProcRank % 2 == 1) {
			MPI_Recv(&DataFirstSend[0], 1, TYPE1000,  NewProcRank - 1,0, newcomm1, &Status);
			cout << "1." << NewProcRank << " got " << NewProcRank - 1 << endl;
			ResFirstSend = compareVector(DataFirstSend,temp,1000,1000);
			
		
		}
		
		MPI_Barrier(newcomm1);
		//2
		if (NewProcRank == 1) cout << "Second sending: \n";
		if (NewProcRank % 4 == 1 && NewProcRank != 13) { //from 1 - 3, from 5 - 7, from 9 to 11
			MPI_Send(&ResFirstSend[0], 1, TYPE2000, NewProcRank + 2, 0, newcomm1);
			cout << "2." << NewProcRank << " send " << NewProcRank +2 << endl;
		}
		if (NewProcRank == 13) {
			MPI_Send(&ResFirstSend[0], 1, TYPE2000, NewProcRank + 1, 0, newcomm1);
			cout << "2.13 send 14!" << endl;
		}
		if (NewProcRank % 4 == 3) {
			MPI_Recv(&DataSecondSend[0], 1, TYPE2000,  NewProcRank - 2, 0, newcomm1, &Status);
			ResSecondSend = compareVector(DataSecondSend, ResFirstSend, 2000, 2000);
			cout << "2.Sorted array :" << endl;
			
			cout << "2." << NewProcRank<< " got "<< NewProcRank-2 << endl;
		}
		if(NewProcRank==14) {//14 p
			MPI_Recv(&DataSecondSend[0], 1, TYPE2000,  13, 0, newcomm1, &Status);
			ResSecondSendFor14 = compareVector(DataSecondSend, temp, 2000, 1000);
			cout << "2.Sorted array :" << endl;
			for (int i = 0; i < 3000; i++) {
				cout << ResSecondSendFor14[i] << " ";
			}
			cout << "2." << "14 got 13 !" << endl;
		}
	
		MPI_Barrier(newcomm1);

		if (NewProcRank == 1) cout << "3 sending: \n";
		//3
		if (NewProcRank == 3) { //from 3 -7 
			MPI_Send(&ResSecondSend[0], 1, TYPE4000, NewProcRank + 4, 0, newcomm1);
			cout << "3." << NewProcRank << " send " << NewProcRank +4 << endl;
		}
		if (NewProcRank == 7) { //from 7 - 14
			MPI_Recv(&DataThirdSend[0], 1, TYPE4000,  NewProcRank - 4, 0,newcomm1, &Status);
			ResThirdSend = compareVector(DataThirdSend, ResSecondSend,4000,4000);
			
			cout << "3." << NewProcRank<< " got "<< NewProcRank-4 << endl;
			MPI_Send(&ResThirdSend[0], 1, TYPE8000, 14, 0, newcomm1);
			cout << "3." << NewProcRank << " send " << 14<< endl;
		}
		if (NewProcRank == 11) { //from 11 - 14
			MPI_Send(&ResSecondSend[0], 1, TYPE4000, NewProcRank + 3, 0, newcomm1);
			cout << "3." << NewProcRank << " send " << NewProcRank +3 << endl;
		}
		if (NewProcRank == 14) {

			MPI_Recv(&DataThirdSend[0], 1, TYPE4000,  NewProcRank - 3, 0,newcomm1, &Status);
			ResThirdSend7000 = compareVector(DataThirdSend,ResSecondSendFor14,4000,3000);
			cout << "3." << NewProcRank<< " got "<< NewProcRank-3 << endl;
			
			MPI_Recv(&DataFourthSend[0], 1, TYPE8000,  NewProcRank - 7, 0,newcomm1, &Status);
			cout << "3." << NewProcRank<< " got "<< NewProcRank-7 << endl;
			ResultSorting15000= compareVector(DataFourthSend,ResThirdSend7000,8000,7000);
			cout << "3.Sorted array :" << endl;
			for (int i = 0; i < 15000; i++) {
				cout << ResultSorting15000[i] << " ";
			}
			//cout << "Sorted array :" << endl;
			//for (int i = 0; i < 10000; i++) {
				//cout << ResultSorting15000[i] << " ";
			//}
		}
		
		delete[] array, temp, answer, result1, result2, ResSecondSend, ResSecondSend7000, ResultSorting15000, ResThirdSend, ResThirdSend7000,DataFourthSend,DataSecondSend, DataThirdSend, DataFirstSend, ResFirstSend;
		MPI_Group_free(&base_grp);
		MPI_Group_free(&newgroup);
		MPI_Comm_free(&newcomm1);
		MPI_Type_free(&TYPE1000);
		MPI_Type_free(&TYPE2000);
		MPI_Type_free(&TYPE4000);
		MPI_Type_free(&TYPE7000);
		MPI_Type_free(&TYPE8000);
		MPI_Finalize();
		
	}
	

	return 0;
}




/*int main(int argc, char* argv[]) {
	MPI_Status Status;
	MPI_Init(&argc, &argv); int ProcCount, myRank, NewProcRank;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	int* a = new int[8]{ 2,6,10,12,12,22,44,66 };
	int* b = new int[5]{ 1,2,3,4,5 };
	cout << "dfd" << endl;
	int* res = compareVector(a, b, 8, 5);
	for (int i = 0; i <5+8; i++) {
		cout<<res[i]<<" ";
	}
	MPI_Finalize();
	delete[]a, b, res;


return 0;
}*/
