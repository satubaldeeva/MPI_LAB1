#include <iostream>
#include "mpi.h"
#include <vector>
#include <algorithm>

using namespace std;

vector<int> compareVector(vector<int> a, vector<int> b) {
	vector<int> res;
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

}

/*int main(int argc, char* argv[])
{
	const int N = 15000;
	const int sizeGroup = 1000;


	int ProcCount, myRank, myRank1;
	vector<int> array;
	
	int* result;
	int count = 0;

	double dt = 0;

	MPI_Status Status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	//Access to default group
	MPI_Group base_grp, gr1, gr2, gr3, gr4, gr5;
	MPI_Comm newcomm1, newcomm2, newcomm3, newcomm4, newcomm5;
	//default_group
	MPI_Comm_group(MPI_COMM_WORLD, &base_grp);
	//массивы для передачи значений рангов
	int rank1[] = { 1, 2,  3 };
	int rank2[] = { 4, 5,  6 };
	int rank3[] = { 7, 8,  9 };
	int rank4[] = { 10, 11,  12 };
	int rank5[] = { 13, 14,  15 };
	if (myRank == 0)
			dt -= MPI_Wtime();
		//init vector
	if (myRank == 0) {
		for (int i = 0; i < N; i++)
			array[i] = rand() * 100;
	}
	//create 5 groups of proscces
	MPI_Group_incl(base_grp, 3, rank1, &gr1);
	MPI_Group_incl(base_grp, 3, rank2, &gr2);
	MPI_Group_incl(base_grp, 3, rank3, &gr3);
	MPI_Group_incl(base_grp, 3, rank4, &gr4);
	MPI_Group_incl(base_grp, 3, rank5, &gr5);
	//cоздание ноыого коммуникаторв с одной группе
	MPI_Comm_create(MPI_COMM_WORLD, gr1, &newcomm1);
	MPI_Comm_create(MPI_COMM_WORLD, gr2, &newcomm2);
	MPI_Comm_create(MPI_COMM_WORLD, gr3, &newcomm3);
	MPI_Comm_create(MPI_COMM_WORLD, gr4, &newcomm4);
	MPI_Comm_create(MPI_COMM_WORLD, gr5, &newcomm5);
	
	//wait
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Datatype type;
	MPI_Type_contiguous(sizeGroup, MPI_INT, &type);
	MPI_Type_commit(&type);
	
	
	if (myRank == 0) {
		//send data to newcomm1's groups
		vector<int> v(sizeGroup);

		for (int i = 0; i < 3; i++) {
			
			copy(array[i*sizeGroup], array[(i+1)*sizeGroup - 1], v.begin());
			MPI_Send(&v, 1, type, i+1, 0, newcomm1);
			v.clear();
		}
		for (int i = 3; i < 6; i++) {

			copy(array[i * sizeGroup], array[(i + 1) * sizeGroup - 1], v.begin());
			MPI_Send(&v, 1, type, i + 1, 0, newcomm2);
			v.clear();
		}
		for (int i = 6; i < 9; i++) {

			copy(array[i * sizeGroup], array[(i + 1) * sizeGroup - 1], v.begin());
			MPI_Send(&v, 1, type, i + 1, 0, newcomm3);
			v.clear();
		}
		for (int i = 9; i < 12; i++) {

			copy(array[i * sizeGroup], array[(i + 1) * sizeGroup - 1], v.begin());
			MPI_Send(&v, 1, type, i + 1, 0, newcomm4);
			v.clear();
		}
		for (int i = 12; i < 15; i++) {

			copy(array[i * sizeGroup], array[(i + 1) * sizeGroup - 1], v.begin());
			MPI_Send(&v, 1, type, i + 1, 0, newcomm5);
			v.clear();
		}


	}
	vector<int> temp;
	for (int i = 1; i < 4; i++) {
		
		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm1,&Status);
			sort(temp.begin(), temp.end());
		}
		temp.clear();
	}
	for (int i = 4; i < 7; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm2, &Status);
			sort(temp.begin(), temp.end());
		}
		temp.clear();

	}
	for (int i = 7; i < 10; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm3, &Status);
			sort(temp.begin(), temp.end());
		}
		temp.clear();
	}
	for (int i = 10; i < 13; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm4, &Status);
			sort(temp.begin(), temp.end());
		}
		temp.clear();
	}
	for (int i = 13; i < 16; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm5, &Status);
			sort(temp.begin(), temp.end());
		}
		temp.clear();
	}
	

	MPI_Type_free(&type);
	MPI_Finalize();

	return 0;
}*/


int main(int argc, char* argv[]) {
	vector<int> a = { 1,3,5,12,45,53,66 };
	vector<int> b = { 6,7,9,34,53,69,133 };
	vector<int> res = compareVector(a, b);
	for (int i = 0; i < res.size(); i++) {
		cout << res[i] << " ";
	}

}