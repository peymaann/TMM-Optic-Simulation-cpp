#ifndef TMM_functions_H
#define TMM_functions_H
#include "matrix2by2.h"
#include<cmath>
using namespace std;

namespace Utils_tmm {
	std::complex<double> cmlx_sqrt(std::complex<double> in)
	{
		double real_part=real(in);
		double imag_part=imag(in);
		
		if (imag_part == 0)
			imag_part = 0.0;
		if (real_part == 0)
			real_part = 0.0;
		std::complex<double> out(real_part,imag_part);
		out = sqrt(out);
		
		return(out);
	
	}
	matrix2by2 get_M(double n_real,double n_imag,double lenght,double lambda, double kr, double phase)
	{
		matrix2by2 new_Matrix_2by2;
		complex<double> j (0,1);
		complex<double> ki ((2*M_PI*n_real)/lambda , (2*M_PI*n_imag)/lambda);
		complex<double> kz;
		kz  = ki * Utils_tmm::cmlx_sqrt(complex<double> (1.0 - (pow(kr,2)/pow(ki,2))));
		kz *= lenght;
		complex<double> ps (0,phase);
		
		new_Matrix_2by2.set(0,exp((-j * kz) + ps));
		new_Matrix_2by2.set(1,0);
		new_Matrix_2by2.set(2,0);
		new_Matrix_2by2.set(3,exp((j * kz) - ps));
		
		return(new_Matrix_2by2);
	}
	
	matrix2by2 get_D(double n1_real,double n1_imag,double n2_real,double n2_imag,double kr, double lambda, double mode)
	{
		matrix2by2 new_Matrix_2by2;
		complex<double> j (0,1);
		complex<double> Ni1 (n1_real,n1_imag);
		complex<double> Ni2 (n2_real,n2_imag);
		complex<double> ki1 ((2*M_PI*n1_real)/lambda , (2*M_PI*n1_imag)/lambda);
		complex<double> ki2 ((2*M_PI*n2_real)/lambda , (2*M_PI*n2_imag)/lambda);
		ki1  = ki1 * Utils_tmm::cmlx_sqrt(complex<double> (1.0 - (pow(kr,2)/pow(ki1,2))));
		ki2  = ki2 * Utils_tmm::cmlx_sqrt(complex<double> (1.0 - (pow(kr,2)/pow(ki2,2))));
		complex<double> r,t;
		
		if (mode == 0)  //TE Mode
		{
			r   =  (ki1-ki2)/(ki1+ki2);
			t   =  (2.0*ki1)/(ki1+ki2);
		}else 
		{
			r   =   ((-ki1 * Ni2 * Ni2)+(ki2 * Ni1 * Ni1))/((ki1 * Ni2 * Ni2)+(ki2 * Ni1 * Ni1));
			t   =   ( 2.0 * ki1 * Ni1 * Ni2              )/((ki1 * Ni2 * Ni2)+(ki2 * Ni1 * Ni1));  
		}
		
		new_Matrix_2by2.set(0,1.0/(t));
		new_Matrix_2by2.set(1,(r)/(t));
		new_Matrix_2by2.set(2,(r)/(t));
		new_Matrix_2by2.set(3,1.0/(t));
		
		return(new_Matrix_2by2);
	}
	

}
#endif