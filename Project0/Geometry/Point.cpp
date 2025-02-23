#include "Point.h"
#include "Edge.h"
#include "PointEdge.h"

PointEdge Point::classify(const Edge& e) const
{
	return classify(e.org(), e.dst());
}

double Point::distance(const Edge& e)
{
	Edge ab = e;
	ab.rot(Orientation::COUNTER_CLOCKWISE);
	Point n(ab.dst() - ab.org()); // normal to e
	n = n * (1.0 / n.length());
	Edge f(*this, *this + n); // f = n positioned at *this point
	double t;
	f.intersect(e, t);
	// t = signed distance along f to e
	return t;
}