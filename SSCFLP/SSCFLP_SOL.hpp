#ifndef SSCFLP_SOL_HPP
#define SSCFLP_SOL_HPP

class SSCFLP_SOL {
	private:
		int N;
		int M;
		double z;
		double *facilites;
		double **liaisons;
		
	public:
		SSCFLP_SOL();
		SSCFLP_SOL(int N, int M, double z, double *facilites, double **liaisons);
		int getN();
		int getM();
		double getZ();
		double *getFacilites();
		double **getLiaisons();
		int setN(int N);
		int setM(int M);
		double setZ(double z);
		double *setFacilites(double *facilites);
		double **setLiaisons(double **facilites);
		void afficherSol();
};


#endif //SSCFLP_SOL_HPP
