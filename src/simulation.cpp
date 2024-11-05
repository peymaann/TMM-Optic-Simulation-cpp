#include "simulation.h"
#include <algorithm>
#include "tmm.h"
#include <stdexcept>
#include <fstream>
#include "externalsource.h"
#include<iostream>
#include <omp.h>

#ifndef num_threads
#define num_threads 8 
#endif

simulation::simulation(const vector<stack_layer>& Layers, const pair<double, double> wavelength_lim)
:_Layers(Layers)
{
	cout << "# Simulation model\n";
	_lower_lim = wavelength_lim.first;
	_uper_lim = wavelength_lim.second;
	if (_lower_lim > _uper_lim)
	{
		_lower_lim = wavelength_lim.second;
		_uper_lim = wavelength_lim.first;
	}
	for (const auto& layer : Layers)
	{
		cout << layer.name << ", " <<layer.thickness << ", " << layer.data_file<<endl;
	}

}

simulation::simulation(const vector<stack_layer>& Layers, const string irradinace_filename, const pair<double, double> wavelength_lim)
:_Layers(Layers),
_irradinace_filename(irradinace_filename)
{
	cout << "# Simulation model\n";
	_lower_lim = wavelength_lim.first;
	_uper_lim = wavelength_lim.second;
	if (_lower_lim > _uper_lim)
	{
		_lower_lim = wavelength_lim.second;
		_uper_lim = wavelength_lim.first;
	}
	for (const auto& layer : Layers)
	{
		cout << layer.name << ", " <<layer.thickness << ", " << layer.data_file<<endl;
	}

}
void simulation::init(void)
{
	for (const auto& layer : _Layers)
		_mesh += layer.thickness;
	this->load_refractiveIndex();
	this->read_irradiance_file();

}
simulation::~simulation()
{

}

void simulation::load_refractiveIndex(void)
{
	
	for (const auto& layer : _Layers) 
	{
		vector<double> n_data;
		vector<double> k_data;
		vector<double> wl_data;
        ifstream file(layer.data_file);
        string line;

        if (!file.is_open()) 
		{
            cerr << "Error opening file: " << layer.data_file << endl;
            continue;
        }
		while (getline(file, line)) 
		{
			if (line.empty() || line[0] == '#') {
				continue;
			}
			std::istringstream iss(line);
			double wl, n, k;
	
			if (iss >> wl >> n >> k)
				//if (wl <= _uper_lim && wl >= _lower_lim )
				{
					wl_data.push_back(wl);
					n_data.push_back(n);
					k_data.push_back(k);
					//_material_data[{layer.name, wl}] = make_pair(n,k);
				}
				

		}
		for (size_t it = _lower_lim ; it<=_uper_lim ; ++it)
		{
			double n,k;
			n = this->interpolate(it,wl_data,n_data);
			k = this->interpolate(it,wl_data,k_data);
			_material_data[{layer.name, it}] = make_pair(n,k);
		}
    
        file.close();
	}
	//auto it = _material_data.find({"MAPI", 305});
	//if (it != _material_data.end()) 
	//{
	//	cout << it->second.first << ", " << it->second.second<<endl;
	//}
	//for (const auto& entry : _material_data) {
    //    const auto& key = entry.first;   // key is a pair<string, double>
    //    const auto& value = entry.second; // value is a pair<double, double>
	//
    //    cout << "Material: " << key.first      // Access the string
    //         << ", Wavelength: " << key.second  // Access the double
    //         << " -> Value1: " << value.first   // Access the first double
    //         << ", Value2: " << value.second    // Access the second double
    //         << endl;
    //}

}

