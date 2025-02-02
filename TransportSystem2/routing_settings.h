#pragma once
#include "graph.h"
#include "transport_catalogue.h"
#include "router.h"

namespace transport {
	class RouteSettings {
	private: 
		int bus_wait_time_ = 0;
		double bus_velocity_ = 0.0;
		graph::DirectedWeightedGraph<double> graph_;
		std::map<std::string, graph::VertexId> stop_ids_;
		std::unique_ptr<graph::Router<double>> router_;

	public:

		RouteSettings() = default;

		RouteSettings(const int bus_wait_time, const double bus_velocity)
			: bus_wait_time_(bus_wait_time)
			, bus_velocity_(bus_velocity) {}

		RouteSettings(const RouteSettings& settings, const Catalogue& catalogue) {
			bus_wait_time_ = settings.bus_wait_time_;
			bus_velocity_ = settings.bus_velocity_;
			BuildGraph(catalogue);
		}

		const graph::DirectedWeightedGraph<double> BuildGraph(const Catalogue& catalogue);

		const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view stop_from, const std::string_view stop_to) const;

		const graph::DirectedWeightedGraph<double>& GetGraph() const {
			return graph_;
		}
	};
}