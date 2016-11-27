#include <iostream>
#include <glpk.h>
#include <time.h>
#include "Donnees.hpp"
#include "SSCFLP.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	clock_t debut, fin;
	
	//~ Donnees d("datas/75x30.dat");
	Donnees dp1("datas/20x10_1.dat");
	Donnees dp3("datas/20x10_2.dat");
	Donnees dp4("datas/20x10_3.dat");
	Donnees dp5("datas/20x10_4.dat");
	Donnees dp11("datas/30x15_1.dat");
	Donnees dp14("datas/30x15_2.dat");
	Donnees dp25("datas/40x20.dat");
	Donnees dp32("datas/50x20.dat");
	Donnees dp35("datas/60x30.dat");
	Donnees dp45("datas/75x30.dat");
	
	//~ d.afficherCoutsAllocation();
	//~ d.afficherDemandes();
	//~ d.afficherCoutsOuverture();
	//~ d.afficherCapacites();
	
	//~ cout << "Résolution de P1" << endl;
	//~ SSCFLP sscflp1(&dp1);
	//~ sscflp1.glpkModeliserProbleme();
	//~ debut = clock(); sscflp1.glpkResoudreProbleme(); fin = clock();
	//~ sscflp1.glpkAfficherSolutionLatex();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	//~ 
	//~ cout << "Résolution de P3" << endl;
	//~ SSCFLP sscflp3(&dp3);
	//~ sscflp3.glpkModeliserProbleme();
	//~ debut = clock(); sscflp3.glpkResoudreProbleme(); fin = clock();
	//~ sscflp3.glpkAfficherSolutionLatex();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	//~ 
	//~ cout << "Résolution de P4" << endl;
	//~ SSCFLP sscflp4(&dp4);
	//~ sscflp4.glpkModeliserProbleme();
	//~ debut = clock(); sscflp4.glpkResoudreProbleme(); fin = clock();
	//~ sscflp4.glpkAfficherSolutionLatex();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	//~ 
	//~ cout << "Résolution de P5" << endl;
	//~ SSCFLP sscflp5(&dp5);
	//~ sscflp5.glpkModeliserProbleme();
	//~ debut = clock(); sscflp5.glpkResoudreProbleme(); fin = clock();
	//~ sscflp5.glpkAfficherSolutionLatex();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	//~ cout << "Résolution de P11" << endl;
	//~ SSCFLP sscflp11(&dp11);
	//~ sscflp11.glpkModeliserProbleme();
	//~ debut = clock(); sscflp11.glpkResoudreProbleme(); fin = clock();
	//~ sscflp11.glpkAfficherSolutionLatex();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	//~ delete &sscflp11;
	
	//~ cout << "Résolution de P14" << endl;
	//~ SSCFLP sscflp14(&dp14);
	//~ sscflp14.glpkModeliserProbleme();
	//~ debut = clock(); sscflp14.glpkResoudreProbleme(); fin = clock();
	//~ sscflp14.glpkAfficherSolutionLatex();
	//~ cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	cout << "Résolution de P25" << endl;
	SSCFLP sscflp25(&dp25);
	sscflp25.glpkModeliserProbleme();
	debut = clock(); sscflp25.glpkResoudreProbleme(); fin = clock();
	sscflp25.glpkAfficherSolutionLatex();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	cout << "Résolution de P32" << endl;
	SSCFLP sscflp32(&dp32);
	sscflp32.glpkModeliserProbleme();
	debut = clock(); sscflp32.glpkResoudreProbleme(); fin = clock();
	sscflp32.glpkAfficherSolutionLatex();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	cout << "Résolution de P35" << endl;
	SSCFLP sscflp35(&dp35);
	sscflp35.glpkModeliserProbleme();
	debut = clock(); sscflp35.glpkResoudreProbleme(); fin = clock();
	sscflp35.glpkAfficherSolutionLatex();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	cout << "Résolution de P45" << endl;
	SSCFLP sscflp45(&dp45);
	sscflp45.glpkModeliserProbleme();
	debut = clock(); sscflp45.glpkResoudreProbleme(); fin = clock();
	sscflp45.glpkAfficherSolutionLatex();
	cout << "Résolution en " << (fin-debut)*1000/CLOCKS_PER_SEC << " millisecondes" << endl << endl;
	
	return 0;
}
