/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Joshua Brooklyn
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() 
   { 
   }
   set(const set &  rhs) : bst(rhs.bst)
   { 
   }
   set(set && rhs) : bst(std::move(rhs.bst))
   { 
   }
   set(const std::initializer_list <T> & il)
   {
	   insert(il); // make new set from initializer list byt inserting each element into the set
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
	   insert(first, last); // same as above but with iterators
   }
  ~set() { }

   //
   // Assign
   //


  // Assign operators take care of the moving and copying the data depending on what gets passed in is witch operator is called
   set & operator = (const set & rhs)
   {
       bst = rhs.bst;      
       return *this;
   }
   set & operator = (set && rhs)
   {
       bst = std::move(rhs.bst);
       return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
       bst = il;
       return *this;
   }
   void swap(set& rhs) noexcept
   {
       bst.swap(rhs.bst);
   }

   //
   // Iterator
   //

   // This class is a iterator needed for the set class so we can iterate or move through the set accessing the data at each node
   class iterator;
   iterator begin() const noexcept 
   { 
	   return bst.begin(); // send back a iterator that starts at the beginning of the set
   }
   iterator end() const noexcept 
   { 
	   return bst.end(); // or send back a iterator that starts at the end of the set
   }

   //
   // Access
   //

   // The find function in the iterator class is used to find a specific element in the set
   iterator find(const T& t) 
   { 
	   return bst.find(t); // when called, find will return the binary search tree's find function that will return the iterator of the element we are looking for
   }

   //
   // Status
   //

   // Empte and size status functions are used to check if the set is empty or how many elements are in the set. This piggie backs off of last weeks BST
   bool   empty() const noexcept 
   { 
       return bst.empty();
   }
   size_t size() const noexcept 
   { 
       return bst.size();
   }

   //
   // Insert
   //

   // Our inserts are the same as last weeks BST inserts. 
   // They insert a new element into the bst then send back the iterator
   std::pair<iterator, bool> insert(const T& t)
   {
       return bst.insert(t);
   }

   std::pair<iterator, bool> insert(T&& t)
   {
       return bst.insert(std::move(t));
   }

   void insert(const std::initializer_list <T>& il)
   {
       for (auto&& t : il)
           bst.insert(t);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       for (auto it = first; it != last; it++)
           bst.insert(*it);
   }


   //
   // Remove
   //

   // These remove functions... well... remove stuff.
   void clear() noexcept 
   { 
       bst.clear();
   }
   iterator erase(iterator &it)
   { 
	   it = bst.erase(it.it);   // dont care just erase it
	   return it;               // now return your empty maybe sad iterator
   }
   size_t erase(const T & t) 
   {
	   auto it = find(t); // its like wheres wolldo, but with data

       if (it == end())
		   return 0; // failed successfully to find the element so return 0

       erase(it); // or successfully did not fail to find... so erase it and make it go away

       return 1; // then return 1 beacuse why not
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
       auto it = itBegin;   // okay start here

       while (it != itEnd) // did i say stop? Then keep going
		   it = erase(it); // erase me

	   return it; // send back trash can with less trash
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
   // constructors, destructors, and assignment operator
   iterator() 
   { 
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) 
   {  
	   this->it = itRHS; // Dont know what type you are but i like you and want to copy you
   }
   iterator(const iterator & rhs) 
   { 
	   this->it = rhs.it; // yeah i like you too so i will copy you
   }
   iterator & operator = (const iterator & rhs)
   {
	   this->it = rhs.it; // its like looking it to a mirror and seeing the same thing
	   return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
	   return it != rhs.it; // just a really long way of saying not equal, equal with a bool becuase c++
   }
   bool operator == (const iterator & rhs) const 
   { 
       return it == rhs.it; // same this as above but backwards and not as fun
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
	   return *it; // what am i pointing at? Call me and find out //Pointing is rude!
   }

   // prefix increment
   iterator & operator ++ ()
   {
	   ++it;        // increment the iterator because c++ not like the other languages and it I want to ++ and make it my self so i can show off my ++ skills
       return *this; 
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
	   auto tmp = *this; // what I said above but on steroids
       it++;
       return tmp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
	   --it;    // what I said above but backwards
       return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
	   auto tmp = *this; // also what I said above but backwards and on steroids
       it--;
       return tmp;
   }
   
private:

   typename custom::BST<T>::iterator it;
};


}; // namespace custom



