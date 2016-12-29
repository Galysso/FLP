#include "SSCFLP_SOL.hpp"
#include "../Donnees/Donnees.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

SSCFLP_SOL::SSCFLP_SOL() {
	this->N = 0;
	this->M = 0;
	this->z = 0;
	this->facilites = NULL;
	this->liaisons = NULL;
}

SSCFLP_SOL::SSCFLP_SOL(int N, int M, double z, double *facilites, double **liaisons) {
	this->N = N;
	this->M = M;
	this->z = z;
	this->facilites = facilites;
	this->liaisons = liaisons;
}

SSCFLP_SOL::~SSCFLP_SOL() {
	if (this->facilites != NULL) {
		//~ delete this->facilites;
	}
	if (this->liaisons != NULL) {
		for (int i = 0; i < this->N; ++i) {
			//~ delete liaisons[i];
		}
		//~ delete liaisons;
	}
}

int SSCFLP_SOL::getN() {
	return this->N;
}

int SSCFLP_SOL::getM() {
	return this->M;
}

double SSCFLP_SOL::getZ() {
	return this->z;
}

double *SSCFLP_SOL::getFacilites() {
	return this->facilites;
}

double **SSCFLP_SOL::getLiaisons() {
	return this->liaisons;
}

int SSCFLP_SOL::setN(int N) {
	this->N = N;
}

int SSCFLP_SOL::setM(int M) {
	this->M = M;
}

double SSCFLP_SOL::setZ(double z) {
	this->z = z;
}

double *SSCFLP_SOL::setFacilites(double *facilites) {
	if (this->facilites != NULL) {
		delete this->facilites;
	}
	this->facilites = facilites;
}

double **SSCFLP_SOL::setLiaisons(double **liaisons) {
	if (this->liaisons != NULL) {
		for (int i = 0; i < this->N; ++i) {
			delete liaisons[i];
		}
		delete liaisons;
	}
	this->liaisons = liaisons;
}


void SSCFLP_SOL::afficherSol() {
	int i, j;
	
	cout << "LES FACILITES :" << endl;
	i = 0; j = 0;
	while ((i < N) && !(facilites[i] > 0+delta)) {
		++i;
	}
	cout << "{";
	if (i < N) {
		cout << i;
		for (i=i+1; i < N; ++i) {
			if (facilites[i] > 0+delta) {
				cout << "," << i;
			}
		}
	}
	cout << "}" << endl;
	
	cout << "LES LIAISONS :" << endl;
	cout << setprecision(2);
	i = 0;
	while (i < N) {
		j = 0;
		while ((j < M) && !(liaisons[i][j] > 0+delta)) {
			++j;
		}
		if (j < M) {
			cout << "(" << i << ",{(" << j << "," << liaisons[i][j] << ")";
			for (j=j+1; j < M; ++j) {
				if (liaisons[i][j] > 0+delta) {
					cout << ",(" << j << "," << liaisons[i][j] << ")";
				}
			}
			cout << "})" << endl;
		}
		++i;
	}
	cout << setprecision(-1);
	cout << "Z = " << z << endl;
}
