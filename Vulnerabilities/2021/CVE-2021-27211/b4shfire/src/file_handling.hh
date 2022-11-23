// © 2021 Lorian Richmond

#ifndef FILE_HANDLING_HH
#define FILE_HANDLING_HH

#include "ExtractedData.hh"

#include <vector>


namespace file_handling{

	// Automatically detect the file type based on magic bytes
	std::vector<bool> auto_load(FILE* p_file);

	std::vector<bool> load_jpeg(FILE* p_file);
	std::vector<bool> load_bmp(FILE* p_file);
	std::vector<bool> load_wav(FILE* p_file);
	std::vector<bool> load_au(FILE* p_file);

	void write_to_file(const ExtractedData& d, std::string filename);

}

#endif
