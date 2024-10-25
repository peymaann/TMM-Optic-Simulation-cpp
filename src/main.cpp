#include<iostream>
#include "matrix2by2.h"
#include "tmm.h"
#include "externalsource.h"
#include <complex>
#include <algorithm>
#include <utility>
#include <stdexcept>

using namespace std;

int main(void)
{	try {
	
		
		size_t mesh = 500;
		vector<double> n_vector(mesh);
		vector<double> k_vector(mesh);
		vector<double> l_vector(mesh);
		
		
		tmm* model = new externalsource;
		model->set_msh(mesh);
		

		
		fill(n_vector.begin(), n_vector.end(), 2);
		fill(k_vector.begin(), k_vector.end(), 0.001);
		fill(l_vector.begin(), l_vector.end(), 100);
		
		model->set_n(move(n_vector));
		model->set_k(move(k_vector));
		model->set_l(move(l_vector));
		model->set_lambda(300);
		model->set_esun(1.0);
		
		
		model->solve();
		model->print_solution();
		
		delete  model;
	}catch (...) {
        cout << "Caught an unknown exception." << endl;
    }
	
	return 0;
}