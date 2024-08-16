#pragma once
#include <iostream>

class StreamUntier {
public:
	StreamUntier(std::ostream& st) : st_(st), tied_before_(st.tie(nullptr)){
	}

	~StreamUntier() {
		st_.tie(tied_before_);
	}

private:
	std::ostream* tied_before_;
	std::ostream& st_;
};