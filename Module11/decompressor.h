#pragma once
#include <iostream>
#include <string>
#include <fstream>

// напишите эту функцию
inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
	std::ifstream in(src_name, std::ios::binary);
	if (!in) {
		return false;
	}

	std::ofstream out(dst_name, std::ios::binary);
	if (!out) {
		return false;
	}

	while (in) {
		unsigned char header = in.get();
		int block_type = (header & 1);
		int data_size = (header >> 1) + 1;
		if (block_type == 1 && !in.eof()) {
			// Literal block
			std::string str (data_size, in.get());
			out.write(str.data(), str.size());
		}
		else {
			char buffer[1024];
			in.read(buffer, data_size);
			size_t readed = in.gcount();
			out.write(buffer, readed);
		}
	}
	return true;
}
