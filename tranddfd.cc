#include "tranddfd.h"

// This function is the main function call of the tranddfd command
//
// Its main purpose is to read in data and call other functions
void TRANDDFD::do_it(CS& Cmd, CARD_LIST* Scope)
{
	
	// TODO: Take out before anyone sees!	
	std::cout << "\n\nSanity Check Print!\n" << std::endl;
	
	// Use complex literals so I don't needd to declare complex variable j
	using namespace std::complex_literals;

	// Take the name of an S-parameter file as an argument from Gnucap
	// Will eventually need to add more flexibility here
	// Reading in S-param file is good as it means we can treat anything 
	// as defined in freq domain
	//
	//TODO:
	//	Make it so that S-param files with multiple ports can be inputted.
	//	(Currently just 1 port networks supported)
	//
	// Stretch goal would be to integrate the S-param sweep Felix has in 
	// Gnucsator as a prelim step to running this code
	std::string S_params_name;
	Cmd >> S_params_name;


	// Call to function call to read S-parameter file
	read_S_params(S_params_name);

	// Initialize VF poles
	get_starting_poles(30, 0.01);

	//std::cout << "S Parameters!" << std::endl;

	//for(std::complex<double> s_val : S_vector )
	//{
	//	
	//	std::cout << s_val << std::endl;
	//
	//}
	
	std::cout << "\n\nPoles!\n" << std::endl;

	for(std::complex<double> pole : poles)
	{
		std::cout << pole << std::endl;
	}

}

void TRANDDFD::read_S_params(std::string S_params_name)
{


	// Open the S-param file
	std::fstream S_param_file;
	S_param_file.open(S_params_name);

	// Variable for storing raw text input fropm S-param file
	std::string S_param_text;

	// Temporary variable to hold numerals in S-param file
	double currNum;

	// Temporary variable to hold complex S-param value before stoored in vector
	std::complex<double> S_val;

	// Variable used to keep track of which value is being input
	int valueIndex = 0;

	// Variables for storing real and imaginary parts of S-parameters
	double real;
	double imag;

	// Read file line by line
	//
	// TODO:
	// 	Add support for files that have text headers. Many touchstone files have this.
	while( std::getline( S_param_file, S_param_text ) )
	{
		
		// Format text to be read in word by word		
		std::istringstream iss(S_param_text);
		
		// Read in word by word from file
		while( iss >> currNum )
		{

			switch (valueIndex) {
				case 0:
					// Store frequency values in frequency vector
					freq_vector.push_back(currNum);
					valueIndex++;
					break;	
				case 1:
					// Store real part of S-params in variable and wait for imaginary part
					real = currNum;
					valueIndex++;
					break;
				case 2:
					// Store imaginary part of S-param
					// Add real part and store in S-param vector
					imag = currNum;

					S_val = real + (1j * imag);
					S_vector.push_back(S_val);
					valueIndex = 0;
					break;
			}

		}

	}	

	// Close the file
	S_param_file.close();

}


void TRANDDFD::vector_fitting( int n_bar, int max_iters, double e_w, double e_h )
{

	

}


void TRANDDFD::get_starting_poles(int n_bar, double alpha)
{

	// TODO: handle uneven number of starting poles
	int n_bar_conjugates = n_bar / 2;

	std::complex<double> complex_frequency;

	if(freq_vector[0] == 0)
	{
		for(int n = 0; n < n_bar_conjugates; n++)
		{
			complex_frequency = (-alpha + 1j) * ((freq_vector.back() / n_bar_conjugates) * n);
			
			poles.push_back(complex_frequency);

			poles.push_back(conj(complex_frequency));

		}
	}
	else
	{

		for(int n = 0; n < n_bar_conjugates; n++)
		{

			complex_frequency = (-alpha + 1j) * (freq_vector.front() 
					+ ((freq_vector.back() - freq_vector.front())
					/ (n_bar_conjugates - 1))) * (n - 1);

			poles.push_back(complex_frequency);
			poles.push_back(conj(complex_frequency));	

		}

	}

}

