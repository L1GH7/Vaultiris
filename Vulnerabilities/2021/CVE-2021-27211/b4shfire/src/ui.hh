// © 2021 Lorian Richmond

#ifndef UI_HH
#define UI_HH

#include "ExtractedData.hh"

#include <iostream>


namespace ui{

	// Display a yes/no question with input validation
	bool get_choice(const char* msg, const char positive, const char negative = false);

	// Display an updating progress bar to inform the user of the threads' progress
	void create_progress_bar(uint32_t thread_progress[], int num_threads);

	// Prompt the user to save the extracted file
	void save_dialogue(const ExtractedData& d);
}

#endif
