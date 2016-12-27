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
	
	this->modelisation->glpkAfficherSolution();
	
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
	this->colsToBound.resize(M*N);
	this->traceur.resize(M*N);
}

BBound::~BBound() {
	delete(modelisation);
}

/// TODO : fonction		d'ajout d'une contrainte
					/// de retrait de la dernière contrainte
					/// condition d'arrêt d'exploration d'une branche
					
void BBound::casSolutionEntiere() {
	cout << "SOLUTION ENTIERE" << endl;
	bornePrimale = modelisation->getZ();
	bestSol = modelisation->getSolution();
	--profondeur;
}

void BBound::casColonneLibre(int col) {
	cout << "COLONNE LIBRE" << endl;
	modelisation->setLinkToOne(col);
	modelisation->glpkResoudreProbleme();
	++profondeur;
	setColsToBound();
}

void BBound::casColonneUn(int col) {
	cout << "COLONNE FIXEE A 1" << endl;
	modelisation->setLinkToZero(col);
	modelisation->glpkResoudreProbleme();
	++profondeur;
	setColsToBound();
}

void BBound::casColonneZero(int col) {
	cout << "COLONNE FIXEE A 0" << endl;
	modelisation->setLinkToFree(col);
	if (traceur.at(profondeur)+1 == colsToBound.at(profondeur).size()) {
		--profondeur;
	} else {
		traceur.at(profondeur) = traceur.at(profondeur) + 1;
	}
}

void BBound::search() {
	//~ bornePrimale = 50;
	// ----- debug
	int nbSetToOne = 0;
	int nbSetToZero = 0;
	int nbSetToFree = 0;
	int nbIter = 0;
	// -----------
	
	int col;
	profondeur = 0;
	setColsToBound();
	bool hasSolution = true;
	
	do {
		modelisation->glpkAfficherSolution();
		col = getCol();
		cout << "COL = " << col << endl;
		
		if (!hasSolution) {
			--profondeur;
		} else if (col == -1) {
			casSolutionEntiere();
		} else if (modelisation->isSetToOne(col)) {
			casColonneUn(col);
			hasSolution = solutionValide();
		} else if (modelisation->isSetToZero(col)) {
			casColonneZero(col);
			hasSolution = solutionValide();
		} else {
			casColonneLibre(col);
			hasSolution = true;
		}
	} while (profondeur != -1);
	bestSol.afficherSol();
}

//~ void BBound::search() {
	//~ bornePrimale = 50;
	//~ // ----- debug
	//~ int nbSetToOne = 0;
	//~ int nbSetToZero = 0;
	//~ int nbSetToFree = 0;
	//~ int nbIter = 0;
	//~ // -----------
	//~ 
	//~ int col;
	//~ bool invalide = false;
	//~ profondeur = 0;
	//~ setColsToBound();
	//~ col = getCol();
	//~ bool fini = false;
	//~ 
	//~ while (!fini/* && nbIter<100*/) {++nbIter;
		//~ if (modelisation->isSetToZero(col)) {
			//~ modelisation->setLinkToFree(col);
			//~ if (traceur.at(profondeur)+1 < colsToBound.at(profondeur).size()) {
				//~ traceur.at(profondeur) = traceur.at(profondeur) + 1;
			//~ } else {
				//~ --profondeur;
				//~ col = getCol();
			//~ }
		//~ } else if (modelisation->isSetToOne(col)) {
			//~ modelisation->setLinkToZero(col);
			//~ modelisation->glpkResoudreProbleme();
			//~ if (solutionValide()) {
				//~ ++profondeur;
				//~ setColsToBound();
				//~ col = getCol();
			//~ } else if (modelisation->isSetToZero(col)) {
				//~ modelisation->setLinkToFree(col);
				//~ if (traceur.at(profondeur)+1 < colsToBound.at(profondeur).size()) {
					//~ traceur.at(profondeur) = traceur.at(profondeur) + 1;
				//~ } else {
					//~ --profondeur;
					//~ col = getCol();
				//~ }
			//~ } else {
				//~ modelisation->setLinkToOne(col);
				//~ modelisation->glpkResoudreProbleme();
				//~ if (solutionValide()) {
					//~ ++profondeur;
					//~ setColsToBound();
					//~ col = getCol();
				//~ } else {
					//~ modelisation->setLinkToZero(col);
					//~ modelisation->glpkResoudreProbleme();
					//~ if (solutionValide()) {
						//~ ++profondeur;
						//~ setColsToBound();
						//~ col = getCol();
					//~ } else {
						//~ modelisation->setLinkToFree(col);
						//~ if (traceur.at(profondeur)+1 < colsToBound.at(profondeur).size()) {
							//~ traceur.at(profondeur) = traceur.at(profondeur) + 1;
						//~ } else {
							//~ --profondeur;
							//~ col = getCol();
						//~ }
					//~ }
				//~ }
			//~ }
		//~ }
		//~ 
		//~ if (col == -1) {
			//~ bornePrimale = modelisation->getZ();
			//~ bestSol = modelisation->getSolution();
			//~ --profondeur;
		//~ }
		//~ 
		//~ fini = (profondeur == -1);
		//~ modelisation->glpkAfficherSolution();
		//~ //bestSol.afficherSol();
	//~ }
	//~ //bestSol.afficherSol();
//~ }

