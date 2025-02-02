#include "json_reader.h"
using namespace std::literals;
/*
 * ����� ����� ���������� ��� ���������� ������������� ����������� ������� �� JSON,
 * � ����� ��� ��������� �������� � ���� � ������������ ������� ������� � ������� JSON
 */
const json::Node& JsonReader::GetBaseRequests() const
{
	if (!input_.GetRoot().AsDict().count("base_requests")) return dummy_;
	return input_.GetRoot().AsDict().at("base_requests");
}

const json::Node& JsonReader::GetStatRequests() const
{
	if (!input_.GetRoot().AsDict().count("stat_requests")) return dummy_;
	return input_.GetRoot().AsDict().at("stat_requests");
}

const json::Node& JsonReader::GetRenderSettings() const
{
	if (!input_.GetRoot().AsDict().count("render_settings")) return dummy_;
	return input_.GetRoot().AsDict().at("render_settings");
}

const json::Node& JsonReader::GetRoutingSettings() const
{
	if (!input_.GetRoot().AsDict().count("routing_settings"s)) return dummy_;
	return input_.GetRoot().AsDict().at("routing_settings"s);
}


void JsonReader::ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const {
	json::Array result;
	for (auto& request : stat_requests.AsArray()) {
		const auto& request_map = request.AsDict();
		const auto& type = request_map.at("type"s).AsString();
		if (type == "Stop"s) result.push_back(PrintStop(request_map, rh).AsDict());
		if (type == "Bus"s) result.push_back(PrintRoute(request_map, rh).AsDict());
		if (type == "Map"s) result.push_back(PrintMap(request_map, rh).AsDict());
		if (type == "Route"s) result.push_back(PrintRouting(request_map, rh).AsDict());
	}

	json::Print(json::Document{ result }, std::cout);
}

std::tuple<std::string_view, geo::Coordinates, std::map<std::string_view, int>> JsonReader::FillStop(const json::Dict& request_map) const
{
	const std::string_view stop_name = request_map.at("name").AsString();
	geo::Coordinates coords = { request_map.at("latitude").AsDouble(), request_map.at("longitude").AsDouble() };
	std::map<std::string_view, int> stop_distances;
	auto& distances = request_map.at("road_distances").AsDict();
	for (auto& distance : distances) {
		stop_distances.emplace(distance.first, distance.second.AsInt());
	}


	return std::make_tuple(stop_name, coords, stop_distances);
}

void JsonReader::FillStopDistances(transport::Catalogue& catalogue) const
{
	const json::Array& arr = GetBaseRequests().AsArray();
	for (auto& requests : arr) {
		const auto& request_stop_map = requests.AsDict();
		const auto& type = request_stop_map.at("type").AsString();
		if (type == "Stop") {
			auto [stop_name, coordinates, stop_distances] = FillStop(request_stop_map);
			for (auto& [to_name, dist] : stop_distances) {
				auto from = catalogue.FindStop(stop_name);
				auto to = catalogue.FindStop(to_name);
				catalogue.SetDistance(from, to, dist);
			}
		}
	}
}

std::tuple<std::string_view, std::vector<const Stop*>, bool> JsonReader::FillRoute(const json::Dict& request_map, transport::Catalogue& catalogue) const
{
	const std::string_view bus_name = request_map.at("name").AsString();
	std::vector<const Stop*> stops;
	for (auto& stop : request_map.at("stops").AsArray()) {
		stops.push_back(catalogue.FindStop(stop.AsString()));
	}
	bool circular_route = request_map.at("is_roundtrip").AsBool();
	return std::make_tuple(bus_name, stops, circular_route);
}

