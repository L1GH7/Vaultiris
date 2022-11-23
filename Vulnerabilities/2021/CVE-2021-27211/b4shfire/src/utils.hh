// © 2021 Lorian Richmond

#ifndef UTILS_HH
#define UTILS_HH

#include "ExtractedData.hh"

#include <vector>


namespace utils{

	// Find all seeds which generate valid magic bytes
	std::vector<uint32_t> find_valid_seeds(const std::vector<bool>& bits, int num_threads);

	// Further filter the possible seeds, and fully extract any that remain
	std::vector<ExtractedData> extract_files(const std::vector<bool>& bits, const std::vector<uint32_t> seeds);

}

#endif
