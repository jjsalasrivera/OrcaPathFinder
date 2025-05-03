//
//  main.cpp
//  OrcaPathFinder
//
//  Created by Juan Jesus Salas on 27/4/25.
//

#include <iostream>
#include <fstream>
#include <geos.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/geom/LineSegment.h>
#include <nlohmann/json.hpp>

#include "Common.h"
#include "Algorithm.h"

#define USE_UNSTABLE_GEOS_CPP_API

#define GATEWAYS_FILE "gateways.json"
#define POINTS_FILE "points.json"

using json = nlohmann::json;
using namespace geos::geom;


int main(int argc, const char * argv[])
{
    // Start the GEOS library
    GeometryFactory::Ptr factory = GeometryFactory::create();
    
    std::ifstream file(GATEWAYS_FILE);
    if (!file.is_open()) {
        std::cerr << "Cant open file " << GATEWAYS_FILE << std::endl;
        return 1;
    }
    
    json j;
    file >> j;
    
    // Read segments gateways
    std::vector<Segment> segments;
    for( size_t i = 0; i < j.size(); i += 2)
    {
        std::string originLabel = j[i]["label"];
        double x1 = j[i]["x"];
        double y1 = j[i]["y"];
        
        std::string destinationLabel = j[i+1]["label"];
        double x2 = j[i+1]["x"];
        double y2 = j[i+1]["y"];
        
        // Crear un punto GEOS
        Coordinate origin(x1, y1);
        Coordinate destination(x2, y2);
        
        Segment segment;
        segment.label = originLabel + "-" + destinationLabel;
        segment.line = LineSegment(origin, destination);

        segments.push_back(segment);
    }
    
    // Read Start and Finish points
    file.close();
    
    file.open(POINTS_FILE);
    if (!file.is_open()) {
        std::cerr << "Cant open file " << POINTS_FILE << std::endl;
        return 1;
    }
    
    j.clear();
    file >> j;
    
    double x = j[0]["x"];
    double y = j[0]["y"];
        Coordinate coord(x, y);
    std::unique_ptr<Point> from_p = factory->createPoint(coord);
    
    x = j[1]["x"];
    y = j[1]["y"];
    
    coord.x = x;
    coord.y = y;
    std::unique_ptr<Point> to_p = factory->createPoint(coord);
    
    file.close();
    
    Algorithm algorithm(std::move(from_p), std::move(to_p), std::move(segments));
    auto path = algorithm.run();
    
    double length = 0;
    std::cout << "Path:" << std::endl;
    for (const auto& segment : path) {
        std::cout << "(" << segment.line.p0.x << ", " << segment.line.p0.y << ") - (" << segment.line.p1.x << ", " << segment.line.p1.y << ")" << std::endl;
        
        length += segment.line.getLength();
    }
    
    std::cout << "Total length: " << length << std::endl;
    
    return 0;
}
