#include "gauss.h"

float gauss2(vector< vector<float> > *v) {
	clock_t start = clock();
	int n = v->size();
	for (int i = 0; i<n; i++) {
		//szukanie maximum dla danego wiersza; wybór elementu podstawowoego(wybór czêœciowy) ograniczony do k tej kolumny dla k tego kroku
		float maxEl = abs(v->at(i).at(i));
		int maxRow = i;
		for (int k = i + 1; k<n; k++) {
			if (abs(v->at(k).at(i)) > maxEl) {
				maxEl = abs(v->at(k).at(i));
				maxRow = k;
			}
		}
		//zamieñ maksymaln¹ wiersz z obecnym
		for (int k = i; k<n + 1; k++) {
			float tmp = v->at(maxRow).at(k);
			v->at(maxRow).at(k) = v->at(i).at(k);
			v->at(i).at(k) = tmp;
		}
		// odjêcie wielokrotnoœci aktualnego wiersza od pozostalych wierszy 
		for (int k = i + 1; k<n; k++) {
				float c = 0; 
				if (v->at(k).at(i) != 0 && v->at(i).at(i) != 0) c = -v->at(k).at(i) / v->at(i).at(i);
			for (int j = i; j<n + 1; j++) {
				if (i == j) {
					v->at(k).at(j) = 0;
				}
				else {
					v->at(k).at(j) += c * v->at(i).at(j);
				}
			}
		}
	}
	//wynikowe równania Ax=b - zapisanie wyników w x
	vector<float> x(n);
	for (int i = n - 1; i >= 0; i--) {
		x[i] = 0;
		if (v->at(i).at(i) != 0) x[i] = v->at(i).at(n) / v->at(i).at(i);
		for (int k = i - 1; k >= 0; k--) {
			v->at(k).at(n) -= v->at(k).at(i) * x[i];
		}
	}

	printf("%.18f -gauss\n", x[0]); //wypisanie wyniku
	printf("Czas wykonywania: %lf ms\n\n", 1000. / CLOCKS_PER_SEC*(clock() - start));
	return x[0];
}
