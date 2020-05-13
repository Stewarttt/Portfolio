/*Name: Connor Raymond Stewart */
/*Student Number: 101041125 */

#include <iostream>
#include <string>
#include <fstream>
#include "logging.h"
using namespace std;

logging::logging(string input){
	write = input;
	active = 0;
}//END logging

void logging::setWrite(string input) {
	write = input;
}//END setWrite

void logging::swapState(int i) {
	active = i;
}

int logging::getState() {
	return active;
}//END getState

void logging::writeToFile() {
	
	if (active > NO_STATE) {
		ofstream output;
		output.open(fileName, ios_base::app);
		output << write << endl;
		output.close();
	}//END IF
}//END writeToFile