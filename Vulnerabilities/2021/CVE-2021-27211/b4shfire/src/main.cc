// © 2021 Lorian Richmond

#include "ui.hh"
#include "utils.hh"
#include "ExtractedData.hh"
#include "file_handling.hh"

#include <math.h>
#include <fstream>
#include <iostream>

using namespace std;


void display_usage(string run_path){

	cout << "Usage: " << run_path << " <filepath> <number of threads> (default: 4)" << endl;
}


int main(int argc, char** argv){

	string filename;
	int num_threads = 4;

	switch (argc){

		case 3:
			num_threads = atoi(argv[2]);

		case 2:
			filename = argv[1];
			break;

		default:
			display_usage(argv[0]);
			return 1;
	}


	if (num_threads < 1 || num_threads > 100){

		cout << "Number of threads must be between 1 and 100" << endl;
	}


	cout << "Opening file...";
	FILE* p_file = fopen(filename.c_str(), "rb");

	if (p_file == NULL){

		cerr << endl << "Error: file could not be opened" << endl;
		return 1;
	}
	cout << " done." << endl;

	cout << "Reading file data...";
	const vector<bool> bits = file_handling::auto_load(p_file);
	cout << " done." << endl;

	cout << "Brute-forcing seeds...";
	const vector<uint32_t> valid_seeds = utils::find_valid_seeds(bits, num_threads);
	cout << " done." << endl;

	cout << "Attempting file extraction...";
	const vector<ExtractedData> extracted_files = utils::extract_files(bits, valid_seeds);
	cout << " done." << endl;


	if (extracted_files.size() == 0){

		cout << "\033[31m" << "No embedded files found" << "\033[0m" << endl;

	}else{

		// The probability of a non-stego file of this size generating valid metadata
		//                               num seeds      magic       version              payload_size	          enc_algo
		double coincidence_probability = pow(2, 32) / pow(2, 24) / pow(2, 1) * (bits.size()/3-65) / pow(2, 32) * 22.0 / 32;

		cout << "\033[32m" << extracted_files.size() << " embedded file" << (extracted_files.size() == 1 ? "" : "s") << " found" << "\033[0m" << endl;
		cout << "(Confidence: " <<  (1-coincidence_probability) * 100 << "%)" << endl;

		for (int i=0, s=(int)extracted_files.size(); i<s; i++){

			ui::save_dialogue(extracted_files[i]);
		}
	}

	return 0;
}
