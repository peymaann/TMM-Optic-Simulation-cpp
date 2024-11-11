#include<iostream>
#include <chrono>
#include "simulation.h"
#include "stack_layer.h"
#include <memory>
using namespace std;
int main()
{	try {
	
		auto start = std::chrono::high_resolution_clock::now();
		
		vector<stack_layer> Layers = {
			{"Air1", 1000, "materials/Air.nk"},
			{"MAPI", 15000, "materials/MAPI.nk"},
			{"Ag", 100, "materials/Ag.nk"},
			{"Air2", 1000, "materials/Air.nk"}
		};
		string irradinace_filename = "materials/Sun1p5am.dat";
		
		pair<int, int> wavelength_lim(300,950);
		
		unique_ptr<simulation> dummy = make_unique<simulation>(Layers,irradinace_filename, wavelength_lim);
		
		dummy->init();
		dummy->run();
		
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	}
	catch (const exception& e) {
        cout << "Caught exception: " << e.what() << std::endl;
    }
	catch (...) {
        cout << "Caught an exception." << endl;
    }
	
	return 0;
}