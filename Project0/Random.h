#pragma once
#include <random>
enum class NumberType
{
	DOUBLE = 0,
	FLOAT = 1,
	INT = 2
};
class Random
{
private:
	std::random_device m_rd;
	std::uniform_real_distribution<double> m_dist;
	std::mt19937 m_gen;
public:
	Random(double a, double b);
	double operator()();
};

Random::Random(double a, double b)
{
	m_gen = std::mt19937(m_rd());
	m_dist = std::uniform_real_distribution<double>(a, b);	
}
double Random::operator()()
{
	return m_dist(m_gen);
}