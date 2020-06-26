#include "BranchAndBound.h"
#include <iostream>

using namespace std;

BranchAndBound::BranchAndBound(int matrixSize, int** readMatrix)
{
	this->matrixSize = matrixSize;

	this->matrix = new int* [matrixSize];

	for (int i = 0; i < matrixSize; i++)
		matrix[i] = new int[matrixSize];

	for (int a = 0; a < matrixSize; a++)
	{
		for (int b = 0; b < matrixSize; b++)
		{
			matrix[a][b] = readMatrix[a][b];
		}
	}
}

bool BranchAndBound::FindElement(int* visited, int elem)
{
	for (int b = 0; b < matrixSize; b++)
	{
		if (visited[b] == elem)
		{
			return true;
		}
	}
	return false;
}

int** BranchAndBound::CopyMatrix(int** oldMatrix)
{
	int** newMatrix = new int* [matrixSize];

	for (int i = 0; i < matrixSize; i++)
	{
		newMatrix[i] = new int[matrixSize];

		for (int j = 0; j < matrixSize; j++)
		{
			newMatrix[i][j] = oldMatrix[i][j];
		}
	}

	return newMatrix;
}

int BranchAndBound::StartMatrixReducing()
{
	int startBound = 0;

	// redukcja macierzy po wierszach

	int rowBound = 0;
	vector<int> rowMinimum;
	int currentMinimum = INT_MAX;

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[i][j] == -1)
				continue;

			if (matrix[i][j] < currentMinimum)
				currentMinimum = matrix[i][j];
		}
		
		rowMinimum.push_back(currentMinimum);
		currentMinimum = INT_MAX;
	}

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[i][j] == -1)
				continue;

			matrix[i][j] -= rowMinimum[i];
		}
	}

	for (int a = 0; a < rowMinimum.size(); a++)
		rowBound += rowMinimum[a];


	// redukcja macierzy po kolumnach

	int columnBound = 0;
	vector<int> columnMinimum;
	currentMinimum = INT_MAX;;

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[j][i] == -1)
				continue;

			if (matrix[j][i] < currentMinimum)
				currentMinimum = matrix[j][i];
		}

		columnMinimum.push_back(currentMinimum);
		currentMinimum = INT_MAX;
	}

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[j][i] == -1)
				continue;

			matrix[j][i] -= columnMinimum[i];
		}
	}

	for (int a = 0; a < columnMinimum.size(); a++)
		columnBound += columnMinimum[a];

	// poczatkowe ograniczenie

	startBound = rowBound + columnBound;

	return startBound;
}

int BranchAndBound::matrixReducing(int beginVertex, int endVertex, int** matrix)
{
	int rowBound = 0;
	int columnBound = 0;
	//vector<int> rowMinimum;
	int* rowMinimum = new int[matrixSize];
	//vector<int> columnMinimum;
	int* columnMinimum = new int[matrixSize];
	int currentMinimum = INT_MAX;
	int bound = matrix[beginVertex][endVertex];

	// inicjalizacja kolumny endVertex, wiersza beginVertex oraz c(endVertex, beginVertex) ujemnymi wartosciami
	for (int i = 0; i < matrixSize; i++)
	{
		matrix[i][endVertex] = -1;
		matrix[beginVertex][i] = -1;
	}
	matrix[endVertex][beginVertex] = -1;


	// redukcja macierzy po wierszach
	for (int i = 0; i < matrixSize; i++)
	{
		if (i == beginVertex)
		{
			rowMinimum[i] = 0;
			continue;
		}

		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[i][j] == -1)
				continue;
			
			if (matrix[i][j] < currentMinimum)
				currentMinimum = matrix[i][j];

			if (currentMinimum == 0)
				break;
		}

		//rowMinimum.push_back(currentMinimum);
		rowMinimum[i] = currentMinimum;
		//cout << rowMinimum[i] << " ";
		currentMinimum = INT_MAX;
	}

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[i][j] == -1 || matrix[i][j] == 0)
				continue;

			matrix[i][j] -= rowMinimum[i];
		}
	}

	for (int a = 0; a < matrixSize; a++)
		rowBound += rowMinimum[a];

	currentMinimum = INT_MAX;

	// redukcja macierzy po kolumnach
	for (int i = 0; i < matrixSize; i++)
	{
		if (i == endVertex)
		{
			columnMinimum[i] = 0;
			continue;
		}

		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[j][i] == -1)
				continue;

			if (matrix[j][i] < currentMinimum)
				currentMinimum = matrix[j][i];

			if (currentMinimum == 0)
				break;
		}

		//columnMinimum.push_back(currentMinimum);
		columnMinimum[i] = currentMinimum;
		//cout << columnMinimum[i] << " ";
		currentMinimum = INT_MAX;
	}

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (matrix[j][i] == -1)
				continue;

			matrix[j][i] -= columnMinimum[i];
		}
	}

	for (int a = 0; a < matrixSize; a++)
		columnBound += columnMinimum[a];

	bound += columnBound + rowBound;

	return bound;
}

int* BranchAndBound::ComputePath()
{
	// poczatkowa redukcja macierzy kosztów
	StartMatrixReducing();

	// tablica przechowujaca aktualna sciezke
	int* currentPath = new int[matrixSize];

	// tablica przechowujaca odwiedzone wierzcholki
	int* visited = new int[matrixSize + 1];
	for (int k = 0; k < matrixSize + 1; k++)
		visited[k] = 0;
	
	// zmienna przechowujaca wage najmniejszego wierzcholka
	int lowestBound = INT_MAX;
	int lowestVertex = 0;
	int iteration = matrixSize;

	// algorytm
	for (int x = 0; x < matrixSize; x++)
	{
		for (int i = 1; i < iteration; i++)
		{
			int** tempMatrix = CopyMatrix(matrix);

			// jesli nie znajdzie elementu i w tabeli visited 
			if (!FindElement(visited, i))
			{
				int bound = matrixReducing(currentTreeLevel, i, tempMatrix);

				cout << "aktualna granica: " << bound << endl;

				if (bound < lowestBound)
				{
					lowestBound = bound;
					lowestVertex = i;
				}
			}

			delete[] tempMatrix;
		}
		visited[currentTreeLevel] = lowestVertex;
		currentTreeLevel += 1;
		iteration -= 1;
	}

	return visited;
}
