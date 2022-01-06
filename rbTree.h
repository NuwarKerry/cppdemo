#pragma once
#include <utility>  
#include<iostream>
using namespace std;
 
enum COLOR{ RED, BLACK };
 
template<class K, class V>
struct RBTreeNode{
	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	pair<K, V> _value;
	COLOR _color;
	RBTreeNode(const K& key = K(), const V& value = V(), COLOR color = RED)
		: _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _value(key, value)
		, _color(color)
	{}
};
 
 
template<class K, class V>
class RBTreeiterator
{
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
	typedef RBTreeiterator<K, V> Self;
 
public:
	RBTreeiterator(PNode pNode = NULL)
		: _pNode(pNode)
	{}
 
	RBTreeiterator(const Self& s)
		: _pNode(s._pNode)
	{}
 
	pair<K, V>& operator*()
	{
		return _pNode->_value;
	}
 
	pair<K, V>* operator->()
	{
		return &(operator*());
	}
 
	Self& operator++()
	{
		RBTreeItIncrement();
		return *this;
	}
 
	Self operator++(int)
	{
		Self temp(*this);
		RBTreeItIncrement();
		return temp;
	}
 
	Self& operator--()
	{
		RBTreeItDecrement();
		return *this;
	}
 
	Self operator--(int)
	{
		Self temp(*this);
		RBTreeItDecrement();
		return temp;
	}
 
	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}
 
	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}
 
private:
	void RBTreeItIncrement()
	{
		if (_pNode->_pRight)
		{
			_pNode = _pNode->_pRight;
			while (_pNode->_pLeft)
				_pNode = _pNode->_pLeft;
		}
		else
		{
			PNode pParent = _pNode->_pParent;
			while (pParent->_pRight == _pNode)
			{
				_pNode = pParent;
				pParent = _pNode->_pParent;
			}
 
			// 如果树的根节点没有右孩子的情况且迭代器起始位置在根节点
			if (_pNode->_pRight != pParent)
				_pNode = pParent;
		}
	}
 
	void RBTreeItDecrement()
	{
		if (_pNode->_pParent->_pParent == _pNode && RED == _pNode->_color)
		{
			_pNode = _pNode->_pRight;
		}
		else if (_pNode->_pLeft)
		{
			// 在当前节点左子树中找最大的结点
			_pNode = _pNode->_pLeft;
			while (_pNode->_pRight)
				_pNode = _pNode->_pRight;
		}
		else
		{
			PNode pParent = _pNode;
			while (pParent->_pLeft == _pNode)
			{
				_pNode = pParent;
				pParent = _pNode->_pParent;
			}
 
			_pNode = pParent;
		}
	}
 
private:
	PNode _pNode;
};
 
 
template<class K, class V>
class RBTree{
	//typedef RBTreeiterator<K, V> Iterator;
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
public:
	typedef RBTreeiterator<K, V> Iterator;
public:
	RBTree()
		:_pHead(new Node)
	{}
	Iterator Begin()
	{
		return Iterator(_pHead->_pLeft);
	}
	Iterator End()
	{
		return Iterator(_pHead);
	}
	PNode& GetRoot()
	{
		return _pHead->_pParent;
	}
	pair<Iterator,bool> InsertUnique(const pair<K, V>& value)
	{
		PNode& _pRoot = GetRoot();
		PNode newNode = NULL;
		if (NULL == _pRoot){
			newNode = _pRoot = new Node(value.first, value.second, BLACK);
			_pRoot->_pParent = _pHead;
		}
		else{
			PNode pCur = _pRoot;
			PNode pParent = pCur;
			while (pCur){
				if (pCur->_value.first < value.first){
					pParent = pCur;
					pCur = pCur->_pRight;
				}
				else if (pCur->_value.first>value.first){
					pParent = pCur;
					pCur = pCur->_pLeft;
				}
				else
					return pair<Iterator,bool>(Iterator(pCur),false);
			}
			newNode = pCur = new Node(value.first, value.second);
			if (value.first < pParent->_value.first)
				pParent->_pLeft = pCur;
			else
				pParent->_pRight = pCur;
			pCur->_pParent = pParent;
			while (pParent&& pParent->_color == RED){
				PNode grandParent = pParent->_pParent;
				if (pParent == grandParent->_pLeft){
					PNode pUncle = grandParent->_pRight;
					if (pUncle&&pUncle->_color == RED){
						pParent->_color = BLACK;
						pUncle->_color = BLACK;
						grandParent->_color = RED;
						grandParent = pCur;
						pParent = pCur->_pParent;
					}
					else {
						if (pCur == pParent->_pRight){
							rotateL(pParent);
							swap(pCur, pParent);
						}
						grandParent->_color = RED;
						pParent->_color = BLACK;
						rotateR(grandParent);
					}
				}
				else{
					PNode pUncle = grandParent->_pLeft;
					if (pUncle&&pUncle->_color == RED){
						pParent->_color = BLACK;
						pUncle->_color = BLACK;
						grandParent->_color = RED;
						grandParent = pCur;
						pParent = pCur->_pParent;
					}
					else {
						if (pCur == pParent->_pLeft){
							rotateR(pParent);
							swap(pCur, pParent);
						}
						grandParent->_color = RED;
						pParent->_color = BLACK;
						rotateL(grandParent);
					}
				}
			}
		}
 
		_pRoot->_color = BLACK;
		_pHead->_pLeft = MostLeft();
		_pHead->_pRight = MostRight();
		return make_pair(Iterator(newNode),true);
	}
 
