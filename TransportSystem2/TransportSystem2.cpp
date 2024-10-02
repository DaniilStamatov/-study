
#include "json_reader.h"
#include "request_handler.h"
#include "routing_settings.h"
int main() {
    transport::Catalogue catalogue;
    JsonReader json_doc(std::cin);

    json_doc.FillCatalogue(catalogue);

    const auto& stat_requests = json_doc.GetStatRequests();
    const auto& render_settings = json_doc.GetRenderSettings().AsDict();
    const auto& renderer = json_doc.FillRenderSettings(render_settings);
    const auto& routing_settings = json_doc.FillRouteSettings(json_doc.GetRoutingSettings());
    const transport::RouteSettings router { routing_settings, catalogue };


    RequestHandler rh(catalogue, renderer, router);
    json_doc.ProcessRequests(stat_requests, rh);
   // rh.RenderMap().Render(std::cout);
}