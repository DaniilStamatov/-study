#pragma once

/*
 * ����� ����� ���������� ��� ������������� �����������
 */


#include <iostream>
#include <deque>
#include <unordered_map>
#include <stdexcept>
#include <optional>
#include <unordered_set>
#include "domain.h"
#include <map>
namespace transport {

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

		const std::map<std::string_view, const Bus*> GetSortedAllBuses() const;

		const std::map<std::string_view, const Stop*> GetSortedAllStops() const;


	private:
		std::deque<Stop> all_stops_;
		std::deque<Bus> all_buses_;
		std::unordered_map<std::string, Stop*> stopname_to_stop_;
		std::unordered_map<std::string, Bus*> busname_to_bus_;
		std::unordered_map<std::pair<const Stop*, const Stop*>, int, StopDistancesHasher> distances_;
	};
}
