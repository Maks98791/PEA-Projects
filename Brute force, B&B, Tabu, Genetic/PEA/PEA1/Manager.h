#pragma once
#include <fstream>
#include "BruteForce.h"
#include "BranchAndBound.h"

using namespace std;

class Manager
{
public:
	int operation;
	BruteForce bruteForce;

	void Menu();
};

