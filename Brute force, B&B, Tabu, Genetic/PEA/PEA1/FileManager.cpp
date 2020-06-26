#include "FileManager.h"
#include <iostream>

void FileManager::ReadFromFile()
{
	file.open("E://Code//C++//PEA//dane.txt", ios::in);

	file >> matrixSize;

	matrix = new int* [matrixSize];

	for (int i = 0; i < matrixSize; i++)
	{
		matrix[i] = new int[matrixSize];

		for (int j = 0; j < matrixSize; j++)
			file >> matrix[i][j];
	}

	file.close();
}

void FileManager::ShowMatrix(int** matrix, int matrixSize)
{
	if (matrixSize)
	{
		for (int i = 0; i < matrixSize; i++)
		{
			for (int j = 0; j < matrixSize; j++)
			{
				if (matrix[i][j + 1] == -1)
					cout << matrix[i][j];
				else
					cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

int FileManager::getElement(int n, int m)
{
	return matrix[n][m];
}

int** FileManager::getMatrixPtr()
{
	return matrix;
}
