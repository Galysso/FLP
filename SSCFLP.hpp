#ifndef SSCFLP_HPP
#define SSCFLP_HPP

#include "Donnees.hpp"

class SSCFLP {
	private:
		Donnees *d;
		
	public:
		SSCFLP(Donnees *d);
		
		void resolutionGLPK();
};

#endif //SSCFLP_HPP
