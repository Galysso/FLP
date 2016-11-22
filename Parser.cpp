#include <iostream>
#include <string>
#include "Parser.hpp"
#include "Donnees.hpp"

using namespace std;

Parser::Parser() {
	
}

void Parser::lecture(Donnees *d, string nomFichier) const { 
	ifstream f(nomFichier.c_str());
	if (f) {
		// M : nombre de clients
		int M;
		f >> M;
		d->setM(M);
		
		// N : nombre de locations
		int N;
		f >> N;
		d->setN(N);
		
		// coûts allocation
		double **coutsAllocation = new double * [N];
		for (int plant = 0; plant < N; ++plant) {
			coutsAllocation[plant] = new double [M];
			for (int client = 0; client < M; ++client) {
				f >> coutsAllocation[plant][client];
			}
		}
		d->setCoutsAllocation(coutsAllocation);
		
		// demandes
		double *demandes = new double [M];
		for (int client = 0; client < M; ++client) {
			f >> demandes[client];
		}
		d->setDemandes(demandes);
		
		// coûts ouverture
		double *coutsOuverture = new double [N];
		for (int plant = 0; plant < N; ++plant) {
			f >> coutsOuverture[plant];
		}
		d->setCoutsOuverture(coutsOuverture);
		
		// capacites
		double *capacites = new double [N];
		for (int plant = 0; plant < N; ++plant) {
			f >> capacites[plant];
		}
		d->setCapacites(capacites);
	} else {
		cerr << "Le fichier ne peut pas être lu" << endl;
	}
}
