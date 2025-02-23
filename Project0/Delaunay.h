#pragma once
#include "Geometry/Point.h"
#include "Geometry/Orientation.h"
#include "Geometry/Polygon.h"
#include "Data Structures/List.h"
#include "Data Structures/SearchTree.h"
#include <vector>
#include <limits>

void printEdge(Edge* const e)
{
	print(*e);
}
void print(const char* c)
{
	std::cout << c << std::endl;
}
void print(int i)
{
	std::cout << i << std::endl;
}
void print(double d)
{
	std::cout << d << std::endl;
}
void print(Polygon* p)
{
	print("Triangle points:\n");
	for (int i = 0; i < 3; i++)
	{
		print(p->advance()->point());
	}
}
struct Segment;
namespace delaunay
{
	class Delaunay
	{
	private:
		List<Polygon*>* m_triangles;
		std::vector<RandomizedSearchTree<Edge*>*> m_boundaries;
		RandomizedSearchTree<const Point*>* m_boundaryPoints;
	public:
		Delaunay(std::vector<Point>& points, std::vector<std::vector<Point>>& boundaries);
		Delaunay(const std::vector<Point>& points, const std::vector<Point>& boundary);
		Delaunay(std::vector<Point>& points);
		List<Polygon*>* triangles() const { return m_triangles; }
	private:
		Polygon* minMaxTriangle(const std::vector<Point>& points);
		Polygon* superTriangle(const std::vector<Point>& points, Point& p0, Point& p1, Point& p2);
		void processBoundaries(std::vector<std::vector<Point>>& boundaries);
		void addPoint(const Point& p);
		void removeSuperPoints(const Point& p0, const Point& p1, const Point& p2);
		void boundaryToTree(const std::vector<Point>& boundary);
		void removeBoundaryTriangles();
	};

	int cmp(Edge* a, Edge* b)
	{
		Point a0, a1, b0, b1;
		if (a->org() < a->dst())
		{
			a0 = a->org();
			a1 = a->dst();
		}
		else
		{
			a0 = a->dst();
			a1 = a->org();
		}
		if (b->org() < b->dst())
		{
			b0 = b->org();
			b1 = b->dst();
		}
		else
		{
			b0 = b->dst();
			b1 = b->org();
		}

		if (a0 == b0 && a1 == b1)
			return 0;
		if (a0 == b1 && a1 == b0)
			return 0;
		if (a0 < b0)
			return -1;
		if (a0 > b0)
			return 1;
		if (a1 < b1)
			return -1;
		if (a1 > b1)
			return 1;
		std::cerr << "delunay::Edge cmp error\n";
	}
	Polygon* triangle(const Point& a, const Point& b, const Point& c)
	{
		Polygon* t = new Polygon;
		t->insert(a);
		t->insert(b);
		t->insert(c);
		return t;
	}
	struct Circle
	{
		Point center;
		double radius;
		Circle(const Point& p, double r) : center(p), radius(r) {}
		Circle(Polygon* p)
		{
			// points of triangle
			Point p0, p1, p2;
			p0 = (p->advance())->point();
			p1 = (p->advance())->point();
			p2 = (p->advance())->point();
			*this = Circle(p0, p1, p2);
		}
		// Circumcircle of 3 points
		Circle(const Point& p0, const Point& p1, const Point& p2)
		{
			Edge e(p0, p1);
			Edge f(p1, p2);
			e.rot();
			f.rot();
			double t;
			e.intersect(f, t);
			center = e.org() + t * (e.dst() - e.org());
			radius = (p0 - center).length();
		}
		bool isInside(const Point& p) const
		{
			double distFromCenter = (p - center).length();
			return (distFromCenter < radius);
		}

