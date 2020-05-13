/*Name: Connor Raymond Stewart */
/*Student Number: 101041125 */

#include <iostream>
#include <string>
#include <string.h>
#include <locale>

using namespace std;
const int MAX_CHAR_LENGTH = 100;
const int MAX_WORD_LENGTH = 10;
const int LOG_BOTH = 2;
const int LOG_COMMANDS = 1;
const int LOG_NOTHING = 0;
const int FIRST_CHAR = 0;
const int DEFAULT_MESSAGE = 0;
const int LOG_COMMANDS_UI = 27;
const int LOG_BOTH_UI = 28;
const int COMMENT = 29;
const int READ = 26;
	
class input {
	private:
	bool init;
	int toUI;
	int word;
	char userInput[MAX_CHAR_LENGTH];
	string arrayOne[MAX_WORD_LENGTH];
	string inputVal;
	string output;
	int length;
	locale loc;
	
	public:
	input(bool current);
	void run();
};