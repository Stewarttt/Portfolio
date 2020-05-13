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

#ifndef _MYTUNES_COLLECTION_H
#define _MYTUNES_COLLECTION_H

#include <ostream>
#include <vector>
#include <string>
using namespace std;
#include "UI.h"

template <typename T>
class MyTunesCollection {
	/*
	Tracks represents a collection of compositions, specifically Track objects
	A Tracks container OWNS the Tracks within it.
	If the container is destroyed it deletes the Track objects it contains
	If a Track is removed from the container the Track object is deleted from heap space.
	
	//explanation about typename
	https://stackoverflow.com/questions/20866892/c-iterator-with-template
	
	*/
	public:
	MyTunesCollection(); 
	~MyTunesCollection(void);
	T * findByID(int anID); //find by numeric ID
	T * findByID(const string & anID); //find by string ID
	vector<T*> getCollection();
	void add(T & aT);
	void remove(T & aT);
	void showOn(UI & aView);
	void showOn(UI & aView, int memberID);
	void showOn(UI & aView, string memberID);
	private:
	vector<T*> collection;
	typename vector<T*>::iterator findPosition(T & aT);
};

template <typename T>
MyTunesCollection<T>::MyTunesCollection(){
}

template <typename T>
MyTunesCollection<T>::~MyTunesCollection(void){
	for(int i=0; i<collection.size(); i++)
		delete collection[i]; //delete elements in this container
}

template <typename T>
vector<T*> MyTunesCollection<T>::getCollection(){return collection;}

template <typename T>
typename vector<T*>::iterator MyTunesCollection<T>::findPosition(T & aT){
	for (typename vector<T*>::iterator it = collection.begin(); it != collection.end(); ++it)
		if(*it == &aT) return it;
	return collection.end();
}

template <typename T>
T * MyTunesCollection<T>::findByID(int anID){
	//find by numeric ID
	for (typename vector<T*>::iterator it = collection.begin(); it != collection.end(); ++it)
		if((*it)->getID() == anID) return *it;
	return NULL;
}

template <typename T>
T * MyTunesCollection<T>::findByID(const string & anID) {
	//find by string ID
	for (typename vector<T*>::iterator it = collection.begin() ; it != collection.end(); ++it)
		if(((*it)->getIDString()).compare(anID) == 0) return *it;
	return NULL;
}

template <typename T>
void MyTunesCollection<T>::add(T & aT){
	collection.push_back(&aT);
}

template <typename T>
void MyTunesCollection<T>::remove(T & aT){
	typename vector<T*>::iterator index = findPosition(aT);
	if(index != collection.end()) {
		T * theT = *index;
		collection.erase(index);
		delete theT; //free the memory of theT
	}
}

template <typename T>
void MyTunesCollection<T>::showOn(UI & view) {
  view.printOutput("MyTunesCollection:");
  for(int i=0; i<collection.size(); i++)
       view.printOutput((*collection[i]).toString());	  
}

template <typename T>
void MyTunesCollection<T>::showOn(UI & view, int memberID)  {
  view.printOutput("Element:");
  T * t = findByID(memberID);
  if( t != NULL)
       view.printOutput(t->toString());	  
}

template <typename T>
void MyTunesCollection<T>::showOn(UI & view, string memberID)  {
  view.printOutput("Element:");
  T * t = findByID(memberID);
  if( t != NULL)
       view.printOutput(t->toString());	  
}

#endif