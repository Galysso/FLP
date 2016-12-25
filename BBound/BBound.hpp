#ifndef BBOUND_HPP
#define BBOUND_HPP

#include "../Donnees/Donnees.hpp"
#include "../SSCFLP/SSCFLP_GLPK.hpp"
#include "../SSCFLP/SSCFLP_HEUR.hpp"
#include "../SSCFLP/SSCFLP_SOL.hpp"

/// Critère d'arrêt de recherche dans une branche : solution relâchée trouvée supérieure à borne duale

class BBound {
	private :
		Donnees *d;
		SSCFLP_SOL *bestSol;					// Meilleure solution entière trouvée
		SSCFLP_GLPK *modelisation;			// Modélisation à laquelle on ajoute et retire des contraintes
		double bornePrimale;				// Borne supérieure (meilleure solution entière trouvée)
		double borneDuale;					// Borne inférieure du problème (solution relâchée du problème)
		
	public :
		BBound(Donnees *d);
};

#endif //BBOUND_HPP
