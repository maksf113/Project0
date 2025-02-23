#pragma once
#include "List.h"

template<class T>
class Stack
{
private:
	List<T>* m_list;
public:
	Stack();
	~Stack();
	void push(T);
	T pop();
	bool empty();
	int size();
	T top();
	T nextToTop();
	T bottom();
};

template<class T>
Stack<T>::Stack() : m_list(new List<T>) {}

template<class T>
Stack<T>::~Stack()
{
	delete m_list;
}

template<class T>
void Stack<T>::push(T item)
{
	m_list->prepend(item);
}

template<class T>
T Stack<T>::pop()
{
	m_list->first();
	return m_list->remove();
}

template<class T>
bool Stack<T>::empty()
{
	return (m_list->length() == 0);
}

template<class T>
int Stack<T>::size()
{
	return (m_list->length());
}

template<class T>
T Stack<T>::top()
{
	return (m_list->first());
}

template<class T>
T Stack<T>::nextToTop()
{
	m_list->first();
	return (m_list->next());
}

template<class T>
T Stack<T>::bottom()
{
	return (m_list->lats());
}

void reverse(char* a[], int n)
{
	Stack<char*> s;
	for (int i = 0; i < n; i++)
		s.push(a[i]);
	for (int i = 0; i < n; i++)
		a[i] = s.pop();
}