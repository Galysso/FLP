#ifndef SSCFLP_GLPK_HPP
#define SSCFLP_GLPK_HPP

#include <glpk.h>
#include <vector>
#include "../Donnees/Donnees.hpp"
#include "SSCFLP_SOL.hpp"

#define delta 0.000001

class SSCFLP_GLPK {
	private:
		bool entier;
		int *ia;
		int *ja;
		double *ar;
		glp_prob *prob;
		Donnees *d;
		int nbRow;
		
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
		double getZ();
		
	// Pour le branch and bound
		int getColLink(int i, int j) const;
		int getColFac(int i) const;
		int getColLinkNearestToOne() const;
		void setLinkToOne(int i);
		void setLinkToZero(int i);
		void setLinkToFree(int i);
		std::vector<int> getColsToConstraint();
		bool isSetToOne(int i);
		bool isSetToZero(int i);
};

#endif //SSCFLP_GLPK_HPP
