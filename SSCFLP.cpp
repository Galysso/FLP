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
	
	// On a M*N + N variables
	glp_add_cols(prob, M*N+N);
	
	cout << "M = " << M << endl;
	cout << "N = " << N << endl;
	cout << "nombre colonne = " << 1+N+M*N << endl;
	
	int taille = M*N+N*(M+1);
	cout << "taille = " << taille << endl;
	double tab[1+M+N][1+taille];
	
	int *ia, *ja;
	double *ar;
	ia = new int [1+taille];
	ja = new int [1+taille];
	ar = new double [1+taille];
	
	int pos = 1;
	int i, j;
	int col, client, facil;
	
	// La demande de chaque client est satisfaite
	for (i = 1; i <= M; ++i) {
		client = i-1;
		
		//~ cout << "i="<<i<< "\tborne =\tx=1.0" << endl;
		glp_set_row_bnds(prob, i, GLP_FX, 1.0, 1.0);
		
		for (j = 1; j <= N; ++j) {
			facil = j-1;
			col = i+(j-1)*M;
			
			//~ cout << "j="<<col<</*"\tborne =\t0<=x<=1 (int)*/"\tcoef="<<d->coutAlloc(facil,client)<< endl;
			
			//~ glp_set_col_bnds(prob, col, GLP_DB, 0.0, 1.0);
			//~ glp_set_col_kind(prob, col, GLP_IV);
			glp_set_col_kind(prob, col, GLP_BV);
			glp_set_obj_coef(prob, col, d->coutAlloc(facil,client));
			
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
		glp_set_row_bnds(prob, i, GLP_UP, 0.0, 0.0);
		
		for (j = 1; j <= M; ++j) {
			client = j-1;
			col = (i-1-M)*M+j;//j+(i-M-1)*(M+1);
			
			ia[pos] = i;
			ja[pos] = col;
			ar[pos] = d->demande(client);
			++pos;
		}
		col = (i-M)+(j-1)*N;

		//~ cout << "j="<<col<</*"\tborne =\t0<=x<=1 (int)*/"\tcoef="<<d->coutOuverture(facil)<< endl;

		//~ glp_set_col_bnds(prob, col, GLP_DB, 0.0, 1.0);
		//~ glp_set_col_kind(prob, col, GLP_IV);
		glp_set_col_kind(prob, col, GLP_BV);
		glp_set_obj_coef(prob, col, d->coutOuverture(facil));
		
		ia[pos] = i;
		ja[pos] = col;
		ar[pos] = -d->capacite(facil);
		
		++pos;
	}
	
	// TEST TABLEAU ----------------------------------------------------
	///-----------------------------------------------------------------
	///-----------------------------------------------------------------
	for (i = 0; i <= M+N; ++i) {							///---------
		for (j = 0; j <= M*N+N; ++j) {						///---------
			tab[i][j] = 0.0;								///---------
		}													///---------
	}														///---------
	for (i = 1; i <= taille; ++i) {							///---------
		tab[ia[i]][ja[i]] = ar[i];							///---------
	}														///---------
	for (i = 0; i <= M*N+N; ++i) {							///---------
		cout << i << " ";									///---------
	}														///---------
	cout << endl;											///---------
	for (i = 1; i <= M+N; ++i) {							///---------
		cout << i << " ";									///---------
		for (j = 1; j <= M*N+N; ++j) {						///---------
			cout << tab[i][j] << " ";						///---------
		}													///---------
		cout << endl;										///---------
	}														///---------
	cout << endl;											///---------
	///-----------------------------------------------------------------
	///-----------------------------------------------------------------
	// FIN TEST --------------------------------------------------------
	
	
	// Chargement de la matrice
	glp_load_matrix(prob, taille, ia, ja, ar);
	glp_write_lp(prob, NULL, "SSCFLP_TEST");
	
	// Résolution
	glp_simplex(prob, NULL);
	glp_intopt(prob,NULL);
	double z = glp_mip_obj_val(prob);
	
	cout << "FACILITES :\n{";
	for (i = M*N+1; i <= M*N+N; ++i) {
		//~ cout << i << endl;
		//~ cout << glp_mip_col_val(prob, i) << ";";
		if (glp_mip_col_val(prob, i)) {
			cout << i-M*N-1 << ",";
		}
	}
	cout << "}"<<endl;
	
	cout << "LIAISONS :\n{";
	for (i = 1; i <= M*N; ++i) {
		if (glp_mip_col_val(prob, i) == 1) {
			cout <<"{"<< i/M << "," << i/N << "},";
		}
	}
	cout << "}"<<endl;
	
	cout << endl;
	cout << "Z = " << z << endl;
}
