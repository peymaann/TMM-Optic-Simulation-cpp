#include "tmm.h"
#include"matrix2by2.h"
#include <complex>
#include<iostream>
#include <sstream>
#include <fstream>
#include<cmath>
#include <sstream>
#include <iomanip>
//#include <filesystem>
using namespace std;

tmm::tmm() 
{

}


tmm::~tmm()
{

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

void tmm::set_lambda(double lambda)
{
	_lambda = lambda;
}

void tmm::set_msh(unsigned int mesh_size)
{
	_mesh_size = mesh_size;
}

void tmm::set_irrediance(double irrediance)
{
	_irrediance = irrediance;
}
void tmm::set_outputfile(string filename)
{
	_res_file = filename;	
}
vector<double> tmm::get_generation(void)
{
	return _Generation;
}
vector<double> tmm::get_tra()
{
	vector<double> empty;
	return empty;
}
bool tmm::chech_inputs()
{
	if (_n.size() != _mesh_size)
	{
		throw std::runtime_error("n vector dosn't match the mesh size");
		return false;
	}
	if (_k.size() != _mesh_size)
	{
		throw std::runtime_error("k vector dosn't match the mesh size");
		return false;
	}
	if (_l.size() != _mesh_size)
	{
		cout <<_l.size() <<" Vs " <<_mesh_size<<endl;
		throw std::runtime_error("length vector dosn't match the mesh size");
		return false;
	}
	if (_lambda<300 || _lambda>1200)
	{
		throw std::runtime_error("wavelength should be in range of 300-1200");
		return false;
	}
	if (_irrediance <= 0)
	{
		throw std::runtime_error("irrediance should be postive values");
		return false;
	}	
	return true;
	
}
void tmm::print_solution()
{

	//std::string result_path = "results";
	//if (!std::filesystem::exists(result_path))
    //    if (std::filesystem::create_directory(result_path))
			;
	
	std::ostringstream oss;
	oss << "#Printing the results\n";
	oss << "#E_F, E_B, I, G\n";
    for (size_t i = 0; i < _E_Forward.size(); ++i) {
        oss << abs(_E_Forward[i]) << "," << abs(_E_Backward[i]) << "," << abs(_Intensity[i]) << "," << _Generation[i] << "\n"; 
    }
	ostringstream filename;
	filename << fixed << std::setprecision(1) << "results/tmm_study_" << _lambda <<".dat";  // Set precision as needed
			//std::string result = oss.str();
	//string filename = "results/" + _res_file + ".dat";
    ofstream outFile(filename.str());
    if (!outFile) {
        throw std::runtime_error("Can't write the results.");
        return;
    }

    outFile << oss.str();
    outFile.close();

    //std::cout << "written to " << filename  << std::endl;
}


