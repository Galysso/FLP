#ifndef toto
#define toto

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <glpk.h>

/**
 * typedef struct {
 * int nbvar; // n
 * int nbcontr; // m
 * int *couts; // Tableau des c j
 * int **contr; // Tableau de tableaux indiquant les
 * indices des variables dans chaque contrainte
 * int *sizeContr; // Tableau des nombres de variables
 * dans chacune des contraintes
 * int *droite; // Tableau des b i
 * } donnees;
 **/

using namespace std;

class Coco {
	public:
		void creerGLPK();
};

void Coco::creerGLPK() {
	int M = 10;
	int N = 5;
	double couts[M][N];
	double capacites[N];
	double demandes[M];
	
	
	glp_prob *prob;
	prob = glp_create_prob();
	glp_set_prob_name(prob, "SSCFLP");
	glp_set_obj_dir(prob, GLP_MIN);
	glp_add_rows(prob, M+N);
	for (int i = 1; i <= N; ++i) {
		glp_set_row_bnds(prob, i, GLP_LO, capacites[i-1], 0.0);
	}
	for (int i = N+1; i <= M; ++i) {
		glp_set_row_bnds(prob, i, GLP_UP, demandes[i-N-1], 0.0);
	}
}

int main() {
	
}

#endif
