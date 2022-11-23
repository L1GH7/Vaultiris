// © 2021 Lorian Richmond

#include "ui.hh"
#include "file_handling.hh"

#include <math.h>
#include <iomanip>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <thread>

using namespace std;


namespace ui{

void save_cursor_position(){

	cout << "\033[s";
}


void restore_cursor_position(){

	cout << "\033[u";
}


// Display a yes/no question with input validation
bool get_choice(const char* msg, char positive, char negative){

	char choice;

	do{
		cout << msg;
		cin >> choice;

	}while (choice != positive && choice != negative);

	return choice == positive;
}


// Get the number of columns in the terminal window
int get_console_width(){

	// Structure to hold the window information
	struct winsize size;

	// Get the window information
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	// Get the number of columns (width)
	return(size.ws_col);
}


void draw_progress_bar(float progress){

	cout << endl;

	string start_char = "|";
	string done_char = "█";
	string current_char = "|";
	string not_done_char = " ";
	string end_char = "| ";

	// -11 to leave space for percentage
	int bar_width = get_console_width() - 11;

	cout << start_char;
	int pos = bar_width * progress;

	for (int i=0; i<bar_width; i++) {

		if (i < pos) cout << done_char;
		else if (i == pos) cout << current_char;
		else cout << not_done_char;
	}

	cout << end_char << fixed << setprecision(2) << float(progress * 100) << " %\r\033[F";
	cout.flush();
}


void clear_progress_bar(){

	// Go down to the next line, clear it, then go back up
	cout << "\033[E" << "\033[K" << "\033[F";
}


// Display an updating progress bar to inform the user of the threads' progress
void create_progress_bar(uint32_t thread_progress[], int num_threads){

	save_cursor_position();
	cout << endl;

	uint64_t total_seeds = 0;

	while (total_seeds < UINT32_MAX){

		total_seeds = 0;

		// Iterate over each thread and add up the total seeds tested
		for (int i = 0; i < num_threads; i++){
			total_seeds += thread_progress[i];
		}

		float progress = (float)total_seeds / pow(2, 32);
		draw_progress_bar(progress);

		this_thread::sleep_for(chrono::milliseconds(100));
	}

	clear_progress_bar();
	restore_cursor_position();
	cout << "\033[2B" << "\033[2A"; // Fixes misalignment caused by terminal scrolling
	cout.flush();
}


void print_info(const ExtractedData& d){

	const char* ENC_ALGOS[23] = {"None", "Twofish", "AES-128", "AES-192", "AES-256", "Saferplus", "RC2",
				"XTEA", "Serpent", "SaferSK64", "SaferSK128", "Cast256", "Loki97", "Gost", "Threeway",
				"Cast128", "Blowfish", "DES", "Tripledes", "Enigma", "Arcfour", "Panama", "Wake"};

	const char* ENC_MODES[8] = {"ECB", "CBC", "OFB", "CFB", "NOFB", "NCFB", "CTR", "Stream"};


	cout << "----------------------------" << endl;

	if (d.is_encrypted){

		cout << "Encryption: " << "\033[31m" << ENC_ALGOS[d.info.enc_algo] << " " << ENC_MODES[d.info.enc_mode] << "\033[0m";

		// AES-128 CBC is steghide's default encryption setting
		if (d.info.enc_algo == 2 && d.info.enc_mode == 1){

			cout << " (Steghide's default encryption)";
		}

		cout << endl;

	}else{

		cout << "Encryption: " << "\033[32m" << "None" << "\033[0m" << endl;
		cout << "Filename: " << d.data.filename << endl;
		cout << "Size: " << d.data.file_contents.size() << " bytes" << endl;
	}

	cout << "----------------------------" << endl;
}


// Prompt the user to save the extracted file
void save_dialogue(const ExtractedData& d){

	print_info(d);

	if (get_choice("Extract data to file? [y/n]: ", 'y', 'n') == false){

		return;
	}

	string default_filename = d.data.filename;

	if (default_filename == ""){

		if (d.is_encrypted){

			default_filename = "extracted.enc";
		}else{

			default_filename = "extracted.unknown";
		}
	}

	cout << "Filename (default is " << default_filename << "): ";

	// Get rid of newline character left in buffer
	cin.ignore();

	string filename;
	getline(cin, filename);

	if (filename == ""){

		filename = default_filename;
	}

	file_handling::write_to_file(d, filename);
}

}
