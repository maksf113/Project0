#pragma once
enum class PointEdge
{
	INVALID = -1,
	LEFT = 0,
	RIGHT = 1,
	BEYOND = 2,
	BEHIND = 3,
	BETWEEN = 4,
	ORIGIN = 5,
	DESTINATION = 6
};

inline void print(const PointEdge& c)
{
	switch (c)
	{
	case PointEdge::INVALID:
		std::cout << "INVALID" << std::endl;
		break;
	case PointEdge::LEFT:
		std::cout << "LEFT" << std::endl;
		break;
	case PointEdge::RIGHT:
		std::cout << "RIGHT" << std::endl;
		break;
	case PointEdge::BEYOND:
		std::cout << "BEYOND" << std::endl;
		break;
	case PointEdge::BEHIND:
		std::cout << "BEHIND" << std::endl;
		break;
	case PointEdge::ORIGIN:
		std::cout << "ORIGIN" << std::endl;
		break;
	case PointEdge::DESTINATION:
		std::cout << "DESTINATION" << std::endl;
		break;
	}	
}