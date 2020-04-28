#include <iostream>
#include<vector>
#include <time.h>
#include "gauss.h"
#include "gauss_seidel.h"
#include "monte_carlo_h.h"
using namespace std;

struct rownanie
{
	int poz1;
	int poz2;
	int kto_ma_ruch;
	int wspolczynnik=0;
	int pozycja=0;   
};

//sprawdza czy w danej kolejce znajduje sie podany element, zwraca ten element z kolejki, zaczyna poszukiwanie od miejsca wskazywanego przez-pozycja
rownanie* czy_jest_w_kolejce(vector<rownanie>* kolejka, rownanie a, int pozycja)
{
	for (int i = pozycja; i < kolejka->size(); i++)
	{
		if (kolejka->at(i).poz1 == a.poz1 && kolejka->at(i).poz2 == a.poz2&&  kolejka->at(i).kto_ma_ruch == a.kto_ma_ruch) return &kolejka->at(i);
	}
	return 0;
}

//generuje wszystkie mo¿liwoœci ruchu dla sytuacji podanej przez-pocz¹tek
void generuj(vector<rownanie>* kolejka, rownanie poczatek, vector<int>* plansza, int ilosc_pol, int pozycja)
{
	for (int i = 1; i <= 6; i++)
	{
		rownanie pom;	
		pom.kto_ma_ruch = (poczatek.kto_ma_ruch % 2) + 1;
		int tmp = 0;
		int pulapka = 0;	
		if (poczatek.kto_ma_ruch == 1) //pierwszy gracz
		{
			tmp = poczatek.poz1 + i;   //wykonanie kolejnego ruchu
			pulapka = sprawdz(plansza, tmp); //sprawdzenie czy nie ma pu³apki
			if (tmp - pulapka >= ilosc_pol)
			{
				pom.poz1 = 1000;			//sytuacja gdy koñczymy grê, prawdopodobieñstwo zawiera tylko wspó³czynnik
				pom.poz2 = 1000;
				pom.kto_ma_ruch = 10;
			}
			else
			{
				pom.poz1 = tmp - pulapka;
				pom.poz2 = poczatek.poz2;
			}
		}
		else  //drugi gracz
		{
			tmp = poczatek.poz2 + i;
			pulapka = sprawdz(plansza, tmp);
			if (tmp - pulapka >= ilosc_pol)
			{
				pom.poz1 = 1000;
				pom.poz2 = 1000;
				pom.kto_ma_ruch = 10;
				pom.wspolczynnik = 0;
				if (czy_jest_w_kolejce(kolejka, pom, pozycja) == 0) 
				{
					kolejka->push_back(pom);
					pom.pozycja = kolejka->size() - 1;
					break;
				}
			}
			else
			{
				pom.poz2 = tmp - pulapka;
				pom.poz1 = poczatek.poz1;
			}
		}
		if (czy_jest_w_kolejce(kolejka, pom, pozycja) == 0) 
		{
			pom.wspolczynnik++;  //je¿eli nie ma w kolejce
			pom.pozycja = kolejka->size();
			kolejka->push_back(pom);
		}
		else 
		{
			czy_jest_w_kolejce(kolejka, pom, pozycja)->wspolczynnik++; //je¿eli jest w kolejce to powiekszmy tylko wspó³czynnik			
		}	
	}
}

//parsuje vector na macierz typu vector
vector < vector <float> > macierz(vector<rownanie>* kolejka, vector<rownanie>* wyniki)
{
	vector < vector <float> > v(wyniki->size(), vector<float>(wyniki->size()+1, 0));
	for (int i = wyniki->size()-1; i >=0; i--)
	{  //kolejne równaniaa, wiersze
		int index = 0;
		int p;
		int koniec;
			if (i == 0)
			{
				koniec = wyniki->at(0).pozycja;
			}
			else 
			{
				p = wyniki->at(i - 1).pozycja;
				koniec = wyniki->at(i).pozycja-p;	
			}
			rownanie cos ; //pobieramy kolejny element
			cos = kolejka->back();
			kolejka->pop_back();

			v.at(i ).at(i) = 1;
			for (int j = 0; j <koniec; j++)
			{ //kolejne elementy z równania
			for (int k=0; k < wyniki->size(); k++)
			{  //kolumna
				if (cos.poz1 == wyniki->at(k).poz1 &&cos.poz2 == wyniki->at(k).poz2 && cos.kto_ma_ruch == wyniki->at(k).kto_ma_ruch
						&& cos.kto_ma_ruch != 10)
				{
						v.at(i ).at(k) = -(float)cos.wspolczynnik / 6;
						if (kolejka->size() != 0 && j!=koniec-1)
						{
							cos = kolejka->back();
							kolejka->pop_back();			
						}
						break;
					}
				else  if (cos.kto_ma_ruch == 10)
				{
					v.at(i ).at(wyniki->size()) = (float)cos.wspolczynnik / 6;
					if (kolejka->size() != 0 && j != koniec-1 )
					{
						cos = kolejka->back();
						kolejka->pop_back();
					}
					break;
				}
			}		
		}
	}
	return v;
}


