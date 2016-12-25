#include <iostream>
#include "BBound.hpp"
#include "../SSCFLP/SSCFLP_GLPK.hpp"
#include "../SSCFLP/SSCFLP_SOL.hpp"
#include "../SSCFLP/SSCFLP_HEUR.hpp"

using namespace std;

BBound::BBound(Donnees *d) {
	// Données
	this->d = d;
	
	// Modélisation courante
	this->modelisation = new SSCFLP_GLPK (d);
	this->modelisation->glpkSetRelache();
	this->modelisation->glpkModeliserProbleme();
	this->modelisation->glpkResoudreProbleme();
	
	// Borne duale
	this->borneDuale = this->modelisation->getZ();
	
	// Meilleure solution entière de départ
	SSCFLP_HEUR heur(d);

	// Meilleure solution de départ;
	this->bestSol = new SSCFLP_SOL(heur.getSolution());
	
	// Borne primale
	this->bornePrimale = bestSol->getZ();
	
	modelisation->glpkAfficherSolution();
	
	cout << "colonne la plus proche de 1 : " << modelisation->getColLinkNearestToOne() << endl;
}

/// TODO : fonction		d'ajout d'une contrainte
					/// de retrait de la dernière contrainte
					/// condition d'arrêt d'exploration d'une branche


void BBound::testP1() {
	this->modelisation->glpkModeliserProbleme();
	this->modelisation->setLinkToOne(22);
	this->modelisation->setLinkToOne(23);
	this->modelisation->setLinkToOne(15);
	this->modelisation->setLinkToOne(180);
	this->modelisation->setLinkToOne(137);
	this->modelisation->setLinkToOne(127);
	this->modelisation->setLinkToOne(72);
	
	//modelisation->glpkModeliserProbleme();
	modelisation->glpkResoudreProbleme();
	modelisation->glpkAfficherSolution();
	cout << "colonne la plus proche de 1 : " << modelisation->getColLinkNearestToOne() << endl;
}
