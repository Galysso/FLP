#ifndef SSCFLP_HPP
#define SSCFLP_HPP

#include "Donnees.hpp"
#include <glpk.h>

class SSCFLP {
	private:
		bool entier;
		int *ia;
		int *ja;
		double *ar;
		glp_prob *prob;
		Donnees *d;
		
	public:
		SSCFLP(Donnees *d);
		void resolutionGLPK();
		void glpkModeliserProblemeEntier();
		void glpkResoudreProbleme();
		void glpkAfficherSolution();
		void glpkAfficherModelisation();
};

#endif //SSCFLP_HPP