	bool Empty()const
	{
		return NULL == GetRoot();
	}
 
	size_t Size()const
	{
		size_t count = 0;
		Iterator it = Iterator(_pHead->_pLeft);
		Iterator ed = Iterator(_pHead);
		while (it != ed){
			++count;
			++it;
		}
		return count;
	}
 
	void InOrder()
	{
		cout << "InOrder:";
		_InOrder(GetRoot());
		cout << endl;
	}
	bool IsRBTree()
	{
		PNode& _pRoot = GetRoot();
		if (NULL == _pRoot)
			return true;
		if (RED == _pRoot->_color){
			cout << "根节点为红色违反性质2" << endl;
			return false;
		}
		// 统计单条路径中黑色结点的个数
		size_t blackCount = 0;
		PNode pCur = _pRoot;
		while (pCur){
			if (BLACK == pCur->_color)
				++blackCount;
			pCur = pCur->_pLeft;
		}
		size_t pathCount = 0;
		return _IsRBTree(_pRoot, pathCount, blackCount);
	}
private:
	void rotateL(PNode pParent)
	{
		PNode pSubR = pParent->_pRight;
		PNode pSubRL = pSubR->_pLeft;
		pParent->_pRight = pSubRL;
		if (pSubRL)
			pSubRL->_pParent = pParent;
		pSubR->_pLeft = pParent;
		PNode pPParent = pParent->_pParent;
		pParent->_pParent = pSubR;
		pSubR->_pParent = pPParent;
		if (_pHead == pPParent){
			GetRoot() = pSubR;
		}
		else{
			if (pPParent->_pLeft == pParent)
				pPParent->_pLeft = pSubR;
			else
				pPParent->_pRight = pSubR;
		}
	}
	void rotateR(PNode pParent)
	{
		PNode pSubL = pParent->_pLeft;
		PNode pSubLR = pSubL->_pRight;
		pParent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = pParent;
		pSubL->_pRight = pParent;
		PNode pPParent = pParent->_pParent;
		pParent->_pParent = pSubL;
		pSubL->_pParent = pPParent;
		if (_pHead == pPParent){
			GetRoot() = pSubL;
		}
		else{
			if (pPParent->_pLeft == pParent)
				pPParent->_pLeft = pSubL;
			else
				pPParent->_pRight = pSubL;
		}
	}
	void _InOrder(PNode pRoot)
	{
		if (pRoot){
			_InOrder(pRoot->_pLeft);
			cout << "<" << pRoot->_value.first << "," << pRoot->_value.second << ">";
			_InOrder(pRoot->_pRight);
		}
	}
 
	bool _IsRBTree(PNode pRoot, size_t n, size_t blackCount)
	{
		if (NULL == pRoot)
			return true;
		if (BLACK == pRoot->_color)
			++n;
		PNode pParent = pRoot->_pParent;
		if (pParent && RED == pRoot->_color && RED == pParent->_color){
			cout << "有连在一起的红色结点违反性质3" << endl;
			return false;
		}
		if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight){
			if (n != blackCount){
				cout << "路径中黑色结点个数不同违反性质4" << endl;
				return false;
			}
		}
		return _IsRBTree(pRoot->_pLeft, n, blackCount) && _IsRBTree(pRoot->_pRight, n, blackCount);
	}
	PNode MostLeft()
	{
		PNode pCur = GetRoot();
		if (NULL == pCur)
			return NULL;
		while (pCur->_pLeft)
			pCur = pCur->_pLeft;
		return pCur;
	}
	PNode MostRight()
	{
		PNode pCur = GetRoot();
		if (NULL == pCur)
			return NULL;
		while (pCur->_pRight)
			pCur = pCur->_pRight;
		return pCur;
	}
private:
	PNode _pHead;
};
 
 
void test(){
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> rt;
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
		rt.InsertUnique(pair<int, int>(a[i], i));
	rt.InOrder();
	if (rt.IsRBTree())
		cout << "是红黑树" << endl;
	else
		cout << "不是红黑树" << endl;
 
	RBTree<int, int>::Iterator it = rt.Begin();
	while (it != rt.End())
	{
		cout << (*it).first << " ";
		++it;
	}
	cout << endl;
}