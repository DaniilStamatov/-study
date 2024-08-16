#include <map>
#include <string>
#include <string_view>

using namespace std;
class Translator {

public:
	void Add(string_view source, string_view target) {
		eng_to_rus[string(source)] = target;
		rus_to_eng[string(target)] = source;
	}

	string_view TranslateForward(string_view source) const{
		if (auto search = eng_to_rus.find(static_cast<string>(source)); search != eng_to_rus.end()) {
			return (*search).second;
		}
		return {};
	}

	string_view TranslateBackward(string_view source) const {
		if (auto search = rus_to_eng.find(string(source)); search != rus_to_eng.end()) {
			return search->second;
		}
	}

private:
	map<string, string> eng_to_rus;
	map<string, string> rus_to_eng;
};