#pragma once

/*
 * � ���� ����� �� ������ ���������� ���, ���������� �� ������������ ����� ��������� � ������� SVG.
 * ������������ ��������� ��� ����������� �� ������ ����� ��������� �������.
 * ���� ������ �������� ���� ������.
 */
#include <map>

#include "geo.h"
#include "svg.h"
#include "transport_catalogue.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

inline const double EPSILON = 1e-6;


class SphereProjector {
public:
    // points_begin � points_end ������ ������ � ����� ��������� ��������� geo::Coordinates
    template <typename PointInputIt>
    SphereProjector(PointInputIt points_begin, PointInputIt points_end,
        double max_width, double max_height, double padding)
        : padding_(padding) //
    {
        // ���� ����� ����������� ����� �� ������, ��������� ������
        if (points_begin == points_end) {
            return;
        }

        // ������� ����� � ����������� � ������������ ��������
        const auto [left_it, right_it] = std::minmax_element(
            points_begin, points_end,
            [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
        min_lon_ = left_it->lng;
        const double max_lon = right_it->lng;

        // ������� ����� � ����������� � ������������ �������
        const auto [bottom_it, top_it] = std::minmax_element(
            points_begin, points_end,
            [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
        const double min_lat = bottom_it->lat;
        max_lat_ = top_it->lat;

        // ��������� ����������� ��������������� ����� ���������� x
        std::optional<double> width_zoom;
        if (!IsZero(max_lon - min_lon_)) {
            width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
        }

        // ��������� ����������� ��������������� ����� ���������� y
        std::optional<double> height_zoom;
        if (!IsZero(max_lat_ - min_lat)) {
            height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
        }

        if (width_zoom && height_zoom) {
            // ������������ ��������������� �� ������ � ������ ���������,
            // ���� ����������� �� ���
            zoom_coeff_ = std::min(*width_zoom, *height_zoom);
        }
        else if (width_zoom) {
            // ����������� ��������������� �� ������ ���������, ���������� ���
            zoom_coeff_ = *width_zoom;
        }
        else if (height_zoom) {
            // ����������� ��������������� �� ������ ���������, ���������� ���
            zoom_coeff_ = *height_zoom;
        }
    }

    // ���������� ������ � ������� � ���������� ������ SVG-�����������
    svg::Point operator()(geo::Coordinates coords) const {
        return {
            (coords.lng - min_lon_) * zoom_coeff_ + padding_,
            (max_lat_ - coords.lat) * zoom_coeff_ + padding_
        };
    }

private:

    bool IsZero(double value) {
        return std::abs(value) < EPSILON;
    }
    double padding_;
    double min_lon_ = 0;
    double max_lat_ = 0;
    double zoom_coeff_ = 0;
};

struct RenderSettings {
    double width = 0.0;
    double height = 0.0;
    double padding = 0.0;
    double line_width = 0.0;
    double stop_radius = 0.0;
    int bus_label_font_size = 0;
    svg::Point bus_label_offset = { 0, 0 };
    int stop_label_font_size = 0;
    svg::Point stop_label_offset = { 0,0 };
    svg::Color underlayer_color = { svg::NoneColor };
    double underlayer_width = 0.0;
    std::vector<svg::Color> color_palette{};
};

class MapRenderer {
private:
    const RenderSettings settings_;
public:
    MapRenderer(const RenderSettings& settings) : settings_(settings) {

    }

    std::vector<svg::Polyline> GetRouteLines(const std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const;
    std::vector<svg::Text> GetBusLabels(const std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const;
    std::vector<svg::Circle> GetStopSymbols(const std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const;
    std::vector<svg::Text> GetStopLabels(const std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const;
    svg::Document GetSVG(const std::map<std::string_view, const Bus*>& buses) const;
};
