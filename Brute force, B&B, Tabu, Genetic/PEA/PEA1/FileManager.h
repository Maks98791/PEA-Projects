#pragma once
#include <fstream>
using namespace std;

class FileManager
{
public:
	fstream file;
	int matrixSize;
	int** matrix;

	void ReadFromFile();
	void ShowMatrix(int** matrix, int matrixSize);
	int getElement(int n, int m);
	int** getMatrixPtr();
};

