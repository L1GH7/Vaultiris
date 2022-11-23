// © 2021 Lorian Richmond

#include "file_handling.hh"

#include <iostream>
#include <fstream>
#include <math.h>
#include "jpeglib.h"

using namespace std;


namespace file_handling{

void write_to_file(const ExtractedData& d, string filename){

	ofstream outfile(filename, ios::out | ios::trunc | ios::binary);

	if (d.is_encrypted){

		outfile.write((char*)&d.encrypted_payload[0], d.encrypted_payload.size());
	}else{

		outfile.write((char*)&d.data.file_contents[0], d.data.file_contents.size());
	}

	outfile.close();
}


// Automatically detect the file type based on magic bytes
vector<bool> auto_load(FILE* p_file){

	// Seek to the start of the file
	fseek(p_file, 0, SEEK_SET);

	// Magic bytes of Steghide's supported filetypes:
	// AU - 2E 73 6E 64
	// JPEG - FF D8 FF
	// WAV - 52 49 46 46
	// BMP - 42 4D

	int b = fgetc(p_file);
	ungetc(b, p_file);

	// Get the first byte of the file; then use it to determine the filetype
	switch (b){
		case 0xFF:
			return load_jpeg(p_file);

		case 0x42:
			return load_bmp(p_file);

		case 0x52:
			return load_wav(p_file);

		case 0x2E:
			return load_au(p_file);

		default:
			cerr << "Unsupported file type." << endl;
			exit(1);
	}
}


// Convert the JPEG pointed to by p_file into a vector of bits
vector<bool> load_jpeg(FILE* p_file){

	// The return value, a simple vector of bits ready for analysis
	vector<bool> bits;

	// The number of DCT coefficients per JPEG image block
	const int COEFFS_PER_BLOCK = 64;

	// Decompression object
	jpeg_decompress_struct image_data;

	// Array to hold the DCT coefficients
	jvirt_barray_ptr* dct_coeffs;

	// Set up error handling
	jpeg_error_mgr error_manager;
	image_data.err = jpeg_std_error(&error_manager);

	// Create the decompressor
	jpeg_create_decompress(&image_data);

	// Initialise state for input
	jpeg_stdio_src(&image_data, p_file);

	// Get information from the file header
	jpeg_read_header(&image_data, TRUE);

	// Get the DCT coefficients
	dct_coeffs = jpeg_read_coefficients(&image_data);

	// The number of components (channels) in the image
	const int num_components = image_data.num_components;

	// The height and width of the image in pixels
	const int image_height = image_data.image_height;
	const int image_width = image_data.image_width;

	// The largest vertical and horizontal sample factors out of all components
	int max_v_samp_factor = 0;
	int max_h_samp_factor = 0;

	// Find the highest sample factors in the image
	for (int i=0; i<num_components; i++){

		max_v_samp_factor = max(max_v_samp_factor, image_data.comp_info[i].v_samp_factor);
		max_h_samp_factor = max(max_h_samp_factor, image_data.comp_info[i].h_samp_factor);
	}

	// Arrays to store the block dimensions of each component
	int heights_in_blocks[num_components];
	int widths_in_blocks[num_components];

	// For each component
	for (int i=0; i<num_components; i++){

		// The sample factors of the current component
		int v_samp_factor = image_data.comp_info[i].v_samp_factor;
		int h_samp_factor = image_data.comp_info[i].h_samp_factor;

		// Calculate the block dimensions of the current component
		// (Cast to float to avoid integer truncation problems)
		heights_in_blocks[i] = ceil((float)(image_height * v_samp_factor) / (8 * max_v_samp_factor));
		widths_in_blocks[i] = ceil((float)(image_width * h_samp_factor) / (8 * max_h_samp_factor));
	}
	

	// Read data from jpeglib's multidimensional virtual array into the linear 'bits' vector
	for (int icomp=0; icomp<num_components; icomp++) {
		for (int currow=0; currow<heights_in_blocks[icomp]; currow++) {

			JBLOCKARRAY array = (*(image_data.mem->access_virt_barray))((j_common_ptr) &image_data, dct_coeffs[icomp], currow, 1, FALSE);

				for (int iblock=0; iblock<widths_in_blocks[icomp]; iblock++) {
					for (int icoeff=0; icoeff<COEFFS_PER_BLOCK; icoeff++) {

						int coef = array[0][iblock][icoeff];

						// Only non-zero coefficients are used
						if (coef != 0){

							// The bit is given by the parity of the DCT coefficient
							bits.push_back(coef % 2);
						}
					}
				}
		}
	}

	jpeg_destroy_decompress(&image_data);

	return bits;
}


vector<bool> load_bmp(FILE* p_file){

	cerr << "Error. BMP files are not supported yet." << endl;
	exit(1);
}


vector<bool> load_wav(FILE* p_file){

	cerr << "Error. WAV files are not supported yet." << endl;
	exit(1);
}


vector<bool> load_au(FILE* p_file){

	cerr << "Error. AU files are not supported yet." << endl;
	exit(1);
}

}
