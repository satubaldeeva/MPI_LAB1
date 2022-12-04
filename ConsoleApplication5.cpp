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

int main(int argc, char* argv[])
{
	const int N = 15000;
	const int sizeGroup = 1000;


	int ProcCount, myRank, myRank1;
	vector<int> array;
	
	
	int count = 0;

	double dt = 0;

	MPI_Status Status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	//Access to default group
	MPI_Group base_grp, gr1, gr2, gr3, gr4, gr5;
	MPI_Comm newcomm1, newcomm2, newcomm3, newcomm4, newcomm5,inter_comm12, inter_comm34;
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
		for (int i = 0; i < N; i++) {
			array[i] = rand() * 100;
			cout << array[i] << " ";
		}
		
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
	//1000
	MPI_Datatype type;
	MPI_Type_contiguous(sizeGroup, MPI_INT, &type);
	MPI_Type_commit(&type);
	//3000
	MPI_Datatype type1;
	MPI_Type_contiguous(sizeGroup*3, MPI_INT, &type1);
	MPI_Type_commit(&type1);
	//6000
	MPI_Datatype type2;
	MPI_Type_contiguous(sizeGroup * 6, MPI_INT, &type2);
	MPI_Type_commit(&type2);
	
	
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
	vector<int> temp;vector<int> temp13, temp123, temp23,temp13_14_15, temp13_15, temp14_15; vector<int> temp46, temp56,temp789, temp79, temp89; vector<int> temp10_12, temp11_12;
	//sort and send in the group
	for (int i = 1; i < 4; i++) {
		
		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm1,&Status);
			sort(temp.begin(), temp.end());
		}
		if (myRank == 1) {
			MPI_Send(&temp, 1, type, myRank+2, 0, newcomm1);
		}
		if (myRank == 2) {
			MPI_Send(&temp, 1, type, myRank + 1, 0, newcomm1);
		}
		if (myRank == 3) {
			
			MPI_Recv(&temp13, 1, type, myRank-2, 0, newcomm1, &Status);
			MPI_Recv(&temp23, 1, type, myRank-1, 0, newcomm1, &Status);
			vector<int>temp12 = compareVector(temp13, temp23);
			temp123 = compareVector(temp12, temp);
		}	
	}
	for (int i = 4; i < 7; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm2, &Status);
			sort(temp.begin(), temp.end());
		}
		if (myRank == 4) {
			MPI_Send(&temp, 1, type, myRank + 2, 0, newcomm1);
		}
		if (myRank == 5) {
			MPI_Send(&temp, 1, type, myRank + 1, 0, newcomm1);
		}
		if (myRank == 6) {
			
			MPI_Recv(&temp46, 1, type, myRank - 2, 0, newcomm1, &Status);
			MPI_Recv(&temp56, 1, type, myRank - 1, 0, newcomm1, &Status);
			vector<int>temp45 = compareVector(temp46, temp56);
			vector<int>temp456 = compareVector(temp45, temp);
		}
	}
	for (int i = 7; i < 10; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm3, &Status);
			sort(temp.begin(), temp.end());
		}
		if (myRank == 7) {
			MPI_Send(&temp, 1, type, myRank + 2, 0, newcomm1);
		}
		if (myRank == 8) {
			MPI_Send(&temp, 1, type, myRank + 1, 0, newcomm1);
		}
		if (myRank == 9) {
			
			MPI_Recv(&temp79, 1, type, myRank - 2, 0, newcomm1, &Status);
			MPI_Recv(&temp89, 1, type, myRank - 1, 0, newcomm1, &Status);
			vector<int>temp78 = compareVector(temp79, temp89);
			temp789 = compareVector(temp78, temp);
		}
		
	}
	for (int i = 10; i < 13; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm4, &Status);
			sort(temp.begin(), temp.end());
		}
		if (myRank == 10) {
			MPI_Send(&temp, 1, type, myRank + 2, 0, newcomm1);
		}
		if (myRank == 11) {
			MPI_Send(&temp, 1, type, myRank + 1, 0, newcomm1);
		}
		if (myRank == 12) {
			
			MPI_Recv(&temp10_12, 1, type, myRank - 2, 0, newcomm1, &Status);
			MPI_Recv(&temp11_12, 1, type, myRank - 1, 0, newcomm1, &Status);
			vector<int>temp10_11 = compareVector(temp11_12, temp10_12);
			vector<int>temp10_11_12 = compareVector(temp10_11, temp);
		}
		
	}
	for (int i = 13; i < 16; i++) {

		if (myRank == i) {
			MPI_Recv(&temp, 1, type, 0, 0, newcomm5, &Status);
			sort(temp.begin(), temp.end());
		}
		if (myRank == 13) {
			MPI_Send(&temp, 1, type, myRank + 2, 0, newcomm1);
		}
		if (myRank == 14) {
			MPI_Send(&temp, 1, type, myRank + 1, 0, newcomm1);
		}
		if (myRank == 15) {
			
			MPI_Recv(&temp13_15, 1, type, myRank - 2, 0, newcomm1, &Status);
			MPI_Recv(&temp14_15, 1, type, myRank - 1, 0, newcomm1, &Status);
			vector<int>temp13_14 = compareVector(temp13_15, temp14_15);
			temp13_14_15 = compareVector(temp13_14, temp);
		}
		
	}


	//wait
	MPI_Barrier(newcomm1);
	MPI_Barrier(newcomm2);
	MPI_Barrier(newcomm3);
	MPI_Barrier(newcomm4);
	MPI_Barrier(newcomm5);

	MPI_Intercomm_create(newcomm1, 3, newcomm2,6, 0, &inter_comm12);
	MPI_Intercomm_create(newcomm1, 3, newcomm2, 6, 0, &inter_comm34);
	vector<int> dataFromGroup1,dataFromGroup3;
	vector<int> resGroup12,resGroup34;

	//TO 3-6
	if (myRank == 3) {
		MPI_Send(&temp123, 1,type2, 6, 0, inter_comm12);
	}
	if (myRank == 6) {
		MPI_Recv( &dataFromGroup1,	1,type1,3,0,inter_comm12, &Status);
		resGroup12 = compareVector(temp, dataFromGroup1);
		MPI_Send(&resGroup12, 1, type2, 0, 1, MPI_COMM_WORLD);

	}
	//TO 9-12
	if (myRank == 9) {
		MPI_Send(&temp789, 1, type2, 12, 0, inter_comm34);
	}
	if (myRank == 12) {
		MPI_Recv(&dataFromGroup3, 1, type1, 9, 0, inter_comm34, &Status);
		resGroup34 = compareVector(temp, dataFromGroup3);
		MPI_Send(&resGroup34, 1, type2, 0, 1, MPI_COMM_WORLD);
	}
	//to15-0
	if (myRank == 15) {
		MPI_Send(&temp13_14_15, 1, type2, 0, 1, MPI_COMM_WORLD);
	}

	//receive from 6,15,12
	vector<int> dataFromGroup12, dataFromGroup34, dataFromGroup5, result;
	
	
	if (myRank == 0) {
		MPI_Recv(&dataFromGroup12, 1, type2, 6, 1, MPI_COMM_WORLD, &Status);
		MPI_Recv(&dataFromGroup34, 1, type2, 9, 1, MPI_COMM_WORLD, &Status);
		MPI_Recv(&dataFromGroup5, 1, type, 15, 1, MPI_COMM_WORLD, &Status);

		result = compareVector(compareVector(dataFromGroup12, dataFromGroup34),dataFromGroup5);
		cout << endl;
		for (int i = 0; i < 100; i++) {
			cout << result[i] << " ";
		}

	}
	
	
	

	

	MPI_Type_free(&type);
	MPI_Type_free(&type1);
	MPI_Type_free(&type2);
	MPI_Finalize();

	return 0;
}


