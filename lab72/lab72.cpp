// Написать программу, в которой происходит обмен информацией 
//между процессами с соседними номерами в соответствии с топологией
//кольца.Использовать неблокирующую буферизованную передачу сообщений, блокирующий прием сообщений и функции присоединения и
//отсоединения буфера для передачи сообщений.


//MPI_Cycl_Send_Recv_Buff-c-Bind.cpp
#include "mpi.h"
#include <iostream>

using namespace std;
int main(int argc, char** argv)
{
	double a = 0, b = 0, c = 0;
	int const BufSize = sizeof(double) + MPI_BSEND_OVERHEAD;
	MPI_Init(NULL, NULL);
	int Size;
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	int Rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	int Posl = (Rank + 1) % Size; //Номер последующего процесса
	int Pred = Rank ? Rank - 1 : Size - 1; //Номер предшествующего процесса
	a = Rank + 0.7;
	//void *Addr=NULL;
	void* Buf = malloc(BufSize);
	MPI_Buffer_attach(Buf, BufSize);
	MPI_Bsend(&a, 1, MPI_DOUBLE, Posl, 5, MPI_COMM_WORLD);
	MPI_Status St;
	MPI_Recv(&b, 1, MPI_DOUBLE, Pred, 5, MPI_COMM_WORLD, &St);
	int Tmp;
	MPI_Buffer_detach(Buf, &Tmp);
	MPI_Buffer_attach(Buf, BufSize);
	MPI_Bsend(&a, 1, MPI_DOUBLE, Pred, 5, MPI_COMM_WORLD);
	MPI_Recv(&c, 1, MPI_DOUBLE, Posl, 5, MPI_COMM_WORLD, &St);
	MPI_Buffer_detach(Buf, &Tmp);
	if (!Buf) free(Buf);
	cout << "Process " << Rank << " a=" << a << " b=" << b << " c=" << c << endl;
	MPI_Finalize();
	return 0;
}