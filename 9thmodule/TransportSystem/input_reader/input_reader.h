#pragma once
#include <iostream>
#include "transport_catalogue/transport_catalogue.h"
namespace transport {

	//заполняет класс каталог по inputstream
	void FillCatalog(std::istream& in, Catalogue& catalog);

	//распределяет остановки из строки
	std::pair<std::string, geo::Coordinates> FillStop(std::string& line);
	
	//распределяет маршруты из строки
	std::tuple<std::string, std::vector<const Stop*>, bool> FillRoute(std::string& line, const Catalogue& catalog);

	//распределяет расстояние из строки
	void FillStopDistances(std::string& line, Catalogue& catalog);
}	