svg::Color JsonReader::FillRenderColor(const json::Dict& request_map, const json::Node& name) const
{
	if (name.IsString()) return name.AsString();
	else if (name.IsArray()) {
		const json::Array& color_type = name.AsArray();
		if (color_type.size() == 3) {
			return svg::Rgb(color_type[0].AsInt(), color_type[1].AsInt(), color_type[2].AsInt());
		}
		else if (color_type.size() == 4) {
			return svg::Rgba(color_type[0].AsInt(), color_type[1].AsInt(), color_type[2].AsInt(), color_type[3].AsDouble());
		}
		else {
			return svg::NoneColor;
		}
	}
}

MapRenderer JsonReader::FillRenderSettings(const json::Dict& request_map) const
{
	RenderSettings render_settings;
	render_settings.width = request_map.at("width").AsDouble();
	render_settings.height = request_map.at("height").AsDouble();
	render_settings.padding = request_map.at("padding").AsDouble();
	render_settings.stop_radius = request_map.at("stop_radius").AsDouble();
	render_settings.line_width = request_map.at("line_width").AsDouble();
	render_settings.bus_label_font_size = request_map.at("bus_label_font_size").AsInt();
	const json::Array& bus_label_offset = request_map.at("bus_label_offset").AsArray();
	render_settings.bus_label_offset = { bus_label_offset[0].AsDouble(), bus_label_offset[1].AsDouble() };
	render_settings.stop_label_font_size = request_map.at("stop_label_font_size").AsInt();
	const json::Array& stop_label_offset = request_map.at("stop_label_offset").AsArray();
	render_settings.stop_label_offset = { stop_label_offset[0].AsDouble(), stop_label_offset[1].AsDouble() };

	if (request_map.at("underlayer_color").IsString()) render_settings.underlayer_color = request_map.at("underlayer_color").AsString();
	else if (request_map.at("underlayer_color").IsArray()) {
		const json::Array& underlayer_color = request_map.at("underlayer_color").AsArray();
		if (underlayer_color.size() == 3) {
			render_settings.underlayer_color = svg::Rgb(underlayer_color[0].AsInt(), underlayer_color[1].AsInt(), underlayer_color[2].AsInt());
		}
		else if (underlayer_color.size() == 4) {
			render_settings.underlayer_color = svg::Rgba(underlayer_color[0].AsInt(), underlayer_color[1].AsInt(), underlayer_color[2].AsInt(), underlayer_color[3].AsDouble());
		}
		else throw std::logic_error("wrong underlayer colortype");
	}
	else throw std::logic_error("wrong underlayer color");

	render_settings.underlayer_width = request_map.at("underlayer_width").AsDouble();

	const json::Array& color_palette = request_map.at("color_palette").AsArray();
	for (const auto& color_element : color_palette) {
		if (color_element.IsString()) render_settings.color_palette.push_back(color_element.AsString());
		else if (color_element.IsArray()) {
			const json::Array& color_type = color_element.AsArray();
			if (color_type.size() == 3) {
				render_settings.color_palette.push_back(svg::Rgb(color_type[0].AsInt(), color_type[1].AsInt(), color_type[2].AsInt()));
			}
			else if (color_type.size() == 4) {
				render_settings.color_palette.push_back(svg::Rgba(color_type[0].AsInt(), color_type[1].AsInt(), color_type[2].AsInt(), color_type[3].AsDouble()));
			}
			else throw std::logic_error("wrong color_palette type");
		}
		else throw std::logic_error("wrong color_palette");
	}

	return render_settings;
}

transport::RouteSettings JsonReader::FillRouteSettings(const json::Node& settings) const
{
	return transport::RouteSettings({ settings.AsDict().at("bus_wait_time").AsInt(), settings.AsDict().at("bus_velocity").AsDouble() });
}

void JsonReader::FillCatalogue(transport::Catalogue& catalogue)
{
	const json::Array& arr = GetBaseRequests().AsArray();
	for (auto& requests : arr) {
		const auto& request_stop_map = requests.AsDict();
		const auto& type = request_stop_map.at("type").AsString();
		if (type == "Stop") {
			auto [stop_name, coords, distance] = FillStop(request_stop_map);
			catalogue.AddStop(stop_name, coords);
		}
	}

	FillStopDistances(catalogue);

	for (auto& requests : arr) {
		const auto& request_bus_map = requests.AsDict();
		const auto& type = request_bus_map.at("type").AsString();
		if (type == "Bus") {
			auto [bus_name, stops, is_circle] = FillRoute(request_bus_map, catalogue);
			catalogue.AddBus(bus_name, stops, is_circle);
		}
	}

}

