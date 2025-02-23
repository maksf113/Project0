#pragma once
#include "TreeNode.h"

// less -> left, more -> right
template<class T>
class SearchTree
{
private:
	TreeNode<T>* m_root;
	int (*_compare)(T, T);
	TreeNode<T>* _findMin(TreeNode<T>*);
	TreeNode<T>* _findMax(TreeNode<T>*);
	void _remove(T, TreeNode<T>*&);
	void _inorder(TreeNode<T>*, void (*)(T));
public:
	SearchTree(int(*)(T, T));
	~SearchTree();
	int isEmpty(void);
	T find(T);
	T findMin();
	T findMax();
	void inorder(void (*)(T));
	void insert(T);
	void remove(T);
	T removeMin();
};

template<class T>
void heapSort(T s[], int n, int(*compare)(T, T))
{
	SearchTree<T> t(compare);
	for (int i = 0; i < n; i++)
		t.insert(s[i]);
	for (int i = 0; i < n; i++)
		s[i] = t.removeMin();
}

template<class T>
SearchTree<T>::SearchTree(int(*compare)(T, T)) : m_root(nullptr), _compare(compare) {}

template<class T>
TreeNode<T>* SearchTree<T>::_findMin(TreeNode<T>* n)
{
	if (n == nullptr)
		return nullptr;
	while (n->m_lChild)
		n = n->m_lChild;
	return n;
}

template<class T>
TreeNode<T>* SearchTree<T>::_findMax(TreeNode<T>* n)
{
	if (n == nullptr)
		return nullptr;
	while (n->m_rChild)
		n = n->m_rChild;
	return n;
}

template<class T>
void SearchTree<T>::_remove(T item, TreeNode<T>*& n)
{
	if (n == nullptr)
		return;
	int result = (*_compare)(item, n->m_item);
	if (result < 0)
		_remove(item, n->m_lChild);
	else if (result > 0)
		_remove(item, n->m_rChild);
	else // n->m_item == item
	{
		// no right child
		if (n->m_rChild == nullptr)
		{
			std::cout << "n->m_rChild == nullptr" << std::endl;
			TreeNode<T>* old = n;
			n = old->m_lChild;
			delete old;
		}
		// no left child
		else if (n->m_lChild == nullptr)
		{
			std::cout << "n->m_lChild == nullptr" << std::endl;
			TreeNode<T>* old = n;
			n = old->m_rChild;
			delete old;
		}
		// no empty child
		else
		{
			TreeNode<T>* m = _findMin(n->m_rChild);
			std::cout << "no empty children" << std::endl;
			if (m)
			{
				n->m_item = m->m_item;
				_remove(m->m_item, n->m_rChild);
			}
			
		}
	}
}

template<class T>
void SearchTree<T>::_inorder(TreeNode<T>* n, void(*visit)(T))
{
	if (n)
	{
		_inorder(n->m_lChild, visit);
		(*visit)(n->m_item);
		_inorder(n->m_rChild, visit);
	}
}

template<class T>
inline SearchTree<T>::~SearchTree()
{
	if (m_root)
		delete m_root;
}

template<class T>
inline int SearchTree<T>::isEmpty(void)
{
	return (m_root == nullptr);
}

template<class T>
T SearchTree<T>::find(T item)
{
	TreeNode<T>* n = m_root;
	while (n)
	{
		int result = (*_compare)(item, n->m_item);
		if (result < 0)
			n = n->m_lChild;
		else if (result > 0)
			n = n->m_rChild;
		else
			return n->m_item;
	}
	return nullptr;
}

template<class T>
T SearchTree<T>::findMin()
{
	TreeNode<T>* n = _findMin(m_root);
	return (n ? n->m_item : nullptr);
}

template<class T>
T SearchTree<T>::findMax()
{
	TreeNode<T>* n = _findMax(m_root);
	return (n ? n->m_item : nullptr);
}

template<class T>
void SearchTree<T>::inorder(void(*visit)(T))
{
	_inorder(m_root, visit);
}

template<class T>
inline void SearchTree<T>::insert(T item)
{
	if (m_root == nullptr)
	{
		m_root = new TreeNode<T>(item);
		return;
	}
	else
	{
		int result;
		// n - current node, p its parent
		TreeNode<T>* p = nullptr, * n = m_root;
		while (n)
		{
			p = n;
			result = (*_compare)(item, p->m_item);
			if (result < 0)
				n = p->m_lChild;
			else if (result > 0)
				n = p->m_rChild;
			else
				return;
		}
		if (result < 0)
			p->m_lChild = new TreeNode<T>(item);
		else
			p->m_rChild = new TreeNode<T>(item);
	}
}

template<class T>
inline void SearchTree<T>::remove(T item)
{
	_remove(item, m_root);
}

template<class T>
inline T SearchTree<T>::removeMin()
{
	T item = findMin();
	remove(item);
	return item;
}
