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
#else
#define debug(x)
#endif

#include <cassert>
#include <utility>
#include <memory>
#include <functional>
#include <utility>

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{
    template <class TT>
    class set;
    template <class KK, class VV>
    class map;

    template <typename T>
    class BST
    {
        // all of the friend classes are needed for the unit tests as access all the BST
        friend class ::TestBST;
        friend class ::TestMap;
        friend class ::TestSet;

        template <class KK, class VV>
        friend class map;

        template <class TT>
        friend class set;

        template <class KK, class VV>
        friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
#ifdef DEBUG
    public:
#else
    private:
#endif
        //Bnode is a node in the tree, we use this to create data points in the tree that are linked to other data points creating our tree
        class BNode;
        BNode* root; //root node of the tree
        size_t numElements;  // number of elements in the tree or the size of the tree

        void deleteBinaryTree(BNode*& p); // this will delete the tree
        BNode* copyBinaryTree(const BNode* pSrc);
        void assignBinaryTree(BNode*& pDest, const BNode* pSrc); // this will assign the tree to another tree
    public:
        BST(); //default constructor
        BST(const BST& rhs); // copy constructor
        BST(BST&& rhs) : BST() { *this = std::move(rhs); } // move constructor, the move calls the move assignment operator
        BST(const std::initializer_list<T>& il) : BST() { *this = il; } // initializer list constructor
        ~BST(); // destructor

        BST& operator = (const BST& rhs);
        BST& operator = (BST&& rhs);
        BST& operator = (const std::initializer_list<T>& il);
        void swap(BST& rhs);

        class iterator; // will use iterator to move about the tree
        iterator begin() const noexcept; 
        iterator end() const noexcept { return iterator(nullptr); }

        iterator find(const T& t); // find an element in the tree

        std::pair<iterator, bool> insert(const T& t, bool keepUnique = true); // insert an element into the tree, keepUnique is true by default because when do we not want to keep it unique?
        std::pair<iterator, bool> insert(T&& t, bool keepUnique = true); 

        iterator erase(iterator& it); // erase an element from the tree
        void clear() noexcept; // clear the tree

        void deleteNode(BNode*& pDelete, bool toRight); // delete a node from the tree

        bool empty() const noexcept { return numElements == 0; }    // check if the tree is empty
        size_t size() const noexcept { return numElements; } // return the size of the tree
    };

    template <typename T>
    class BST <T> ::BNode
    {
    public:
        // Bnode constructors
        BNode() : data(T()), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) {}
        BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) {}
        BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true) {}

        // Bnode functions, these functions handle the where the node is placed in the tree
        void addLeft(BNode* pNode);
        void addRight(BNode* pNode);
        void addLeft(const T& t);
        void addRight(const T& t);
        void addLeft(T&& t);
        void addRight(T&& t);

        // checks and balances for the tree
        bool isRightChild() const { return pParent && pParent->pRight == this; }
        bool isLeftChild() const { return pParent && pParent->pLeft == this; }

        // our nodes attributes and pointers
        T data;
        BNode* pLeft;
        BNode* pRight;
        BNode* pParent;
        bool isRed;
    };


    // The iterator will move about the tree and allow us to access the data in the tree
    template <typename T>
    class BST <T> ::iterator
    {
        // more unit test access
        friend class ::TestBST;
        friend class ::TestMap;
        friend class ::TestSet;

        template <class KK, class VV>
        friend class map;

        template <class TT>
        friend class set;
    public:
        // iterator constructors
        iterator(BNode* p = nullptr) : pNode(p) {}
        iterator(const iterator& rhs) : pNode(rhs.pNode) {}

        // iterator assignment operator
        iterator& operator = (const iterator& rhs)
        {
            pNode = rhs.pNode;
            return *this;
        }

        // iterator comparison operators
        bool operator == (const iterator& rhs) const
        {
            return pNode == rhs.pNode;
        }

        // iterator not comparison operators
        bool operator != (const iterator& rhs) const
        {
            return pNode != rhs.pNode;
        }

        // iterator dereference operator
        const T& operator * () const
        {
            return pNode->data;
        }

        // iterator prefix increment
        iterator& operator ++ ();
        iterator   operator ++ (int postfix)
        {
            return *this;
        }

        // iterator prefix decrement
        iterator& operator -- ();
        iterator   operator -- (int postfix)
        {
            return *this;
        }

        friend BST <T> ::iterator BST <T> ::erase(iterator& it);

    private:
        // the only attribute of the iterator is the node as this is our vehicle to move about the tree
        BNode* pNode;
    };

    // The BST class constructors implementations, well some, others are defined at class declaration
    // could move these to the .cpp file but keeping them in the header for convenience
    template <typename T>
    BST <T> ::BST() : numElements(0), root(nullptr) {}

    template <typename T>
    BST<T>::BST(const BST<T>& rhs) : numElements(0), root(nullptr)
    {
        *this = rhs;
    }

    template <typename T>
    BST <T> :: ~BST()
    {
        clear();
    }

    // using recursion find the leaf node left and right then on the way back up th tree delete the node
    template <class T>
    void BST<T>::deleteBinaryTree(BST<T>::BNode*& node)
    {
        if (!node)
            return;

        deleteBinaryTree(node->pLeft);
        deleteBinaryTree(node->pRight);

        delete node;
        node = nullptr;
    }

    // also using recursion find the left or right most node and on the way back up the tree assign the nodes
    template <class T>
    void BST<T>::assignBinaryTree(BST<T>::BNode*& pDest, const BST<T>::BNode* pSrc)
    {
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

    // 
    template <typename T>
    BST <T>& BST <T> :: operator = (const BST <T>& rhs)
    {
        assignBinaryTree(root, rhs.root);
        numElements = rhs.numElements;
        return *this;
    }
 
    // if we get a list of elements, make room, and loop through the list and insert the elements
    template <typename T>
    BST <T>& BST <T> :: operator = (const std::initializer_list<T>& il)
    {
        clear();
        for (auto&& it : il)
            insert(it);
        return *this;
    }

    template <typename T>
    BST <T>& BST <T> :: operator = (BST <T>&& rhs)
    {
        clear();
        swap(rhs);
        return *this;
    }

    // swap, switch, flip, exchange, you get the idea
    // just swap the old tree with the new tree
    template <typename T>
    void BST <T> ::swap(BST <T>& rhs)
    {
        std::swap(root, rhs.root);
        std::swap(numElements, rhs.numElements);
    }


    // insert takes two parameters, the element to insert and a bool to keep it unique, the unique is true by default
    // if the tree is empty, create a new node and set it as the root, other
    // wise loop through the tree and insert the element in the correct place
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

        return std::make_pair(end(), false);
    }


    // the main difference with this insert is that we are moving the element into the tree instead of copying it
    template <typename T>
    std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique)
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

    // erase an element from the tree using the iterator
    template <typename T>
    typename BST<T>::iterator BST<T>::erase(iterator& it)
    {
        if (it == end())
            return end();

        iterator itNext(it);
        BNode* pDelete = it.pNode;

        if (!pDelete->pLeft)
        {
            ++itNext;
            deleteNode(pDelete, true);
        }
        else if (!pDelete->pRight)
        {
            ++itNext;
            deleteNode(pDelete, false);
        }
        else
        {
            BNode* pIOS = pDelete->pRight;
            while (pIOS->pLeft)
                pIOS = pIOS->pLeft;

            pIOS->pLeft = pDelete->pLeft;
            if (pDelete->pLeft)
                pDelete->pLeft->pParent = pIOS;

            if (pDelete->pRight != pIOS)
            {
                if (pIOS->pRight)
                    pIOS->pRight->pParent = pIOS->pParent;
                pIOS->pParent->pLeft = pIOS->pRight;

                pIOS->pRight = pDelete->pRight;
                pDelete->pRight->pParent = pIOS;
            }

            pIOS->pParent = pDelete->pParent;
            if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
                pDelete->pParent->pLeft = pIOS;
            if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
                pDelete->pParent->pRight = pIOS;

            if (root == pDelete)
                root = pIOS;

            itNext = iterator(pIOS);
        }

        numElements--;
        delete pDelete;
        return itNext;
    }


    // delete a node from the tree, right is a bool to determine if we are deleting the right node or the left node
    // the client should know which node they are deleting
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

    // clear the tree, delete the tree
    template <typename T>
    void BST <T> ::clear() noexcept
    {
        deleteBinaryTree(root);
        numElements = 0;
    }

    template <typename T>
    typename BST <T> ::iterator custom::BST <T> ::begin() const noexcept
    {
        BNode* current = root;
        while (current && current->pLeft)
            current = current->pLeft;
        return iterator(current);
    }

    // the iterator searches the tree for the element, if it finds it, it returns the iterator, if not it does not find anything returns the end iterator
    template <typename T>
    typename BST <T> ::iterator BST<T> ::find(const T& t)
    {
        BNode* current = root;
        while (current)
        {
            if (current->data == t)
                return iterator(current);
            else if (t < current->data)
                current = current->pLeft;
            else
                current = current->pRight;
        }
        return end();
    }


    // the BNode class functions implementations moving the node to the left or right of the parent node
    template <typename T>
    void BST <T> ::BNode::addLeft(BNode* pNode)
    {
        if (pNode)
            pNode->pParent = this;
        this->pLeft = pNode;
    }

    template <typename T>
    void BST <T> ::BNode::addRight(BNode* pNode)
    {
        if (pNode)
            pNode->pParent = this;
        this->pRight = pNode;
    }

    template <typename T>
    void BST<T> ::BNode::addLeft(const T& t)
    {
        BNode* newNode = new BNode(t);
        addLeft(newNode);
    }

    template <typename T>
    void BST<T> ::BNode::addLeft(T&& t)
    {
        BNode* newNode = new BNode(t);
        addLeft(newNode);
    }

    template <typename T>
    void BST <T> ::BNode::addRight(const T& t)
    {
        BNode* newNode = new BNode(t);
        addRight(newNode);
    }

    template <typename T>
    void BST <T> ::BNode::addRight(T&& t)
    {
        BNode* newNode = new BNode(t);
        addRight(newNode);
    }

    // the iterator prefix increment operator, this will move the iterator to the next node in the tree
    template <typename T>
    typename BST <T> ::iterator& BST <T> ::iterator :: operator ++ ()
    {
        if (!pNode)
            return *this;
        if (pNode->pRight)
        {
            pNode = pNode->pRight;
            while (pNode->pLeft)
                pNode = pNode->pLeft;
        }
        else
        {
            BNode* parent = pNode->pParent;
            while (parent && pNode == parent->pRight)
            {
                pNode = parent;
                parent = parent->pParent;
            }
            pNode = parent;
        }
        return *this;
    }

    template <typename T>
    typename BST <T> ::iterator& BST <T> ::iterator :: operator -- ()
    {
        if (!pNode)
            return *this;
        if (pNode->pLeft)
        {
            pNode = pNode->pLeft;
            while (pNode->pRight)
                pNode = pNode->pRight;
        }
        else
        {
            BNode* parent = pNode->pParent;
            while (parent && pNode == parent->pLeft)
            {
                pNode = parent;
                parent = parent->pParent;
            }
            pNode = parent;
        }
        return *this;
    }

} // namespace custom
