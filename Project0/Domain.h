#pragma once
#include <vector>
#include <cmath>
#include "Random.h"
#include "Geometry/Point.h"
#include "Geometry/Polygon.h"
#include "PoissonDiscSampler.h"

class Domain
{
private:
    double m_scale = 0.8;
    double m_radius = m_scale * 0.5;
    int m_numberOfOuterBoundarySidePoints = 30;
    int m_numberOfInnerBoundaryPoints = 2 * pi() * m_radius *( m_numberOfOuterBoundarySidePoints - 1) / (2.0 * m_scale);
	std::vector<Point> m_points;
    std::vector<Point> m_outerBoundary;
    std::vector<Point> m_innerBoundary;
    void genInnerBoundary();
    void genOuterBoundary();
    void genInnerPoints();
public:
	Domain();
    const std::vector<Point>& points() const { return m_points; }
    const std::vector<Point>& innerBoundary() const { return m_innerBoundary; }
    static bool _isInDomain(const Point&);
    friend bool isInDomain(const Point& p);
};

Domain::Domain()
{
    genInnerBoundary();
    genOuterBoundary();
    for (const auto& p : m_innerBoundary)
        m_points.push_back(p);
    for (const auto& p : m_outerBoundary)
        m_points.push_back(p);
    genInnerPoints();
}
void Domain::genInnerBoundary()
{
    m_innerBoundary.resize(m_numberOfInnerBoundaryPoints);
    double da = 2.0 * pi() / m_numberOfInnerBoundaryPoints;
    for (int i = 0; i < m_numberOfInnerBoundaryPoints; i++)
    {
        double a = da * i;
        m_innerBoundary[i].x(m_radius * cos(a));
        m_innerBoundary[i].y(m_radius * sin(a));
    }
}

void Domain::genOuterBoundary()
{
    double h = 2.0 * m_scale / (m_numberOfOuterBoundarySidePoints - 1);
    for (int i = 0; i < m_numberOfOuterBoundarySidePoints; i++)
    {
        double d = -m_scale + h * i;
        m_outerBoundary.push_back({ d, -m_scale });
        m_outerBoundary.push_back({ d, m_scale });
        if (i > 0 && i < m_numberOfOuterBoundarySidePoints - 1)
        {
            m_outerBoundary.push_back({ -m_scale, d });
            m_outerBoundary.push_back({ m_scale, d });
        }
            
    }
}
//bool Domain::_isInDomain(const Point& p)
//{
//    if (p.x() * p.x() + p.y() * p.y() <= m_radius * m_radius) // radius !!!!!!!!!!!!!!!!!!!!
//        return false;
//    if (p.x() <= -m_scale) // m_scale !!!!!!!!!!!!!!!!!!!!
//        return false;
//    if (p.x() >= m_scale)
//        return false;
//    if (p.y() <= -m_scale)
//        return false;
//    if (p.y() >= m_scale)
//        return false;
//    return true;
//}
bool isInDomain(const Point& p)
{
    if (p.x() * p.x() + p.y() * p.y() <= 0.4 * 0.4) // radius !!!!!!!!!!!!!!!!!!!!
        return false;
    if (p.x() <= -0.8) // m_scale !!!!!!!!!!!!!!!!!!!!
        return false;
    if (p.x() >= 0.8)
        return false;
    if (p.y() <= -0.8)
        return false;
    if (p.y() >= 0.8)
        return false;
    return true;
}
void Domain::genInnerPoints()
{
    PoissonDiscSampler sampler(m_points, isInDomain);
}
