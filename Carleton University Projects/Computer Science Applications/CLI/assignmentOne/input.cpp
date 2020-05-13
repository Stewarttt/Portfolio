/*Name: Connor Raymond Stewart */
/*Student Number: 101041125 */

#include <iostream>
#include <string>
#include <string.h>
#include "input.h"
#include "UI.h"
#include "parser.h"
#include <locale>
#include "reader.h"
#include "logging.h"

using namespace std;

input::input(bool current){
	(void)init;
	toUI = 0;
	word = 0;
} 
	
void input::run() { //runs file
	(void)init;
	logging addLine("__PLACEHOLDER__"); //write to file
	
	while (true) {
		toUI = 0;
		word = 0;
		inputVal = "";
		output = "";
		UI current(toUI);
		output = current.interface();
		cout << output;
	
	
		//writes down output to file
		if (addLine.getState() == LOG_BOTH) {
			addLine.setWrite(output);
			addLine.writeToFile();
		}//END IF
		
		
		
		

		
		
		

		
		  
		cin.getline(userInput, sizeof(userInput)); //gets user input
		int arraySize = (unsigned)strlen(userInput); //gets length of user input
		  
		// writes down user input if logging is on
		addLine.setWrite(userInput);
		addLine.writeToFile();
		  
		  
		//splits user input allong all spaces into separate array indexes
		for(int i = 0; i<arraySize; ++i) {
			if (userInput[i] == ' ') {
				  word += 1;
			}
			else {
				userInput[i] = tolower(userInput[i], loc);
				arrayOne[word] += userInput[i];
			}//END IF
		}  //END IF
		
		
		 
		
		parser thisCommand(arrayOne); //parse array into input, initialize with this array
		toUI = thisCommand.command(); //returns what message to prompt user with
		if (toUI == LOG_COMMANDS_UI) { //user starts logging input
			if (addLine.getState() == LOG_NOTHING) { //start logging input
				addLine.swapState(LOG_COMMANDS);
			}
			else { //stop logging input
				addLine.swapState(LOG_NOTHING);
			}//END IF
		}//END IF
		else if (toUI == LOG_BOTH_UI) { //user logs input and output
			if (addLine.getState() == LOG_NOTHING) { //turn on
				addLine.swapState(LOG_BOTH);
			}
			else { //turn off
				addLine.swapState(LOG_NOTHING);
			}//END IF
		}//END IF
		
		
		current.setVal(toUI); // initalizes UI with message variable
		output = current.interface(); //gets output message

		cout << output; //prints output message to user
		
		//if input/output logging on, puts output message onto log
		if (addLine.getState() == LOG_BOTH) {
			addLine.setWrite(output);
			addLine.writeToFile();
		}//END IF

		//if user requests to open a file for reading scripts, this will prompt them for input reguarding the file name
		if (toUI == READ) {
			string fileName;
			cin >> fileName;

					
			current.setVal(DEFAULT_MESSAGE);
			reader openFile(fileName);
			openFile.getFileArray();
		}//END IF

		//outputs all the words in users input, except comments
		if (toUI != COMMENT) {
			for (int i =0; i<=word; ++i) {
				cout << arrayOne[i] << endl;
				if (addLine.getState() == LOG_BOTH) {
					addLine.setWrite(arrayOne[i]);
					addLine.writeToFile();
				}
			}
		}
		else { //preps comment header, prints headers individual words
			arrayOne[FIRST_CHAR] = "//";
			for (int i =0; i<=word; ++i) {
				cout << arrayOne[i] << " ";
				if (addLine.getState() == LOG_BOTH) {
					addLine.setWrite(arrayOne[i]);
					addLine.writeToFile();
				}
			}
			cout << ("\n"); //next line
		} //END IF
		
		
		//clears the input array, setting everything to "" blank
		length = sizeof(arrayOne)/sizeof(arrayOne[FIRST_CHAR]);
		for (int i = 0; i< length; ++i) {
			arrayOne[i] = "";
		}
		
		length = (unsigned)strlen(userInput);
		for (int i = 0; i< length; ++i) {
			userInput[i] = '\0';
		}
		
	
	}
	
}//END Input
	
	
	

