#ifndef BBOUND_HPP
#define BBOUND_HPP

#include <vector>
#include "../Donnees/Donnees.hpp"
#include "../SSCFLP/SSCFLP_GLPK.hpp"
#include "../SSCFLP/SSCFLP_HEUR.hpp"
#include "../SSCFLP/SSCFLP_SOL.hpp"

/// Critère d'arrêt de recherche dans une branche : solution relâchée trouvée supérieure à borne duale

//~ (M*N)*(M*N)
class BBound {
	private :
		Donnees *d;
		SSCFLP_SOL bestSol;							// Meilleure solution entière trouvée
		SSCFLP_GLPK *modelisation;					// Modélisation dont l'on modifie les bornes des variables
		int profondeur;								// Profondeur actuelle dans l'arborescence
		std::vector<int> traceur;					// Les indices des variables à contraindre
		double bornePrimale;						// Borne supérieure (meilleure solution entière trouvée)
		double borneDuale;							// Borne inférieure du problème (solution relâchée du problème)		
		std::vector<int> openFacil;					// Facilités ouvertes par le branch & bound
		std::vector<int> closeFacil;				// Facilités ouvertes par le branch & bound
		
	public :
		BBound(Donnees *d);
		~BBound();
		/** Cherche la solution optimale du problème avec l'algorithme de Branch & Bound */
		void search();
		
	private :
		/** Place la colonne à modifier à la profondeur courante */
		void setTraceur();
		/** Renvoie vrai si la solution courante est inclus dans ]0, bornePrimale[ */
		bool solutionValide();
		/** Renvoie la colonne à modifier à la profondeur courante */
		int getCol();
		/** Effectue les tâches correspondant au cas où la solution relâchée est entière */
		void casSolutionEntiere();
		/** Effectue les tâches correspondant au cas où la colonne à la profondeur courante est fixée à 1 */
		void casColonneUn(int col);
		/** Effectue les tâches correspondant au cas où la colonne à la profondeur courante est fixée à 0 */
		void casColonneZero(int col);
		/** Effectue les tâches correspondant au cas où la colonne à la profondeur courante est libre */
		void casColonneLibre(int col);
		/** Calcule la relaxation du problème avec la modélisation courante */
		bool calculerRelaxation();
};

#endif //BBOUND_HPP
