#pragma once

/*
 * ����� ����� ���� �� ���������� ��� ����������� �������� � ����, ����������� ������, ������� ��
 * �������� �� �������� �� � transport_catalogue, �� � json reader.
 *
 * � �������� ��������� ��� ���� ���������� ��������� �� ���� ������ ����������� ��������.
 * �� ������ ����������� ��������� �������� ��������, ������� ������� ���.
 *
 * ���� �� ������������� �������, ��� ����� ���� �� ��������� � ���� ����,
 * ������ �������� ��� ������.
 */

 // ����� RequestHandler ������ ���� ������, ����������� �������������� JSON reader-�
 // � ������� ������������ ����������.
 // ��. ������� �������������� �����: https://ru.wikipedia.org/wiki/�����_(������_��������������)
 /*
 class RequestHandler {
 public:
     // MapRenderer ����������� � ��������� ����� ��������� �������
     RequestHandler(const TransportCatalogue& db, const renderer::MapRenderer& renderer);

     // ���������� ���������� � �������� (������ Bus)
     std::optional<BusStat> GetBusStat(const std::string_view& bus_name) const;

     // ���������� ��������, ���������� �����
     const std::unordered_set<BusPtr>* GetBusesByStop(const std::string_view& stop_name) const;

     // ���� ����� ����� ����� � ��������� ����� ��������� �������
     svg::Document RenderMap() const;

 private:
     // RequestHandler ���������� ��������� �������� "������������ ����������" � "������������ �����"
     const TransportCatalogue& db_;
     const renderer::MapRenderer& renderer_;
 };
 */
 //
 //#include "transport_catalogue.h"
 //class RequestHandler {
 //public:
 //    // MapRenderer ����������� � ��������� ����� ��������� �������
 //    RequestHandler(const transport::Catalogue& db, const renderer::MapRenderer& renderer);
 //
 //    // ���������� ���������� � �������� (������ Bus)
 //    std::optional<BusStat> GetBusStat(const std::string_view& bus_name) const;
 //
 //    // ���������� ��������, ���������� �����
 //    const std::unordered_set<BusPtr>* GetBusesByStop(const std::string_view& stop_name) const;
 //
 //    // ���� ����� ����� ����� � ��������� ����� ��������� �������
 //    svg::Document RenderMap() const;
 //
 //private:
 //    // RequestHandler ���������� ��������� �������� "������������ ����������" � "������������ �����"
 //    const TransportCatalogue& db_;
 //    const renderer::MapRenderer& renderer_;
 //};


#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "routing_settings.h"
#include <sstream>
class RequestHandler {
private:
    const transport::Catalogue& catalogue_;
    const MapRenderer& renderer_;
    const transport::RouteSettings& router_;

public:
    RequestHandler(const transport::Catalogue& db, const MapRenderer& renderer, const transport::RouteSettings& router)
        : catalogue_(db)
        , renderer_(renderer)
        , router_(router)
    {
    }

    void ProcessRequests(const json::Node& stat_requests) const;

    const json::Node PrintRoute(const json::Dict& request_map) const;
    const json::Node PrintStop(const json::Dict& request_map) const;

    bool IsBusNumber(const std::string_view bus_number) const;
    bool IsStopName(const std::string_view stop_name) const;

    std::optional<BusInfo> GetBusStat(const std::string_view& bus_number) const;
    const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;
    const std::optional<graph::Router<double>::RouteInfo> GetOptimalRoute(const std::string_view stop_from, const std::string_view stop_to) const;
    const graph::DirectedWeightedGraph<double>& GetRouterGraph() const;
    svg::Document RenderMap() const;


};