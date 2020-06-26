#pragma once
#include "FileManager.h";
#include <vector>;

using namespace std;

class BranchAndBound
{
public:
	int** matrix;
	int matrixSize;
	int currentTreeLevel = 0;

	BranchAndBound(int matrixSize, int** readMatrix);

	bool FindElement(int* visited, int elem);
	int** CopyMatrix(int** oldMatrix);
	int StartMatrixReducing();
	int matrixReducing(int beginVertex, int endVertex, int** matrix);
	int* ComputePath();



	void TSP(int** matrix);
	void TSPRec(int** adj, int curr_bound, int curr_weight, int level, int curr_path[]);
	int secondMin(int** matrix, int i);
	void copyToFinal(int curr_path[]);
	int firstMin(int** matrix, int i);
	// final_path[] stores the final solution ie, the 
	// path of the salesman. 
	int* final_path;
	// visited[] keeps track of the already visited nodes 
	// in a particular path 
	int* visited;
	// Stores the final minimum weight of shortest tour. 
	int final_res = INT_MAX;
};

