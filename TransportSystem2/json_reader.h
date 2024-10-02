#pragma once

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

#include "json.h"
#include "transport_catalogue.h"
#include <iostream>
#include "map_renderer.h"
#include "request_handler.h"
#include "json_builder.h"


class JsonReader {
private:
	json::Document input_;
	json::Node dummy_ = nullptr;
	std::tuple<std::string_view, geo::Coordinates, std::map<std::string_view, int>> FillStop(const json::Dict& request_map) const;
	void FillStopDistances(transport::Catalogue& catalogue) const;
	std::tuple<std::string_view, std::vector<const Stop*>, bool> FillRoute(const json::Dict& request_map, transport::Catalogue& catalogue) const;
	svg::Color FillRenderColor(const json::Dict& request_map, const json::Node& name) const;
public:
	JsonReader(std::istream& input) : input_(json::Load(input)) {
	}

	const json::Node& GetBaseRequests() const;
	const json::Node& GetStatRequests() const;
	const json::Node& GetRenderSettings() const;
	const json::Node& GetRoutingSettings() const;


	MapRenderer FillRenderSettings(const json::Dict& request_map) const;
	transport::RouteSettings FillRouteSettings(const json::Node& settings) const;

	void FillCatalogue(transport::Catalogue& catalogue);
	void ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const;
	const json::Node PrintRoute(const json::Dict& request_map, RequestHandler& rh) const;
	const json::Node PrintStop(const json::Dict& request_map, RequestHandler& rh) const;
	const json::Node PrintMap(const json::Dict& request_map, RequestHandler& rh) const;
	const json::Node PrintRouting(const json::Dict& request_map, RequestHandler& rh) const;
};