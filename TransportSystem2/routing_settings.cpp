#include "routing_settings.h"
#include <algorithm>

const graph::DirectedWeightedGraph<double> transport::RouteSettings::BuildGraph(const Catalogue& catalogue)
{
	const auto& stops = catalogue.GetSortedAllStops();
	const auto& buses = catalogue.GetSortedAllBuses();
	graph::DirectedWeightedGraph<double> graph(stops.size() * 2);
	std::map<std::string, graph::VertexId> stop_ids;
	graph::VertexId vertex_id = 0;
	for (const auto& [stop_name, stop] : stops) {
		stop_ids[stop->name] = vertex_id;
		graph.AddEdge({ stop->name,
			0,
			vertex_id,
			++vertex_id,
			static_cast<double>(bus_wait_time_) });

		++vertex_id;
	}

	stop_ids_ = std::move(stop_ids);

	for_each(buses.begin(), buses.end(), [&graph, this, &catalogue](const auto& item) {
		const auto bus_info = item.second;
		const auto& bus_stops = bus_info->stops;
		size_t stop_size = bus_stops.size();
		for (size_t i = 0; i < stop_size; ++i) {
			for (size_t j = i + 1; j < stop_size; ++j) {
				const Stop* stop_from = bus_stops[i];
				const Stop* stop_to = bus_stops[j];
				int dist_sum = 0;
				int dist_sum_inverse = 0;
				for (size_t k = i + 1; k <= j; ++k) {
					dist_sum += catalogue.GetDistance(bus_stops[k - 1], bus_stops[k]);
					dist_sum_inverse += catalogue.GetDistance(bus_stops[k], bus_stops[k - 1]);
				}

				graph.AddEdge({ bus_info->number,
					j - i,
					stop_ids_.at(stop_from->name) + 1,
					stop_ids_.at(stop_to->name),
					static_cast<double>(dist_sum) / bus_velocity_ * (100.0 / 6.0) });

				if (!bus_info->is_circle) {
					graph.AddEdge({ bus_info->number,
					j - i,
					stop_ids_.at(stop_to->name) + 1,
					stop_ids_.at(stop_from->name),
					static_cast<double>(dist_sum_inverse) / bus_velocity_ * (100.0 / 6.0) });
				}
			}
		}
		});

	graph_ = std::move(graph);
	router_ = std::make_unique<graph::Router<double>>(graph_);

	return graph_;
}

const std::optional<graph::Router<double>::RouteInfo> transport::RouteSettings::FindRoute(const std::string_view stop_from, const std::string_view stop_to) const
{
	return router_->BuildRoute(stop_ids_.at(std::string(stop_from)), stop_ids_.at(std::string(stop_to)));
}
