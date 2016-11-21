#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <fstream>
#include "Donnees.hpp"


/**
 * Format des fichiers :
 * 
 * - Nombre de clients (m), nombre de locations (n)
 * - Matrice de taille m x n des coûts de liaison
 * - Vecteur des demandes des clients
 * - Vecteur des coûts d'installation d'une location
 * - Vecteur des capacités d'une location
 */

class Parser {
	public:
		// Constructeur vide
		Parser();
		
		// Lecture du fichier analyse création des données
		void lecture(Donnees *d, std::string nomFichier) const;
};

#endif //PARSER_HPP
