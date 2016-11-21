#ifndef DONNEES_HPP
#define DONNEES_HPP

#include <string>

class Donnees {
	private:
		int M;							/// Nombre de clients
		int N;							/// Nombre de locations
		long double **coutsAllocation;	/// Vecteur des coûts de liaison
		long double *demandes;			/// Vecteur des demandes des clients
		long double *coutsOuverture;	/// Vecteur des coûts des ouverture
		long double *capacites;			/// Vecteur des capacités des locations
		
	public:
		// Constructeur vide, nécessite un parser
		Donnees(std::string fichier);
		
		// Setters
		void setM(int M);
		void setN(int N);
		void setCoutsAllocation(long double **coutsAllocation);
		void setDemandes(long double *demandes);
		void setCoutsOuverture(long double *coutsOuverture);
		void setCapacites(long double *capacites);
		
		// Affichages
		void afficherCoutsAllocation() const;
		void afficherDemandes() const;
		void afficherCoutsOuverture() const;
		void afficherCapacites() const;
};

#endif //DONNEES_HPP
