/*
Name: bitMath.c
Author: Connor Raymond Stewart
ID: 101041125
cmail: connorrstewart@cmail.carleton.ca
Name: reverseMorseCode.c
Input: 
    userInput - string to be converted to Latin letters
Output: 
    Latin representation of the Morse code
Description:
    a simple program which converts a input string into Latin letters
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
    userInput - user input representing a string to be converted to Latin letters
Output:
    Latin letter representation of userInput
Description:
    Each letter of the alphabet is initialized in a letterToMorseCode struct
    each struct is also initialized with is Morse code representation for each letter
    the program loops through the users inputted string, and matches each letter 
    with its corresponding more code value.
    the program checks the if the input string contains only spaces, line breaks,
    periods, dashes, and question marks. If the string contains invalid characters,
    it prompts the user to enter a new Morse code string.
    The program iterates over the users string and matches each sequence of .'s and -'s 
    to a Morse code letter. Each space represents a new letter, and three spaces represents a
    new word. If the user enters an invalid Morse code letter, the program will prompt the user
    of the incorrect sub-string and prompt to enter a new Morse code string.
*/
int main() {
    //declarations
    struct letterToMorseCode alphabet[26]; //contains one struct for each letter
    char userInput[255]; //buffer for user input
    int userInputLength; //variable to contain the length of the users input
    char morseCodeBuffer[5]; // a buffer containing a Morse code letter
    int correctInput = 0; //represents if the users input is correct Morse code
    int found = 0; //represents if a Morse code string matched this current sub-string
    int spaceNumber = 0; //represents the number of spaces found between two Morse code digits
    int morseDigitCounter = 0; //the number of Morse code digits currently held in the buffer   

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

    //get the users input
    printf("Enter the Morse Code sequence:\n"); //PROMPT user
    fgets(userInput, 255, stdin); //get the users input up to 255 characters

    //find the length of the users input
    userInputLength = strlen(userInput);

    //convert all letters in the users input to upper-case letters
    for (int i = 0; i < userInputLength; i++) {
        userInput[i] = toupper(userInput[i]); //enter the upper-case value for the letter
    } //END FOR

    //PROMPT user
    printf("Here is the decoded message:\n");
    
    //for each character in the user input buffer
    for (int i = 0; i < userInputLength; i++) {             
        found = 0; //reset this to false
        if (userInput[i] == ' ') { //if the current character is a space
            spaceNumber++; //count this space
        }
        else { //if no space is found, we have made it to a new word
            if (spaceNumber > 3) {
                printf("\nError, more then consecutive 3 spaces in string\n"); //PROMPT user of error
                return 0; //EXIT the program
            }
            else if (spaceNumber == 2) {
                printf("\nError, having a two space sequence is invalid"); //PROMPT user of error
                return 0; //EXIT the program
            }
            else if (spaceNumber == 3) {
                printf(" "); //PRINT space
            } //END IF
            spaceNumber = 0; //reset this value to zero
        } //END IF
        /*incorrect Morse code digit sequence entered, no Morse code digit is longer
        then 4 units*/
        if (morseDigitCounter > 4) {
            //PROMPT user of error
            printf("\nError, the next Morse code letter is longer then four units (. or -'s)\n");
            return 0; //EXIT the program
        }
        /*if the current index is a space, or is the last element in the enter user input string
        and if at least one Morse digit has been added to the buffer
        then we have a Morse code phrase to evaluate*/
        else if ((userInput[i] == ' ' || i+1 == userInputLength) && morseDigitCounter != 0) {  
            for (int j = 0; j < 26; j++) { //for the number of letters in the alphabet
                //for the number of Morse code digits in the buffer
                for (int k = 0; k < morseDigitCounter; k++) {
                    //check of the Morse code digit matches this current Morse code letter
                    if (!(alphabet[j].morseCode[k] == morseCodeBuffer[k])) {
                        break; //EXIT the loop, this cannot be the correct Morse code digit
                    }
                    /*if the Morse code letter matches, and this is the last Morse code
                    letter in the buffer, and if the current index being evaluated is 
                    equivalent to the size of the Morse code buffer then we have both 
                    reached the end of the Morse code buffer and have successfully 
                    matched every digit in the buffer with this Morse code letter.
                    Therefore, this is the correct Morse code letter*/
                    else if (alphabet[j].morseCode[k] == morseCodeBuffer[k]
                    && k+1 == morseDigitCounter 
                    && strlen(alphabet[j].morseCode) == morseDigitCounter ) {
                        //PRINT the Latin representation of the Morse code letter
                        printf("%c", alphabet[j].letter); 
                        correctInput = 1; //set this value to true
                        found = 1; //set this to true
                        break; //EXIT the loop, we have found the letter
                    } //END IF
                } //END FOR
            } //END FOR
            if (!found && morseCodeBuffer[0] != '?') {
                /*if we have found an input, then we need a new space to prevent
                the outputted Morse code bring printed on the same line as the error message*/
                if (correctInput) {
                    printf("\n"); //PRINT a new line
                } //END IF
                //PROMPT user of error
                printf("Error, %s is not a Morse code letter\n", morseCodeBuffer);
                return 0; //EXIT the program
            } //END IF
            if (morseCodeBuffer[0] == '?') { //if the buffer contains a ? symbol
                printf("?"); //PRINT ? symbol
            } //END IF
            /*clear the Morse code buffer for this current letter
            buffer is cleared by setting all bytes in the array to the null terminator '\0'*/
            memset(morseCodeBuffer, '\0', sizeof(char)*strlen(morseCodeBuffer));
            morseDigitCounter = 0; //reset this to zero, we have cleared the buffer
        }
        else if (userInput[i] != ' ') { //if there is no space, then this is a Morse digit
            morseCodeBuffer[morseDigitCounter++] = userInput[i]; //add digit to buffer
        } //END IF
    } //END FOR
    printf("\n"); //PRINT a final space so the Terminal window PROMPTs on a new line
} //END main
