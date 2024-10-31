#include<iostream>
#include <chrono>
#include "simulation.h"
#include "stack_layer.h"
#include <utility>
#include<iostream>
#include <sstream>
#include <fstream>
using namespace std;



int main()
{	try {
	
		auto start = std::chrono::high_resolution_clock::now();
	
		vector<stack_layer> Layers = {
			{"Air1", 100, "materials/Air.nk"},
			{"MAPI", 200, "materials/MAPI.nk"},
			{"Ag", 30, "materials/Ag.nk"},
			{"Air2", 100, "materials/Air.nk"}
		};
		pair<double, double> wavelength_lim;
		wavelength_lim.first = 350;
		wavelength_lim.second = 800;
		
		
		simulation dummy(Layers,wavelength_lim);
		dummy.init();
		dummy.run();
		
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