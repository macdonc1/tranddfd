#include "gnucap/include/c_comand.h"
#include "gnucap/include/globals.h"
#include "gnucap/include/u_parameter.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <complex>

class TRANDDFD : public CMD {

private:
	
	// S-param and frequency vectors used in VF
	std::vector<std::complex<double>> S_vector;
	std::vector<double> freq_vector;

	// Vectors for storin poles and residues from vector fitting algorithm
	std::vector<std::complex<double>> poles;
	std::vector<std::complex<double>> residues;

	// Set up poles vector with linearly spaced poles
	void get_starting_poles(int, double);

public:
	// Main function for implementing command
	void do_it(CS&, CARD_LIST*);
	
	// Read S-params from file 
	void read_S_params(std::string);

	// Run vector fitting algorithm
	void vector_fitting(int, int, double, double);
};

TRANDDFD p_tranddfd;

DISPATCHER<CMD>::INSTALL d_tranddfd(&command_dispatcher, "tranddfd", &p_tranddfd);
