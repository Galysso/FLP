#include <iostream>
#include "SSCFLP.hpp"
#include "Donnees.hpp"
#include <glpk.h>
#include <stdlib.h>

using namespace std;

SSCFLP::SSCFLP(Donnees *d) {
	this->ia = NULL;
	this->ja = NULL;
	this->ar = NULL;
	this->prob = NULL;
	this->d = d;
}

SSCFLP::~SSCFLP() {
	if (prob != NULL) {
		glp_delete_prob(prob);
	}
}

void SSCFLP::glpkModeliserProbleme() {
	this->entier = true;
	
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
	
	// M le nombre de clients
	// N le nombre de facilités
	// 1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18
	// 8	9	 35   60   63   85   0    21   80   44   5    4    65   82   67   329  144  408
	// Y11, Y12, Y13, Y14, Y15, Y21, Y22, Y23, Y24, Y25, Y31, Y32, Y33, Y34, Y35, X1,  X2,  X3
	
	// La demande de chaque client est satisfaite
	for (i = 1; i <= M; ++i) {
		client = i-1;
		//~ cout << "i="<<i<< "\tborne =\tx=1.0" << endl;
		glp_set_row_bnds(this->prob, i, GLP_FX, 1.0, 1.0);
		
		for (j = 1; j <= N; ++j) {
			facil = j-1;
			col = i+(j-1)*M;
			
			//PUTAIN D'ERREUR DE MERDE GUIGNOUGUIGNOUGUIGNOU
			
			//~ cout << "j="<<col<</*"\tborne =\t0<=x<=1 (int)*/"\tcoef="<<d->coutAlloc(facil,client)<< endl;
			
			glp_set_col_kind(this->prob, col, GLP_BV);
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

		//~ cout << "j="<<col<</*"\tborne =\t0<=x<=1 (int)*/"\tcoef="<<d->coutOuverture(facil)<< endl;

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
	glp_write_lp(this->prob, NULL, "SSCFLP");
}

void SSCFLP::glpkResoudreProbleme() {
	glp_simplex(this->prob, NULL);
	if (this->entier) {
		glp_intopt(this->prob,NULL);
	}
}

/*
\item Temps de résolution : 15 secondes
\item Les facilités à construire sont : \{1,4,16,18,21,22,27,28\}
\item Les liaisons à faire sont :
\begin{itemize}
\item (1,\{1,4,22,42,52,53\})
\item (4,\{7,9,13,21,30,45\})
\item (16,\{10,11,12,19,28,39,40,43,47,51\})
\item (18,\{3,20,24,32,44,54,56,59\})
\item (21,\{8,16,26,41\})
\item (22,\{14,15,23,29,31,36,37,48,57\})
\item (27,\{0,6,18,25,33,34,46,49,55\})
\item (28,\{2,5,17,27,35,38,50,58\})
\end{itemize}
\end{itemize}
 */

void SSCFLP::glpkAfficherSolutionLatex() {
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

void SSCFLP::glpkAfficherSolution() {
	int M = this->d->getM();
	int N = this->d->getN();
	int i;
	
	double z = glp_mip_obj_val(this->prob);
	
	cout << "FACILITES :\n{";
	for (i = M*N+1; i <= M*N+N; ++i) {
		//~ cout << i << endl;
		//~ cout << glp_mip_col_val(prob, i) << ";";
		if (glp_mip_col_val(this->prob, i)) {
			cout << i-M*N-1 << ",";
		}
	}
	cout << "}"<<endl;
	
	cout << "LIAISONS :\n{";
	for (i = 1; i <= M*N; ++i) {
		if (glp_mip_col_val(this->prob, i) > 0) {
			cout <<"{"<<(i-1)/M << "," << (i-1)%M << "},";
		}
	}
	cout << "}"<<endl;
	
	cout << endl;
	cout << "Z = " << z << endl;
}

void SSCFLP::glpkAfficherModelisation() {
	int i, j;
	int M = this->d->getM();
	int N = this->d->getN();
	int taille = M*N+N*(M+1);
	double tab[1+M+N][1+taille];
	
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
}

void SSCFLP::glpkSetEntier() {
	this->entier = true;
}

void SSCFLP::glpkSetRelache() {
	this->entier = false;
}


void SSCFLP::setDonnees(Donnees *d) {
	this->d = d;
}
