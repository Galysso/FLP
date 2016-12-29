#include <iostream>
#include <string>
#include "Donnees.hpp"
#include "Parser.hpp"

using namespace std;

Donnees::Donnees(string nomFichier) {
	coutsAllocation = NULL;
	demandes = NULL;
	coutsOuverture = NULL;
	capacites = NULL;
	
	Parser p;
	p.lecture(this, nomFichier);
}

Donnees::~Donnees() {
	//~ if (coutsAllocation != NULL) {
		//~ for (int i = 0; i < M; ++i) {
			//~ delete[] coutsAllocation;
		//~ }
	//~ }
	//~ if (demandes != NULL) {
		//~ delete(demandes);
	//~ }
	//~ if (coutsOuverture != NULL) {
		//~ delete(coutsOuverture);
	//~ }
	//~ if (capacites != NULL) {
		//~ delete(capacites);
	//~ }
}

void Donnees::setM(int M) {
	this->M = M;
}

void Donnees::setN(int N) {
	this->N = N;
}

void Donnees::setCoutsAllocation(double **coutsAllocation) {
	if (this->coutsAllocation != NULL) {
		for (int i = 0; i < M; ++i) {
			if (this->coutsAllocation[i] != NULL) {
				delete [] this->coutsAllocation[i];
			}
		}
		delete [] coutsAllocation;
	}
	
	this->coutsAllocation = coutsAllocation;
}

void Donnees::setDemandes(double *demandes) {
	if (this->demandes != NULL) {
		delete [] this->demandes;
	}
	
	this->demandes = demandes;
}

void Donnees::setCoutsOuverture(double *coutsOuverture) {
	if (this->coutsOuverture != NULL) {
		delete [] coutsOuverture;
	}
	this->coutsOuverture = coutsOuverture;
}

void Donnees::setCapacites(double *capacites) {
	if (this->capacites != NULL) {
		delete [] this->capacites;
	}
	this->capacites = capacites;
}

int Donnees::getM() const {
	return this->M;
}

int Donnees::getN() const {
	return this->N;
}

double Donnees::coutAlloc(int i, int j) const {
	return this->coutsAllocation[i][j];
}

double Donnees::demande(int i) const {
	return this->demandes[i];
}

double Donnees::coutOuverture(int i) const {
	return this->coutsOuverture[i];
}

double Donnees::capacite(int i) const {
	return this->capacites[i];
}

void Donnees::afficherCoutsAllocation() const {
	cout << "COUTS ALLOCATION" << endl;
	for (int client = 0; client < M; ++client) {
		cout << "\t" << client;
	}
	cout << endl << endl;
	for (int plant = 0; plant < N; ++plant) {
		cout << plant;
		for (int client = 0; client < M; ++client) {
			cout << "\t" << coutsAllocation[plant][client];
		}
		cout << endl;
	}
}

void Donnees::afficherDemandes() const {
	cout << "DEMANDES" << endl;
	for (int client = 0; client < M; ++client) {
		cout << client << " : " << demandes[client] << endl;
	}
}

void Donnees::afficherCoutsOuverture() const {
	cout << "COUTS OUVERTURE" << endl;
	for (int plant = 0; plant < N; ++plant) {
		cout << plant << " : " << coutsOuverture[plant] << endl;
	}
}

void Donnees::afficherCapacites() const {
	cout << "CAPACITE" << endl;
	for (int plant = 0; plant < N; ++plant) {
		cout << plant << " : " << capacites[plant] << endl;
	}
}