//~ void BBound::search() {
	//~ bornePrimale = 2100;
	//~ // ----- debug
	//~ int nbSetToOne = 0;
	//~ int nbSetToZero = 0;
	//~ int nbSetToFree = 0;
	//~ int nbIter = 0;
	//~ // -----------
	//~ 
	//~ int col;
	//~ bool invalide = false;
	//~ profondeur = 0;
	//~ traceur.at(profondeur) = 0;
	//~ setColsToBound();
	//~ 
	//~ do {
		//~ cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
		//~ col = colsToBound.at(profondeur).at(traceur.at(profondeur));
		//~ cout << "col = " << col << endl;
		//~ 
		//~ for (int i = 0; i < colsToBound.at(profondeur).size(); ++i) {
			//~ cout << colsToBound.at(profondeur).at(i) << endl;
		//~ }
		//~ 
		//~ if ((!invalide) && (col == -1)) {
			//~ cout << "entière" << endl;
			//~ if (modelisation->getZ() < bornePrimale-delta) {
				//~ bornePrimale = modelisation->getZ();
				//~ bestSol = modelisation->getSolution();
			//~ }
			//~ --profondeur;
		//~ } else {
			//~ if (invalide) {
				//~ cout << "invalide" << endl;
				//~ --profondeur;
				//~ invalide = false;
			//~ } else {
				//~ if (modelisation->isSetToOne(col)) {
					//~ cout << "1 -> 0" << endl;
					//~ modelisation->setLinkToZero(col);
					//~ ++nbSetToZero;
					//~ modelisation->glpkResoudreProbleme();
					//~ ++profondeur;
					//~ setColsToBound();
				//~ } else if (modelisation->isSetToZero(col)) {
					//~ cout << "0 -> x" << endl;
					//~ modelisation->setLinkToFree(col);
					//~ ++nbSetToFree;
					//~ if (traceur.at(profondeur)+1 < colsToBound.at(profondeur).size()) {
						//~ traceur.at(profondeur) = traceur.at(profondeur) + 1;
					//~ } else {
						//~ --profondeur;
					//~ }
				//~ } else {
					//~ cout << "x -> 1" << endl;
					//~ modelisation->setLinkToOne(col);
					//~ ++nbSetToOne;
					//~ modelisation->glpkResoudreProbleme();
					//~ ++profondeur;
					//~ setColsToBound();
				//~ }
				//~ invalide = ((modelisation->getZ() < 0+delta) || (modelisation->getZ() > bornePrimale+delta));
			//~ }
		//~ }
//~ 
	//~ modelisation->glpkAfficherSolution();
	//~ cout << "profondeur = " << profondeur << endl;
	//~ cout << "nbSetToOne = " << nbSetToOne << endl;
	//~ cout << "nbSetToZero = " << nbSetToZero << endl;
	//~ cout << "nbSetToFree = " << nbSetToFree << endl;
	//~ cout << "nbIter = " << nbIter << endl;
	//~ 
	//~ if (profondeur == 0) {
		//~ break;
	//~ }
	//~ 
	//~ } while (profondeur != -1);
	//~ bestSol.afficherSol();
//~ }

void BBound::setColsToBound() {
	colsToBound.at(this->profondeur) = this->modelisation->getColsToConstraint();
	traceur.at(profondeur) = 0;
}

bool BBound::solutionValide() {
	return ((modelisation->getZ() > 0+delta) && (modelisation->getZ() < bornePrimale-delta));
}

int BBound::getCol() {
	if ((profondeur) == -1) {
		return -2;
	} else {
		return colsToBound.at(profondeur).at(traceur.at(profondeur));
	}
}

void BBound::testP1() {
	//~ this->modelisation->glpkModeliserProbleme();	
	this->modelisation->setLinkToOne(22);
	this->modelisation->setLinkToOne(23);
	this->modelisation->setLinkToZero(35);
	this->modelisation->setLinkToOne(180);
	this->modelisation->setLinkToOne(137);
	this->modelisation->setLinkToOne(127);
	this->modelisation->setLinkToZero(128);
	this->modelisation->setLinkToOne(145);
	this->modelisation->setLinkToOne(13);
	this->modelisation->setLinkToZero(32);
	this->modelisation->setLinkToOne(71);
	this->modelisation->setLinkToOne(19);
	this->modelisation->setLinkToZero(132);
	this->modelisation->setLinkToOne(146);
	this->modelisation->setLinkToOne(138);
	this->modelisation->setLinkToOne(15);
	this->modelisation->setLinkToOne(172);
	this->modelisation->setLinkToZero(170);
	this->modelisation->setLinkToOne(70);
	this->modelisation->setLinkToZero(76);
	this->modelisation->setLinkToOne(96);
	this->modelisation->setLinkToOne(84);
	this->modelisation->setLinkToOne(89);
	this->modelisation->setLinkToZero(94);
	this->modelisation->setLinkToZero(28);
	this->modelisation->setLinkToZero(34);
	this->modelisation->setLinkToOne(21);
	this->modelisation->setLinkToZero(88);
	this->modelisation->setLinkToZero(174);
	this->modelisation->setLinkToOne(194);
	this->modelisation->setLinkToZero(188);
	this->modelisation->setLinkToZero(168);

	
	//~ //modelisation->glpkModeliserProbleme();
	modelisation->glpkResoudreProbleme();
	modelisation->glpkAfficherSolution();
	//~ cout << "colonne la plus proche de 1 : " << modelisation->getColLinkNearestToOne() << endl;
}
