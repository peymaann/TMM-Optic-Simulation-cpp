#ifndef SIMULATION_H
#define SIMULATION_H
#include<vector>
#include <string>
#include "stack_layer.h"
#include <map>
#include <utility>

using namespace std;

class simulation
{
	public:
		simulation(const vector<stack_layer>& Layers, const pair<double, double> wavelength_lim);
		simulation(const vector<stack_layer>& Layers, const string irradinace_filename, const pair<double, double> wavelength_lim);
		void init(void);
		void load_refractiveIndex(void);
		double interpolate(double x, const std::vector<double>& x_values, const std::vector<double>& y_values);
		~simulation();
		void read_irradiance_file(void);
			
		void plot_global_data();
			
		void run();

	private:
		vector<double> _wavelengths;
		vector<double> _irradiances;
		vector<stack_layer> _Layers;
		string _irradinace_filename = "materials/Sun1p5am.dat";
		size_t _mesh = 0;
		vector<double> _Generations;	
		map<double, vector<double>> _TRA;
		
		double _lower_lim;
		double _uper_lim;
		map<pair<string, double>, pair<double, double>> _material_data;

};
#endif