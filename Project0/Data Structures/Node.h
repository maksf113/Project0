#pragma once

class Node
{
protected:
	Node* m_next; 
	Node* m_prev;
public:
	Node(); // creates a node doubly linked to itself
	virtual ~Node();
	Node* next();
	Node* prev();
	// insert next
	Node* insert(Node*);
	Node* remove();
	void splice(Node*);
};

// creates a node doubly linked to itself
Node::Node() : m_next(this), m_prev(this) {}
Node::~Node() {}
Node* Node::next()
{
	return m_next;
}

Node* Node::prev()
{
	return m_prev;
}

// insert next
Node* Node::insert(Node* b)
{
	Node* a = m_next;
	b->m_next = a;
	b->m_prev = this;
	m_next = b;
	a->m_prev = b;
	return b;
}

// removes this node from its linked list, returns pointer to his node (for dealocation)
Node* Node::remove()
{
	m_prev->m_next = m_next;
	m_next->m_prev = m_prev;

	m_next = m_prev = this;
	return this;
}

// splice divide into two lists if a (this) and b in the same list
// merge to lists if a and b in separate lists
void Node::splice(Node* b)
{
	Node* a = this;
	Node* aNext = a->m_next;
	Node* bNext = b->m_next;
	a->m_next = bNext;
	b->m_next = aNext;
	aNext->m_prev = b;
	bNext->m_prev = a;
}
