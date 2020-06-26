#include  <vector>
#include  <iostream>
#include <ctime>
#include <windows.h>
#include <iomanip>

using namespace std;

class TabuSearch
{
public:
	int N; //Liczba wierzcho³ków w grafie = matrix size
	int** matrix_distance;
	int** tabu_list;
	bool neighbour_criterion;
	int best_cost;

	TabuSearch(int matrixSize, int** matrix);
	~TabuSearch();
	int Calculate_cost(std::vector<int>& permutation);
	void Clear_tabu_list();
	void Random_swap(std::vector<int>& route);
	std::vector<int> Generate_best_neighbour(std::vector<int>permutation, int lifetime);
	std::vector<int> Generate_best_neighbour_random(std::vector<int> permutation, int lifetime);
	void Solve(int iterations, int lifetime, int max_critical_events, double max_time_s, int neighbour_criterion);
};