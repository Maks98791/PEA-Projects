#pragma once
#include "FileManager.h";
#include <vector>;

using namespace std;

class BruteForce
{
public:
	int shortestRoad;
	int** matrix;

	int* ComputeShortestRoad(int** matrix, int size);
};

