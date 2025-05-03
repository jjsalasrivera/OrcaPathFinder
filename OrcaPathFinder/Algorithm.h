//
//  Algorithm.h
//  OrcaPathFinder
//
//  Created by Juan Jesus Salas on 28/4/25.
//

#ifndef Algorithm_h
#define Algorithm_h

#include <iostream>
#include <geos.h>
#include <geos/geom/Point.h>

#include "Common.h"

class Algorithm {
    
public:
    Algorithm(std::unique_ptr<Point> from, std::unique_ptr<Point> to, std::vector<Segment>);

    std::vector<Segment> run() const;
    
    ~Algorithm();
    
private:
    std::unique_ptr<Point> _from;
    std::unique_ptr<Point> _to;
    std::vector<Segment> _segments;
    
    int getSubPath(Point * from, Point * to, std::vector<Segment> segments, Segment& res) const;
};


#endif /* Algorithm_h */
