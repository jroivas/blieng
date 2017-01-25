#include <blieng/blieng.h>
#include <blieng/maps.h>
#include <blieng/town.h>
#include <blieng/path.h>

int main(int argc, char **argv)
{
    boost::shared_ptr<blieng::BliengState> state(new blieng::BliengState());
    state->initialize(state);

    blieng::Maps map(state, "map1");

    std::vector<blieng::Town *> towns = map.getTowns();
    std::vector<blieng::Path> paths = map.getPaths();

    for (auto town : towns) {
        std::cout << "Town: " << town->getName() << "\n";
        std::cout << " Population: " << town->getPopulation() << "\n";
        std::cout << " Position  : " << town->getPositionX() << ", " << town->getPositionY() << "\n";
    }

    for (auto path : paths) {
        std::cout << "Path: " << path.toString() << "\n";
        std::cout << "  Length: " << path.length() << "\n";
        std::cout << "  Area  : " << path.area() << "\n";
    }

    blieng::Path example;
    example.addPoint(blieng::Point(34.05995658, -118.25632555)); // 0.44 km from last point
    example.addPoint(blieng::Point(34.07592613, -118.25789243)); // 1.78 km from previous point
    example.addPoint(blieng::Point(34.06234011, -118.26007226)); // 1.52 km from previous point
    // Length totals ~3.3km
    // Area totals ~0.28 km^2

    std::cout << "Path: " << example.toString() << "\n";
    std::cout << "  Length geo: " << example.lengthGeo() / 1000 << " km\n";
    std::cout << "  Area geo  : " << example.areaGeo() / 1000000 << " km^2\n";


    return 0;
}
