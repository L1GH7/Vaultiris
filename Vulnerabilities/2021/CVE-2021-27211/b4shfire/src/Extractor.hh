// © 2021 Lorian Richmond

#ifndef EXTRACTOR_HH
#define EXTRACTOR_HH

#include "ExtractedData.hh"

#include <vector>
#include <unordered_map>
#include <math.h>
#include <string.h>


class Extractor{

	const std::vector<bool>& _bits;

	// This is a pointer in order to speed up initialisation in fast mode, when the map is not used
	//                  index   backup index
	std::unordered_map<uint32_t, uint32_t>* _backup_indices;

	bool _fast_mode;

	uint32_t _seed;
	uint32_t _max;
	uint32_t _bits_read = 0;

	uint32_t LCG();
	bool get_bit();

	uint32_t get_backup_index(uint32_t i);
	void set_backup_index(uint32_t i, uint32_t b);
	void create_new_backup_index(uint32_t i);
	bool already_used(uint32_t i);


public:

	Extractor(const std::vector<bool>& b, uint32_t s, bool f) : _bits(b),  _fast_mode(f), _seed(s), _max((uint32_t)b.size()) {

		if (_fast_mode == false){

			_backup_indices = new std::unordered_map<uint32_t, uint32_t>;
		}
	};

	// Extract the first 3 bytes and check whether they are correct
	bool check_magic();

	// Read 'num_bits' bits directly into the memory pointed to by p_data
	template <class T>
	void get_data(T* p_data, int num_bits){

		// Zero out the target memory area, rounded up to the nearest byte
		memset(p_data, 0, ceil((float)num_bits / 8));

		for (int i=0; i<num_bits; i++){

			((uint8_t*)p_data)[i/8] |= get_bit() << i % 8;
		}
	}
};

#endif
