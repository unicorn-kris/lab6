// “о же самое, что в задании 36, но с ожиданием ответа от процесса, прин€вшего сообщение. 
//»спользовать блокирующие посылку и прием сообщений
//

//MPI_Send_Recv_Block-c-Bind.cpp
#include <mpi.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv)
{
	setlocale(LC_ALL, ".ACP");
	double a = 0, b = 0;
	MPI_Init(NULL, NULL);
	int Size;
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	int Rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	if (Rank == 0) {//Ѕлокирующа€ посылка сообщени€
		a = 7.7;
		MPI_Send(&a, 1, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD);
		MPI_Status Status;
		MPI_Recv(&b, 1, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD,
			&Status);
	}
	if (Rank == 1) {//Ѕлокирующий прием сообщени€
		b = 3.3;
		MPI_Status Status;
		MPI_Recv(&a, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD,
			&Status);
		MPI_Send(&b, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
	}
	cout << "Process " << Rank << " a=" << a << " b=" << b << endl;
	MPI_Finalize();
	return 0;
}
