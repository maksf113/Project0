#pragma once
#include "../Data Structures/Node.h"
#include "Point.h"
#include "Orientation.h"

class Vertex : public Node, public Point
{
public:
	Vertex(double, double);
	Vertex(const Point&);
	Vertex* cw();
	Vertex* ccw();
	Vertex* neighbour(Orientation);
	const Point& point() const;
	//const Point& point() const;
	Vertex* insert(Vertex*);
	Vertex* remove();
	void splice(Vertex*); 
	Vertex* split(Vertex*);
	friend class Polygon;
};

Vertex::Vertex(double x, double y) : Point(x, y) {}
Vertex::Vertex(const Point& p) : Point(p) {}

Vertex* Vertex::cw()
{
	return (Vertex*)m_next;
}

Vertex* Vertex::ccw()
{
	return (Vertex*)m_prev;
}



Vertex* Vertex::neighbour(Orientation orientation)
{
	return (orientation == Orientation::CLOCKWISE ? cw() : ccw());
}

const Point& Vertex::point() const
{
	return *((Point*)this);
}

Vertex* Vertex::insert(Vertex* v)
{
	return (Vertex*)(Node::insert(v));
}

Vertex* Vertex::remove()
{
	return (Vertex*)(Node::remove());
}

void Vertex::splice(Vertex* v)
{
	Node::splice(v);
}

inline Vertex* Vertex::split(Vertex* b)
{
	// insert bp before b
	Vertex* bp = b->ccw()->insert(new Vertex(b->point()));
	insert(new Vertex(point())); // insert ap before this vertex
	splice(bp);
	return bp;
}