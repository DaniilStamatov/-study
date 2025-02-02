#pragma once

#include "input_reader/input_reader.h"

namespace transport {

	void ProcessRequests(std::istream& in, Catalogue& catalogue, std::ostream& out);

	void PrintRoute(std::string& line, Catalogue& catalogue, std::ostream& out);
	void PrintStop(std::string& line, Catalogue& catalogue, std::ostream& out);

} // namespace transport