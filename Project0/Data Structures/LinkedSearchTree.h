#pragma once
#include "LinkedTreeNode.h"

template<class T>
class LinkedSearchTree
{
private:
	LinkedTreeNode<T>* m_root; // header node
	LinkedTreeNode<T>* m_window;
	int (*_compare)(T, T);
	void _remove(T, TreeNode<T>*&);
public:
	LinkedSearchTree(int(*)(T, T));
	~LinkedSearchTree();
	T next();
	T prev();
	void inorder(void(*)(T));
	T item();
	bool isFirst();
	bool isLast();
	bool isHead();
	bool isEmpty();
	T find(T);
	T findMin();
	T insert(T);
	void remove();
	T remove(T);
	T removeMin(); 
};

template<class T>
inline LinkedSearchTree<T>::LinkedSearchTree(int(*compare)(T, T)) : _compare(compare)
{
	m_window = m_root = new LinkedTreeNode<T>(nullptr);
}

template<class T>
inline void LinkedSearchTree<T>::_remove(T item, TreeNode<T>*& n)
{
	if (n == nullptr)
		return;
	int result = (*_compare)(item, n->m_item);
	if (result < 0)
		_remove(item, n->m_lChild);
	else if (result > 0)
		_remove(item, n->m_rChild);
	else
	{
		if (n->m_lChild == nullptr)
		{
			LinkedTreeNode<T>* old = (LinkedTreeNode<T>*)n;
			if (m_window == old)
				m_window = old->prev();
			n = old->rChild();
			old->Node::remove();
			delete old;
		}
		else if (n->m_rChild == nullptr)
		{
			LinkedTreeNode<T>* old = (LinkedTreeNode<T>*)n;
			if (m_window == old)
				m_window = old->prev();
			n = old->lChild();
			old->Node::remove();
			delete old;
		}
		else
		{
			LinkedTreeNode<T>* m = ((LinkedTreeNode<T>*)n)->next();
			n->m_item = m->m_item;
			_remove(m->m_item, n->m_rChild);
		}
	}
}

template<class T>
inline LinkedSearchTree<T>::~LinkedSearchTree()
{
	delete m_root;
}

template<class T>
inline T LinkedSearchTree<T>::next()
{
	m_window = m_window->next();
	return m_window->m_item;
}

template<class T>
inline T LinkedSearchTree<T>::prev()
{
	m_window = m_window->prev();
	return m_window->m_item;
}

template<class T>
inline void LinkedSearchTree<T>::inorder(void(*visit)(T))
{
	LinkedTreeNode<T>* n = m_root->next();
	while (n != m_root)
	{
		(*visit)(n->m_item);
		n = n->next();
	}
}

template<class T>
inline T LinkedSearchTree<T>::item()
{
	return m_window->m_item;
}

template<class T>
inline bool LinkedSearchTree<T>::isFirst()
{
	return ((m_window == m_root->next()) && (m_root != m_root->next()));
}

template<class T>
inline bool LinkedSearchTree<T>::isLast()
{
	return ((m_window == m_root->prev()) && (m_root != m_root->next()));
}

template<class T>
inline bool LinkedSearchTree<T>::isHead()
{
	return (m_window == m_root);
}

template<class T>
inline bool LinkedSearchTree<T>::isEmpty()
{
	return (m_root == m_root->next());
}

template<class T>
inline T LinkedSearchTree<T>::find(T item)
{
	LinkedTreeNode<T>* n = m_root->rChild();
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
inline T LinkedSearchTree<T>::findMin()
{
	m_window = m_root->next();
	return m_window->m_item;
}

template<class T>
inline T LinkedSearchTree<T>::insert(T item)
{
	int result = 1;
	LinkedTreeNode<T>* p = m_root;
	LinkedTreeNode<T>* n = m_root->rChild();
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
	m_window = new LinkedTreeNode<T>(item);
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
	return item;
}

template<class T>
inline void LinkedSearchTree<T>::remove()
{
	if(m_window != m_root)
		_remove(m_window->m_item, m_root->m_rChild);
}

template<class T>
inline T LinkedSearchTree<T>::remove(T item)
{
	_remove(item, m_root->rChild());
	return item;
}

template<class T>
inline T LinkedSearchTree<T>::removeMin()
{
	T min = m_root->next()->m_item;
	if (m_root != m_root->next())
		_remove(min, m_root->m_rChild);
	return min;
}
