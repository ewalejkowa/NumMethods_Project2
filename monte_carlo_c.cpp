#include "monte_carlo_h.h"

//sprawdza, czy na danym polu jest pu³apka
int sprawdz(vector <int>*plansza, int pole)
{
	for (int i = 0; i < plansza->size(); i++)
	{
		if (plansza->at(i) == pole)
		{
			int pom = abs(plansza->at(i + 1));
			pom += sprawdz(plansza, plansza->at(i) - pom); // je¿eli na polu cofniêtym równie¿ jest pu³apka
			if (pom>pole) pom = pole;
			return pom;
		}
	}
	return 0;
}

//przebieg gry
bool gra(vector<int>* plansza, int ilosc_pol)
{
	int wygrana = 0;
	int pozycja1 = 0;
	int pozycja2 = 0;
	int pom = 0;
	int kolejka = 1;
	while (wygrana == 0)
	{
		int rzut = rand() % 6 + 1;
		if (kolejka % 2 == 1)
		{  //pierwszy gracz
			pom = pozycja1 + rzut;
			int pulapka = sprawdz(plansza, pom);
				pozycja1 = pom - pulapka;
			if (pozycja1 >= ilosc_pol + 1){
				wygrana = 1;
				break;
			}
			kolejka = 2;

		}
		else if (kolejka % 2 == 0)
		{  //drugi gracz 
			pom = pozycja2 + rzut;
			int pulapka = sprawdz(plansza, pom);
				pozycja2 = pom - pulapka;
			if (pozycja2 >= ilosc_pol + 1)
			{
				wygrana = 2;
				break;
			}
			kolejka = 1;
		}
	}
	if (wygrana == 1) return true;
	else return false;
}


float monte_carlo(vector<int>* plansza, int ilosc_pol, int ilosc_prob){
	clock_t start = clock();
	float ilosc_wygranych = 0;
	for (int i = 0; i < ilosc_prob; i++){
		if (gra(plansza, ilosc_pol)) ilosc_wygranych++;
	}
	printf("%.18f -monte_carlo\n", (ilosc_wygranych) / ((float)ilosc_prob));
	printf("Czas wykonywania: %lf ms\n\n", 1000. / CLOCKS_PER_SEC*(clock() - start));
	return (ilosc_wygranych) / ((float)ilosc_prob);
}