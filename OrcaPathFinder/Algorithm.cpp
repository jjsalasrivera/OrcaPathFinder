//
//  Algorithm.cpp
//  OrcaPathFinder
//
//  Created by Juan Jesus Salas on 29/4/25.
//

#include "Algorithm.h"
#include <geos/algorithm/Orientation.h>

using geos::algorithm::Orientation;

Algorithm::Algorithm(std::unique_ptr<Point> from, std::unique_ptr<Point> to, std::vector<Segment> segments) : _from(std::move(from)), _to(std::move(to)), _segments(std::move(segments))
{
}

Algorithm::~Algorithm()
{
}

std::vector<Segment> Algorithm::run() const
{
    std::cout << "Calculating path..." << std::endl;
    
    std::vector<Segment> result;
    
    Point* from = _from.get();
    int numSegmentVisited = 0;
    
    CoordinateXY segmentCoordinate;
    do
    {
        std::vector<Segment> subSegments(_segments.begin() + numSegmentVisited, _segments.end());
        
        Segment subPath;
        numSegmentVisited += getSubPath(from, _to.get()->clone().get(), subSegments, subPath);
        
        result.push_back(subPath);
        from->setXY(subPath.line.p1.x, subPath.line.p1.y);
        
        segmentCoordinate = subPath.line.p1;
    } while (!_to->getCoordinate()->equals(segmentCoordinate));
    
    return result;
}

int Algorithm::getSubPath(Point * from, Point * toPoint, std::vector<Segment> segments, Segment& res) const
{
    Coordinate s1(from->getX(), from->getY());
    size_t i = segments.size();
    
    for(i = segments.size() - 1; i >= 0; --i)
    {
        Coordinate s2(toPoint->getX(), toPoint->getY());
        LineSegment temporaryPath((Coordinate(s1)), Coordinate(s2));
        
        std::vector<Segment> subSegments(segments.begin(), segments.begin() + i + 1);
        
        bool found = true;
        
        for (auto segment_it = subSegments.rbegin(); segment_it != subSegments.rend(); ++segment_it)
        {
            Segment* segment = &(*segment_it);
            
            Coordinate intersecctionPoint = segment->line.intersection(temporaryPath);
            
            // Check if cross the segment and point 0 is on the left
            if(intersecctionPoint.isNull() || Orientation::index(temporaryPath.p0, temporaryPath.p1, segment->line.p0) != Orientation::LEFT)
            {
                // Now the end point is the nearest point of the neartest segment
                Coordinate closestPoint;
                Coordinate toCoordinate(toPoint->getX(), toPoint->getY());
                segment->line.closestPoint(toCoordinate, closestPoint);
                toPoint->setXY(closestPoint.x, closestPoint.y);
                
                found = false;
                
                break;
            }
        }
        
        if(found)
        {
            res.line = temporaryPath;
            break;
        }
    }
    
    // +1 for the index and +1 to discard the last segment
    return (int)i + 2;
}
