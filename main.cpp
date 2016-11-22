#include <iostream>
#include <glpk.h>
#include "Donnees.hpp"
#include "SSCFLP.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	//~ Donnees d("datas/20x10_1.dat");
	Donnees d("datas/20x10_2.dat");
	//~ Donnees d("datas/test.dat");
	
	d.afficherCoutsAllocation();
	d.afficherDemandes();
	d.afficherCoutsOuverture();
	d.afficherCapacites();
	
	SSCFLP sscflp(&d);
	sscflp.resolutionGLPK();
	
	return 0;
}
