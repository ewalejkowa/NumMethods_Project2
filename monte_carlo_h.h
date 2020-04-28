#ifndef monte_carlo_h
#define monte_carlo_h
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

//sprawdza czy na danym polu nie ma pu³apki
int sprawdz(vector <int>*plansza, int pole);

//przebieg gry
bool gra(vector<int>* plansza, int ilosc_pol);
float monte_carlo(vector<int>* plansza, int ilosc_pol, int ilosc_prob);


#endif