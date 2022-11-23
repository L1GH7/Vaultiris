// © 2021 Lorian Richmond

#ifndef EXTRACTED_DATA_HH
#define EXTRACTED_DATA_HH

#include <string>
#include <vector>


struct Metadata{
	
	char magic_bytes[3] = {};
	
	int version = 0;
	
	int enc_algo = 0;
	int enc_mode = 0;
	
	uint32_t payload_size = 0;
};


struct Payload{
	
	bool is_compressed = false;
	uint32_t uncompressed_size = 0;
	
	bool has_checksum = false;
	uint32_t checksum = 0;
	
	std::string filename;
	std::vector<uint8_t> file_contents;
};


struct ExtractedData{
	
	Metadata info;
	bool is_encrypted;
	
	std::vector<uint8_t> encrypted_payload;
	Payload data;
};

#endif
