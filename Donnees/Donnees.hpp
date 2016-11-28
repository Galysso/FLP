#ifndef DONNEES_HPP
#define DONNEES_HPP

#include <string>

class Donnees {
	private:
		int M;							/// Nombre de clients
		int N;							/// Nombre de locations
		double **coutsAllocation;	/// Vecteur des coûts de liaison
		double *demandes;			/// Vecteur des demandes des clients
		double *coutsOuverture;	/// Vecteur des coûts des ouverture
		double *capacites;			/// Vecteur des capacités des locations
		
	public:
		// Constructeur vide, nécessite un parser
		Donnees(std::string fichier);
		
		// Setters
		void setM(int M);
		void setN(int N);
		void setCoutsAllocation(double **coutsAllocation);
		void setDemandes(double *demandes);
		void setCoutsOuverture(double *coutsOuverture);
		void setCapacites(double *capacites);
		
		// Getters
		int getM() const;
		int getN() const;
		double coutAlloc(int i, int j) const;
		double demande(int i) const;
		double coutOuverture(int i) const;
		double capacite(int i) const;
		
		// Affichages
		void afficherCoutsAllocation() const;
		void afficherDemandes() const;
		void afficherCoutsOuverture() const;
		void afficherCapacites() const;
};

#endif //DONNEES_HPP
