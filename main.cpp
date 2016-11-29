#include <iostream>
#include <glpk.h>
#include <time.h>
#include "Donnees/Donnees.hpp"
#include "SSCFLP/SSCFLP_GLPK.hpp"
#include "SSCFLP/SSCFLP_SOL.hpp"
#include "SSCFLP/SSCFLP_HEUR.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	clock_t debut, fin;
	
	//~ Donnees d("datas/test.dat");
	//~ Donnees d("datas/20x10_1.dat");
	Donnees d("datas/20x10_2.dat");
	//~ Donnees d("datas/20x10_3.dat");
	//~ Donnees d("datas/20x10_4.dat");
	//~ Donnees d("datas/30x15_1.dat");
	//~ Donnees d("datas/30x15_2.dat");
	//~ Donnees d("datas/40x20.dat");
	//~ Donnees d("datas/50x20.dat");
	//~ Donnees d("datas/60x30.dat");
	//~ Donnees d("datas/75x30.dat");
	
	//~ d.afficherCoutsAllocation();
	//~ d.afficherDemandes();
	//~ d.afficherCoutsOuverture();
	//~ d.afficherCapacites();
	
	SSCFLP_GLPK sscflp_glpk(&d);
	sscflp_glpk.glpkSetRelache();
	sscflp_glpk.glpkModeliserProbleme();
	debut = clock(); sscflp_glpk.glpkResoudreProbleme(); fin = clock();
	//~ sscflp_glpk.glpkAfficherSolution();
	SSCFLP_SOL sol_glpk = sscflp_glpk.getSolution();
	//~ sol.afficherSol();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;

	SSCFLP_HEUR sscflp_heur(&d);
	SSCFLP_SOL sol_heur = sscflp_heur.getSolution();
	sol_heur.afficherSol();
	sol_glpk.afficherSol();

	return 0;
}
