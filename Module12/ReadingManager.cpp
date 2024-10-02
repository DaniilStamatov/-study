#include <vector>

class ReadingManager {
private:
	std::vector<int> pages_read_;
	std::vector<int> number_of_people_;

public:
	void Read(int user, int pages_num) {
		for (int i = pages_read_[user] + 1; i <= pages_num; i++) {
			++number_of_people_[i];
		}

		pages_read_[user] = pages_num;
	}
	
};