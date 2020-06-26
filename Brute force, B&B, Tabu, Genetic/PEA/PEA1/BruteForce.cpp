#include "BruteForce.h"
#include <iostream>
#include <vector>
#include <algorithm>

int* BruteForce::ComputeShortestRoad(int** matrix, int size)
{
	int bestRoad = INT_MAX;
	// ilosc miast bez miasta startowego
	vector<int> newMatrix;
	int* currentPath = new int[size-1];
	int* bestPath = new int[size];

	// dodanie miast do wektora z wyjatkiem startowego
	for (int i = 1; i < size; i++)
		newMatrix.push_back(i);

	// leci po wszystkich permutacjach newMatrix
	do {

		int currentRoad = 0;
		int a = 0;

		for (int j = 0; j < newMatrix.size(); j++)
		{
			currentRoad += matrix[a][newMatrix[j]];
			a = newMatrix[j];

			currentPath[j] = newMatrix[j];
		}

		currentRoad += matrix[a][0];

		if (currentRoad < bestRoad)
		{
			bestRoad = currentRoad;

			for (int i = 0; i < size-1; i++)
				bestPath[i] = currentPath[i];
		}

	} while (next_permutation(newMatrix.begin(), newMatrix.end()));

	bestPath[size - 1] = bestRoad;

	return bestPath;
}