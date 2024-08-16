#include "stat_reader/stat_reader.h"

using namespace transport;

int main() {
    Catalogue catalogue;
    FillCatalog(std::cin, catalogue);
    ProcessRequests(std::cin, catalogue, std::cout);
}