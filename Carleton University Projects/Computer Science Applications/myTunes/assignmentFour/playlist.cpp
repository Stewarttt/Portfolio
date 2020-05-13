/* * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                   */
/*  Program:  MyTunes Music Player                   */
/*   Contributors:  Connor Stewart 101041125         */
/*                   Louis Nel                       */
/*  Date:     Nov 2017                               */
/*                                                   */
/*  (c) Nov 2017                                     */
/*  All rights reserved.  Distribution and           */
/*  reposting, in part or in whole, requires         */
/*  written consent of the author.                   */
/*                                                   */
/*  COMP 2404 students may reuse this content for    */
/*  their course assignments without seeking consent */
/* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "playlist.h"
	
Playlist::Playlist(const string & aPlaylistName){
	cout << "Playlist(string&)" << endl;
	name = aPlaylistName;
	following = 0;
}
Playlist::Playlist(const Playlist & aPlaylist){
	cout << "Playlist(const Playlist & aPlaylist)" << endl;
	cout << "ERROR: Playlist(const Playlist & aPlaylist) --should never run" << endl;
}

Playlist::~Playlist(){
	this->setState(-1);
	this->update(this);
	
	
	cout << "~Playlist(void)" << endl;
}
int Playlist::getID(){return -1;}
string Playlist::getName() const {return name;}

vector<Track*> & Playlist::getTracks() {
	return tracks;
}




vector<Track*>::iterator Playlist::findPosition(Track & aTrack){
	for (vector<Track*>::iterator itr = tracks.begin() ; itr != tracks.end(); ++itr)
		if(*itr == &aTrack) return itr;
	return tracks.end();
}

void Playlist::addTrack(Track & aTrack){
	//add track if it does not already exist
	vector<Track*>::iterator itr = findPosition(aTrack);
	if(itr == tracks.end()) {
		tracks.push_back(&aTrack);
	}	
}


void Playlist::setSelf(Playlist & aSelf) { //ADDED by connor, this contains references to the playlists being followed
	self.push_back(&(aSelf));
}

vector<Playlist*> Playlist::getSelf() { //ADDED by connor stewart, this returns the self pointer
	return self;
}

void Playlist::removePlaylist(Playlist & playlist) {
	//self.erase(*(&playlist));
}

void Playlist::setTracks(vector<Track*> aTracks) {
	tracks.clear();
	for (int i = 0; i<aTracks.size(); i++) {
		tracks.push_back(aTracks[i]);
	}
	
}


void Playlist::removeTrack(Track & aTrack){
	vector<Track*>::iterator itr = findPosition(aTrack);
	if(itr != tracks.end()) {
		tracks.erase(itr);
	}
}

string Playlist::toString()const {
	string indent = "     ";
	string s;
	s.append(name);
	s.append("\n");
	s.append(indent + indent + "Playlist Tracks:\n");
	for (vector<Track*>::size_type i = 0 ; i < tracks.size(); i++){
		   s.append(indent + indent + to_string(i) + " " + (tracks[i])->toString() + "\n");
	}
	
	return s;
}

int Playlist::getState() {
	return following;
}

void Playlist::startAttach(Playlist & playlistTwo) {
	cout << this << " " << &playlistTwo << endl;
	playlistTwo.attach(*this);
}

void Playlist::execute() {
	notify();
}

void Playlist::setState(int newState) {
	following = newState;
}

ostream & operator<<(ostream & out, const Playlist & aPlaylist){
	return out;
}
