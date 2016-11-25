#include <iostream>
#include <glpk.h>
#include <time.h>
#include "Donnees.hpp"
#include "SSCFLP.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	clock_t debut, fin;
	
	Donnees d("datas/75x30.dat");
	//~ d.afficherCoutsAllocation();
	//~ d.afficherDemandes();
	//~ d.afficherCoutsOuverture();
	//~ d.afficherCapacites();
	
	SSCFLP sscflp(&d);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	sscflp.glpkAfficherSolution();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
		
	return 0;
}
