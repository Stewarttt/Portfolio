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

#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
using namespace std;

/*
Represents commands supported by the application
These include: App commands, UI shell commands,
some developer commands not intended for production
*/

//Application commands
const string CMD_ADD = "add"; //add items to collections
const string CMD_DELETE = "delete"; //delete items from collections
const string CMD_SHOW = "show"; //display collections
const string CMD_FOLLOW = "follow"; //allows one playlist to follow changes made to another - added by Connor Stewart

//UI shell only commands
const string UICMD_LOG = ".log"; //log comands and output
const string UICMD_HELP = ".help"; //show command help documentation
const string UICMD_QUIT = ".quit"; //quit the application
const string UICMD_READ = ".read"; //read commands from script file

//Developer commands (for debugging)
const string DEV_trim = ".trim"; 
const string DEV_endsWith = ".endsWith"; 
const string DEV_startsWith = ".startsWith";
const string DEV_toTitleCase = ".toTitleCase";

const string UICommands[] = {
	UICMD_QUIT, 
	UICMD_LOG, 
	UICMD_HELP,
	UICMD_READ
	};
const int NumberOfUICommands = 4;
const string AppCommands[] = {
	CMD_ADD,
	CMD_DELETE,
	CMD_SHOW,
	CMD_FOLLOW, //added by Connor Stewart
	};
const int NumberOfAppCommands = 4; //updated by Connor Stewart

const string DevCommands[] = {
	DEV_trim,
	DEV_startsWith,
	DEV_endsWith,
    DEV_toTitleCase
	};
const int NumberOfDevCommands = 4;


class Command
{
  public:   
   Command(string cmdStr);
   string getCommandString();
   string getCommandName(); //first word in command string
   bool hasToken(string s); //answer whether a token s exists
   int indexOfToken(string s); //answer index of token s
   string getToken(int i); //get i'th token of parsed command
   string getToken(string s); //get token indexed by string s
   bool isCommand(string aCommandName);
   bool isTokenAt(int index, const string & s);
   bool isValidIndex(string anIndex);
   bool isUICommand();
   bool isAppCommand();
   bool isDevCommand();
   
  private:
   string commandString; //command string;
   vector<string> tokens; //parsed command tokens
   void parse(string commandString); //parse the command string
   
};

#endif
