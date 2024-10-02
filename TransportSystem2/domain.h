#pragma once
#include <string>
#include <set>
#include "geo.h"
#include <vector>
/*
 * В этом файле вы можете разместить классы/структуры, которые являются частью предметной области (domain)
 * вашего приложения и не зависят от транспортного справочника. Например Автобусные маршруты и Остановки.
 *
 * Их можно было бы разместить и в transport_catalogue.h, однако вынесение их в отдельный
 * заголовочный файл может оказаться полезным, когда дело дойдёт до визуализации карты маршрутов:
 * визуализатор карты (map_renderer) можно будет сделать независящим от транспортного справочника.
 *
 * Если структура вашего приложения не позволяет так сделать, просто оставьте этот файл пустым.
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
