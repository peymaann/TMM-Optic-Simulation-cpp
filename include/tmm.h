#ifndef TMM_H
#define TMM_H
#include"matrix2by2.h"
#include<vector>

using namespace std;
class tmm
{
	public:
		tmm();
		virtual ~tmm();
		void set_n(vector<double> n);
		void set_k(vector<double> k);
		void set_l(vector<double> l);
		void set_msh(unsigned int mesh_size);
		void set_lambda(unsigned int lambda);
		void set_esun(double ESun);
		virtual void solve() =0;
		void print_solution();
	
	protected:
		matrix2by2 get_M(double n_real,double n_imag,double length,double lambda, double kr, double phase);
		matrix2by2 get_D(double n1_real,double n1_imag,double n2_real,double n2_imag, double kr, double lambda,double mode);
		complex<double> cmlx_sqrt(complex<double> in);
		vector<double> _n;
		vector<double> _k;
		vector<double> _l;
		unsigned int _lambda;
		double _ESun2;
		vector<complex<double>> _E_Forward;
		vector<complex<double>> _E_Backward;
		vector<complex<double>> _Intensity;
		vector<double> _Generation;		
		unsigned int _mesh_size;

	
	private:
	
	

	 
};
#endif