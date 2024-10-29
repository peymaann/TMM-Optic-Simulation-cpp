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
		virtual void solve() = 0;
		virtual void print_solution();
	protected:
		bool chech_inputs();
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