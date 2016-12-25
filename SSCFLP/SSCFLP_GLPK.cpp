#include <iostream>
#include "SSCFLP_GLPK.hpp"
#include "SSCFLP_SOL.hpp"
#include "../Donnees/Donnees.hpp"
#include <glpk.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

SSCFLP_GLPK::SSCFLP_GLPK(Donnees *d) {
	this->ia = NULL;
	this->ja = NULL;
	this->ar = NULL;
	this->prob = NULL;
	this->d = d;
	this->entier = true;
}

SSCFLP_GLPK::~SSCFLP_GLPK() {
	if (prob != NULL) {
		glp_delete_prob(prob);
	}
}

void SSCFLP_GLPK::glpkModeliserProbleme() {
	//~ this->entier = true;
	
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
	double tab[1+M+N][1+taille];
	
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
	for (i = 1; i <= M; ++i) {
		client = i-1;
		//~ cout << "i="<<i<< "\tborne =\tx=1.0" << endl;
		glp_set_row_bnds(this->prob, i, GLP_FX, 1.0, 1.0);
		
		for (j = 1; j <= N; ++j) {
			facil = j-1;
			col = i+(j-1)*M;
				
			if (this->entier) {
				glp_set_col_kind(this->prob, col, GLP_BV);
			} else {
				glp_set_col_bnds(this->prob, col, GLP_DB, 0.0, 1.0);
			}
			glp_set_obj_coef(this->prob, col, this->d->coutAlloc(facil,client));
			
			ia[pos] = i;
			ja[pos] = col;
			ar[pos] = 1;
			
			++pos;
		}
	}
	
	// Aucune facilité ne fournit plus qu'elle n'a de capacité
	for (i = M+1; i <= M+N; ++i) {
		facil = i-M-1;
		
		//~ cout << "i="<<i<< "\tborne =\tx<=0" << endl;
		glp_set_row_bnds(this->prob, i, GLP_UP, 0.0, 0.0);
		
		for (j = 1; j <= M; ++j) {
			client = j-1;
			col = (i-1-M)*M+j;//j+(i-M-1)*(M+1);
			
			this->ia[pos] = i;
			this->ja[pos] = col;
			this->ar[pos] = d->demande(client);
			++pos;
		}
		col = (i-M)+(j-1)*N;

		//~ glp_set_col_kind(prob, col, GLP_IV);
		glp_set_col_kind(this->prob, col, GLP_BV);
		//~ glp_set_col_bnds(prob, col, GLP_DB, 0.0, 1.0);
			
		glp_set_obj_coef(this->prob, col, this->d->coutOuverture(facil));
		
		this->ia[pos] = i;
		this->ja[pos] = col;
		this->ar[pos] = -d->capacite(facil);
		
		++pos;
	}
	
	// Chargement de la matrice
	glp_load_matrix(this->prob, taille, ia, ja, ar);
	glp_write_lp(this->prob, NULL, "modelisation");
}

void SSCFLP_GLPK::glpkResoudreProbleme() {
	glp_simplex(this->prob, NULL);
	glp_intopt(this->prob,NULL);
}

void SSCFLP_GLPK::glpkAfficherSolutionLatex() {
	int M = this->d->getM();
	int N = this->d->getN();
	int i, j, col;
	
	double z = glp_mip_obj_val(this->prob);
	
	cout << endl << "\\begin{itemize}"<<endl;
	cout << "\\item Temps de résolution : " << endl;
	cout << "\\item Valeur de la fonction objectif : " << z << endl;
	cout << "\\item Les facilités à construire sont : \\{";
	for (i = M*N+1; i <= M*N+N; ++i) {
		if (glp_mip_col_val(this->prob, i)) {
			cout << i-M*N-1 << ",";
		}
	}
	cout << "\\}"<<endl;
	
	cout << "\\item Les liaisons à faire sont : \n\\begin{itemize}" << endl;
	for (int fac = 1; fac <= N; ++fac) {
		cout << "\\item (" << fac-1 <<",\\{";
		for (int client = 1; client <= M; ++client) {
			col = client+(fac-1)*M;
			if (glp_mip_col_val(this->prob, col) > 0) {
				cout << client-1 << ",";
			}
		}
		cout << "\\})" << endl;
	}
}

void SSCFLP_GLPK::glpkAfficherSolution() {
	int M = this->d->getM();
	int N = this->d->getN();
	int i, col;
	
	double z = glp_mip_obj_val(this->prob);
	
	cout << "FACILITES :\n{";
	for (i = M*N+1; i <= M*N+N; ++i) {
		//~ cout << i << endl;
		//~ cout << glp_mip_col_val(prob, i) << ";";
		if (glp_mip_col_val(this->prob, i) == 1) {
			cout << i-M*N-1 << ",";
		}
	}
	cout << "}"<<endl;
	
	cout << "LIAISONS :\n";
	cout << setprecision(2);
	for (int fac = 1; fac <= N; ++fac) {
		cout << "("<< fac-1 <<",{";
		for (int client = 1; client <= M; ++client) {
			col = client+(fac-1)*M;
			if (glp_mip_col_val(this->prob, col) > 0) {
				cout << "(" << client-1 << "," << glp_mip_col_val(this->prob, col) << "),";
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
