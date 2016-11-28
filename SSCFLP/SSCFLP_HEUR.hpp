#ifndef SSCFLP_HEUR_HPP
#define SSCFLP_HEUR_HPP

#include "../Donnees/Donnees.hpp"

/** Construction
 * A chaque etape :
 * 	- Une facilité est ouverte
 * 	- Des clients non déjà assignés y sont assignés de manière gloutonne
 *    en respectant la contrainte de capacité
 *
 ** 
 */

class SSCFLP_HEUR {
	private:
		Donnees *d;
		double **deltas;	// delta[i][j] = c[i][j] - min(c[*][j]),    le client j fixé
		double *efficacites;// (cOuverture[i] + Ej(deltas[i][j])) / M
		int *indicesEfficacites; // Les indices des efficacités triées
		int *facilites;
		int **liaisons;
		bool *clientsAssignes;
		bool *facilitesOuvertes;
		double *capacites;
		
	public:
		SSCFLP_HEUR(Donnees *d);
		
	private:
		void construction();
		void initialisation();
};

#endif //SSCFLP_HEUR_HPP
