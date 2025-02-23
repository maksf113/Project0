#pragma once

enum class Edges
{
	INVALID = -1,
	COLINEAR = 0,
	PARALLEL = 1,
	SKEW = 2,
	SKEW_CROSS = 3,
	SKEW_NO_CROSS = 4
};

inline void print(const Edges& c)
{
	switch (c)
	{
	case Edges::INVALID:
		std::cout << "INVALID" << std::endl;
		break;
	case Edges::COLINEAR:
		std::cout << "COLINEAR" << std::endl;
		break;
	case Edges::PARALLEL:
		std::cout << "PARALLEL" << std::endl;
		break;
	case Edges::SKEW:
		std::cout << "SKEW " << std::endl;
		break;
	case Edges::SKEW_CROSS:
		std::cout << "SKEW_CROSS" << std::endl;
		break;
	case Edges::SKEW_NO_CROSS:
		std::cout << "SKEW_NO_CROSS" << std::endl;
		break;
	}
}