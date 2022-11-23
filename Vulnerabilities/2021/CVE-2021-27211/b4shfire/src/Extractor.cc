// © 2021 Lorian Richmond

#include "Extractor.hh"
#include <iostream>
#include <assert.h>

using namespace std;


// The Linear Congruential Generator at the heart of Steghide's algorithm
uint32_t Extractor::LCG(){

	const int MULTIPLIER = 1367208549;
	const int INCREMENT = 1;

	// Get the next seed in the series by stepping the LCG
	_seed *= MULTIPLIER;
	_seed += INCREMENT;

	// Turn the new seed into the next bit index (which must be between _bits_read and _max)
	// This is done by scaling the seed's range down from 2^32 to _max-_bits_read, then adding _bits_read
	uint32_t index = (((uint64_t)_seed * (_max - _bits_read)) >> 32) + _bits_read;

	_bits_read++;

	return index;
}


// Check if a certain index has already been used to embed stego data
bool Extractor::already_used(uint32_t i){

	return (*_backup_indices)[i] != 0;
}


// Get the backup index to be used when index i has already been used
uint32_t Extractor::get_backup_index(uint32_t i){

	return (*_backup_indices)[i];
}


// Set the backup index for i to b
void Extractor::set_backup_index(uint32_t i, uint32_t b){

	(*_backup_indices)[i] = b;
}


// Store a backup index in _backup_indices in case index i is generated again
void Extractor::create_new_backup_index(uint32_t i){

	// Check if _bits_read (the current number of bits read) has already been used as an index
	if (not already_used(_bits_read-1)){

		// Set the backup value to _bits_read
		set_backup_index(i, _bits_read-1);
	}else{

		// Set the backup value to the backup value of _bits_read
		set_backup_index(i, get_backup_index(_bits_read-1));
	}
}


// Extract the next bit from the stego file
bool Extractor::get_bit(){

	// Each bit is stored in 3 DCT coefficients, indices given by the LCG, XORed together

	if (_fast_mode == true){

		// Nice fast logic, however does not work if an index is generated more than once
		// (A single bit cannot be used multiple times)
		return _bits[LCG()] ^ _bits[LCG()] ^ _bits[LCG()];

	}else{

		bool bit = 0;

		for(int i=0; i<3; i++){

			uint32_t generated_index = LCG();
			uint32_t index;

			// If the generated index has already been used to embed data
			if (already_used(generated_index)){

				// Use the backup index instead
				index = get_backup_index(generated_index);

				// The backup index must now be changed, in case it must be used again
				create_new_backup_index(generated_index);

			}else{

				// We can simply use the generated index
				index = generated_index;
			}

			// Add a backup index to use if this index is generated again in future
			create_new_backup_index(index);

			bit ^= _bits[index];
		}

		return bit;
	}
}


// The first 24 embedded bits are always the same
// Reading these bits is used to filter the number of possible seeds from 2^32 to 2^8
bool Extractor::check_magic(){

	// Checking the magic bits must be done before anything else is read
	assert (_bits_read == 0);

	//                          s(teg)               h(ide)             m(agic)
	const bool MAGIC[24] = {0,1,1,1,0,0,1,1,    0,1,1,0,1,0,0,0,    1,0,0,0,1,1,0,1};

	// Iterate over the first 24 embedded bits
	for (int i=0; i<24; i++){

		// Check if the next bit is incorrect (the bits are embedded backwards, hence the -i)
		if (get_bit() != MAGIC[23-i]){

			return false;
		}
	}

	return true;
}
