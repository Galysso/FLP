#include "SSCFLP_HEUR.hpp"
#include "SSCFLP_SOL.hpp"
#include <iostream>

using namespace std;

SSCFLP_HEUR::SSCFLP_HEUR(Donnees *d) {
	int i, j;
	
	this->d = d;
	this->z = 0;
	int N = d->getN();
	int M = d->getM();
	this->deltas = new double * [N];
	this->indicesDeltas = new int * [N];
	this->efficacites = new double [N];
	this->indicesEfficacites = new int [N];
	this->facilites = new bool [N];
	this->liaisons = new bool * [N];
	this->clientsAssignes = new bool [M];
	this->capacites = new double[N];
	
	for (i = 0; i < N; ++i) {
		this->deltas[i] = new double [M];
		this->capacites[i] = d->capacite(i);
		this->facilites[i] = false;
		this->liaisons[i] = new bool [M];
		this->indicesDeltas[i] = new int [M];
		this->indicesEfficacites[i] = i;
		
		for (j = 0; j < M; ++j) {
			this->liaisons[i][j] = false;
		}
	}
	for (i = 0; i < M; ++i) {
		this->clientsAssignes[i] = false;
	}
	
	initialisation();
	construction();
}

SSCFLP_SOL SSCFLP_HEUR::getSolution() {
	int N = this->d->getN();
	int M = this->d->getM();
	
	double *facilites = new double [N];
	double **liaisons = new double * [N];
	
	for (int i = 0; i < N; ++i) {
		liaisons[i] = new double [M];
		if (this->facilites[i]) {
			facilites[i] = 1.0;
		} else {
			facilites[i] = 0.0;
		}
		
		for (int j = 0; j < M; ++j) {
			if (this->liaisons[i][j]) {
				liaisons[i][j] = 1.0;
			} else {
				liaisons[i][j] = 0.0;
			}
		}
	}
	return SSCFLP_SOL(N, M, this->z, facilites, liaisons);
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
			this->indicesDeltas[j][i] = i;
		}
	}
	
	trierDeltas();
}

void SSCFLP_HEUR::setEfficacites() {
	double somme;
	int N = this->d->getN();
	int M = this->d->getM();
	int i, j, indF, indC;	
	
	for (i = 0; i < N; ++i) {
		indF = this->indicesEfficacites[i];
		if (!facilites[indF]) {
			somme = this->d->coutOuverture(indF);
			for (j = 0; j < M; ++j) {
				indC = this->indicesDeltas[indF][j];
				if (!clientsAssignes[indC] && (this->d->demande(indC) < this->capacites[indF])) {
					somme = somme + this->deltas[i][j];
				}
			}
			this->efficacites[i] = somme / M;
		}
	}
	
	trierEfficacites();
}

void SSCFLP_HEUR::trierDeltas() {
	bool fini;
	double deltaTmp;
	int indTmp;
	int N = this->d->getN();
	int M = this->d->getM();
	
	for (int i = 0; i < N; ++i) {
		do {
			fini = true;
			for (int j = 1; j < M; ++j) {
				if (deltas[i][j] < deltas[i][j-1]) {
					deltaTmp = deltas[i][j];
					deltas[i][j] = deltas[i][j-1];
					deltas[i][j-1] = deltaTmp;
					
					indTmp = indicesDeltas[i][j];
					indicesDeltas[i][j] = indicesDeltas[i][j-1];
					indicesDeltas[i][j-1] = indTmp;
					
					fini = false;
				}
			}
		} while (!fini);
	}
}

void SSCFLP_HEUR::trierEfficacites() {
	bool fini;
	double efficaciteTmp;
	int indTmp;
	int N = this->d->getN();
	int M = this->d->getM();
	
	do {
		fini = true;
		for (int i = 1; i < N; ++i) {
			if (efficacites[i] < efficacites[i-1]) {
				efficaciteTmp = efficacites[i];
				efficacites[i] = efficacites[i-1];
				efficacites[i-1] = efficaciteTmp;
				
				indTmp = indicesEfficacites[i];
				indicesEfficacites[i] = indicesEfficacites[i-1];
				indicesEfficacites[i-1] = indTmp;
				
				fini = false;
			}
		}
	} while (!fini);
}

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

void SSCFLP_HEUR::construction() {
	int N = this->d->getN();
	int M = this->d->getM();
	int nbClientLibre = M;
	int i, j, indF, indC;
	
	i = 0;
	while ((nbClientLibre > 0) && (i < N)) {
		setEfficacites();
		indF = getBestFacilite();
		this->facilites[indF] = true;
		this->z = this->z + this->d->coutOuverture(indF);
		
		for (j = 0; j < M; ++j) {
			indC = this->indicesDeltas[indF][j];
			if ((!clientsAssignes[indC]) && (this->d->demande(indC) <= this->capacites[indF])) {				
				this->capacites[indF] = this->capacites[indF] - this->d->demande(indC);
				this->clientsAssignes[indC] = true;
				this->z = this->z + this->d->coutAlloc(indF, indC);
				this->liaisons[indF][indC] = 1;
				nbClientLibre = nbClientLibre - 1;
			}
		}
		++i;
	}
}

int SSCFLP_HEUR::getBestFacilite() {
	int i = 0;
	int N = this->d->getN();
	int indF = this->indicesEfficacites[i];
	
	while ((this->facilites[indF]) && (i < N)) {
		++i;
		indF = this->indicesEfficacites[i];
	}
	
	if (i == N) {
		indF = -1;
	}
	
	return indF;
}

void SSCFLP_HEUR::afficherDeltas() {
	int N = this->d->getN();
	int M = this->d->getM();
	
	cout << "DELTAS" << endl;
	for (int i = 0; i < N; ++i) {
		cout << i << " :\t";
		for (int j = 0; j < M; ++j) {
			cout << this->deltas[i][j] << "|";
			cout << this->indicesDeltas[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

void SSCFLP_HEUR::afficherEfficacites() {
	int N = this->d->getN();
	
	cout << "EFFICACITES" << endl;
	for (int i = 0; i < N; ++i) {
		cout << this->efficacites[i] << "|";
		cout << this->indicesEfficacites[i] << "\t";
	}
	cout << endl;
}

void SSCFLP_HEUR::afficherSolution() {
	int N = this->d->getN();
	int M = this->d->getM();
	
	cout << "SOLUTION HEURISTIQUE" << endl;
	cout << "FACILITES" << endl;
	cout << "{";
	for (int i = 0; i < N; ++i) {
		if (facilites[i]) {
			cout << i << ", ";
		}
	}
	cout << "}" << endl;
	
	cout << "LIAISONS" << endl;
	for (int i = 0; i < N; ++i) {
		cout << "(" << i << ", {";
		for (int j = 0; j < M; ++j) {
			if (liaisons[i][j]) {
				cout << j << ", ";
			}
		}
		cout << "})" << endl;
	}
	
	cout << "Z = " << this->z << endl;
}





