#ifndef SSCFLP_GLPK_HPP
#define SSCFLP_GLPK_HPP

#include <glpk.h>
#include <vector>
#include "../Donnees/Donnees.hpp"
#include "SSCFLP_SOL.hpp"

class SSCFLP_GLPK {
	private:
		bool entier;		// Vrai indique qu'il s'agît de la résolution exacte, faux pour la résolution relâchée
		int *ia;			// Le tableau des lignes de GLPK (contraintes)
		int *ja;			// Le tableau des colonnes de GLPK (variables)
		double *ar;			// Le tableau des coefficients des variables de GLPK
		glp_prob *prob;		// Le problème GLPK
		Donnees *d;			// L'instance du problème
		
	public:
		SSCFLP_GLPK(Donnees *d);
		~SSCFLP_GLPK();
		/** Modélise le problème */
		void glpkModeliserProbleme();
		/** Résout le problème modélisé */
		void glpkResoudreProbleme();
		/** Affiche la solution trouvée par GLPK */
		void glpkAfficherSolution();
		/** Set le booléen entier à vrai (une nouvelle modélisation est nécessaire pour prendre effet) */
		void glpkSetEntier();
		/** Set le booléen entier à faux (une nouvelle modélisation est nécessaire pour prendre effet) */
		void glpkSetRelache();
		/** Détruit la structure de GLPK */
		void glpkDelete();
		/** Modifie l'instance */
		void setDonnees(Donnees *d);
		/** Renvoie la solution dans un format spécifique */
		SSCFLP_SOL getSolution();
		/** Renvoie la valeur de la fonction objectif */
		double getZ();
		
	// Pour le branch and bound
		/** Renvoie l'indice de la colonne correspondant à la liaison entre la facilité i et le client j */
		int getColLink(int i, int j) const;
		/** Renvoie l'indice de la colonne correspondant à l'ouverture de la facilité i */
		int getColFac(int i) const;
		/** Renvoie l'indice de la colonne correspondant à l'ouverture de la facilité désignée par la colonne de liaison d'indice indLink */
		int getColFacFromLink(int indLink) const;
		/** Renvoie l'indice de la facilité désignée par la colonne de la liaison d'indice indLink */
		int getFacFromLink(int indLink) const;
		/** Renvoie l'indice de la colonne dont la valeur est non entière et la plus proche de 1 */
		int getColLinkNearestToOne() const;
		/** Fixe la colonne i à 1 et fixe l'ouverture de la facilité correspondante à 1 si nécessaire */
		void setLinkToOne(int i, int nbUn);
		/** Fixe la colonne i à 0 et fixe l'ouverture de la facilité correspondante à 0 si nécessaire */
		void setLinkToZero(int i, int nbUn, int nbZero);
		/** Libère la colonne i et libère l'ouverture de la facilité correspondante si nécessaire */
		void setLinkToFree(int i, int nbZero);
		/** Renvoie vrai si la colonne i est fixée à 1 */
		bool isSetToOne(int i);
		/** Renvoie vrai si la colonne i est fixée à 0 */
		bool isSetToZero(int i);
};

#endif //SSCFLP_GLPK_HPP
