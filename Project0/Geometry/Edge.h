#pragma once
#include <limits>
#include "Point.h"
#include "Edges.h"
#include "PointEdge.h"
#include "Orientation.h"

class Edge
{
private:
	Point m_org;
	Point m_dst;
public:
	Edge();
	Edge(const Point& org, const Point& dst);
	const Point org() const;
	const Point dst() const;
	const Point& orgRef() const;
	const Point& dstRef() const;
	Edge& rot(); // 90 deg clockwise around midpoint
	Edge& rot(Orientation); 
	Edge& flip();
	const Point point(double); // point along the edge with parameter t
	// t = 0 -> origin, t = 1 -> destination
	Edges intersect(const Edge&, double&) const; // lines 
	Edges cross(const Edge&, double&) const; // edges (segments)
	bool isVertical();
	double slope();
	double y(double); // returns y on line for given x
};

inline Edge::Edge() : m_org(Point(0.0, 0.0)), m_dst(Point(0.0, 0.0)) {}

inline Edge::Edge(const Point& org, const Point& dst) : m_org(org), m_dst(dst) {}


inline const Point Edge::org() const
{
	return m_org;
}

inline const Point Edge::dst() const
{
	return m_dst;
}
inline const Point& Edge::orgRef() const
{
	return m_org;
}

inline const Point& Edge::dstRef() const
{
	return m_dst;
}

inline Edge& Edge::rot()
{
	Point m = 0.5 * (m_org + m_dst);
	Point v = m_dst - m_org;
	Point n(v.y(), - v.x()); // clockwise
	m_org = m - 0.5 * n;
	m_dst = m + 0.5 * n;
	return *this;
}

inline Edge& Edge::rot(Orientation r)
{
	Point m = 0.5 * (m_org + m_dst);
	Point v = m_dst - m_org;
	Point n;
	if (r == Orientation::CLOCKWISE)
		n = Point(v.y(), -v.x());
	else
		n = Point(-v.y(), v.x());
	m_org = m - 0.5 * n;
	m_dst = m + 0.5 * n;
	return *this;
}

inline Edge& Edge::flip()
{
	const Point temp = m_org;
	m_org = m_dst;
	m_dst = temp;
	return (*this);
}

inline const Point Edge::point(double t)
{
	return Point(m_org + t * (m_dst - m_org));
}
/***************************************************\
    e           this
	d			  b
	|			   \
	|	n			\
	|--------		 \
	|				  \
	|				   a
	c
	P - cross point
	P(t) = a + t(b - a) 
	P(t) - c lies on e therefore perpendicular to n
	n * (P(t) - c) = 0
	t = -(n * (a - c)) / (n * (b - a))
	n * (b - a) =0 -> e and this parallel
\***************************************************/
inline Edges Edge::intersect(const Edge& e, double& t) const
{
	Point a = m_org;
	Point b = m_dst;
	Point c = e.org();
	Point d = e.dst();
	Point n = Point((d - c).y(), -(d - c).x());
	if (a == b)
	{
		if (c == d)
		{
			std::cout << "Intersection error, both edges invalid\n";
			return Edges::INVALID;
		}
		std::cout << "Intersection error, left edge invalid\n";
		return Edges::INVALID;
	}
	if (c == d)
	{
		std::cout << "Intersection error, right edge invalid\n";
		return Edges::INVALID;
	}
	
	double denom = dot(n, b - a);
	if (abs(denom) < 0.0)
	{
		PointEdge aclass = m_org.classify(e);
		if ((aclass == PointEdge::LEFT) || (aclass == PointEdge::RIGHT))
			return Edges::PARALLEL;
		else
			return Edges::COLINEAR;
	}
	double num = dot(n, c - a);
	t = num / denom;
	return Edges::SKEW;
}

inline Edges Edge::cross(const Edge& e, double& t) const
{
	double s;
	Edges crossType = e.intersect(*this, s);
	if ((crossType == Edges::COLINEAR) || (crossType == Edges::PARALLEL))
		return crossType;
	if ((s < 0.0) || (s > 1.0))
		return Edges::SKEW_NO_CROSS;

	intersect(e, t);
	if ((t >= 0.0) && (t <= 1.0))
		return Edges::SKEW_CROSS;
	else
		return Edges::SKEW_NO_CROSS;
}

inline bool Edge::isVertical()
{
	return (m_org.x() == m_dst.x());
}

inline double Edge::slope()
{
	if (m_org.x() != m_dst.x())
		return (m_dst.y() - m_org.y()) / (m_dst.x() - m_org.x());
	if (m_dst.y() > m_org.y())
		return std::numeric_limits<double>::infinity();
	else 
		return -std::numeric_limits<double>::infinity();
}

inline double Edge::y(double x)
{
	return slope() * (x - m_org.x()) + m_org.y();
}

inline void print(const Edge& e)
{
	std::cout << "Origin:" << std::endl;
	print(e.org());
	std::cout << "Destination:" << std::endl;
	print(e.dst());
}

