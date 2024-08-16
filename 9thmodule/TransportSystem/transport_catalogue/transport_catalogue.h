#pragma once

#include "geo/geo.h"

#include <iostream>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <optional>
#include <unordered_set>
#include <set>

namespace transport {
	struct Stop {
		std::string name;
		geo::Coordinates coordinates;
		std::set<std::string> buses_by_stop;
	};

	struct Bus {
		std::string number;
		std::vector<const Stop*> stops;
		bool is_circle;
	};

	struct BusInfo {
		size_t stop_count;
		size_t unique_stop_count;
		double route_length;
		double curvature;
	};

	class Catalogue {
	public:

		struct StopDistancesHasher {
			size_t operator()(const std::pair<const Stop*, const Stop*>& points) const {
				size_t hash_first = std::hash<const void*>{}(points.first);
				size_t hash_second = std::hash<const void*>{}(points.second);
				return hash_first + hash_second * 37;
			}
		};

		//��������� ����� ��������� � all_stops_ �  stopname_to_stop
		void AddStop(std::string_view name, const geo::Coordinates coords);

		//��������� ������� � ������� bus_number � all_buses_ �  busname_to_bus_
		//���� ���������� ��������� �� ������, �� � ������ �� ��� ����������� ���������� ����� ��� �������
		void AddBus(std::string_view bus_number, const std::vector<const Stop*>& stops, bool is_circle);

		//������� ������� �� ��� ������
		const Bus* FindBus(std::string_view bus_number) const;

		//������� ��������� �� �� ��������
		const Stop* FindStop(std::string_view name) const;

		//�������� ���������� � ��������
		const std::optional<BusInfo> GetBusInfo(std::string_view number) const;

		//�������� ������ ��������� ���������� ����� ���������
		const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;

		//������������� ���������� ����� �����������
		void SetDistance(const Stop* from, const Stop* to, const int distance);

		//�������� ���������� ����� �����������
		int GetDistance(const Stop* from, const Stop* to) const;

		//�������� ���������� ���������� ��������� �� ������ ��������
		size_t UniqueStopsCount(std::string_view bus_number) const;

	private:
		std::deque<Stop> all_stops_;
		std::deque<Bus> all_buses_;
		std::unordered_map<std::string, Stop*> stopname_to_stop_;
		std::unordered_map<std::string, Bus*> busname_to_bus_;
		std::unordered_map<std::pair<const Stop*, const Stop*>, int, StopDistancesHasher> distances_;
	};
}