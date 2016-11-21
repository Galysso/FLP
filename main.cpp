#include <iostream>
#include <glpk.h>
#include "Donnees.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	Donnees d("datas/20x10_1.dat");
	
	d.afficherCoutsAllocation();
	d.afficherDemandes();
	d.afficherCoutsOuverture();
	d.afficherCapacites();
	
	return 0;
}
