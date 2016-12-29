#include <iostream>
#include <glpk.h>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "SSCFLP_GLPK.hpp"
#include "SSCFLP_SOL.hpp"
#include "../Donnees/Donnees.hpp"

using namespace std;

SSCFLP_GLPK::SSCFLP_GLPK(Donnees *d) {
	this->ia = NULL;
	this->ja = NULL;
	this->ar = NULL;
	this->prob = NULL;
	this->d = d;
	this->entier = true;
	glp_term_out(0);
}

SSCFLP_GLPK::~SSCFLP_GLPK() {
	if (prob != NULL) {
		glp_delete_prob(prob);
	}
	if (ia != NULL) {
		delete(ia);
	}
	if (ja != NULL) {
		delete(ja);
	}
	if (ar != NULL) {
		delete(ar);
	}
}

void SSCFLP_GLPK::glpkModeliserProbleme() {
	//~ this->entier = true;
	stringstream colName;
	
	int M = this->d->getM();
	int N = this->d->getN();
	
	// Suppression de l'ancien problème
	if (prob != NULL) {
		glp_delete_prob(prob);
	}
	
	// Creation du nouveau problème
	this->prob = glp_create_prob();
	glp_set_prob_name(prob, "SSCFLP");
	
	// Problème de minimisation
	glp_set_obj_dir(prob, GLP_MIN);
	
	// On a M+N contraintes 
	glp_add_rows(this->prob, M+N);
	
	// On a M*N + N variables
	glp_add_cols(this->prob, M*N+N);
	
	int taille = M*N+N*(M+1);
	
	if (this->ia != NULL) {
		delete [] this->ia;
	}
	this->ia = new int [1+taille];
	
	if (this->ja != NULL) {
		delete [] this->ja;
	}
	this->ja = new int [1+taille];
	
	if (this->ar != NULL) {
		delete [] this->ar;
	}
	this->ar = new double [1+taille];
	
	int pos = 1;
	int i, j;
	int col, client, facil;
		
	// La demande de chaque client est satisfaite
	for (i = 0; i < M; ++i) {
		glp_set_row_bnds(this->prob, i+1, GLP_FX, 1.0, 1.0);
		
		for (j = 0; j < N; ++j) {
			col = getColLink(j, i);
			
			colName.str("");
			colName << "X" << j << "," << i;
			glp_set_col_name(this->prob, col, colName.str().c_str());
			
			if (this->entier) {
				glp_set_col_kind(this->prob, col, GLP_BV);
			} else {
				glp_set_col_bnds(this->prob, col, GLP_DB, 0.0, 1.0);
			}
			glp_set_obj_coef(this->prob, col, this->d->coutAlloc(j,i));
			
			ia[pos] = i+1;
			ja[pos] = col;
			ar[pos] = 1;
			
			++pos;
		}
	}
	
	// Aucune facilité ne fournit plus qu'elle n'a de capacité
	for (i = 0; i < N; ++i) {		
		for (j = 0; j < M; ++j) {
			col = getColLink(i, j);
			
			this->ia[pos] = M+i+1;
			this->ja[pos] = col;
			this->ar[pos] = d->demande(j);
			++pos;
		}
		
		glp_set_row_bnds(this->prob, M+i+1, GLP_UP, 0.0, 0.0);
		
		col = getColFac(i);
		colName.str("");
		colName << "Y" << i;
		
		glp_set_col_kind(this->prob, col, GLP_BV);
		glp_set_col_name(this->prob, col, colName.str().c_str());
		glp_set_obj_coef(this->prob, col, this->d->coutOuverture(i));
				
		this->ia[pos] = M+i+1;
		this->ja[pos] = col;
		this->ar[pos] = -d->capacite(i);
		
		++pos;
	}
	
	// Chargement de la matrice
	glp_load_matrix(this->prob, taille, ia, ja, ar);
	//~ glp_write_lp(this->prob, NULL, "modelisation");
}

void SSCFLP_GLPK::glpkResoudreProbleme() {
	glp_simplex(this->prob, NULL);
	glp_intopt(this->prob, NULL);
}

