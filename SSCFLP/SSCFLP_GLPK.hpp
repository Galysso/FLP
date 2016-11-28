#ifndef SSCFLP_GLPK_HPP
#define SSCFLP_GLPK_HPP

#include "../Donnees/Donnees.hpp"
#include "SSCFLP_SOL.hpp"
#include <glpk.h>

class SSCFLP_GLPK {
	private:
		bool entier;
		int *ia;
		int *ja;
		double *ar;
		glp_prob *prob;
		Donnees *d;
		
	public:
		SSCFLP_GLPK(Donnees *d);
		~SSCFLP_GLPK();
		void resolutionGLPK();
		void glpkModeliserProbleme();
		void glpkResoudreProbleme();
		void glpkAfficherSolutionLatex();
		void glpkAfficherSolution();
		void glpkSetEntier();
		void glpkSetRelache();
		void glpkDelete();
		void setDonnees(Donnees *d);
		SSCFLP_SOL getSolution();
};

#endif //SSCFLP_GLPK_HPP
