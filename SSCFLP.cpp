#include <iostream>
#include "SSCFLP.hpp"
#include "Donnees.hpp"
#include <glpk.h>
#include <stdlib.h>

using namespace std;

SSCFLP::SSCFLP(Donnees *d) {
	this->d = d;
}

void SSCFLP::resolutionGLPK() {
	int M = d->getM();
	int N = d->getN();
	
	// Creation du problème
	glp_prob *prob;
	prob = glp_create_prob();
	glp_set_prob_name(prob, "SSCFLP");
	
	// Problème de minimisation
	glp_set_obj_dir(prob, GLP_MIN);
	
	// On a M+N contraintes 
	glp_add_rows(prob, M+N);
	
	long double *ia, *ja, *ar;
	ia = new long double [1+2*M*N];
	ja = new long double [1+2*M*N];
	ar = new long double [1+2*M*N];
	
	int pos = 1;
	
	for (int i = 0; i < M; ++i) {
		
	}
}

//~ void SSCFLP::resolutionGLPK() {
	//~ /// Chaque facilité ne fournit pas plus que sa capacité
	//~ for (int i = 1; i <= N; ++i) {
		//~ glp_set_row_bnds(prob, i, GLP_LO, d->capacite(i-1), 0.0);
	//~ }
	//~ /// Chaque demande liée à une et une seule facilité
	//~ for (int i = 1; i <= M; ++i) {
		//~ glp_set_row_bnds(prob, N+i, GLP_UP, 1.0, 1.0);
	//~ }
	//~ 
	//~ // On a N + M*N contraintes
	//~ glp_add_cols(prob, N+M*N);
	//~ 
	//~ for (int i = 1; i <= N; ++i) {
		//~ glp_set_col_kind(prob, i, GLP_BV);
		//~ glp_set_obj_coef(prob, i, d->coutOuverture(i-1));
	//~ }
	//~ 
	//~ for (int i = 0; i < N; ++i) {
		//~ for (int j = 1; j < M; ++j) {
			//~ glp_set_col_kind(prob, N+j+M*i, GLP_BV);
			//~ glp_set_obj_coef(prob, N+j+M*i, d->coutAlloc(i, j-1));
		//~ }
	//~ }
//~ }
