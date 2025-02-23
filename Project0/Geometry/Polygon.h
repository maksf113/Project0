#pragma once
#include "Vertex.h"
#include "Edge.h"
#include "Orientation.h"
#include "PointEdge.h"

class Polygon
{
private:
	Vertex* m_v;
	int m_size;
	void _resize();
public:
	Polygon();
	Polygon(Polygon&);
	Polygon(Vertex*);
	~Polygon();
	Vertex* v() const;
	const int size() const;
	Point point() const;
	Edge edge() const;
	Vertex* cw() const;
	Vertex* ccw() const;
	Vertex* neighbour(Orientation r = Orientation::CLOCKWISE) const;
	Vertex* advance(Orientation r = Orientation::CLOCKWISE);
	Vertex* setV(Vertex*); // set window over existing polygons vertex
	Vertex* insert(const Point&);
	void remove();
	Polygon* split(Vertex*);
};

Polygon::Polygon() : m_v(nullptr), m_size(0) {}
Polygon::Polygon(Polygon& p)
{
	m_size = p.size();
	if (m_size == 0)
		m_v = nullptr;
	else
	{
		m_v = new Vertex(p.point());
		for (int i = 0; i < m_size; i++)
		{
			p.advance(Orientation::CLOCKWISE);
			m_v = m_v->insert(new Vertex(point()));
		}
		p.advance(Orientation::CLOCKWISE);
		m_v = m_v->cw();
	}
}

Polygon::Polygon(Vertex* v) : m_v(v)
{
	_resize();
}

void Polygon::_resize()
{
	if (m_v == nullptr)
		m_size = 0;
	else
	{
		Vertex* v = m_v->cw();
		for (m_size = 1; v != m_v; ++m_size, v = v->cw());
	}
}

Polygon::~Polygon()
{
	if (m_v)
	{
		Vertex* w = m_v->cw();
		while (w != m_v)
		{
			delete w->remove();
			w = m_v->cw();
		}
	}
}

inline Vertex* Polygon::v() const
{
	return m_v;
}

inline const int Polygon::size() const
{
	return m_size;
}

inline Point Polygon::point() const
{
	return m_v->point();
}

inline Edge Polygon::edge() const
{
	return Edge(point(), m_v->cw()->point());
}

inline Vertex* Polygon::cw() const
{
	return m_v->cw();
}

inline Vertex* Polygon::ccw() const
{
	return m_v->ccw();
}

inline Vertex* Polygon::neighbour(Orientation r) const
{
	return m_v->neighbour(r);
}

inline Vertex* Polygon::advance(Orientation r) 
{
	return m_v = m_v->neighbour(r);
}

inline Vertex* Polygon::setV(Vertex* v) // !! v must be polygon vertex
{
	return m_v = v;
}

inline Vertex* Polygon::insert(const Point& p)
{
	if (m_size++ == 0)
		m_v = new Vertex(p);
	else
		m_v = m_v->insert(new Vertex(p));
	return m_v;
}

inline void Polygon::remove()
{
	Vertex* w = m_v;
	m_v = (--m_size == 0) ? nullptr : m_v->ccw();
	delete m_v->remove();
}

// if b is successor of curren m_v, split leaves polygon unchanged
// if cut along diagonal that is not a chor -> self crossing
// if m_v and b belong to two different polygons, 
// split joins them with two coincident edges that connect the two vertices
inline Polygon* Polygon::split(Vertex* b)
{
	Vertex* bp = m_v->split(b);
	_resize();
	return new Polygon(bp);
}

bool pointInConvexPolygon(const Point& s, Polygon& p)
{
	if (p.size() == 1)
		return (s == p.point());
	if (p.size() == 2)
	{
		PointEdge c = s.classify(p.edge());
		return ((c == PointEdge::BETWEEN) || (c == PointEdge::ORIGIN) 
			|| (c == PointEdge::DESTINATION));
	}
	Vertex* org = p.v();
	for (int i = 0; i < p.size(); i++, p.advance(Orientation::CLOCKWISE))
		if (s.classify(p.edge()) == PointEdge::LEFT)
		{
			p.setV(org);
			return false;
		}
	return true;
}

Vertex* leastVertex(Polygon& p, int (*cmp)(const Point*, const Point*))
{
	Vertex* bestV = p.v();
	p.advance(Orientation::CLOCKWISE);
	for(int i = 1; i < p.size(); p.advance(Orientation::CLOCKWISE), i++)
		if((*cmp)(p.v(), bestV) < 0) 
			bestV = p.v();
	p.setV(bestV);
	return bestV;
}

int leftToRightCmp(Point* a, Point* b)
{
	if (*a < *b) return -1;
	if (*a > *b) return 1;
	return 0;
}

int rightToLeftCmp(Point* a, Point* b)
{
	return leftToRightCmp(b, a);
}