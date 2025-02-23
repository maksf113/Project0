#pragma once
#include <vector>
#include "Polygon.h"
#include "Orientation.h"

class StarPolygon : public Polygon
{
public:
	StarPolygon(std::vector<Point>&);
};

StarPolygon::StarPolygon(std::vector<Point>& points) : Polygon()
{
	insert(points[0]);
	Vertex* origin = v();

	PolarCmp polarCmp(origin->point());

	for (auto& p : points)
	{
		setV(origin);
		advance(Orientation::CLOCKWISE);
		while(polarCmp(&p, v()) < 0)
			advance(Orientation::CLOCKWISE);
		advance(Orientation::COUNTER_CLOCKWISE);
		insert(p);
	}
}