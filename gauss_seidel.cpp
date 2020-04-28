#include "gauss_seidel.h"


float blad_wzgledny(float a1, float a2){
	return abs((a1 - a2) / a1);
}

void gauss_seidel2(int ilosc_iteracji, vector < vector <float> >* v, int size , float monte_carlo_wynik, float epsilon)
{
	clock_t start = clock();
	float  x[1000], y[1000];
	int   i = 0, j = 0;
	for (i = 0; i < size; i++)
	{	
	 x[i]=0;
	}
	int tmp=0;
	while ( ilosc_iteracji>0) //abs(monte_carlo_wynik - x[0]) >epsilon)
		{
			for (i = 0; i < size; i++)
			{
				y[i] = (v->at(i).at(size) / v->at(i).at(i));
				for (j = 0; j < size; j++)
				{
					if (j == i)
						continue;
					y[i] = y[i] - ((v->at(i).at(j) / v->at(i).at(i)) * x[j]);
					x[i] = y[i];
				}

			}
			tmp++;
			ilosc_iteracji--;
		}

		//printf("%.18f -monte_carlo\n", monte_carlo_wynik);
		//printf("%.18f -epsilon\n", epsilon);
		printf("%.18f -gauss-seidel ilosc iteracji- %d\n", x[0], tmp);
		printf("Czas wykonywania: %lf ms\n\n", 1000. / CLOCKS_PER_SEC*(clock() - start));
	//	printf("%.18f -blad wzgledny\n", blad_wzgledny(monte_carlo_wynik, x[0]));
}