#include "ini.h"

using namespace std;
namespace ini {
	Section& Document::AddSection(std::string name)
	{
		return sections_[name];
		// TODO: вставьте здесь оператор return
	}
	const Section& Document::GetSection(const std::string& name) const
	{
		static Section empty{};
		if (sections_.count(name) == 0 || sections_.at(name).empty()) return empty;
		return sections_.at(name);
		// TODO: вставьте здесь оператор return
	}
	std::size_t Document::GetSectionCount() const
	{
		return sections_.size();
	}

	void ClearString(string& str) {
		if (!str.empty())
			str = str.substr(str.find_first_not_of(' '), str.find_last_not_of(' ') - str.find_first_not_of(' ') + 1);
	}

	Document Load(std::istream& input)
	{
		std::string line;
		Document result;
		Section* section = nullptr;
		while (std::getline(input, line)) {
			ClearString(line);
			if (line[0] == '[') {
				std::string section_name = line.substr(1, line.find(']')-1);
				section = &result.AddSection(section_name);
			}
			else if(line.find('=') && !line.empty()) {
				std::string section_key = line.substr(0, line.find_first_of('='));
				std::string section_value = line.substr(line.find_first_of('=') + 1, line.size() - line.find_first_of('='));
				ClearString(section_key);
				ClearString(section_value);
				section->insert({ section_key, section_value });
			}
			else {
				continue;
			}
		}

		return result;
	}
}

// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini