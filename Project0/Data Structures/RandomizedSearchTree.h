#pragma once
#include "RandomizedNode.h"

template<class T>
class RandomizedSearchTree
{
private:
	RandomizedNode<T>* m_root;
	RandomizedNode<T>* m_window;
	int (*_compare)(T, T);
	void _remove(RandomizedNode<T>*);
public:
	RandomizedSearchTree(int(*)(T, T), int seed = -1);
	~RandomizedSearchTree();
	T next();
	T prev();
	void inorder(void (*)(T));
	T item();
	bool isFirst();
	bool isLast();
	bool isHead();
	bool isEmpty();
	T find(T);
	T findMin();
	T locate(T); // returns largest item in the tree not greater than argument
	T insert(const T);
	void remove();
	T remove(T);
	T removeMin();
};

template<class T>
inline RandomizedSearchTree<T>::RandomizedSearchTree(int(*compare)(T, T), int seed):
	_compare(compare)
{
	m_window = m_root = new RandomizedNode<T>(nullptr, seed);
	m_root->m_priority = -1.0;
}

template<class T>
inline void RandomizedSearchTree<T>::_remove(RandomizedNode<T>* n)
{
	n->m_priority = 1.5; //more than 1.0, less than 2.0 (external)
	n->bubbleDown();
	RandomizedNode<T>* p = n->parent();
	if (p->lChild() == n)
		p->m_lChild = nullptr;
	else
		p->m_rChild = nullptr;
	if (m_window == n)
		m_window = n->prev();
	n->Node::remove();
	delete n;
}



template<class T>
inline RandomizedSearchTree<T>::~RandomizedSearchTree()
{
	delete m_root;
}

template<class T>
inline T RandomizedSearchTree<T>::next()
{
	m_window = m_window->next();
	return m_window->m_item;
}

template<class T>
inline T RandomizedSearchTree<T>::prev()
{
	m_window = m_window->prev();
	return m_window->m_item;
}

template<class T>
inline void RandomizedSearchTree<T>::inorder(void(*visit)(T))
{
	RandomizedNode<T>* n = m_root->next();
	while (n != m_root)
	{
		(*visit)(n->m_item);
		n = n->next();
	}
}

template<class T>
inline T RandomizedSearchTree<T>::item()
{
	return m_window->m_item;
}

template<class T>
inline bool RandomizedSearchTree<T>::isFirst()
{
	return ((m_window == m_root->next()) && (m_root != m_root->next()));
}

template<class T>
inline bool RandomizedSearchTree<T>::isLast()
{
	return ((m_window == m_root->prev()) && (m_root != m_root->next()));
}

template<class T>
inline bool RandomizedSearchTree<T>::isHead()
{
	return (m_window == m_root);
}

template<class T>
inline bool RandomizedSearchTree<T>::isEmpty()
{
	return (m_root == m_root->next());
}

template<class T>
inline T RandomizedSearchTree<T>::find(T item)
{
	RandomizedNode<T>* n = m_root->rChild();
	while (n)
	{
		int result = (*_compare)(item, n->m_item);
		if (result < 0)
			n = n->lChild();
		else if (result > 0)
			n = n->rChild();
		else
		{
			m_window = n;
			return n->m_item;
		}
	}
	return nullptr;
}

template<class T>
inline T RandomizedSearchTree<T>::findMin()
{
	m_window = m_root->next();
	return m_window->m_item;
}

template<class T>
inline T RandomizedSearchTree<T>::locate(T item)
{
	RandomizedNode<T>* b = m_root;
	RandomizedNode<T>* n = m_root->rChild();
	while (n)
	{
		int result = (*_compare)(item, n->m_item);
		if (result < 0)
			n = n->lChild();
		else if (result > 0)
		{
			b = n;
			n = n->rChild();
		}
		else
		{
			m_window = n;
			return m_window->m_item;
		}
	}
	m_window = b;
	return m_window->m_item;
}

template<class T>
inline T RandomizedSearchTree<T>::insert(const T item)
{
	// stage 1: insert as in normal search tree
	// stage 2: bubble the item
	// // also move the window and return the item up until priority is preserved
	int result = 1; // if the tree is empty, insert to the right child of the root
	RandomizedNode<T>* p = m_root;
	RandomizedNode<T>* n = m_root->rChild();

	// stage 1
	while (n)
	{
		p = n;
		result = (*_compare)(item, p->m_item);
		if (result < 0)
			n = p->lChild();
		else if (result > 0)
			n = p->rChild();
		else return nullptr;
	}
	m_window = new RandomizedNode<T>(item);
	m_window->m_parent = p;
	if (result < 0)
	{
		p->m_lChild = m_window;
		p->prev()->Node::insert(m_window);
	}
	else
	{
		p->m_rChild = m_window;
		p->Node::insert(m_window);
	}
	// stage 2
	m_window->bubbleUp();
	return item;
}

template<class T>
inline void RandomizedSearchTree<T>::remove()
{
	if (m_window != m_root)
		_remove(m_window);
}

template<class T>
inline T RandomizedSearchTree<T>::remove(T item)
{
	T it = find(item);
	if (it)
	{
		remove();
		return it;
	}
	return nullptr;
}

template<class T>
inline T RandomizedSearchTree<T>::removeMin()
{
	T min = m_root->next()->m_item;
	if (m_root != m_root->next())
		_remove(m_root->next());
	return min;
}
