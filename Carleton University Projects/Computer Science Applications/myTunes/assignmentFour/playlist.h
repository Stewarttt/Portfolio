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

#ifndef _Playlist_H
#define _Playlist_H

#include <ostream>
#include <string>
#include <vector>

#include "track.h"
#include "subject.h"


class Playlist : public Observer, public Subject {
	/*
	Playlist represents an user playlist of tracks.
	*/
	public:
	Playlist(const string & aPlaylistName); 
	~Playlist(void);
    int getID();
	string getName() const;
	void removeTrack(Track & aTrack);
	void addTrack(Track & aTrack);
	vector<Track*> & getTracks();
    string toString() const;
	void setState(int following);
	int getState();
	void startAttach(Playlist & playlistTwo);
	void execute();
	void setSelf(Playlist & aSelf);
	void setTracks(vector<Track*> aTracks);
	vector<Playlist*> getSelf();
	void removePlaylist(Playlist & playlist);
//	void setTracks(vector<Track*> newTracks);
	
	//concrete implementation of virtual functions
	virtual void update(Subject * subject) {
		int size = self.size();

		
		if (this != subject) {
			for (int i = 0; i<size; i++) {
				if (self[i]->getState() == 1) {
					self[i]->setTracks(this->getTracks());
				}
				else if (self[i]->getState() == 0) { //if an element is no longer being follow it is removed from the array
					self[i]->dettach(*this);
					self.erase(self.begin() + i);
				}
			}
		}
		if (this->getState() == -1) { //this is used to deinitialize referental pointers for a playlist about to be deleted
			for (int i = 0; i<size; i++) {
				self[i]->dettach(*this);
				self.erase(self.begin() + i);
			}
		}
	}

	virtual void printOn(ostream & out) const { 
		out << "RUN";
		
	} 
	
	private:
	string name;
	int following;
	vector<Track*> tracks;	
	vector<Track*>::iterator findPosition(Track & aTrack);
	Playlist(const Playlist & aPlaylist); //hide copy constructor in private section
	
	protected:
	vector<Playlist*> self;
	
};

ostream & operator<<(ostream & out, const Playlist & aPlaylist);

#endif