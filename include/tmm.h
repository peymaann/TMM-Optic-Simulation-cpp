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
		void set_lambda(double lambda);
		void set_irrediance(double ESun);
		void set_outputfile(string filename);
		vector<double> get_generation(void);
		virtual void solve() = 0;
		virtual void print_solution();
		virtual vector<double> get_tra();
	protected:
		bool chech_inputs();
		vector<double> _n;
		vector<double> _k;
		vector<double> _l;
		double _lambda;
		double _irrediance;
		vector<complex<double>> _E_Forward;
		vector<complex<double>> _E_Backward;
		vector<complex<double>> _Intensity;
		vector<double> _Generation;		
		unsigned int _mesh_size;
		string _res_file = "output";

	
	private:
	
	

	 
};
#endif