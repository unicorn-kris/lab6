// Стандартными средствами MPI создать коммуникатор с виртуальной топологией графа для взаимодействия процессов по
//схеме «ведущий процесс – ведомые процессы» таким образом, чтобы все процессы в 
//рамках данной топологии обменивались сообщениями лишь с нулевым
//процессом.Разрешить перенумерацию процессов при создании нового
//коммуникатора.

//
//MPI_Graph-c-Bind.cpp
#include <mpi.h>
#include <iostream>

using namespace std;
int main(int argc, char** argv)
{
	MPI_Init(NULL, NULL);
	int Size;
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	int* Index = new int[Size];
	for (int k = 0; k < Size; k++)
		Index[k] = Size - 1 + k;
	int* Edges = new int[2 * Size - 2];
	for (int k = 0; k < Size - 1; k++) {
		Edges[k] = k + 1;
		Edges[Size - 1 + k] = 0;
	}
	MPI_Comm Gr_Comm;
	MPI_Graph_create(MPI_COMM_WORLD, Size, Index, Edges, true,
		&Gr_Comm);
	int Rank;
	MPI_Comm_rank(Gr_Comm, &Rank);
	int* Neighbors = new int[Size - 1];
	int Num;
	MPI_Graph_neighbors_count(Gr_Comm, Rank, &Num);
	MPI_Graph_neighbors(Gr_Comm, Rank, Num, Neighbors);
	for (int k = 0; k < Num; k++) {
		int Rank1;
		MPI_Status St;
		MPI_Sendrecv(&Rank, 1, MPI_INT, Neighbors[k], 5,
			&Rank1, 1, MPI_INT, Neighbors[k], 5, Gr_Comm, &St);
		cout << "Process " << Rank << " co-operates with process " << Rank1 << endl;
	}
	MPI_Comm_free(&Gr_Comm);
	delete[] Neighbors;
	delete[] Edges;
	delete[] Index;
	MPI_Finalize();
	return 0;
}
