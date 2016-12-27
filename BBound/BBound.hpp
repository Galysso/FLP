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
		SSCFLP_GLPK *modelisation;					// Modélisation à laquelle on ajoute et retire des contraintes
		int profondeur;								// Profondeur actuelle dans l'arborescence
		std::vector<std::vector<int> > colsToBound;	// Les indices des variables à contraindre
		std::vector<int> traceur;					// Liste du fils actuellement visité au ième noeud;
		double bornePrimale;						// Borne supérieure (meilleure solution entière trouvée)
		double borneDuale;							// Borne inférieure du problème (solution relâchée du problème)
		
	public :
		BBound(Donnees *d);
		~BBound();
		void search();
		void testP1();
		
	private :
		void setColsToBound();
		bool solutionValide();
		int getCol();
		void casSolutionEntiere();
		void casColonneUn(int col);
		void casColonneZero(int col);
		void casColonneLibre(int col);
		bool calculerRelaxation();
};

#endif //BBOUND_HPP