double simulation::interpolate(double x, const std::vector<double>& x_values, const std::vector<double>& y_values) {
    if (x_values.size() != y_values.size() || x_values.size() < 2) {
        throw std::invalid_argument(to_string(x_values.size())+ " Vs " + to_string(y_values.size()) + " x_values and y_values must be the same size and contain at least two points.");
    }

    if (x < x_values.front() || x > x_values.back()) {
       throw std::out_of_range("x = " + std::to_string(x) + " is out of the interpolation range.");
    }

    auto it = lower_bound(x_values.begin(), x_values.end(), x);

    if (it != x_values.end() && *it == x) {
        return y_values[it - x_values.begin()];
    }

    size_t idx1 = distance(x_values.begin(), it) - 1;
    size_t idx2 = idx1 + 1;

    double x0 = x_values[idx1], y0 = y_values[idx1];
    double x1 = x_values[idx2], y1 = y_values[idx2];

    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

void simulation::read_irradiance_file(void)
{
	vector<double> wl;
	vector<double> radiation;
	ifstream inputFile(_irradinace_filename);
    
    if (!inputFile.is_open()) {
        throw std::runtime_error("Couldn't open irradiance file");
        return;
    }

    std::string line;
	double wavelength, irradiance;
    while (std::getline(inputFile, line))
	{
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        

        if (iss >> wavelength >> irradiance) {
			wl.push_back(wavelength);
			radiation.push_back(irradiance);
        }
    }
	for (size_t it = _lower_lim ; it<=_uper_lim ; ++it)
	{
		irradiance = this->interpolate(it,wl,radiation);
		_wavelengths.push_back(it);
		_irradiances.push_back(irradiance);
	}

    inputFile.close();
}

void simulation::plot_global_data(void)
{
	// Wrting Generation values
	ostringstream oss;
	oss << "#Printing the results\n";
	oss << "#Msh_element G_Total\n";
    for (size_t i = 0; i < _mesh; ++i) {
        oss << i+1 << "," << _Generations[i] << "\n"; 
    }
	string filename = "results/total_Generation.dat";
    ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("Can't write the generation results.");
        return;
    }

    outFile << oss.str();
    outFile.close();

    std::cout << "written to " << filename  << std::endl;
	
	// Wrting TRA values
	oss.str("");
    oss.clear();
	oss << "#Printing the results\n";
	oss << "#Wavelenth Transmission Reflection Absorption\n";
    for (size_t i = 0; i < _wavelengths.size(); ++i) {
		auto it = _TRA.find(_wavelengths[i]);
        oss <<_wavelengths[i] << "," << it->second[0]<< "," << it->second[1]<< "," << it->second[2] << "\n"; 
    }
	filename = "results/TRA.dat";
    outFile.open(filename);
    if (!outFile) {
        throw std::runtime_error("Can't write the TRA results.");
        return;
    }

    outFile << oss.str();
    outFile.close();

    std::cout << "written to " << filename  << std::endl;
	
}
void simulation::run()
{
	_Generations.resize(_mesh);
	
	vector<double> l_vector(_mesh);
	fill(l_vector.begin(), l_vector.end(), 1);
	
	// main loop over wavelengths
	omp_set_num_threads(num_threads);
	size_t wavelength_size = _wavelengths.size();
	size_t completed = 0;
	//std::cout<< "Progress: 0% completed" << std::flush;
	#pragma omp parallel for
	for ( size_t i = 0 ; i  < wavelength_size ; ++i)
	{
		double wavelength = _wavelengths[i];
		vector<double> n_vector(_mesh);
		vector<double> k_vector(_mesh);
		double x_initial = 0;
		for (auto layer : _Layers)
		{
			auto it = _material_data.find({layer.name, wavelength});
			fill(n_vector.begin() + x_initial, n_vector.begin() + x_initial + layer.thickness, it->second.first );
			fill(k_vector.begin() + x_initial, k_vector.begin() + x_initial + layer.thickness, it->second.second );
			x_initial += layer.thickness;
		}
		
		
		tmm* model = new externalsource;	
		
		model->set_msh(_mesh);
		model->set_n(move(n_vector));
		model->set_k(move(k_vector));
		model->set_l(l_vector);	
		
		model->set_lambda(wavelength);
		model->set_irrediance(_irradiances[i]);
	
		
		model->solve();
		model->print_solution();
		vector<double> generation(_mesh);
		generation = model->get_generation();
		auto tra = model->get_tra();
		
		#pragma omp critical
		{
			transform(_Generations.begin(), _Generations.end(), generation.begin(), _Generations.begin(), std::plus<double>());
			_TRA[wavelength] = tra;
			completed++; 
			std::cout << "\rProgress: " << (int)(completed * 100 )/wavelength_size << " % completed" << std::flush;
		}
		
		delete  model;		
	}
	std::cout<<std::endl;
	std::cout<<"---------------------------------------------------------------"<<std::endl;
	std::cout<<std::endl;

	this->plot_global_data();
}