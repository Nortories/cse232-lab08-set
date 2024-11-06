/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Joshua Sooaemalelagi & Brooklyn Sowards
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree

   void deleteBinaryTree(BNode*& p);
   BNode* copyBinaryTree(const BNode* pSrc);
   void assignBinaryTree(BNode*& pDest, const BNode* pSrc);
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(BST&& rhs) : BST() { *this = std::move(rhs); }
   BST(const std::initializer_list<T>& il) : BST() { *this = il; }
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T& t, bool keepUnique = true); // changed this to allows true because well sets... and sets dont like twins 
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = true);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   void deleteNode(BNode*& pDelete, bool toRight);

   // 
   // Status
   //

   bool empty() const noexcept { return numElements == 0; } //Checking if the tree is empty now
   size_t size() const noexcept { return numElements; } //Returning the number of elements now
   



};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : data(T()), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) {}
   BNode(const T &  t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) {}
   BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) {}  //Corrected Constructors

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild() const { return pParent && pParent->pRight == this; }
   bool isLeftChild() const { return pParent && pParent->pLeft == this; }  //Now returning actuall statuses

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode * p = nullptr) : pNode(p)
   { 
   }
   iterator(const iterator & rhs) : pNode(rhs.pNode)
   { 
   }
   iterator & operator = (const iterator & rhs)
   {
       pNode = rhs.pNode;
       return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
       return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
       return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
       return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST() : numElements(0), root(nullptr)
{
   //numElements = 99;
   //root = new BNode;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST<T>::BST(const BST<T>& rhs) : numElements(0), root(nullptr)
{
    *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
//template <typename T>
//BST <T> :: BST(BST <T> && rhs) : numElements(rhs.numElements), root(rhs.root)
//{
//    rhs.root = nullptr;
//    rhs.numElements = 0;
//}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
//template <typename T>
//BST <T> ::BST(const std::initializer_list<T>& il) : numElements(0), root(nullptr)
//{
//    *this = il;
//}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
    clear();
}

// deleteBinaryTree goes down each branch and deletes deleteing the leaf nodes on the way back up
template <class T>
void BST<T>::deleteBinaryTree(BST<T>::BNode*& node)
{
    if (!node)
        return;

    deleteBinaryTree(node->pLeft);
    deleteBinaryTree(node->pRight);

    delete node;
	node = nullptr; // apperently deleting the node doesn't do this
}

// copyBinaryTree goes down each branch and copies the nodes on the way back up
template <class T>
void BST<T>::assignBinaryTree(BST<T>::BNode*& pDest, const BST<T>::BNode* pSrc)
{   
	// if the source is null, clear dest before returning
    if (!pSrc)
    {
        deleteBinaryTree(pDest);
        return;
    }

    if (!pDest)
        pDest = new BNode(pSrc->data);
    else
        pDest->data = pSrc->data;

    assignBinaryTree(pDest->pLeft, pSrc->pLeft);
    if (pDest->pLeft)
        pDest->pLeft->pParent = pDest;

    assignBinaryTree(pDest->pRight, pSrc->pRight);
    if (pDest->pRight)
        pDest->pRight->pParent = pDest;
}



/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
    assignBinaryTree(root, rhs.root);
    numElements = rhs.numElements;
    return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
    clear();
    for (auto&& it : il)
        insert(it);
    return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    clear();
    swap(rhs);
    return *this;
}


/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
	std::swap(root, rhs.root);
	std::swap(numElements, rhs.numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique)
{
    if (!root)
    {
        root = new BNode(t);
        numElements = 1;
        return std::make_pair(iterator(root), true);
    }

    BNode* currentNode = root;
    while (currentNode)
    {
        if (keepUnique && t == currentNode->data)
            return std::make_pair(iterator(currentNode), false);

        if (t < currentNode->data)
        {
            if (currentNode->pLeft)
                currentNode = currentNode->pLeft;
            else
            {
                currentNode->pLeft = new BNode(t);
                currentNode->pLeft->pParent = currentNode;
                ++numElements;
                return std::make_pair(iterator(currentNode->pLeft), true);
            }
        }
        else
        {
            if (currentNode->pRight)
                currentNode = currentNode->pRight;
            else
            {
                currentNode->pRight = new BNode(t);
                currentNode->pRight->pParent = currentNode;
                ++numElements;
                return std::make_pair(iterator(currentNode->pRight), true);
            }
        }
    }

	return std::make_pair(end(), false); // Should not reach here but need for compiler
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
    if (!root)
    {
        root = new BNode(std::move(t));
        numElements = 1;
        return std::make_pair(iterator(root), true);
    }

    BNode* currentNode = root;
    BNode* parentNode = nullptr;

    while (currentNode)
    {
        parentNode = currentNode;
        if (keepUnique && t == currentNode->data)
            return std::make_pair(iterator(currentNode), false);

        if (t < currentNode->data)
            currentNode = currentNode->pLeft;
        else
            currentNode = currentNode->pRight;
    }

    BNode* newNode = new BNode(std::move(t));
    newNode->pParent = parentNode;

    if (newNode->data < parentNode->data)
        parentNode->pLeft = newNode;
    else
        parentNode->pRight = newNode;

    ++numElements;
    return std::make_pair(iterator(newNode), true);
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
    // do nothing if there is nothing to do
    if (it == end())
        return end();

    // remember where we were
    iterator itNext(it);
    BNode* pDelete = it.pNode;

    // if there is only one child (right) or no children (how sad!)
    if (!pDelete->pLeft)
    {
        ++itNext;
        deleteNode(pDelete, true /* go right */);
    }

    // there is only one child (left)
    else if (!pDelete->pRight)
    {
        ++itNext;
        deleteNode(pDelete, false);
    }

    // otherwise, swap places with the in-order successor
    else
    {
        // find the in-order successor ('I.O.S.')
        BNode* pIOS = pDelete->pRight;
        while (pIOS->pLeft)
            pIOS = pIOS->pLeft;

        // the IOS must not have a right node. Now it will take pDelete's place.
        assert(pIOS->pLeft == nullptr);
        pIOS->pLeft = pDelete->pLeft;
        if (pDelete->pLeft)
            pDelete->pLeft->pParent = pIOS;

        // if the IOS is not a direct right sibling, then put it in the place of pDelete
        if (pDelete->pRight != pIOS)
        {
            // if the IOS has a right sibling, then it takes his place
            if (pIOS->pRight)
                pIOS->pRight->pParent = pIOS->pParent;
            pIOS->pParent->pLeft = pIOS->pRight;

            // make IOS' right child pDelete's right child
            assert(pDelete->pRight != nullptr);
            pIOS->pRight = pDelete->pRight;
            pDelete->pRight->pParent = pIOS;
        }

        // hook up pIOS' successor
        pIOS->pParent = pDelete->pParent;
        if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
            pDelete->pParent->pLeft = pIOS;
        if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
            pDelete->pParent->pRight = pIOS;

        // what if that was the root?!?!
        if (root == pDelete)
            root = pIOS;

        itNext = iterator(pIOS);
    }

    numElements--;
    delete pDelete;
    return itNext;
}


template <typename T>
void BST<T>::deleteNode(BNode*& pDelete, bool right)
{   
    BNode* pNext = (right) ? pDelete->pRight : pDelete->pLeft;

    if (pDelete != root)
    {
        if (pDelete->pParent->pLeft == pDelete)
        {
            pDelete->pParent->pLeft = nullptr;
            pDelete->pParent->addLeft(pNext);
        }
        else
        {
            pDelete->pParent->pRight = nullptr;
            pDelete->pParent->addRight(pNext);
        }
    }
    else
    {
        this->root = pNext;
        pNext->pParent = nullptr;
    }
}



/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
	//// recursivly go down the rabit hole of the tree, once it hits the bottom will delete the leaf nodes on the way back up
 //   std::function<void(BNode*)> deleteNodes = [&](BNode* node) {
 //       if (node) {
 //           deleteNodes(node->pLeft);
 //           deleteNodes(node->pRight);
 //           node->pLeft = nullptr;
 //           node->pRight = nullptr; 
 //           delete node;
 //       }
 //       };

	//// use lambda function to delete all the nodes then reset the root and numElements
 //   deleteNodes(root);
 //   root = nullptr;
 //   numElements = 0;

    //if (root)
    deleteBinaryTree(root);
    numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
    BNode* current = root;
    // move left until we find the left most node 
    while (current && current->pLeft)
        current = current->pLeft;
    return iterator(current);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
 BNode* current = root;
    while (current) 
    {
        if (current->data == t)
            return iterator(current);
        // move left 
        else if (t < current->data)
            current = current->pLeft;  
        // move right 
        else
            current = current->pRight;
    }
    return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    if (pNode)
        pNode->pParent = this;
    this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    if (pNode)
        pNode->pParent = this;
    this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    BNode* newNode = new BNode(t);
    addLeft(newNode);
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    BNode* newNode = new BNode(t);
    addLeft(newNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
	BNode* newNode = new BNode(t);
	addRight(newNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
	BNode* newNode = new BNode(t);
	addRight(newNode);

}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (!pNode)
        return *this;
    // move to the right if there is a node to the right 
    if (pNode->pRight)
    {
        pNode = pNode->pRight;
        while (pNode->pLeft)
            pNode = pNode->pLeft;//return this 
    }
    // or go up to an ancestor 
    else
    {
        BNode* parent = pNode->pParent;
        while (parent && pNode == parent->pRight)
        {
            pNode = parent;
            parent = parent->pParent;
        }
        pNode = parent;  //return this
    }
    return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
    // same as incremement but swaps left/right 

    if (!pNode)
        return *this;
    // move to the left if there is a node to the left
    if (pNode->pLeft)
    {
        pNode = pNode->pLeft;
        while (pNode->pRight)
            pNode = pNode->pRight;//return this 
    }
    // or go up to an ancestor 
    else
    {
        BNode* parent = pNode->pParent;
        while (parent && pNode == parent->pLeft)
        {
            pNode = parent;
            parent = parent->pParent;
        }
        pNode = parent;  //return this
    }
    return *this;
}


} // namespace custom


