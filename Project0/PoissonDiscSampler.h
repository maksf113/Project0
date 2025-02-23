#pragma once
#include <vector>
#include "Geometry/Point.h"
#include "Random.h"
#include "Grid.h"


class PoissonDiscSampler
{
private:
	Grid* m_grid;
	int m_maxAttempts = 30;
	double m_discRadius;
	std::vector<Point>& m_points;
	bool (*_isInDomain)(const Point& p);
	void getRadiusFromBoundary();
public:
	PoissonDiscSampler(std::vector<Point>& points, bool (*isInDoamin)(const Point&)); // points should have all boundary points and the size should be number of boundary points
};


PoissonDiscSampler::PoissonDiscSampler(std::vector<Point>& points, bool (*isInDomain)(const Point&)) :  m_points(points), _isInDomain(isInDomain)
{
	getRadiusFromBoundary();
	Random randomRadius(m_discRadius, 3.0 * m_discRadius);
	Random randomAngle(0.0, 2.0 * pi());
	m_grid = new Grid(points);
	while (!m_grid->isActiveEmpty())
	{
		int i, j;
		m_grid->getNextActiveIndices(i, j);
		int activePointIndex = m_grid->getPointIndex(i, j);
		Point activePoint = m_points[activePointIndex];
		
		for (int k = 0; k < m_maxAttempts; k++)
		{
			double radius = randomRadius();
			double angle = randomAngle();
			Point p = activePoint + Point{ cos(angle) * radius, sin(angle) * radius };
			if(isInDomain(p))
				m_grid->tryToAddPoint(p, m_discRadius);
			if (k == m_maxAttempts - 1)
				m_grid->popActivePoint();
		}
	}
	delete m_grid;
}

inline void PoissonDiscSampler::getRadiusFromBoundary()
{
	double minDist = std::numeric_limits<double>::max();
	for (size_t i = 0; i < m_points.size(); i++)
	{
		size_t next = (i + 1) % m_points.size();
		double d = m_points[i].distance(m_points[next]);
		if (d < minDist)
			minDist = d;
	}
	m_discRadius = minDist * 0.8;
}


