#pragma once
#include "ListNode.h"

template<class T>
class List
{
private:
	ListNode<T>* m_header;
	ListNode<T>* m_window;
	int m_length;
public:
	List();
	~List();
	T insert(T);
	T append(T);
	List* append(List*);
	T prepend(T);
	T remove();
	void item(T);
	T item();
	T next(); 
	T prev();
	T first();
	T last();
	int length();
	bool isFirst();
	bool isLast();
	bool isHead();
};

template<class T>
List<T>* arrayToList(T a[], int n)
{
	List<T>* s = new List<T>;
	for (int i = 0; i < n; i++)
		s->append(a[i]);
	return s;
}

template<class T>
T leastItem(List<T>& s, int(*compare)(T, T))
{
	if (s.length() == 0)
		return nullptr;
	T it = s.first();
	for (s.next(); !s.isHead(); s.next())
		if (compare(s.item(), it) < 0)
			it = s.item();
	return it;
}

template<class T>
List<T>::List() : m_length(int(0))
{
	m_header = new ListNode<T>(nullptr);
	m_window = m_header;
}

template<class T>
List<T>::~List()
{
	while (m_length > 0)
	{
		first();
		remove(); // remove decreases legnth
	}
	delete m_header; // data items not deallocated
}


// insert rigth after current window
template<class T>
T List<T>::insert(T item)
{
	m_window->insert(new ListNode<T>(item));
	m_length++;
	return item;
}

// insert at the beginning
template<class T>
T List<T>::prepend(T item)
{
	m_header->insert(new ListNode<T>(item));
	m_length++;
	return item;
}

// insert at the end
template<class T>
T List<T>::append(T item)
{
	m_header->prev()->insert(new ListNode<T>(item));
	m_length++;
	return item;
}

// append list l at the end of current list and make l empty
template<class T>
List<T>* List<T>::append(List<T>* l)
{
	ListNode<T>* a = (ListNode<T>*)m_header->prev();
	a->splice(l->m_header);
	m_length += l->m_length;
	l->m_header->remove();
	l->m_length = 0;
	l->m_window = m_header;
	return this;
}

// remove item in the window, set window to prev, return removed item / nothing if window = head
template<class T>
T List<T>::remove()
{
	if (m_window == m_header)
		return nullptr;
	T item = m_window->m_item; // void* instead of T in book
	m_window = (ListNode<T>*)m_window->prev();
	delete (ListNode<T>*)m_window->next()->remove(); // remove node returns T (pointer)
	m_length--;
	return item;
}

// setting item
template<class T>
void List<T>::item(T it)
{
	if (m_window != m_header)
		m_window->m_item = it;
}

// return the item in te window
template<class T>
T List<T>::item()
{
	return m_window->m_item;
}

// moves window to next (with wraparound -> next of last is header), 
// and returns item in the windows new position
template<class T>
T List<T>::next()
{
	m_window = (ListNode<T>*)m_window->next();
	return m_window->m_item;
}

template<class T>
T List<T>::prev()
{
	m_window = (ListNode<T>*)m_window->prev();
	return m_window->m_item;
}

// moves window to first position (after header) and returns item in the new win pos
template<class T>
T List<T>::first()
{
	m_window = (ListNode<T>*)m_header->next();
	return m_window->m_item;
}

// moves window to last position (before header) and returns item in the new win pos
template<class T>
T List<T>::last()
{
	m_window = (ListNode<T>*)m_header->prev();
	return m_window->m_item;
}

template<class T>
int List<T>::length()
{
	return m_length;
}

// isFirst, isLast false if list empty
template<class T>
bool List<T>::isFirst()
{
	return ((m_window == m_header->next()) && (m_length > 0));
}

template<class T>
bool List<T>::isLast()
{
	return ((m_window == m_header->prev()) && (m_length > 0));
}

template<class T>
bool List<T>::isHead()
{
	return (m_window == m_header);
}



