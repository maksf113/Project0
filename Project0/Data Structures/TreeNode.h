#pragma once
template<class T>
class SearchTree;

template<class T>
class LinkedSearchTree;

template<class T>
class TreeNode
{
protected:
	TreeNode* m_lChild;
	TreeNode* m_rChild;
	T m_item;
public:
	TreeNode(T);
    virtual ~TreeNode();
	friend class SearchTree<T>;
	friend class LinkedSearchTree<T>;
};

template<class T>
TreeNode<T>::TreeNode(T item) : m_item(item), m_lChild(nullptr), m_rChild(nullptr) {}

template<class T>
TreeNode<T>::~TreeNode()
{
	//if (m_lChild)
	//	delete m_lChild;
	//if (m_rChild)
	//    delete m_rChild;
}
