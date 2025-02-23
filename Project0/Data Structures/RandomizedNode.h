#pragma once
#include <stdlib.h>
#include "LinkedTreeNode.h"

template<class T>
class RandomizedSearchTree;

template<class T>
class RandomizedNode : public LinkedTreeNode<T>
{
protected:
	RandomizedNode* m_parent;
	double m_priority;
	void rotateRight();
	void rotateLeft();
	void bubbleUp(); // for insertion
	void bubbleDown(); // for removal
public:
	RandomizedNode(T item, int seed = -1);
	RandomizedNode<T>* lChild();
	RandomizedNode<T>* rChild();
	RandomizedNode<T>* next();
	RandomizedNode<T>* prev();
	RandomizedNode<T>* parent();
	double priority();
	friend class RandomizedSearchTree<T>;
};
/****************************************************************
			 Y               					 X
			/ \				 				    / \
		   /   \		rotateRight----->	   /   \
		  X	   T3		<------rotateLeft	  T1	Y
		 / \   				 					   / \
	    /   \ 				 					  /   \
	   T1   T2				 					 T2   T3
****************************************************************/

//template<class T>
//inline void RandomizedNode<T>::rotateRight()
//{
//	RandomizedNode<T>* y = this;
//	RandomizedNode<T>* x = y->lChild(); // mioght be nullptr
//	RandomizedNode<T>* p = y->parent();
//
//	y->m_lChild = x->rChild();
//	y->m_lChild->m_parent = y;
//	x->m_parent = p;
//
//}
template<class T>
inline void RandomizedNode<T>::rotateRight()
{
	RandomizedNode<T>* y = this;
	RandomizedNode<T>* x = y->lChild();
	RandomizedNode<T>* p = y->parent();

	y->m_lChild = x->rChild();
	if (y->lChild() != nullptr)
		y->lChild()->m_parent = y;
	if (p->rChild() == y)
		p->m_rChild = x;
	else
		p->m_lChild = x;
	x->m_parent = p;
	x->m_rChild = y;
	y->m_parent = x;
}

template<class T>
inline void RandomizedNode<T>::rotateLeft()
{
	RandomizedNode<T>* x = this;
	RandomizedNode<T>* y = x->rChild();
	RandomizedNode<T>* p = x->parent();

	x->m_rChild = y->lChild();
	if (x->rChild() != nullptr)
		x->rChild()->m_parent = x;
	if (p->lChild() == x)
		p->m_lChild = y;
	else
		p->m_rChild = y;
	y->m_parent = p;
	y->m_lChild = x;
	x->m_parent = y;
}

template<class T>
inline void RandomizedNode<T>::bubbleUp()
{
	RandomizedNode<T>* p = parent();
	if (priority() < p->priority())
	{
		if (p->lChild() == this)
			p->rotateRight();
		else
			p->rotateLeft();
		bubbleUp();
	}
}

template<class T>
inline void RandomizedNode<T>::bubbleDown()
{
	float lcPriority = lChild() ? lChild()->priority() : 2.0; // 2.0 -> external node
	float rcPriority = rChild() ? rChild()->priority() : 2.0;

	float minPriority = (lcPriority < rcPriority) ? lcPriority : rcPriority;

	if (priority() <= minPriority)
		return;

	if (lcPriority < rcPriority)
		rotateRight();
	else 
		rotateLeft();

	bubbleDown();
}



template<class T>
inline RandomizedNode<T>::RandomizedNode(T item, int seed) : LinkedTreeNode<T>(item)
{
	if (seed != -1)
		srand(seed);
	m_priority = (rand() % 32767) / 32767.0;
	m_parent = nullptr;
}

template<class T>
inline RandomizedNode<T>* RandomizedNode<T>::lChild()
{
	if (!this)
		__debugbreak();
	return (RandomizedNode<T>*)this->m_lChild;
}

template<class T>
inline RandomizedNode<T>* RandomizedNode<T>::rChild()
{
	return (RandomizedNode<T>*)this->m_rChild;
}

template<class T>
inline RandomizedNode<T>* RandomizedNode<T>::next()
{
	return (RandomizedNode<T>*)this->m_next;
}

template<class T>
inline RandomizedNode<T>* RandomizedNode<T>::prev()
{
	return (RandomizedNode<T>*)this->m_prev;
}

template<class T>
inline RandomizedNode<T>* RandomizedNode<T>::parent()
{
	return (RandomizedNode<T>*)m_parent;
}

template<class T>
inline double RandomizedNode<T>::priority()
{
	return m_priority;
}

