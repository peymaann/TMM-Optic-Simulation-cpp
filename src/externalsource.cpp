#include "externalsource.h"
#include "matrix2by2.h"
#include "Utils_tmm.h"
#include <iostream>
using namespace std;

#define c0 (2.998e8 * 1e9)
#define e0 8.85e-12
#define plank_const 1.055e-34

              

externalsource::externalsource()
{

}

externalsource::~externalsource()
{

}

vector<double> externalsource::get_tra()
{
	//if (isnan(this->_T)) this->_T = 0.0;
	//if (isnan(this->_R)) this->_R = 0.0;
	//if (isnan(this->_A)) this->_A = 1.0;
	vector<double> res = {_T,_R,_A};
	return res;
}
void externalsource::solve()
{
	if (!chech_inputs())
		return;
	matrix2by2 DD;
	matrix2by2 MM;
	matrix2by2 TT_load;
	matrix2by2 T(1,0,0,1);
    matrix2by2 E_N(1,0,0,0);
	matrix2by2 E_I;
	vector<complex<double>> E_F;
	vector<complex<double>> E_B;	
	size_t N0 = check_highly_absobing();
	if (N0 != 0)
		for (size_t i = 0 ; i< N0; ++i)
		{
			E_F.push_back(0);
			E_B.push_back(0);
		}
	E_F.push_back(1);
    E_B.push_back(0);
	for (size_t i = _mesh_size - N0 - 2; i < _mesh_size; --i)
    {	
        DD = Utils_tmm::get_D(_n[i],-_k[i],_n[i+1],-_k[i+1],0,_lambda,0);
        MM = Utils_tmm::get_M(_n[i],_k[i],_l[i],_lambda,0,0);
        TT_load = DD * T;
        T = MM* TT_load;
        E_I = T * E_N  ;
        E_F.push_back(E_I.get(0));
        E_B.push_back(E_I.get(2));
    }
	for(size_t i=0; i < _mesh_size ; ++i)
    {
        _E_Forward.push_back(E_F[_mesh_size-1-i] /E_F[_mesh_size-1]);
        _E_Backward.push_back(E_B[_mesh_size-1-i]/E_F[_mesh_size-1]);
    }
	
	double w = 2 * M_PI * c0 / _lambda;
	double ESun2 = 2 *	(_irrediance / 1e3)/ (c0 * 1e-9 * e0);
	
	for(size_t i=0; i < _mesh_size ; ++i)
    {
		complex<double> Etot = _E_Forward[i] + _E_Backward[i];
		_Intensity.push_back(0.5 * c0 * 1e-9 * e0 * _n[i] * ESun2* pow(abs(Etot), 2));
		_Generation.push_back(1 / (plank_const* w) * (4 * M_PI * _k[i] * 1e7/(_lambda)) * real(_Intensity.back())/ 1e4 );
    }
	
	_R = real( pow(abs(T.get(2)/T.get(0)),2));
	complex<double> RI_first_elem(_n[0],_k[0]);
	complex<double> RI_last_elem(_n[_mesh_size-1],_k[_mesh_size-1]);
	_T = real((RI_last_elem / RI_first_elem) * pow(abs(1.0/T.get(0)),2));
	_A = (1-_T-_R);
	
}
int externalsource::check_highly_absobing()
{
	size_t N0 = 0;
    matrix2by2 DD;
    matrix2by2 MM;
    matrix2by2 TT_load;
    matrix2by2 TT(1,0,0,1);
    for (size_t i = 0 ; i <= _mesh_size-2 ; ++i)
    {
		DD = Utils_tmm::get_D(_n[i],_k[i],_n[i+1],_k[i+1],0,_lambda,0);
		MM = Utils_tmm::get_M(_n[i],_k[i],_l[i],_lambda,0,0);
		TT_load = MM * DD;
		TT = TT * TT_load ;
		if (real(abs(TT.get(0))) > 1e120)
		{
			N0 = _mesh_size - i;
			return N0; //number of elements from back sild that will be iqnored
		}
    }
	return 0;
}


