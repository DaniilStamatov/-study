#include "transport_catalogue.h"

namespace transport {
	void Catalogue::AddStop(std::string_view name, const geo::Coordinates coords)
	{
		all_stops_.push_back(Stop{ std::string(name), coords, {} });
		stopname_to_stop_[all_stops_.back().name] = &all_stops_.back();
	}

	void Catalogue::AddBus(std::string_view bus_number, const std::vector<const Stop*>& stops, bool is_circle)
	{
		all_buses_.push_back(Bus{ std::string(bus_number), stops, is_circle });
		busname_to_bus_[all_buses_.back().number] = &all_buses_.back();

		for (const auto& stop : stops) {
			auto it = stopname_to_stop_.find(stop->name);
			if (it != stopname_to_stop_.end()) {
				it->second->buses_by_stop.insert(std::string(bus_number));
			}
		}
	}

	const Bus* Catalogue::FindBus(std::string_view bus_number) const
	{
		return busname_to_bus_.count(std::string(bus_number)) > 0 ? busname_to_bus_.at(std::string(bus_number)) : nullptr;
	}

	const Stop* Catalogue::FindStop(std::string_view name) const
	{
		return stopname_to_stop_.count(std::string(name)) > 0 ? stopname_to_stop_.at(std::string(name)) : nullptr;
	}

	const std::optional<BusInfo> Catalogue::GetBusInfo(std::string_view number) const
	{
		BusInfo businfo{};
		const Bus* bus = FindBus(number);
		if (!bus) throw std::invalid_argument("bus not found");
		if (bus->is_circle)
			businfo.stop_count = bus->stops.size();
		else {
			businfo.stop_count = bus->stops.size() * 2 - 1;
		}

		businfo.unique_stop_count = UniqueStopsCount(number);

		double geographic_length = 0.0;
		int route_length = 0;
		for (size_t i = 0; i < bus->stops.size(); i++) {
			auto from = bus->stops[i];
			auto to = bus->stops[i + 1];

			if (bus->is_circle) {
				route_length += GetDistance(from, to);
				geographic_length += ComputeDistance(from->coordinates, to->coordinates);
			}
			else {
				route_length += GetDistance(from, to) + GetDistance(to, from);
				geographic_length += ComputeDistance(from->coordinates, to->coordinates) * 2;
			}
		}

		businfo.route_length = route_length;
		businfo.curvature = route_length / geographic_length;
		return businfo;
	}

	const std::set<std::string> Catalogue::GetBusesByStop(std::string_view stop_name) const
	{
		return stopname_to_stop_.at(std::string(stop_name))->buses_by_stop;
	}

	size_t Catalogue::UniqueStopsCount(std::string_view bus_number) const {
		std::unordered_set<std::string_view> unique_stops;
		for (const auto& stop : busname_to_bus_.at(std::string(bus_number))->stops) {
			unique_stops.insert(stop->name);
		}

		return unique_stops.size();
	}


	void Catalogue::SetDistance(const Stop* from, const Stop* to, const int distance)
	{
		distances_[{from, to}] = distance;
	}

	int Catalogue::GetDistance(const Stop* from, const Stop* to) const
	{
		if (distances_.count({ from, to })) return distances_.at({ from, to });
		else if (distances_.count({ to, from })) return distances_.at({ to, from });
		else return 0;
	}
}