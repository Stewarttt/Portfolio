/*Name: Connor Raymond Stewart */
/*Student Number: 101041125 */

#include <iostream>
#include <string>
using namespace std;
const int NO_STATE = 0;
class logging {
	private:
	string write;
	string fileName = "output.txt";
	
	int active = 0;
	public:
	logging(string input);
	void writeToFile();
	void setWrite(string input);
	void swapState(int i);
	int getState();
};