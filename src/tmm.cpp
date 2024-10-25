#include "tmm.h"
#include"matrix2by2.h"
#include <complex>
#include<iostream>
#include <sstream>
#include <fstream>
#include<cmath>


using namespace std;

tmm::tmm() 
{
	cout<<" tmm defined\n";
}


tmm::~tmm()
{
	cout<<" tmm removed\n";
}


matrix2by2 tmm::get_M(double n_real,double n_imag,double lenght,double lambda, double kr, double phase)
{
  matrix2by2 new_Matrix_2by2;
  complex<double> j (0,1);
  complex<double> ki ((2*M_PI*n_real)/lambda , (2*M_PI*n_imag)/lambda);
  complex<double> kz;
  kz  = ki * tmm::cmlx_sqrt(complex<double> (1.0 - (pow(kr,2)/pow(ki,2))));
  kz *= lenght;
  complex<double> ps (0,phase);

  new_Matrix_2by2.set(0,exp((-j * kz) + ps));
  new_Matrix_2by2.set(1,0);
  new_Matrix_2by2.set(2,0);
  new_Matrix_2by2.set(3,exp((j * kz) - ps));

  return(new_Matrix_2by2);
}

matrix2by2 tmm::get_D(double n1_real,double n1_imag,double n2_real,double n2_imag,double kr, double lambda, double mode)
{
  matrix2by2 new_Matrix_2by2;
  complex<double> j (0,1);
  complex<double> Ni1 (n1_real,n1_imag);
  complex<double> Ni2 (n2_real,n2_imag);
  complex<double> ki1 ((2*M_PI*n1_real)/lambda , (2*M_PI*n1_imag)/lambda);
  complex<double> ki2 ((2*M_PI*n2_real)/lambda , (2*M_PI*n2_imag)/lambda);
  ki1  = ki1 * cmlx_sqrt(complex<double> (1.0 - (pow(kr,2)/pow(ki1,2))));
  ki2  = ki2 * cmlx_sqrt(complex<double> (1.0 - (pow(kr,2)/pow(ki2,2))));
  complex<double> r,t;

  
  if (mode == 0)  //TE Mode
  {
      r   =  (ki1-ki2)/(ki1+ki2);
      t   =  (2.0*ki1)/(ki1+ki2);
  }else 
  {
      r   =   ((-ki1 * Ni2 * Ni2)+(ki2 * Ni1 * Ni1))/((ki1 * Ni2 * Ni2)+(ki2 * Ni1 * Ni1));
      t   =   ( 2.0 * ki1 * Ni1 * Ni2                )/((ki1 * Ni2 * Ni2)+(ki2 * Ni1 * Ni1));  
  }

  new_Matrix_2by2.set(0,1.0/(t));
  new_Matrix_2by2.set(1,(r)/(t));
  new_Matrix_2by2.set(2,(r)/(t));
  new_Matrix_2by2.set(3,1.0/(t));

  return(new_Matrix_2by2);
}

std::complex<double> tmm::cmlx_sqrt(std::complex<double> in)
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
void tmm::set_n(vector<double> n)
{
	_n = move(n);;
}
void tmm::set_k(vector<double> k)
{
	_k = move(k);;
}
void tmm::set_l(vector<double> l)
{
	_l = move(l);;
}

void tmm::set_lambda(unsigned int lambda)
{
	_lambda = lambda;
}

void tmm::set_msh(unsigned int mesh_size)
{
	_mesh_size = mesh_size;
}

void tmm::set_esun(double ESun)
{
	_ESun2 = ESun;
}

void tmm::print_solution()
{
	std::ostringstream oss;
	oss << "#Printing the results\n";
	oss << "#E_F, E_B, I, G\n";
    for (size_t i = 0; i < _E_Forward.size(); ++i) {
        oss << abs(_E_Forward[i]) << "," << abs(_E_Backward[i]) << "," << abs(_Intensity[i]) << "," << _Generation[i] << "\n"; 
    }
	string filename = "results/output.dat";
    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("Can't write the results.");
        return;
    }

    outFile << oss.str();
    outFile.close();

    std::cout << "Data written to " << filename << " successfully." << std::endl;
}


