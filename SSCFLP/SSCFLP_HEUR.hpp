#ifndef SSCFLP_HEUR_HPP
#define SSCFLP_HEUR_HPP

#include "../Donnees/Donnees.hpp"
#include "SSCFLP_SOL.hpp"

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
		double z;
		double **deltas;		// delta[i][j] = c[i][j] - min(c[*][j]),    le client j fixé
		int **indicesDeltas;	// Les indices des deltas triés
		double *efficacites;	// (cOuverture[i] + Ej(deltas[i][j])) / M
		int *indicesEfficacites;// Les indices des efficacités triées
		bool *facilites;
		bool **liaisons;
		bool *clientsAssignes;
		double *capacites;
		
	public:
		SSCFLP_HEUR(Donnees *d);
		SSCFLP_SOL getSolution();
		
		void afficherDeltas();
		void afficherEfficacites();
		void afficherSolution();
		
	private:
		void construction();
		void initialisation();
		void setEfficacites();
		void trierDeltas();
		void trierEfficacites();
		int getBestFacilite();
};

#endif //SSCFLP_HEUR_HPP
