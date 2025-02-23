#pragma once
#include <vector>
#include "Geometry/Point.h"
#include "Data Structures/Stack.h"
#include "Domain.h"

class Grid
{
private:
	std::vector<std::vector<int>> m_grid;
	std::vector<Point>& m_points;
	Stack<std::pair<int, int>*> m_activeIndices;
	int m_rows = -1;
	int m_cols = -1;
	double m_cellSize;
	Point m_BL, m_TR; // top right, bottom left
	void getBLTR();
	void getCellSize();
	void calcIndices(const Point& p, int& i, int& j);
	void initActiveIndicesAndGrid();
public:
	Grid(std::vector<Point>& points);
	void getNextActiveIndices(int& i, int& j);
	int getPointIndex(int i, int j);
	bool tryToAddPoint(const Point& p, double radius);
	void popActivePoint();
	bool isActiveEmpty();
};

Grid::Grid(std::vector<Point>& points) : m_points(points)
{
	getCellSize();
	getBLTR();
	initActiveIndicesAndGrid();
}
void Grid::popActivePoint()
{
	delete m_activeIndices.pop();
}
bool Grid::tryToAddPoint(const Point& p, double radius)
{
	int i, j;
	calcIndices(p, i, j);
	for(int k = i - 2; k <= i + 2; k++)
		for (int m = j - 2; m <= j + 2; m++)
		{
			if (k < 0 || m < 0)
				continue;
			if (k > m_cols - 1 || m > m_rows - 1)
				continue;
			if (m_grid[k][m] == -1)
				continue;
			Point q = m_points[getPointIndex(k, m)];
			if (p.distance(q) <= radius)
				return false;
		}
	if (m_grid[i][j] == -1)
	{
		m_grid[i][j] = m_points.size();
		m_activeIndices.push(new std::pair<int, int>(i, j));
		m_points.push_back(p);
		return true;
	}
	else
		return false;
}
bool Grid::isActiveEmpty()
{
	return m_activeIndices.empty();
}
int Grid::getPointIndex(int i, int j)
{
	return m_grid[i][j];
}
void Grid::getNextActiveIndices(int& i, int& j)
{
	i = m_activeIndices.top()->first;
	j = m_activeIndices.top()->second;
}
void Grid::initActiveIndicesAndGrid()
{
	m_rows = std::ceil((m_TR.y() - m_BL.y()) / m_cellSize); // number of rows
	m_cols = std::ceil((m_TR.x() - m_BL.x()) / m_cellSize); // number of columns
	m_grid.resize(m_cols);
	for (auto& row : m_grid)
		row.resize(m_rows);
	for (auto& row : m_grid)
		for (auto& cell : row)
			cell = -1;
	for (size_t k = 0; k < m_points.size(); k++)
	{
		int i, j;
		calcIndices(m_points[k], i, j);
		m_grid[i][j] = k;
		m_activeIndices.push(new std::pair<int, int>(i, j));
	}
}
void Grid::calcIndices(const Point& p, int& i, int& j)
{
	i = std::floor((p.x() - m_BL.x()) / m_cellSize);
	j = std::floor((p.y() - m_BL.y()) / m_cellSize);
}
void Grid::getCellSize()
{
	double minDist = std::numeric_limits<double>::max();
	for (size_t i = 0; i < m_points.size(); i++)
	{
		size_t next = (i + 1) % m_points.size();
		double d = m_points[i].distance(m_points[next]);
		if (d < minDist)
			minDist = d;
	}
	m_cellSize = minDist / sqrt(2.0);
}
inline void Grid::getBLTR()
{
	m_BL = m_points[0];
	m_TR = m_points[0];
	for (const auto& p : m_points)
	{
		double x = p.x(), y = p.y();
		if (x < m_BL.x())
			m_BL.x(x);
		if (x > m_TR.x())
			m_TR.x(x);
		if (y < m_BL.y())
			m_BL.y(y);
		if (y > m_TR.y())
			m_TR.y(y);
	}
}