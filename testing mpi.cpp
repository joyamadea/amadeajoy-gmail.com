#include "mpi.h"
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

void binarySearch(int* arr, int start, int end, int key, int rank)
{
	while (start <= end)
	{
		int mid = (start + end) / 2;
		if (arr[mid] == key)
		{
			//cout << "Element is found by processor " << rank << " .\n";
			int found = 1;
			MPI_Send(&found, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			return;
		}
		else if (arr[mid] < key)
		{
			start = mid + 1;
		}
		else
		{
			end = mid - 1;
		}
	}
}
int main(int argc, char** argv) {
	int n;

	cout << "How many elements in array? ";
	cin >> n;
	int* arr = new int[n];
	int found = 0;

	//membuat array yang telah disort menurut n
	for (int i = 0; i < n; i++)
	{
		arr[i] = i + 1;
	}

	int key;
	cout << "Search key: ";
	cin >> key;
	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout << "Processor rank: " << rank << "\nSize : " << size << endl;
	int blocks = size-1;
	int blockSize = n / blocks;


	/*if (rank == 0)
	{
		double start = MPI_Wtime();
		binarySearch(arr, rank * blockSize, (rank + 1) * blockSize - 1, key, rank);
		double end = MPI_Wtime();
		cout << "Execution time of Processor " << rank << " is " << (end - start) * 1000 << endl;
	}
	else if (rank == 1)
	{
		double start = MPI_Wtime();
		binarySearch(arr, rank * blockSize, (rank + 1) * blockSize - 1, key, rank);
		double end = MPI_Wtime();
		cout << "Execution time of Processor " << rank << " is " << (end - start) * 1000 << endl;
	}
	else if (rank == 2)
	{
		double start = MPI_Wtime();
		binarySearch(arr, rank * blockSize, (rank + 1) * blockSize - 1, key, rank);
		double end = MPI_Wtime();
	}*/

	if (rank != 0) {
		double start = MPI_Wtime();
		binarySearch(arr, rank * blockSize, (rank + 1) * blockSize - 1, key, rank);
		double end = MPI_Wtime();
		cout << "Execution time of Processor " << rank << " is " << (end - start) * 1000 << endl;
	}
	else if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Recv(&found, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (found == 1) {
				cout << "Element is found by processor " << i << " .\n";
			}
		}
		
	}

	MPI_Finalize();

	return 0;
}