#include "SSCFLP_HEUR.hpp"
#include <iostream>

using namespace std;

/** Construction
 * A chaque etape :
 * 	- Une facilité est ouverte
 * 	- Des clients non déjà assignés y sont assignés de manière gloutonne
 *    en respectant la contrainte de capacité
 *
 ** 
 */
 
//~ int **deltas;	// delta[i][j] = c[i][j] - min(c[*][j]),    le client j fixé
//~ int *efficacites;// (cOuverture[i] + Ej(deltas[i][j])) / M

SSCFLP_HEUR::SSCFLP_HEUR(Donnees *d) {
	int i;
	
	this->d = d;
	int N = d->getN();
	int M = d->getM();
	this->deltas = new double * [N];
	this->efficacites = new double [N];
	this->clientsAssignes = new bool [M];
	this->facilitesOuvertes = new bool [N];
	this->capacites = new double[N];
	
	for (i = 0; i < N; ++i) {
		this->deltas[i] = new double [M];
		this->capacites[i] = d->capacite(i);
		this->facilitesOuvertes[i] = false;
	}
	for (i = 0; i < M; ++i) {
		this->clientsAssignes[i] = false;
	}
	
	initialisation();
}

void SSCFLP_HEUR::initialisation() {
	int i, j, N, M;
	double min, tmp, somme;
	
	N = this->d->getN();
	M = this->d->getM();
	
	// Calcul des deltas
	for (i = 0; i < M; ++i) {
		min = this->d->coutAlloc(0,i);
		for (j = 1; j < N; ++j) {
			tmp = this->d->coutAlloc(j, i);
			if (tmp < min) {
				min = tmp;
			}
		}
		
		for (j = 0; j < N; ++j) {
			this->deltas[j][i] = this->d->coutAlloc(j, i) - min;
			//~ cout << this->deltas[j][i] << endl;
		}
		//~ cout << endl;
	}
	
	// Calcul des efficacites
	for (i = 0; i < N; ++i) {
		somme = this->d->coutOuverture(i);
		for (j = 0; j < M; ++j) {
			somme = somme + this->deltas[i][j];
		}
		this->efficacites[i] = somme / M;
		//~ cout << this->efficacites[i] << endl;
	}
}

void SSCFLP_HEUR::construction() {
	/// TODO : les efficacites doivent être mises à jour à chaque modification !!!
}
