#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

void RequestHandler::ProcessRequests(const json::Node& stat_requests) const
{
	json::Array result;
	const json::Array& arr = stat_requests.AsArray();
	for (auto& request : arr) {
		const auto& request_map = request.AsDict();
		const auto& type = request_map.at("type").AsString();
		if (type == "Stop") {
			result.push_back(PrintStop(request_map).AsDict());
		}
		if (type == "Bus") {
			result.push_back(PrintRoute(request_map).AsDict());
		}
	}
	json::Print(json::Document{ result }, std::cout);
}

const json::Node RequestHandler::PrintRoute(const json::Dict& request_map) const
{
	json::Dict result;
	const std::string& route_number = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!catalogue_.FindBus(route_number)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		result["culvature"] = GetBusStat(route_number)->curvature;
		result["route_length"] = GetBusStat(route_number)->route_length;
		result["stop_count"] = static_cast<int>(GetBusStat(route_number)->stop_count);
		result["unique_stop_count"] = static_cast<int>(GetBusStat(route_number)->unique_stop_count);
	}

	return json::Node{ result };
}

const json::Node RequestHandler::PrintStop(const json::Dict& request_map) const
{
	json::Dict result;
	const std::string& stop_name = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!catalogue_.FindStop(stop_name)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		json::Array buses;
		for (auto& bus : GetBusesByStop(stop_name)) {
			buses.push_back(bus);
		}
		result["buses"] = buses;
	}
	return json::Node{ result };
}

bool RequestHandler::IsBusNumber(const std::string_view bus_number) const
{
	return catalogue_.FindBus(bus_number);
}

bool RequestHandler::IsStopName(const std::string_view stop_name) const
{
	return catalogue_.FindStop(stop_name);
}

std::optional<BusInfo> RequestHandler::GetBusStat(const std::string_view& bus_number) const {
	BusInfo bus_stat{};
	const Bus* bus = catalogue_.FindBus(bus_number);

	if (!bus) throw std::invalid_argument("bus not found");
	if (bus->is_circle) bus_stat.stop_count = bus->stops.size();
	else bus_stat.stop_count = bus->stops.size() * 2 - 1;

	int route_length = 0;
	double geographic_length = 0.0;

	for (size_t i = 0; i < bus->stops.size() - 1; ++i) {
		auto from = bus->stops[i];
		auto to = bus->stops[i + 1];
		if (bus->is_circle) {
			route_length += catalogue_.GetDistance(from, to);
			geographic_length += geo::ComputeDistance(from->coordinates,
				to->coordinates);
		}
		else {
			route_length += catalogue_.GetDistance(from, to) + catalogue_.GetDistance(to, from);
			geographic_length += geo::ComputeDistance(from->coordinates,
				to->coordinates) * 2;
		}
	}

	bus_stat.unique_stop_count = catalogue_.UniqueStopsCount(bus_number);
	bus_stat.route_length = route_length;
	bus_stat.curvature = route_length / geographic_length;

	return bus_stat;
}

const std::set<std::string> RequestHandler::GetBusesByStop(std::string_view stop_name) const {
	return catalogue_.FindStop(stop_name)->buses_by_stop;
}

const std::optional<graph::Router<double>::RouteInfo> RequestHandler::GetOptimalRoute(const std::string_view stop_from, const std::string_view stop_to) const
{
	return router_.FindRoute(stop_from, stop_to);
}

const graph::DirectedWeightedGraph<double>& RequestHandler::GetRouterGraph() const
{
	return router_.GetGraph();
}

svg::Document RequestHandler::RenderMap() const
{
	return renderer_.GetSVG(catalogue_.GetSortedAllBuses());
}

