#include <iostream>
#include <glpk.h>
#include <time.h>
#include "Donnees.hpp"
#include "SSCFLP.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	clock_t debut, fin;
	
	Donnees d20x10_1("datas/20x10_1.dat");
	Donnees d20x10_2("datas/20x10_2.dat");
	Donnees d20x10_3("datas/20x10_3.dat");
	Donnees d20x10_4("datas/20x10_4.dat");			// Ici, l'heuristique trouve vite la solution
	Donnees d30x15_1("datas/30x15_1.dat");
	Donnees d30x15_2("datas/30x15_2.dat");
	//~ Donnees d("datas/test.dat");
	
	//~ d.afficherCoutsAllocation();
	//~ d.afficherDemandes();
	//~ d.afficherCoutsOuverture();
	//~ d.afficherCapacites();
	
	SSCFLP sscflp(&d20x10_1);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	sscflp.glpkAfficherSolution();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	sscflp.setDonnees(&d20x10_2);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	sscflp.glpkAfficherSolution();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	sscflp.setDonnees(&d20x10_3);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	sscflp.glpkAfficherSolution();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	sscflp.setDonnees(&d20x10_4);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	sscflp.glpkAfficherSolution();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
		
	sscflp.setDonnees(&d30x15_2);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	sscflp.glpkAfficherSolution();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	sscflp.setDonnees(&d30x15_1);
	sscflp.glpkModeliserProbleme();
	debut = clock();
	sscflp.glpkResoudreProbleme();
	fin = clock();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;

	
	return 0;
}
