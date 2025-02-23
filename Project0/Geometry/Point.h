#pragma once
#include <iostream>
#include <cmath>
#include "PointEdge.h"

class Edge;

class Point
{
private:
	double m_x;
	double m_y;
public:
	Point(double x = 0.0, double y = 0.0);
	double x() const; //read only
	double y() const; //read only
	void x(double);
	void y(double);
	Point operator+(const Point&) const;
	Point operator-(const Point&) const;
	Point& operator+=(const Point&);
	Point& operator-=(const Point&);
	friend Point operator*(const double, const Point&);
	friend Point operator*(const Point&, const double);
	friend Point operator/(const Point&, const double);
	double operator[](int); // read - write
	int operator==(const Point&) const;
	int operator!=(const Point&) const;
	int operator<(const Point&) const; // lexicographic order
	int operator>(const Point&) const; // lexicographic order
	PointEdge classify(const Point&, const Point&) const;
	PointEdge classify(const Edge&) const;
	double polarAngle();
	double length();
	double distance(const Edge&); // signed distance to the left
	double distance(const Point&) const;
};
inline const double pi()
{
	return atan(1.0) * 4.0;
}
inline Point::Point(double x, double y) : m_x(x), m_y(y) {}
inline double Point::x() const { return m_x; }
inline double Point::y() const  { return m_y; }
inline void Point::x(double x) { m_x = x; }
inline void Point::y(double y) { m_y = y; }
inline Point& Point::operator+=(const Point& p)
{
	m_x += p.x();
	m_y += p.y();
	return *this;
}
inline Point& Point::operator-=(const Point& p)
{
	m_x -= p.x();
	m_y -= p.y();
	return *this;
}
inline Point Point::operator+(const Point& p) const
{
	return Point(m_x + p.x(), m_y + p.y());
}

inline Point Point::operator-(const Point& p) const
{
	return Point(m_x - p.x(), m_y - p.y());
} 

inline Point operator*(const double s, const Point& p)
{
	return Point(s * p.x(), s * p.y());
}

inline Point operator*(const Point& p, const double s)
{
	return Point(s * p.x(), s * p.y());
}


inline Point operator/(const Point& p, const double s)
{
	return Point(p.x() / s, p.y() / s);
}

inline double Point::operator[](int i)
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else
		std::cerr<<"Point operator[] invalid argument" << std::endl;
}

inline int Point::operator==(const Point& p) const
{
	return ((m_x == p.x()) && (m_y == p.y()));
}

inline int Point::operator!=(const Point& p) const
{
	return ((m_x != p.x()) || (m_y != p.y()));
}

inline int Point::operator<(const Point& p) const
{
	return ((m_x < p.x()) || ((m_x == p.x()) && (m_y < p.y())));
}

inline int Point::operator>(const Point& p) const
{
	return ((m_x > p.x()) || ((m_x == p.x()) && (m_y > p.y())));
}

inline int orientation(const Point& p0, const Point& p1, const Point& p2)
{
	Point a = p1 - p0;
	Point b = p2 - p0;
	
	double sa = a.x() * b.y() - a.y() * b.x();
	if (sa > 0.0)
		return 1;
	if (sa < 0)
		return -1;
	return 0;
}

const double epsilon = 1e-15;
inline PointEdge Point::classify(const Point& p0, const Point& p1) const
{
	if (p0 == p1)
	{
		std::cout << "PointEdge classification warning: edge degraded to point\n";
		return PointEdge::INVALID;
	}
	Point p2 = *this;
	Point a = p1 - p0;
	Point b = p2 - p0;
	double sa = a.x() * b.y() - a.y() * b.x();
	if (sa > 0.0)
		return PointEdge::LEFT;
	if (sa < -epsilon)
		return PointEdge::RIGHT;
	if ((a.x() * b.x() < 0.0) || (a.y() * b.y() < -epsilon))
		return PointEdge::BEHIND;
	if (a.length() < b.length())
		return PointEdge::BEYOND;
	if (p0 == p2)
		return PointEdge::ORIGIN;
	if (p1 == p2)
		return PointEdge::DESTINATION;
	return PointEdge::BETWEEN;
}


inline double Point::polarAngle()
{
	if ((m_x == 0.0) && (m_y == 0.0))
		return -1.0;
	if (m_x == 0.0)
		return (m_y > 0.0 ? 90.0 : 270.0);
	double theta = atan(m_y / m_x);
	theta *= 180.0 / pi();
	if (m_x > 0.0) // quadrants 1, 4
		return (m_y >= 0.0 ? theta : 360.0 + theta);
	else // quadrants 2, 3
		return (180.0 + theta);
}

inline double Point::length()
{
	return sqrt(m_x * m_x + m_y * m_y);
}

inline double dot(const Point& u, const Point& v)
{
	return (u.x() * v.x() + u.y() * v.y());
}

inline double Point::distance(const Point& p) const
{
	return sqrt(dot(*this - p, *this - p));
}

class PolarCmp
{
private:
	const Point origin;
public:
	PolarCmp(const Point& org) : origin(org) {}
	int operator()(Point* p, Point* q)
	{
		Point vp = *p - origin;
		Point vq = *q - origin;
		double pPolar = vp.polarAngle();
		double qPolar = vq.polarAngle();
		if (pPolar < qPolar)
			return -1;
		if (pPolar > qPolar)
			return 1;
		if (vp.length() < vq.length())
			return -1;
		if (vp.length() > vq.length())
			return 1;
		return 0;
	}
	
};
inline void print(const Point& p)
{
	std::cout << "x = " << p.x() << "\t" << "y =  " << p.y() << std::endl;
}