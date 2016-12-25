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
}

/// TODO : fonction		d'ajout d'une contrainte
					/// de retrait de la dernière contrainte
					/// condition d'arrêt d'exploration d'une branche
