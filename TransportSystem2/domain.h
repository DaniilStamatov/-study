#pragma once
#include <string>
#include <set>
#include "geo.h"
#include <vector>
/*
 * � ���� ����� �� ������ ���������� ������/���������, ������� �������� ������ ���������� ������� (domain)
 * ������ ���������� � �� ������� �� ������������� �����������. �������� ���������� �������� � ���������.
 *
 * �� ����� ���� �� ���������� � � transport_catalogue.h, ������ ��������� �� � ���������
 * ������������ ���� ����� ��������� ��������, ����� ���� ����� �� ������������ ����� ���������:
 * ������������ ����� (map_renderer) ����� ����� ������� ����������� �� ������������� �����������.
 *
 * ���� ��������� ������ ���������� �� ��������� ��� �������, ������ �������� ���� ���� ������.
 *
 */

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
