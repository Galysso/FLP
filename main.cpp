#include <iostream>
#include <glpk.h>
#include <time.h>
#include "Donnees/Donnees.hpp"
#include "SSCFLP/SSCFLP_GLPK.hpp"
#include "SSCFLP/SSCFLP_SOL.hpp"
#include "SSCFLP/SSCFLP_HEUR.hpp"
#include "BBound/BBound.hpp"

/**
 * UFLP   : Facilities can satisfy de whole demand of every clients
 * CFLP   : Clients can be linked to multiple facilities
 * SSCFLP : Clients can be linked to one and only one facility
 */

using namespace std;

int main(int argc, char* argv[]) {
	clock_t debut, fin;
	
	//~ Donnees d("datas/test.dat");
	Donnees d("datas/20x10_1.dat");		// p1
	//~ Donnees d("datas/20x10_2.dat");		// p3
	//~ Donnees d("datas/20x10_3.dat");		// p4
	//~ Donnees d("datas/20x10_4.dat");		// p5
	//~ Donnees d("datas/30x15_1.dat");			// p11
	//~ Donnees d("datas/30x15_2.dat");		// p14
	//~ Donnees d("datas/40x20.dat");		// p25	// valeur optimale trouvée une fois
	//~ Donnees d("datas/50x20.dat");		// p32
	//~ Donnees d("datas/60x30.dat");		// p35
	//~ Donnees d("datas/75x30.dat");		// p45
		
	BBound bbound(&d);
	//~ bbound.testP1();
	debut = clock();
	bbound.search();
	fin = clock();	
	cout << "temps : " << (fin-debut)/CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
