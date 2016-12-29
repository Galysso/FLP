#include <iostream>
#include <vector>
#include "BBound.hpp"
#include "../SSCFLP/SSCFLP_GLPK.hpp"
#include "../SSCFLP/SSCFLP_SOL.hpp"
#include "../SSCFLP/SSCFLP_HEUR.hpp"

using namespace std;

BBound::BBound(Donnees *d) {
	int N = d->getN();
	int M = d->getM();
	
	// Données
	this->d = d;
	
	// Modélisation courante
	this->modelisation = new SSCFLP_GLPK (d);
	this->modelisation->glpkSetRelache();
	this->modelisation->glpkModeliserProbleme();
	this->modelisation->glpkResoudreProbleme();
	
	//~ this->modelisation->glpkAfficherSolution();
	
	// Borne duale
	this->borneDuale = this->modelisation->getZ();
	
	// Meilleure solution entière de départ
	SSCFLP_HEUR heur(d);

	// Meilleure solution de départ;
	this->bestSol = heur.getSolution();
	
	// Borne primale
	this->bornePrimale = bestSol.getZ();
	
	// Profondeur
	this->profondeur = 0;
	
	// Resize (plus grand pour moins d'allocations)
		// Au plus M*N liaisons à forcer
	this->traceur.resize(M*N);
	
		// Au plus N facilités ouvertes ou fermées
	this->openFacil.resize(N);
	this->closeFacil.resize(N);
}

BBound::~BBound() {
	delete(modelisation);
}
					
void BBound::casSolutionEntiere() {
	bornePrimale = modelisation->getZ();
	bestSol = modelisation->getSolution();
	--profondeur;
}

void BBound::casColonneLibre(int col) {
	int colFac = modelisation->getFacFromLink(col);;
	openFacil.at(colFac) = openFacil.at(colFac) + 1;
	modelisation->setLinkToOne(col, openFacil.at(colFac));
	modelisation->glpkResoudreProbleme();
	++profondeur;
	setTraceur();
}

void BBound::casColonneUn(int col) {
	int colFac = modelisation->getFacFromLink(col);
	openFacil.at(colFac) = openFacil.at(colFac) - 1;
	closeFacil.at(colFac) = closeFacil.at(colFac) + 1;
	modelisation->setLinkToZero(col, openFacil.at(colFac), closeFacil.at(colFac));
	modelisation->glpkResoudreProbleme();
	++profondeur;
	setTraceur();
}

void BBound::casColonneZero(int col) {
	int colFac = modelisation->getFacFromLink(col);
	closeFacil.at(colFac) = closeFacil.at(colFac) - 1;
	modelisation->setLinkToFree(col, closeFacil.at(colFac));
	--profondeur;
}

void BBound::search() {
	//~ bornePrimale = 2015;
	int nbIter = 0;
	int nbRelaxation = 0;
	// -----------
	
	int col;
	profondeur = 0;
	setTraceur();
	bool hasSolution = solutionValide();
	
	cout << endl << "borne initiale : " << bornePrimale << endl;
	
	do {++nbIter;		
		if (!hasSolution) {
			--profondeur;
			hasSolution = true;			
		} else {
			col = getCol();
			if (col == -1) {
				casSolutionEntiere();
				cout << endl << "nombre d'itérations : " << nbIter << endl;
				cout << "profondeur de l'arbre : " << profondeur << endl;
				cout << "nombre de relaxation : " << nbRelaxation << endl;
				cout << "nouvelle borne trouvée : " << bornePrimale << endl;
			} else if (modelisation->isSetToOne(col)) {
				casColonneUn(col);
				++nbRelaxation;
				hasSolution = solutionValide();
			} else if (modelisation->isSetToZero(col)) {
				casColonneZero(col);
			} else {
				casColonneLibre(col);
				++nbRelaxation;
				hasSolution = solutionValide();
			}
		}
				
	} while (profondeur != -1);
	
	cout << endl << "nombre d'itération : " << nbIter << endl;
	cout << "nombre de relaxation : " << nbRelaxation << endl;
	bestSol.afficherSol();
}

void BBound::setTraceur() {
	traceur.at(this->profondeur) = modelisation->getColLinkNearestToOne();
}

bool BBound::solutionValide() {
	return ((modelisation->getZ() > 0+delta) && (modelisation->getZ() < bornePrimale-delta));
}

int BBound::getCol() {
	if ((profondeur) == -1) {
		return -2;
	} else {
		return traceur.at(profondeur);
	}
}