const json::Node JsonReader::PrintRoute(const json::Dict& request_map, RequestHandler& rh) const {
	json::Node result;
	const std::string& route_number = request_map.at("name").AsString();
	const int id = request_map.at("id").AsInt();

	if (!rh.IsBusNumber(route_number)) {
		result = json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("error_message").Value("not found")
			.EndDict()
			.Build();
	}
	else {
		const auto& route_info = rh.GetBusStat(route_number);
		result = json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("curvature").Value(route_info->curvature)
			.Key("route_length").Value(route_info->route_length)
			.Key("stop_count").Value(static_cast<int>(route_info->stop_count))
			.Key("unique_stop_count").Value(static_cast<int>(route_info->unique_stop_count))
			.EndDict()
			.Build();
	}
	return result;
}

const json::Node JsonReader::PrintStop(const json::Dict& request_map, RequestHandler& rh) const {
	json::Node result;
	const std::string& stop_name = request_map.at("name").AsString();
	const int id = request_map.at("id").AsInt();

	if (!rh.IsStopName(stop_name)) {
		result = json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("error_message").Value("not found")
			.EndDict()
			.Build();
	}
	else {
		json::Array buses;
		for (const auto& bus : rh.GetBusesByStop(stop_name)) {
			buses.push_back(bus);
		}
		result = json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("buses").Value(buses)
			.EndDict()
			.Build();

		
	}
	return result;
}

const json::Node JsonReader::PrintMap(const json::Dict& request_map, RequestHandler& rh) const
{
	json::Node result;
	const int id = request_map.at("id").AsInt();
	std::ostringstream strm;
	svg::Document map = rh.RenderMap();
	map.Render(strm);

	result = json::Builder{}
		.StartDict()
		.Key("request_id").Value(id)
		.Key("map").Value(strm.str())
		.EndDict()
		.Build();

	return result;
}

const json::Node JsonReader::PrintRouting(const json::Dict& request_map, RequestHandler& rh) const
{
	json::Node result;

	const int id = request_map.at("id"s).AsInt();
	const  std::string_view stop_from = request_map.at("from").AsString();
	const  std::string_view stop_to = request_map.at("to").AsString();
	const auto& route = rh.GetOptimalRoute(stop_from, stop_to);

	if (!route) {
		result = json::Builder{}.StartDict()
				.Key("request_id").Value(id)
				.Key("error_message").Value("not found")
			.EndDict()
		.Build();
	}
	else {
		json::Array items;
		double total_time = 0.0;
		items.reserve(route.value().edges.size());
		for (auto& edge_id : route.value().edges) {
			const graph::Edge<double> edge = rh.GetRouterGraph().GetEdge(edge_id);
			if (edge.quality == 0) {
				items.emplace_back(json::Node(json::Builder{}.StartDict()
					.Key("stop_name"s).Value(edge.name)
					.Key("time").Value(edge.weight)
					.Key("type").Value("Wait")
					.EndDict().Build()));
				total_time += edge.weight;

			}
			else
			{
				items.emplace_back(json::Node(json::Builder{}.StartDict()
					.Key("bus").Value(edge.name)
					.Key("span_count").Value(static_cast<double>(edge.quality))
					.Key("time").Value(edge.weight)
					.Key("type").Value("Bus")
					.EndDict()
					.Build()));
				total_time += edge.weight;
			}

		}

		result = json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("total_time").Value(total_time)
			.Key("items").StartArray()
			.Value(items)
			.EndArray()
			.EndDict()
			.Build();

	}

	return result;


}

