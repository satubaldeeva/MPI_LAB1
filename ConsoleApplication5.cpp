#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[])
{

	const size_t SIZE_GENERATE = 3;
	const size_t SIZE = 17; //произведение 8 полиномов третьей степени не превышает 17 степени

	int ProcCount, myRank, myRank1;
	int sendarray[SIZE], temp[SIZE];
	int* result;
	int count = 0;

	double dt = 0;

	MPI_Status Status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (myRank == 0)
		dt -= MPI_Wtime();

	// Каждый процесс генерирует массив полиномов
	for (int i = 0; i < SIZE; i++)
		sendarray[i] = 0;

	for (int i = 0; i < SIZE_GENERATE; i++)
		sendarray[i] = (myRank + rand()) % 6 - 3;

	// Вывод изначальных полиномов для умножения
	if (myRank == 0)
		cout << "Polinoms:\n";
	MPI_Barrier(MPI_COMM_WORLD);
	for (auto i = 0u; i < SIZE; i++)
		cout << sendarray[i] << " ";
	cout << endl;
	cout.flush();

	// Динамически выделяем память под массивы
	result = new int[SIZE];
	for (auto i = 0u; i < SIZE; i++)
		result[i] = 0;
	// Новый тип type
	MPI_Datatype type;
	MPI_Type_contiguous(SIZE, MPI_INT, &type);
	MPI_Type_commit(&type);

	// четные процессы посылают свои массивы нечетным
	if ((myRank % 2) == 0)
		MPI_Send(sendarray, 1, type, myRank + 1, 0, MPI_COMM_WORLD);
	else
	{

		MPI_Recv(temp, 1, type, myRank - 1, 0, MPI_COMM_WORLD, &Status);
		for (auto i = 0u; i < SIZE_GENERATE; i++)
			for (auto j = 0u; j < SIZE_GENERATE; j++)
				result[i + j] += sendarray[i] * temp[j];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if ((myRank % 4) == 1)
		MPI_Send(result, 1, type, myRank + 2, 0, MPI_COMM_WORLD);
	else if ((myRank % 4) == 3)
	{
		MPI_Recv(temp, 1, type, myRank - 2, 0, MPI_COMM_WORLD, &Status);
		for (auto i = 0u; i < SIZE; i++)
			sendarray[i] = 0;
		for (auto i = 0u; i < SIZE_GENERATE * 2; i++)
			for (auto j = 0u; j < SIZE_GENERATE * 2; j++)
				sendarray[i + j] += result[i] * temp[j];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if ((myRank % 8) == 3)
	{
		MPI_Send(sendarray, 1, type, myRank + 4, 0, MPI_COMM_WORLD);
	}
	else if ((myRank % 8) == 7)
	{
		MPI_Recv(temp, 1, type, myRank - 4, 0, MPI_COMM_WORLD, &Status);
		for (auto i = 0u; i < SIZE; i++)
			result[i] = 0;
		for (auto i = 0u; i < SIZE; i++)
			for (auto j = 0u; j < SIZE; j++)
				result[i + j] += sendarray[i] * temp[j];
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if ((myRank % 8) == 7)
		MPI_Send(result, 1, type, 0, 0, MPI_COMM_WORLD);
	else if ((myRank == 0))
	{
		MPI_Recv(result, 1, type, 7, 0, MPI_COMM_WORLD, &Status);
		cout << "Result\n";
		for (auto i = 0u; i < SIZE; i++)
			cout << result[i] << " ";
		dt += MPI_Wtime();
		cout << "\nTime spent: " << dt * 1000 << "ms\n";
		cout.flush();
	}

	delete[] result;
	MPI_Type_free(&type);
	MPI_Finalize();

	return 0;
}