		// equilateral triangle circumscribed on a circle
		Polygon* triangleOnCircle(Point& p0, Point& p1, Point& p2)
		{
			double sq3 = sqrt(3.0);
			p0 = center + Point(-sq3 * radius, -radius);
			p1 = center + Point(sq3 * radius, -radius);
			p2 = center + Point(0.0, 2.0 * radius);
			return triangle(p0, p1, p2);
		}
	};
	int cmp(const Point* a, const Point* b)
	{
		if (*a < *b)
			return -1;
		if (*a > *b)
			return 1;
		if (*a == *b)
			return 0;
		std::cerr << "Point cmp (Delaunay.h) error" << std::endl;
	}
	void Delaunay::boundaryToTree(const std::vector<Point>& boundary)
	{
		m_boundaryPoints = new RandomizedSearchTree<const Point*>(cmp);
		for(size_t i = 0; i < boundary.size(); i++)
			m_boundaryPoints->insert(&(boundary[i]));
		//for (const Point& p : boundary)
		//	m_boundaryPoints->insert(&p);
	}
	void Delaunay::processBoundaries(std::vector<std::vector<Point>>& boundaries)
	{
		m_boundaries.resize(boundaries.size());
		for (size_t i = 0; i < boundaries.size(); i++)
		{
			const std::vector<Point>& boundary = boundaries[i];
			m_boundaries[i] = new RandomizedSearchTree<Edge*>(cmp);

			for (size_t j = 0; j < boundary.size(); j++)
			{
				size_t nextIndex = (j + 1) % boundary.size();
				Edge* e = new Edge(boundary[j], boundary[nextIndex]);
				m_boundaries[i]->insert(e);
			}
		}
	}
	void Delaunay::removeBoundaryTriangles()
	{
		for (auto triangle = m_triangles->first(); !m_triangles->isHead(); triangle = m_triangles->next())
		{
			// check if all vertices of a triangle belong to boundary
			if (m_boundaryPoints->find(triangle->advance()))
				if (m_boundaryPoints->find(triangle->advance()))
					if (m_boundaryPoints->find(triangle->advance()))
						m_triangles->remove();
		}
	}
	void Delaunay::addPoint(const Point& p)
	{
		RandomizedSearchTree<Edge*> edgesToRetriangulate(delaunay::cmp);
		Polygon* removedTriangle = new Polygon;
		for (auto triangle = m_triangles->first(); !m_triangles->isHead(); triangle = m_triangles->next())
		{
			Circle c(triangle);
			if (c.isInside(p))
			{
				removedTriangle = m_triangles->remove();
				for (int i = 0; i < 3; i++)
				{
					Edge* e = new Edge(removedTriangle->v()->point(), removedTriangle->neighbour()->point());
					if (edgesToRetriangulate.find(e))
						edgesToRetriangulate.remove();
					else
						edgesToRetriangulate.insert(e);
					removedTriangle->advance();
				}
			}
		}
		while (!edgesToRetriangulate.isEmpty())
		{
			Edge* e = edgesToRetriangulate.removeMin();
			m_triangles->insert(triangle(p, e->orgRef(), e->dstRef()));
		}
		delete removedTriangle;
	}

	void Delaunay::removeSuperPoints(const Point& p0, const Point& p1, const Point& p2)
	{
		for (auto triangle = m_triangles->first(); !m_triangles->isHead(); triangle = m_triangles->next())
		{
			for (int i = 0; i < 3; i++)
			{
				if (
					triangle->point() == p0 ||
					triangle->point() == p1 ||
					triangle->point() == p2
					)
				{
					m_triangles->remove();
					break;
				}
				triangle->advance();
			}
		}
	}
	inline Delaunay::Delaunay(const std::vector<Point>& points, const std::vector<Point>& boundary)
	{
		boundaryToTree(boundary);
		Point p0, p1, p2; // vertices of super triangle
		m_triangles = new List<Polygon*>;
		m_triangles->insert(superTriangle(points, p0, p1, p2));
		for (const auto& p : points)
			addPoint(p);
		removeSuperPoints(p0, p1, p2);
		removeBoundaryTriangles();
	}
	inline Delaunay::Delaunay(std::vector<Point>& points, std::vector<std::vector<Point>>& boundaries)
	{
		processBoundaries(boundaries);
		Point p0, p1, p2; // vertices of super triangle
		m_triangles = new List<Polygon*>;
		m_triangles->insert(superTriangle(points, p0, p1, p2));
		for (const auto& p : points)
			addPoint(p);
		removeSuperPoints(p0, p1, p2);
	}
	inline Delaunay::Delaunay(std::vector<Point>& points)
	{
		Point p0, p1, p2; // vertices of super triangle
		m_triangles = new List<Polygon*>;
		m_triangles->insert(superTriangle(points, p0, p1, p2));
		for (const auto& p : points)
			addPoint(p);
		removeSuperPoints(p0, p1, p2);
	}

	Polygon* Delaunay::minMaxTriangle(const std::vector<Point>& points)
	{
		double x_min = points[0].x();
		double x_max = points[0].x();
		double y_min = points[0].y();
		double y_max = points[0].y();
		for (const auto& p : points)
		{
			if (p.x() < x_min)
				x_min = p.x();
			if (p.x() > x_max)
				x_max = p.x();
			if (p.y() < y_min)
				y_min = p.y();
			if (p.y() > y_max)
				y_max = p.y();
		}
		return triangle({ x_min, y_min }, { x_max, y_min }, { x_min, y_max });
	}


	inline Polygon* Delaunay::superTriangle(const std::vector<Point>& points, Point& p0, Point& p1, Point& p2)
	{
		Polygon* minMaxTri = minMaxTriangle(points);
		Circle c(minMaxTri); // a circle circumscribed by a triangle
		return c.triangleOnCircle(p0, p1, p2);
	}
}


