#include "Manager.h"
#include <iostream>
#include "FileManager.h"
#include "TabuSearch.h"
#include "Genetic.h"

using namespace std;

void Manager::Menu() {

	FileManager manage;

	do {
		cout << endl << endl << "WYBIERZ OPERACJE I ZATWIERDZ KLIKAJAC ENTER" << endl << endl;

		cout << "1. Wczytaj dane z pliku" << endl;
		cout << "2. Wyswietl wczytane dane" << endl;
		cout << "3. Brute Force (przeglad zupelny)" << endl;
		cout << "4. B & B" << endl;
		cout << "5. Tabu Search" << endl;
		cout << "6. Genetic algorithm" << endl;
		cout << "7. Wyjdz z programu" << endl;

		cout << "Wpisz numer: ";
		cin >> operation;
		cout << endl;

		switch (operation) {
		// Read data from file
		case 1: {
			
			manage.ReadFromFile();

			break;
		}
		// Show read data
		case 2: {
			
			cout << "Liczba miast: " << manage.matrixSize << endl;

			manage.ShowMatrix(manage.matrix, manage.matrixSize);

			break;
		}
		// Brute force algorithm
		case 3: {

			cout << "Dlugosc najkrotszej drogi to: ";

			int* path = new int[manage.matrixSize];
			path = bruteForce.ComputeShortestRoad(manage.matrix, manage.matrixSize);

			cout << path[manage.matrixSize-1] << endl << endl << "1 -> ";

			for (int i = 0; i < manage.matrixSize - 1; i++)
				cout << path[i] + 1 << " -> ";

			cout << "1";
				
			break;
		}
		// Branch & Bound algorithm
		case 4: {

			BranchAndBound bb(manage.matrixSize, manage.matrix);
			
			cout << "Dlugosc najkrotszej drogi to: " << bb.ComputePath();

			break;
		}
		// Tabu search
		case 5: {

			TabuSearch Tabu(manage.matrixSize, manage.matrix);

			// iteration_criterion_TS, lifetime, attempts_until_diversification, time_criterion_TS, neighour_selection
			// lifetime - dlugosc pozostawania elementów na liœcie tabu
			// attemps_until_diversification - przez k iteracji nie by³o lepszego rozwiazania
			// iteration_criterion_TS - ilosc wszystkich iteracji
			// time_criterion_TS - warunek stopu czasowy
			// neighbour_selection - kryterium aspiracji (o ile musi byc lepsze rozwiazanie od aktualnego)
			Tabu.Solve(100, 10, 10, 1000, 5);

			break;
		}
		// Genetic algorithm
		case 6: {

			std::string file = "dane.txt";
			int seconds = 30;
			int population = 10;
			float crosses = 0.8;
			float mutations = 0.01;

			Graph graph;
			graph.readGraph();
			GeneticAlgorithm alg(&graph, seconds, population, crosses, mutations);
			TabuSearch Tabu(manage.matrixSize, manage.matrix);
			// nie rozdzielona sciezka jako jeden dlugi int
			int result = alg.apply(true);

			string res;

			do
			{
				int digit = result % 10;
				result /= 10;
				res += to_string(digit) + " -> ";
			} while (result > 0);


			Tabu.Solve(100, 10, 10, 1000, 5);
			break;
		}
		// Exit program
		case 7: {

			system("PAUSE");

			break;
		}
		// Wrong choice
		default: {

			cout << "Bledny wybor" << endl;

			system("PAUSE");
		}
		}
	} while (true);
}