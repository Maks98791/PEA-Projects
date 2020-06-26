#include "TabuSearch.h";

// Konstruktor
TabuSearch::TabuSearch(int matrixSize, int** matrix)
{
	N = matrixSize;

	// Kopiowanie macierzy wejsciowej do macierzy matrix_distance
	matrix_distance = new int* [N];

	for (int i = 0; i < N; i++) matrix_distance[i] = new int[N];

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			matrix_distance[i][j] = matrix[i][j];
			if (i == j)	matrix_distance[i][j] = 0;
		}
	}

	// Utworznie listy Tabu
	tabu_list = new int* [N];

	for (int i = 0; i < N; i++)
		tabu_list[i] = new int[N];

	// Domyœlne ustawienie na 0
	neighbour_criterion = 0; 
}

// Destruktor
TabuSearch::~TabuSearch()
{
	for (int i = 0; i < N; i++)
	{
		delete[] matrix_distance[i];
	}

	if (matrix_distance) delete[] matrix_distance;

	//Usuwamy macierz z pamiêci komputera
	for (int i = 0; i < N; i++)
	{
		delete[] tabu_list[i];
	}

	if (tabu_list) delete[] tabu_list;
}

void TabuSearch::Solve(int iterations, int lifetime, int max_critical_events, double max_time_s, int neighbour_criterion)
{
	// Zmienna iteracyjna do krytycznego warunku
	int number_of_the_same_order = 0;

	// Aktualna sciezka
	vector <int> permutation(N);
	// Jej koszt
	int permutation_cost = INT_MAX;
	int previous_cost;

	// Najlepsza œcie¿ka
	vector <int> best_route(N);
	// Jej koszt
	best_cost = INT_MAX;

	// Wygeneruj losowo lub heurystycznie punkt startowy x0 (rozwi¹zanie)
	for (int i = 0; i < N; i++)                     // permutacja[0] = 0 itd.
		permutation[i] = i;

	// Przypisz to na razie jako najlepsze rozwi¹zanie
	best_route = permutation;
	best_cost = Calculate_cost(permutation);

	// Tabu List - pusta
	Clear_tabu_list();

	// Sprawdzenie kosztu aktualnej drogi
	permutation_cost = Calculate_cost(permutation);

	// Petla trwa dopoki warunek konca = true (Np. przekroczenie okreœlonej liczby itaracji)
	for (int i = 0; i < iterations; i++)
	{
		// Patrzymy jaki byl koszt przed przemieszaniem (wyborem s¹siada)
		// Previous_cost = calculate_cost(permutation);

		// ZnajdŸ rozwi¹zanie (zmiana elementów w œcie¿ce), dla którego funkcja celu jest nawiêksza
		// (strategia wyboru rozwi¹zania: przejrzeæ wszystkie, braæ pod uwagê listê Tabu i kryterium aspiracji). Albo metoda Aspiration Plus
		// Wyklucz te rozwi¹zania, które s¹ na Tabu List
		// Kryterium aspiracji Aspiration(x,y) = TRUE
		if (neighbour_criterion)
			permutation = Generate_best_neighbour(permutation, lifetime);
		else
			permutation = Generate_best_neighbour_random(permutation, lifetime);

		// Najlepsze rozwi¹zanie <- aktualne rozwi¹zanie
		permutation_cost = Calculate_cost(permutation);

		// Jeœli koszt drogi jest mniejszy ni¿ aktualnie najlepszy, przypisz do najlepszego rozwi¹zanie aktualne rozwi¹zanie
		if (permutation_cost < best_cost)
		{
			best_cost = permutation_cost;
			best_route = permutation;
			number_of_the_same_order = 0;
		}
		else
		{
			number_of_the_same_order++;
		}

		// Dodaj nowe elementy do Tabu List (z okreœlon¹ kadencj¹)
		// Aktualizacja Tabu List
		// Dla ka¿dego rozwi¹zania bêd¹cego na Tabu List, obni¿ kadencjê o 1
		// Jeœli kadencja = 0, usuñ z Tabu List
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (tabu_list[i][j] > 0)
					tabu_list[i][j] = tabu_list[i][j] - 1;
			}
		}

		// Jeœli CriticalEvent = true (czyli zasz³y okreœlone warunki wymagaj¹ce wygenerowania nowego startowego rozwi¹zania
		// (Np. przez k kolejnych iteracji nie by³o lepszego rozwi¹zania lub alg wygenerowa³ k iteracji od wygenerowania ost rozw startowego)
		// -> to Restart (dywersyfikacja)
		if (number_of_the_same_order == max_critical_events && (max_critical_events != 0))
		{
			//Wygenerowanie nowego rozwi¹zania poprzez wywo³anie wielokrotne pojedynczego swapniêcia losowych miast
			for (int i = 0; i < 50; i++) Random_swap(permutation);

			if (Calculate_cost(permutation) < best_cost)
			{
				best_cost = permutation_cost;
				best_route = permutation;
				number_of_the_same_order = 0;
			}

			number_of_the_same_order = 0;

			// Wyczyszczenie TabuList
			Clear_tabu_list();
		}
	}

	//Dodanie zera i wyœwietlenie wyników
	best_route.push_back(0);

	cout << "Minimalny koszt: " << best_cost << endl;

	for (unsigned i = 0; i < N + 1; i++)
	{
		cout << best_route[i];

		if (i != N)	std::cout << " -> ";
	}

	cout << std::endl;
}

