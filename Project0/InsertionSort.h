#pragma once
#include <vector>

template<class T>
void insertionSort(std::vector<T>& a, int(*cmp)(T, T))
{
	for (int i = 1; i < a.size(); i++)
	{
		T v = a[i];
		int j = i;
		while ((j > 0) && ((*cmp)(v, a[j - 1]) < 0))
		{
			a[j] = a[j - 1];
			j--;
		}
		a[j] = v;
	}
}