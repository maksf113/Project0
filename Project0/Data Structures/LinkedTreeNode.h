#pragma once
#include "Node.h"
#include "TreeNode.h"

template<class T>
class LinknedSearchTree;

template<class T>
class LinkedTreeNode : public Node, public TreeNode<T>
{
public:
	LinkedTreeNode(T);
	LinkedTreeNode<T>* rChild();
	LinkedTreeNode<T>* lChild();
	LinkedTreeNode<T>* next();
	LinkedTreeNode<T>* prev();
	friend class LinkedSearchTree<T>;
};

template<class T>
LinkedTreeNode<T>::LinkedTreeNode(T item) : TreeNode<T>(item) {}

template<class T>
inline LinkedTreeNode<T>* LinkedTreeNode<T>::rChild()
{
	return (LinkedTreeNode<T>*) this->m_rChild;
}

template<class T>
inline LinkedTreeNode<T>* LinkedTreeNode<T>::lChild()
{
	return (LinkedTreeNode<T>*) this->m_lChild;
}

template<class T>
inline LinkedTreeNode<T>* LinkedTreeNode<T>::next()
{
	return (LinkedTreeNode<T>*) m_next;
}

template<class T>
inline LinkedTreeNode<T>* LinkedTreeNode<T>::prev()
{
	return (LinkedTreeNode<T>*) m_prev;
}
