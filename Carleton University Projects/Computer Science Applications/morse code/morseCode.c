/*
Name: bitMath.c
Author: Connor Raymond Stewart
ID: 101041125
cmail: connorrstewart@cmail.carleton.ca
Name: morseCode
Input: 
    userInput - string to be converted to Morse code
Output: 
    Morse code representation of userInput
Description:
    a simple program which converts a input string into Morse code
*/
//imported headers
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
Name: letterToMorseCode
Description:
    This struct contains a letter and its corresponding Morse code representation
*/
struct letterToMorseCode {
    char letter;
    char morseCode[5];
}; //END letterToMorseCode

/*
Name: main
Input: 
    userInput - user input representing a string to be converted to Morse code
Output:
    Morse code representation of userInput
Description:
    Each letter of the alphabet is initialized in a letterToMorseCode struct
    each struct is also initialized with is Morse code representation for each letter
    the program loops through the users inputted string, and matches each letter 
    with its corresponding Morse code value.
    the program prints each Morse code value to the shell, and prints a space for each
    space between a letter and three spaces for spaces between words
    a ? symbol is printed for non alphabetic characters
*/
int main() {
    //declarations
    struct letterToMorseCode alphabet[26]; //contains one struct for each letter
    char userInput[255]; //buffer for user input
    int userInputLength; //variable to contain the length of the users input

    //get the users input
    printf("Enter the sentence to be converted to Morse code:\n"); //PROMPT user
    fgets(userInput, 255, stdin); //get the users input up to 255 characters

    //find the length of the users input
    userInputLength = strlen(userInput);

    //convert all letters in the users input to upper-case letters
    for (int i = 0; i < userInputLength; i++) {
        userInput[i] = toupper(userInput[i]); //enter the upper-case value for the letter
    } //END FOR

    //initialize each letter of the alphabet in the struct
    //letter a
    alphabet[0].letter = 'A';
    alphabet[0].morseCode[0] = '.';
    alphabet[0].morseCode[1] = '-';
    alphabet[0].morseCode[2] = '\0';

    //letter b
    alphabet[1].letter = 'B';
    alphabet[1].morseCode[0] = '-';
    alphabet[1].morseCode[1] = '.';
    alphabet[1].morseCode[2] = '.';
    alphabet[1].morseCode[3] = '.';
    alphabet[1].morseCode[4] = '\0';

    //letter c
    alphabet[2].letter = 'C';
    alphabet[2].morseCode[0] = '-';
    alphabet[2].morseCode[1] = '.';
    alphabet[2].morseCode[2] = '-';
    alphabet[2].morseCode[3] = '.';
    alphabet[2].morseCode[4] = '\0';

    //letter d
    alphabet[3].letter = 'D';
    alphabet[3].morseCode[0] = '-';
    alphabet[3].morseCode[1] = '.';
    alphabet[3].morseCode[2] = '.';
    alphabet[3].morseCode[3] = '\0';

    //letter e
    alphabet[4].letter = 'E';
    alphabet[4].morseCode[0] = '.';
    alphabet[4].morseCode[1] = '\0';

    //letter f
    alphabet[5].letter = 'F';
    alphabet[5].morseCode[0] = '.';
    alphabet[5].morseCode[1] = '.';
    alphabet[5].morseCode[2] = '-';
    alphabet[5].morseCode[3] = '.';
    alphabet[5].morseCode[4] = '\0';

    //letter g
    alphabet[6].letter = 'G';
    alphabet[6].morseCode[0] = '-';
    alphabet[6].morseCode[1] = '-';
    alphabet[6].morseCode[2] = '.';
    alphabet[6].morseCode[3] = '\0';

    //letter h
    alphabet[7].letter = 'H';
    alphabet[7].morseCode[0] = '.';
    alphabet[7].morseCode[1] = '.';
    alphabet[7].morseCode[2] = '.';
    alphabet[7].morseCode[3] = '.';
    alphabet[7].morseCode[4] = '\0';

    //letter i
    alphabet[8].letter = 'I';
    alphabet[8].morseCode[0] = '.';
    alphabet[8].morseCode[1] = '.';
    alphabet[8].morseCode[2] = '\0';

    //letter j
    alphabet[9].letter = 'J';
    alphabet[9].morseCode[0] = '.';
    alphabet[9].morseCode[1] = '-';
    alphabet[9].morseCode[2] = '-';
    alphabet[9].morseCode[3] = '-';
    alphabet[9].morseCode[4] = '\0';
    
    //letter k
    alphabet[10].letter = 'K';
    alphabet[10].morseCode[0] = '-';
    alphabet[10].morseCode[1] = '.';
    alphabet[10].morseCode[2] = '-';
    alphabet[10].morseCode[3] = '\0';

    //letter l
    alphabet[11].letter = 'L';
    alphabet[11].morseCode[0] = '.';
    alphabet[11].morseCode[1] = '-';
    alphabet[11].morseCode[2] = '.';
    alphabet[11].morseCode[3] = '.';
    alphabet[11].morseCode[4] = '\0';

    //letter m
    alphabet[12].letter = 'M';
    alphabet[12].morseCode[0] = '-';
    alphabet[12].morseCode[1] = '-';
    alphabet[12].morseCode[2] = '\0';

    //letter n
    alphabet[13].letter = 'N';
    alphabet[13].morseCode[0] = '-';
    alphabet[13].morseCode[1] = '.';
    alphabet[13].morseCode[2] = '\0';

    //letter o
    alphabet[14].letter = 'O';
    alphabet[14].morseCode[0] = '-';
    alphabet[14].morseCode[1] = '-';
    alphabet[14].morseCode[2] = '-';
    alphabet[14].morseCode[3] = '\0';

    //letter p
    alphabet[15].letter = 'P';
    alphabet[15].morseCode[0] = '.';
    alphabet[15].morseCode[1] = '-';
    alphabet[15].morseCode[2] = '-';
    alphabet[15].morseCode[3] = '.';
    alphabet[15].morseCode[4] = '\0';

    //letter q
    alphabet[16].letter = 'Q';
    alphabet[16].morseCode[0] = '-';
    alphabet[16].morseCode[1] = '-';
    alphabet[16].morseCode[2] = '.';
    alphabet[16].morseCode[3] = '-';
    alphabet[16].morseCode[4] = '\0';

    //letter r
    alphabet[17].letter = 'R';
    alphabet[17].morseCode[0] = '.';
    alphabet[17].morseCode[1] = '-';
    alphabet[17].morseCode[2] = '.';
    alphabet[17].morseCode[3] = '\0';

    //letter s
    alphabet[18].letter = 'S';
    alphabet[18].morseCode[0] = '.';
    alphabet[18].morseCode[1] = '.';
    alphabet[18].morseCode[2] = '.';
    alphabet[18].morseCode[3] = '\0';

    //letter t
    alphabet[19].letter = 'T';
    alphabet[19].morseCode[0] = '-';
    alphabet[19].morseCode[1] = '\0';

    //letter u
    alphabet[20].letter = 'U';
    alphabet[20].morseCode[0] = '.';
    alphabet[20].morseCode[1] = '.';
    alphabet[20].morseCode[2] = '-';
    alphabet[20].morseCode[3] = '\0';

    //letter v
    alphabet[21].letter = 'V';
    alphabet[21].morseCode[0] = '.';
    alphabet[21].morseCode[1] = '.';
    alphabet[21].morseCode[2] = '.';
    alphabet[21].morseCode[3] = '-';
    alphabet[21].morseCode[4] = '\0';

    //letter w
    alphabet[22].letter = 'W';
    alphabet[22].morseCode[0] = '.';
    alphabet[22].morseCode[1] = '-';
    alphabet[22].morseCode[2] = '-';
    alphabet[22].morseCode[3] = '\0';

    //letter x
    alphabet[23].letter = 'X';
    alphabet[23].morseCode[0] = '-';
    alphabet[23].morseCode[1] = '.';
    alphabet[23].morseCode[2] = '.';
    alphabet[23].morseCode[3] = '-';
    alphabet[23].morseCode[4] = '\0';

    //letter y
    alphabet[24].letter = 'Y';
    alphabet[24].morseCode[0] = '-';
    alphabet[24].morseCode[1] = '.';
    alphabet[24].morseCode[2] = '-';
    alphabet[24].morseCode[3] = '-';
    alphabet[24].morseCode[4] = '\0';

    //letter z
    alphabet[25].letter = 'Z';
    alphabet[25].morseCode[0] = '-';
    alphabet[25].morseCode[1] = '-';
    alphabet[25].morseCode[2] = '.';
    alphabet[25].morseCode[3] = '.';
    alphabet[25].morseCode[4] = '\0';

    //for each character in the user input buffer
    for (int i = 0; i < userInputLength; i++) {
        //for each letter in the alphabet
        for (int j = 0; j <= 26; j++) {
            //compare char using ASCII ranges for capital characters
            if (!(userInput[i] >= 65 && userInput[i] <= 90)) { 
                //skip the for-loop for efficiency, this is not a valid letter
                break; //exit this loop
            } //END IF
            //if the current letter equals this Morse code letter
            if (userInput[i] == alphabet[j].letter) { 
                 //print the Morse code representation of the letter
                printf("%s", alphabet[j].morseCode);
                break; //exit this loop
            } //END IF
        } //END FOR

        /*compare char using ASCII ranges for capital characters
        //if this is a non-alphabetic character, excluding spaces and new lines, the print a ?*/
        if (!(userInput[i] >= 65 && userInput[i] <= 90) && userInput[i] != ' ' && userInput[i] != '\n') { 
            printf("?");
        } //END IF

        if (userInput[i] == ' ') { //if the current char is a space
            printf("   "); //add a triple space
        }
        //if this is not the last char, check to see if the next one isn't a space, if its not, then print a space
        //this allows for space buffers to exist between characters
        //this prevents extra spaces from being added
        else if (i+1 != userInputLength && userInput[i+1] != ' ') {
            printf(" "); //add a single space
        } //END IF
    } //END FOR
    printf("\n");

} //END main