//wypisuje kolejke
void wypisz_kolejke(vector<rownanie>* kolejka, vector<rownanie>* wyniki, int poz1, int poz2, int element){ //wypisywanie kolejki
	printf("%d)(%d,%d,%d)=", element, wyniki->at(element).poz1, wyniki->at(element).poz2, wyniki->at(element).kto_ma_ruch);
	for (int i = poz1; i < poz2; i++){
		if (kolejka->at(i).kto_ma_ruch != 10){
			printf("%d/6*(%d,%d,%d)+", kolejka->at(i).wspolczynnik, kolejka->at(i).poz1, kolejka->at(i).poz2, kolejka->at(i).kto_ma_ruch);
		}
		else{
			printf("%d/6", kolejka->at(i).wspolczynnik);
		}
	}
	printf("\n");
}

//generowanie metody gaussa i gaussa -seidla
void rownania(vector<int>* plansza, int ilosc_pol, int ilosc_iteracji, float monte_carlo_wynik)
{
	vector<rownanie> kolejka;
	vector<rownanie> wyniki;
	rownanie poczatek;
	poczatek.poz1 = 0;
	poczatek.poz2 = 0;
	poczatek.kto_ma_ruch = 1;

	generuj(&kolejka, poczatek, plansza, ilosc_pol, 0); //wygenerowanie równañ dla pocz¹tku
	poczatek.pozycja = kolejka.size();
	wyniki.push_back(poczatek);

	for (int j=0; j<kolejka.size(); j++)
	{ //tworzenie wszystkich równañ- zapisane w kolejce
		rownanie tmp =kolejka.at(j);
		if (czy_jest_w_kolejce(&wyniki, tmp,0) == 0 && tmp.kto_ma_ruch!=10)
		{	
			generuj(&kolejka, tmp, plansza, ilosc_pol, kolejka.size());
			tmp.pozycja = kolejka.size();
			tmp.wspolczynnik = 1;
			wyniki.push_back(tmp);
		}
	}
	vector<rownanie> kolejka2 = kolejka ; //wypisywanie równañ
	for (int i = 0; i < wyniki.size(); i++)
	{
		if (i + 1>wyniki.size())  wypisz_kolejke(&kolejka, &wyniki, wyniki.at(i).pozycja, wyniki.size(), i - 1);
		if (i == 0)  wypisz_kolejke(&kolejka, &wyniki, 0, wyniki.at(i).pozycja, i);
		else wypisz_kolejke(&kolejka, &wyniki, wyniki.at(i - 1).pozycja, wyniki.at(i).pozycja, i);
	}
	
	printf("\n-******************************************************************************-\n");
	vector < vector <float> > v= macierz(&kolejka2, &wyniki);
	vector < vector <float> > v2 = v;

	float wynik =gauss2(&v2);
	//for (float epsilon = 0.01; epsilon >0.0000000001; epsilon /= 10){
	gauss_seidel2(ilosc_iteracji,&v, wyniki.size(), monte_carlo_wynik, 0);
//	}
		
}
int main(){
	srand(time(NULL));
	vector<int> plansza;
	int tablica[] = { 2, -1, 4, -4, 6, -5, 13, -4, 14, -6, 17, -2, 19, -1, 21, -1, 22, -10, 25, -2, 26, -16 };
	int rozmiar = 27; //rozmiar planszy
	int ilosc = 11; //ilosc pu³apek
	int ilosc_prob = 100000; //ilosc prób dla monte carlo
	int ilosc_iteracji = 120; //ilos iteracji dla gaussa seidla
	float monte_carlo_wynik = 0;
	for (int i = 0; i < ilosc*2; i++)
	{
		plansza.push_back(tablica[i]);
	}
	
	rownania(&plansza, rozmiar,ilosc_iteracji, 0);
	monte_carlo_wynik = monte_carlo(&plansza, rozmiar, ilosc_prob);
	system("pause");
	return 0;
}