// ѕроиллюстрировать возможности коллективных взаимодействий процессов MPI на примере следующей задачи. ѕроцесс 0 
//обновл€ет значение некоторой переменной вещественного типа и широковещательным образом рассылает обновленное 
//значение всем процессам в коммуникаторе. аждый процесс прибавл€ет к полученному значению свой номер, 
// //после чего данные от всех процессов собираютс€ обратно в процессе 0.
//

//MPI_BCAST_GATHER.cpp
#include "mpi.h"
#include <iostream>
using namespace std;
int main(int argc, char** argv)
{
	double a = 0;
	MPI_Init(NULL, NULL);
	int Size;
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	int Rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	if (Rank == 0)
		a = 0.3;
	//"Ўироковещательна€" рассылка сообщени€
	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	if (Rank == 0) {
		double* A = new double[Size];
		MPI_Gather(&a, 1, MPI_DOUBLE, A, 1, MPI_DOUBLE, 0,
			MPI_COMM_WORLD);
		for (int k = 0; k < Size; k++)
			cout << A[k] << endl;
		delete[] A;
	}
	else {
		double b = a + Rank;
		MPI_Gather(&b, 1, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0,
			MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
