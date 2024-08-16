#pragma once
#include <iostream>
#include "transport_catalogue/transport_catalogue.h"
namespace transport {

	//��������� ����� ������� �� inputstream
	void FillCatalog(std::istream& in, Catalogue& catalog);

	//������������ ��������� �� ������
	std::pair<std::string, geo::Coordinates> FillStop(std::string& line);
	
	//������������ �������� �� ������
	std::tuple<std::string, std::vector<const Stop*>, bool> FillRoute(std::string& line, const Catalogue& catalog);

	//������������ ���������� �� ������
	void FillStopDistances(std::string& line, Catalogue& catalog);
}	