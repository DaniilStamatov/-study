#include "map_renderer.h"

/*
 * В этом файле вы можете разместить код, отвечающий за визуализацию карты маршрутов в формате SVG.
 * Визуализация маршрутов вам понадобится во второй части итогового проекта.
 * Пока можете оставить файл пустым.
 */

std::vector<svg::Polyline> MapRenderer::GetRouteLines(const std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const
{
    std::vector<svg::Polyline> result;
    size_t color_num = 0;
    for (const auto& [bus_number, bus] : buses) {
        if (bus->stops.empty()) continue;
        std::vector<const Stop*> route_stops{ bus->stops.begin(), bus->stops.end() };
        if (bus->is_circle == false) route_stops.insert(route_stops.end(), std::next(bus->stops.rbegin()), bus->stops.rend());
        svg::Polyline line;
        for (const auto& stop : route_stops) {
            line.AddPoint(sp(stop->coordinates));
        }
        line.SetStrokeWidth(settings_.line_width);
        line.SetStrokeColor(settings_.color_palette[color_num]);
        line.SetFillColor("none");
        line.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        line.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

        if (color_num < (settings_.color_palette.size() - 1)) ++color_num;
        else color_num = 0;

        result.push_back(line);
    }
    return result;
}

std::vector<svg::Text> MapRenderer::GetBusLabels(const std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const
{
    std::vector<svg::Text> result;
    size_t color_num = 0;
    for (const auto& [bus_number, bus] : buses) {
        if (bus->stops.empty()) continue;
        svg::Text text;
        svg::Text underlayer;
        text.SetPosition(sp(bus->stops[0]->coordinates));
        text.SetOffset(settings_.bus_label_offset);
        text.SetFontSize(settings_.bus_label_font_size);
        text.SetData(bus->number);
        text.SetFontFamily("Verdana");
        text.SetFontWeight("bold");
        text.SetFillColor(settings_.color_palette[color_num]);
        if (color_num < (settings_.color_palette.size() - 1)) ++color_num;
        else color_num = 0;
        underlayer.SetPosition(sp(bus->stops[0]->coordinates));
        underlayer.SetOffset(settings_.bus_label_offset);
        underlayer.SetFontSize(settings_.bus_label_font_size);
        underlayer.SetData(bus->number);
        underlayer.SetFontFamily("Verdana");
        underlayer.SetFontWeight("bold");
        underlayer.SetFillColor(settings_.underlayer_color);
        underlayer.SetStrokeColor(settings_.underlayer_color);
        underlayer.SetStrokeWidth(settings_.underlayer_width);
        underlayer.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        underlayer.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

        result.push_back(underlayer);
        result.push_back(text);

        if (!bus->is_circle && bus->stops[0] != bus->stops[bus->stops.size() - 1]) {
            svg::Text text2{ text };
            svg::Text underlayer2{ underlayer };
            underlayer2.SetPosition(sp(bus->stops[bus->stops.size() - 1]->coordinates));
            text2.SetPosition(sp(bus->stops[bus->stops.size() - 1]->coordinates));
            result.push_back(underlayer2);
            result.push_back(text2);
        }

    }
    return result;
}

std::vector<svg::Circle> MapRenderer::GetStopSymbols(const std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const
{
    std::vector<svg::Circle> result;
    for (const auto& [stop_name, stop] : stops) {
        svg::Circle circle;
        circle.SetCenter(sp(stop->coordinates));
        circle.SetFillColor("white");
        circle.SetRadius(settings_.stop_radius);
        result.push_back(circle);
    }
    return result;
}

std::vector<svg::Text> MapRenderer::GetStopLabels(const std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const
{
    std::vector<svg::Text> result;
    for (const auto& [stop_name, stop] : stops) {
        svg::Text text;
        text.SetPosition(sp(stop->coordinates));
        text.SetOffset(settings_.stop_label_offset);
        text.SetFontSize(settings_.stop_label_font_size);
        text.SetFontFamily("Verdana");
        text.SetData(stop->name);
        text.SetFillColor("black");
        svg::Text underlayer;
        underlayer.SetPosition(sp(stop->coordinates));
        underlayer.SetOffset(settings_.stop_label_offset);
        underlayer.SetFontSize(settings_.stop_label_font_size);
        underlayer.SetFontFamily("Verdana");
        underlayer.SetData(stop->name);
        underlayer.SetFillColor(settings_.underlayer_color);
        underlayer.SetStrokeColor(settings_.underlayer_color);
        underlayer.SetStrokeWidth(settings_.underlayer_width);
        underlayer.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        underlayer.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
        result.push_back(underlayer);
        result.push_back(text);
    }
    return result;
}

svg::Document MapRenderer::GetSVG(const std::map<std::string_view, const Bus*>& buses) const
{
    svg::Document result;
    std::vector<geo::Coordinates> route_stops_coord;
    std::map<std::string_view, const Stop*> all_stops;
    for (const auto& [bus_number, bus] : buses) {
        for (const auto& stop : bus->stops) {
            route_stops_coord.push_back(stop->coordinates);
            all_stops[stop->name] = stop;
        }
    }
    SphereProjector sp(route_stops_coord.begin(), route_stops_coord.end(), settings_.width, settings_.height, settings_.padding);
    for (const auto& line : GetRouteLines(buses, sp)) {
        result.Add(line);
    }
    for (const auto& text : GetBusLabels(buses, sp)) {
        result.Add(text);
    }
    for (const auto& stop : GetStopSymbols(all_stops, sp)) {
        result.Add(stop);
    }
    for (const auto& stop : GetStopLabels(all_stops, sp)) {
        result.Add(stop);
    }

    return result;
}
