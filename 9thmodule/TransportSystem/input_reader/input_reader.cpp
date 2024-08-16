#include "transport_catalogue/transport_catalogue.h"
#include "input_reader.h"
namespace transport {
	void FillCatalog(std::istream& in, Catalogue& catalog)
	{
		size_t request_count;
		in >> request_count;

		std::vector<std::string> buses;
		std::vector<std::string> stops;
		std::vector<std::string> query_stops;
		for (size_t i = 0; i < request_count; ++i) {
			std::string line, keyword;
			in >> keyword;
			std::getline(in, line);

			if (keyword == "Stop") {
				stops.push_back(line);
			}

			if (keyword == "Bus") {
				buses.push_back(line);
			}
		}
		
		query_stops = stops;
		for (auto& stop : stops) {
			auto [stop_name, coords] = FillStop(stop);
			catalog.AddStop(stop_name, coords);
		}

		for (auto& bus : buses) {
			auto [bus_name, coords, is_circle] = FillRoute(bus, catalog);
			catalog.AddBus(bus_name, coords, is_circle);
		}

		for (auto& stop : query_stops) {
			FillStopDistances(stop, catalog);
		}
	}

	std::pair<std::string, geo::Coordinates> FillStop(std::string& line)
	{
		std::string stop_name = line.substr(1, line.find_first_of(':') - line.find_last_of(' '));
		double lat = std::stod(line.substr(line.find_first_of(':') + 2 - line.find_first_of(',') - 1));
		double longitude;
		line.erase(0, line.find_first_of(',') + 2);
		if (line.find_last_of(',') == line.npos) {
			longitude = std::stod(line.substr(0, line.npos - 1));
			line.clear();
		}
		else {
			longitude = std::stod(line.substr(0, line.find_first_of(',') - 1));
			line.erase(0, line.find_first_of(',') + 2);
		}

		return { stop_name, {lat, longitude} };
	}

	
	std::tuple<std::string, std::vector<const Stop*>, bool> FillRoute(std::string& line, Catalogue& catalogue)
	{
		std::string bus_name = line.substr(1, line.find_first_of(':') - line.find_last_of(' '));
		std::vector<const Stop*> stops;

		line.erase(0, line.find_first_of(':') + 2);

		std::string stop_name;
		bool is_circle = line.find('>') != line.npos;
		char pos = is_circle ? '>' : '-';
		while (line.find(pos) != line.npos) {
			stop_name = line.substr(0, line.find_first_of(pos) - 1);
			const Stop* ptr = catalogue.FindStop(stop_name);
			stops.push_back(ptr);
			line.erase(0, line.find_first_of(pos) + 2);
		}

		stop_name = line.substr(0, line.find_first_of(pos) - 1);
		const Stop* ptr = catalogue.FindStop(stop_name);
		stops.push_back(ptr);
		return { bus_name, stops, is_circle };
	}

	void FillStopDistances(std::string& line, Catalogue& catalog)
	{
		if (line.empty()) return;
		std::string stop_from_name = FillStop(line).first;
		const Stop* stop = catalog.FindStop(stop_from_name);

		while (!line.empty()) {
			int distance = 0;
			std::string stop_to_name;
			distance = std::stoi(line.substr(0, line.find_first_of("m to")));
			line.erase(0, line.find_first_of("m to") + 5);
			if (line.find("m to") != line.npos) {
				stop_to_name = line.substr(0, line.npos - 1);
				const Stop* to_stop = catalog.FindStop(stop_to_name);
				catalog.SetDistance(stop, to_stop, distance);
				if (!catalog.GetDistance(stop, to_stop)) {
					catalog.SetDistance(to_stop, stop, distance);
				}
				line.clear();
			}
			else {
				stop_to_name = line.substr(0, line.find_first_of(','));
				const Stop* to = catalog.FindStop(stop_to_name);
				catalog.SetDistance(stop, to, distance);
				if (!catalog.GetDistance(stop, to)) {
					catalog.SetDistance(to, stop, distance);
				}
				line.erase(0, line.find_first_of(',') + 2);
			}
			}
		}
	}
