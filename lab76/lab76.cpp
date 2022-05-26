// Написать программу, иллюстрирующую возможности 
//организации схемы «ведущий процесс – ведомые процессы(masterslave)» 
// стандартными средствами MPI.Процесс с номером 0 считать ведущим, а остальные – ведомыми.
// В ведущем процессе использовать неблокирующий прием сообщений и функцию MPI_WAITSOME
//(MPI::Request::Waitsome).В «ведомых» процессах использовать блокирующую передачу сообщений.
//

//MPI_Waitsome-c-Bind.cpp
#include "mpi.h"
#include <iostream>
#include <cmath>

using namespace std;
//Проверка всех элементов
bool Is_Finished(bool* Prizn, int N)
{
	bool Res = true;
	for (int k = 0; k < N; k++)
		Res = (Prizn[k]) && Res;
	return Res;
}
//Для корректной работы
//Функция Master должна быть быстрее, чем Slave
//Делаем число операций в ней пропорциональным длине массива
double Master(double* A, int N)
{
	double S = 0;
	for (int k = 0; k < N; k++)
		S += A[k];
		return S;
}
//Для корректной работы
//Функция Master должна быть быстрее, чем Slave
//Делаем число операций в ней пропорциональным кубу длине массива
void Slave(double* A, int N, int Rank)
{
	static int Count = 0;
	double* Tmp1 = new double[N];
	double* Tmp2 = new double[N];
	Tmp1[0] = cos(1.0 + Count + Rank);
	Tmp2[0] = cos(1.0 + Count - Rank);
	for (int k = 1; k < N; k++) {
		Tmp1[k] = sin(Tmp1[k - 1] + Tmp2[k - 1]);
		Tmp2[k] = sin(Tmp1[k - 1] - Tmp2[k - 1]);
	}
	//Длинный трехкратно вложенный цикл 
	for (int k = 0; k < N; k++) {
		A[k] = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				A[k] += Tmp1[i] * Tmp2[j] / (1 + k + (i - j) * (i - j));
	}
	Count++;
	delete[] Tmp2;
	delete[] Tmp1;
}
int const NN = 1000, NNN = 3;
int main(int argc, char** argv)
{
	MPI_Init(NULL, NULL);
	int Size;
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	int Rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	if (Size < 2) {
		cout << "It is required not less than two processes!" << endl;
	}
	else {
			if (Rank == 0) {//Ведущий процесс
				bool* Prizn = new bool[Size - 1];
				double* A = new double[(Size - 1) * NN];
				double* R = new double[Size - 1];
				for (int k = 0; k < Size - 1; k++)
					R[k] = 0;
				MPI_Request* Req = new MPI_Request[Size - 1];
				MPI_Status* St = new MPI_Status[Size - 1];
				int* Indx = new int[Size - 1];
				double Tms = MPI_Wtime();
				for (int m = 0; m < NNN; m++) {
					for (int k = 1; k < Size; k++) {
						Prizn[k - 1] = false;
						MPI_Irecv(&A[NN * (k - 1)], NN, MPI_DOUBLE, k, 5,
							MPI_COMM_WORLD, &Req[k - 1]);
					}
					while (!Is_Finished(Prizn, Size - 1)) {//Какой-то из
					//ведомых процессов не переслал очередные данные
						int Num;
						MPI_Waitsome(Size - 1, Req, &Num, Indx, St);
						for (int k = 0; k < Num; k++) {
							Prizn[Indx[k]] = true;
							R[Indx[k]] += Master(&A[NN * Indx[k]], NN);
						}
					}
				}
				Tms = MPI_Wtime() - Tms;
				for (int k = 0; k < Size - 1; k++)
					cout << "R[" << k << "]=" << R[k] << endl;
				cout << "Time=" << Tms << " s" << endl;
				delete[] Indx;
				delete[] St;
				delete[] Req;
				delete[] R;
				delete[] A;
				delete[] Prizn;
			}
			else {//Ведомые процессы
				double* A = new double[NN];
				for (int m = 0; m < NNN; m++) {
						Slave(A, NN, Rank);
					MPI_Send(A, NN, MPI_DOUBLE, 0, 5,
						MPI_COMM_WORLD);
				}
				delete[] A;
			}
	}
	MPI_Finalize();
	return 0;
}
