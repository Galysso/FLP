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
		~SSCFLP();
		void resolutionGLPK();
		void glpkModeliserProbleme();
		void glpkResoudreProbleme();
		void glpkAfficherSolution();
		void glpkAfficherModelisation();
		void glpkSetEntier();
		void glpkSetRelache();
		void glpkDelete();
		
		void setDonnees(Donnees *d);
};

#endif //SSCFLP_HPP