// Obliczenie kosztu aktualnej œcie¿ki
int TabuSearch::Calculate_cost(vector<int>& permutation)
{
	int cost = 0;

	for (int i = 0; i < N - 1; i++)
	{
		cost += matrix_distance[permutation.at(i)][permutation.at(i + 1)];
	}

	// Dodanie kosztu wierzcho³ka 0
	cost += matrix_distance[permutation.at(N - 1)][0];

	return cost;
}

// Losowa zamiana miast w œcie¿ce
void TabuSearch::Random_swap(vector<int>& route)
{
	// Generujemy dwa miasta
	int random_city = rand() % (N - 1) + 1;
	int another_random_city = rand() % (N - 1) + 1;

	// Musz¹ byæ ró¿ne od siebie
	while (random_city == another_random_city)
		another_random_city = rand() % (N - 1) + 1;

	// Zamieniamy miasta
	swap(route[random_city], route[another_random_city]);
}

// Generowanie s¹siedztwa
vector<int> TabuSearch::Generate_best_neighbour(vector<int> permutation, int lifetime)
{
	// Przypisanie byle jakiego pocz¹tkowego s¹siada
	vector<int> best_neighbour = permutation;

	// Aktualne (i póŸniej koñcowe) miasta s¹siada, które zosta³y zeswapowane
	int best_city_x, best_city_y;

	int best_cost_neighbours = INT_MAX;

	int cost_current;

	// Zaczynamy od 1, bo zero musi byæ zawsze na pocz¹tku
	for (int i = 1; i < N; i++)
	{
		for (int j = 2; j < N; j++)
		{
			// S¹siadem nie jest to samo rozwi¹zanie, a stanie siê tak, gdy i == j
			// if (i == j) continue;						

			// Wykonaj tylko dla tych krawêdzi, które nie s¹ na zakazanej liœcie
			if (tabu_list[i][j] == 0)
			{
				vector<int> new_perm = permutation;
				swap(new_perm[i], new_perm[j]);
				cost_current = Calculate_cost(new_perm);

				if (cost_current < best_cost_neighbours)
				{
					best_cost_neighbours = cost_current;
					best_neighbour = new_perm;
					best_city_x = i;
					best_city_y = j;
				}

			}
		}
	}

	// Ustawienie kadencji
	tabu_list[best_city_x][best_city_y] = lifetime;

	return best_neighbour;
}

// Druga funkcja generuj¹ca najlepszego s¹siada, ale zawsze zwracaj¹ca jakieœ przemieszanie
vector<int> TabuSearch::Generate_best_neighbour_random(vector<int> permutation, int lifetime)
{
	//Aktualne (i póŸniej koñcowe) miasta s¹siada, które zosta³y swapniête
	int best_city_x = 0, best_city_y = 0;

	std::vector<int> next_neighbour = permutation;

	//Zaczynamy od 1, bo zero musi byæ zawsze na pocz¹tku
	for (int i = 1; i < N; i++)
	{
		//dwie losowe wartoœci ró¿ne od zera
		int random_city = rand() % (N - 1) + 1;
		int another_random_city = rand() % (N - 1) + 1;

		while (random_city == another_random_city)
		{
			another_random_city = rand() % (N - 1) + 1;
		}

		vector<int> new_neighbour = next_neighbour;

		// Zamieñ miasta
		swap(new_neighbour[random_city], new_neighbour[another_random_city]);

			//Wykonaj tylko dla tych krawêdzi, które nie s¹ na zakazanej liœcie
		if (tabu_list[random_city][another_random_city] == 0)
		{
			if (Calculate_cost(new_neighbour) < Calculate_cost(next_neighbour))
			{
				next_neighbour = new_neighbour;
				best_city_x = random_city;
				best_city_y = another_random_city;
			}
		}

	}

	// Ustawienie kadencji
	tabu_list[best_city_x][best_city_y] = lifetime;

	return next_neighbour;
}

// Czyszczenie listy Tabu
void TabuSearch::Clear_tabu_list()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tabu_list[i][j] = 0;
}