void SSCFLP_GLPK::glpkAfficherSolution() {
	int M = this->d->getM();
	int N = this->d->getN();
	int i, col;
	
	double z = glp_mip_obj_val(this->prob);
	
	cout << "FACILITES :\n{";
	for (i = 0; i < N; ++i) {
		col = getColFac(i);
		if (glp_mip_col_val(this->prob, col) > 1-delta) {
			cout << i << ",";
		}
	}
	cout << "}"<<endl;
	
	cout << "LIAISONS :\n";
	cout << setprecision(2);
	for (int fac = 0; fac < N; ++fac) {
		cout << "("<< fac <<",{";
		for (int client = 0; client < M; ++client) {
			col = getColLink(fac, client);
			//~ cout << "col=" << col << endl;
			if (glp_mip_col_val(this->prob, col) > 0+delta) {
				cout << "(" << client << "," << glp_mip_col_val(this->prob, col) << "),";
			}
		}
		cout << "})" << endl;
	}
	cout << setprecision(-1);
	cout << "Z = " << z << endl;
}

void SSCFLP_GLPK::glpkSetEntier() {
	this->entier = true;
}

void SSCFLP_GLPK::glpkSetRelache() {
	this->entier = false;
}

void SSCFLP_GLPK::setDonnees(Donnees *d) {
	this->d = d;
}

SSCFLP_SOL SSCFLP_GLPK::getSolution() {
	int N = this->d->getN();
	int M = this->d->getM();
	double z = glp_mip_obj_val(this->prob);
	double *facilites = new double [N];
	double **liaisons = new double * [N];
	
	for (int i = M*N+1; i <= M*N+N; ++i) {
		facilites[i-M*N-1] = glp_mip_col_val(this->prob, i);
	}
	
	for (int fac = 1; fac <= N; ++fac) {
		liaisons[fac-1] = new double [M];
		for (int client = 1; client <= M; ++client) {
			liaisons[fac-1][client-1] = glp_mip_col_val(this->prob, client+(fac-1)*M);
		}
	}
	
	return SSCFLP_SOL(N, M, z, facilites, liaisons);
}

double SSCFLP_GLPK::getZ() {
	return glp_mip_obj_val(this->prob);
}

int SSCFLP_GLPK::getColLink(int indFacil, int indClient) const {
	return indClient + indFacil*this->d->getM() + 1;
}

int SSCFLP_GLPK::getColFac(int indFacil) const {
	return (this->d->getM()*this->d->getN()) + indFacil + 1;
}

int SSCFLP_GLPK::getColFacFromLink(int indLink) const {
	return ((d->getM()*d->getN()) + ((indLink-1)/d->getM()) + 1);
}

int SSCFLP_GLPK::getFacFromLink(int indLink) const {
	return ((indLink-1)/d->getM());
}

int SSCFLP_GLPK::getColLinkNearestToOne() const {
	double max = 0;
	int indMax = -1;
	int N = this->d->getN();
	int M = this->d->getM();
	int col;
	double val;
	
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			col = getColLink(i, j);
			val = glp_mip_col_val(this->prob, col);
			
			if ((val+delta < 1) && (val-delta > 0) && (val > max)) {
				max = val;
				indMax = col;
			}
		}
	}
		
	return indMax;
}

void SSCFLP_GLPK::setLinkToOne(int i, int nbUn) {
	glp_set_col_bnds(this->prob, i, GLP_FX, 1.0, 1.0);
	if (nbUn == 1) {
		glp_set_col_bnds(this->prob, getColFacFromLink(i), GLP_FX, 1.0, 1.0);
	}
}

void SSCFLP_GLPK::setLinkToZero(int i, int nbUn, int nbZero) {
	glp_set_col_bnds(this->prob, i, GLP_FX, 0.0, 0.0);
	if (nbUn == 0) {
		glp_set_col_bnds(this->prob, getColFacFromLink(i), GLP_DB, 0.0, 1.0);
	}
	if (nbZero == d->getM()) {
		glp_set_col_bnds(this->prob, getColFacFromLink(i), GLP_FX, 0.0, 0.0);
	}
}

void SSCFLP_GLPK::setLinkToFree(int i, int nbZero) {
	glp_set_col_bnds(this->prob, i, GLP_DB, 0.0, 1.0);
	if (nbZero == d->getM()-1) {
		glp_set_col_bnds(this->prob, getColFacFromLink(i), GLP_DB, 0.0, 1.0);
	}
}

bool SSCFLP_GLPK::isSetToOne(int i) {
	return (glp_get_col_lb(this->prob, i) == 1.0);
}

bool SSCFLP_GLPK::isSetToZero(int i) {
	return (glp_get_col_ub(this->prob, i) == 0.0);
}







