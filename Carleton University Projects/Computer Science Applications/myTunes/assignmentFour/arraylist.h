/* * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                   */
/*  Program:  MyTunes Music Player                   */
/*  Author:   Louis Nel                              */
/*  Date:     21-SEP-2017                            */
/*                                                   */
/*  (c) 2017 Louis Nel                               */
/*  All rights reserved.  Distribution and           */
/*  reposting, in part or in whole, requires         */
/*  written consent of the author.                   */
/*                                                   */
/*  COMP 2404 students may reuse this content for    */
/*  their course assignments without seeking consent */
/* * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include <algorithm>
#include <exception>

using namespace std;

/*
Templated ArrayList
*/

template <class T> 
   class ArrayListIterator;

template <class T>
class ArrayList 
{
friend class ArrayListIterator<T>; 

public:
	ArrayList(int initial_size = 4):
		initial_capacity(initial_size),
		capacity(initial_size),
		numberOfElements(0),
		elements(new T*[initial_size])
	{}

	ArrayList(const ArrayList<T> & a):
		initial_capacity(a.initial_capacity),
		capacity(a.capacity),
		numberOfElements(a.numberOfElements),
		elements(new T*[a.capacity])
	{
	    for(int i=0; i<numberOfElements; i++)
			elements[i] = a.elements[i];
	}

	ArrayList<T> & operator=(const ArrayList<T> & a)
	{
		if(this != &a){
			delete [] elements;

	        capacity = a.capacity;
		    numberOfElements = a.numberOfElements;
		    initial_capacity = a.initial_capacity;
			elements = new T*[capacity];

		    for(int i=0; i<numberOfElements; i++)
			    elements[i] = a.elements[i];
		}
		return this;
	}



	~ArrayList() {delete [] elements;}

	void add(T& element)
	{
		if(size() == capacity) 
			resize( capacity * 2 );
		elements[numberOfElements++] = &element; 
	}

	T& first() const
	{
		checkEmpty();
		return *(elements[0]);
	}
	T & elementAt(int index){
		if(isEmpty()) throw  exception("Container is empty");
		if(index < 0 || index > size()-1) throw  exception("Container index out of range");
		return *elements[index];
	}

	T& removeFirst()
	{
		checkEmpty();
		
		T* first = elements[0];	
		for( int i = 1; i < numberOfElements; i++ )
			elements[i-1] = elements[i];
		numberOfElements--;
		checkShrinkArray();
		return *first;
	}

	bool remove(T & anItem)
	{
    //remove first occurence of element == item based on == operator of T 
    //return true if element was found and removed and false otherwise 

		if(size() == 0) return false; //do nothing

		int found = 0; //interpret 0 as false, othewise true

		for( int i = 0; i < numberOfElements; i++ ){
			if(*elements[i] == anItem && !found) found = 1;
			elements[i] = elements[i + found];
		}
		numberOfElements -= found;

		checkShrinkArray();
		
        return found != 0;
	}



	T& removeLast()
	{
		checkEmpty();

		T* last = elements[numberOfElements-1];
		numberOfElements--;

		checkShrinkArray();
		return *last;
	}


	int size() const { return numberOfElements;}
	
	bool isEmpty() const {return size() == 0;}

	void printOn(ostream & ostr) const
	{
		for( int i = 0; i < numberOfElements; ++i )
			cout << *(elements[i]) << endl;
	}

  typedef ArrayListIterator<T> iterator; 
  iterator begin(void)  {return ArrayListIterator<T>(*this);}
  iterator end(void)  {return ArrayListIterator<T>(*this, size());} 
  const iterator begin(void) const {return   ArrayListIterator<T>(*this);}
  const iterator end(void) const {return  ArrayListIterator<T>(*this, size());} 


private: 
	void checkEmpty() const
	{
		if( numberOfElements == 0 ) 
		{
			const char* msg = "List is empty.";
			cout << msg << endl;
			throw exception(msg);
		}
	}

	void checkShrinkArray()
	{
		if((size() < capacity / 4) && (capacity / 2 > initial_capacity))
			resize( capacity / 2 );
	}

	void resize(int newCapacity)
	{
		T** temp = elements;
		capacity = newCapacity;
		elements = new T*[capacity];
		for( int i = 0; i < numberOfElements; ++i )
			elements[i] = temp[i];
		delete[] temp;
		//cout << "List updated with capacity " << capacity << "." << endl; 

	}

    //Instance Variables
	int initial_capacity;	//initial capacity of array
	int capacity;			//size of the available array memory
	int numberOfElements;	//number of elements in the list
	T** elements;		//array container

};

template <class T>
ostream & operator<<(ostream& ostr,  const ArrayList<T>& l) {
	l.printOn(ostr);
	return ostr;
}

template <class T> 
class ArrayListIterator { 
//    =============== 
 mutable int index; 
 ArrayList<T> & s; 
 public: 
  ArrayListIterator(ArrayList<T> & set, int position = 0) : index(position), s(set) {} 
  ArrayListIterator(const ArrayList<T> & set, int position = 0) : index(position), s( (ArrayList<T> &) set) {} 

  T & operator*() {return *(T*)(s.elements[index]);}; 
 const T & operator*() const {return *(T*)(s.elements[index]);}; 
 T * operator->() {return (T*)(s.elements[index]);}; 
 const T * operator->() const {return (T*)(s.elements[index]);}; 

 ArrayListIterator<T> & operator++(int)  {index++; return *this;}; 
 const ArrayListIterator<T> & operator++(int) const {index++; return *this;}; 

 bool operator==(const ArrayListIterator<T> & iter) const
     {return (&(this->s) == &(iter.s))&&(this->index == iter.index);}; 

 bool operator!=(const ArrayListIterator<T> & iter) const {return !(*this == iter);}; 

 bool operator==(ArrayListIterator<T> & iter)
     {return (&(this->s) == &(iter.s))&&(this->index == iter.index);}; 

 bool operator!=(ArrayListIterator<T> & iter) {return !(*this == iter);}; 

}; 

#endif