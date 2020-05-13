/* * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                   */
/*  Program:  MyTunes Music Player                   */
/*  Author:   Connor Raymond Stewart 101041125       */
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
#include "observer.h"
#include "arraylist.h"
//#include "playlist.h"
#include <iostream>
#include <list>
using namespace std; 

class Subject { 
 ArrayList<Observer> observers; 
public: 
  Subject();
  void attach(Observer & obs);
  void dettach(Observer & obs);
  

protected: 
  void notify();
  
  

}; 

