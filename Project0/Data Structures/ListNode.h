#pragma once
#include "Node.h"

// forward class declaration of list for friend declaration
template<class T>
class List;

template<class T>
class ListNode : public Node
{
private:
	T m_item;
public:
	ListNode(T item);
	friend class List<T>;
};

template<class T>
ListNode<T>::ListNode(T item) : m_item(item) {}

