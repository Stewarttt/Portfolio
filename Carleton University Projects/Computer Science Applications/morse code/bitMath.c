/*
Name: bitMath.c
Author: Connor Raymond Stewart
ID: 101041125
cmail: connorrstewart@cmail.carleton.ca
Input:
    two strings from user which represent two's compliment numbers
Output:
    the sum and value of the numbers in both binary and decimal representation
Description:
    converts two two's compliment numbers to a single twos compliment number
    outputs the value of all twos compliment numbers, reports overflows if applicable
*/

//imported headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Name: flipNegative
Input: 
    char * binary - this is the char array representation of the 8-bit two's compliment number
Output:
    int - this number represents the sign value (positive or negative) of the flipped twos compliment
    char * binary - this array is modified to have 1's and 0's flipped, and a single bit added
Description:
    This method flips the two's compliment, and adds one bit. This allows negative two's compliment
    numbers to be presented in magnitude form. Returning the sign value allows the identification of,
    and reversal of this operation in the future.
*/
int flipNegative(char * binary) {
    int signValue = 1; //represents positive or negative number
    //find if the number is a negative
    if (binary[0] == '1') {
        signValue = -1; //this number is negative
        //find the magnitude of the negative number
        for (int i = 0; i < strlen(binary); i++) {
            if (binary[i] == '0') { //if the current digit is a zero
                binary[i] = '1'; //flip to one
            }
            else { //current digit is a one
                binary[i] = '0'; //flip to zero
            } //END IF
        } //END FOR
        binary[strlen(binary)-1] += 1; //adding a value converts 0 to 1 and 1 to 2
        //starting from the first binary digit, count all eight binary digits
        for (int i = strlen(binary)-1; i >= 0; i--) {
            if (binary[i] == '2') { //if the number overflows
                binary[i] = '0'; //set to zero
                binary[i-1] += 1; //increase the next number by one
            } //END IF
        } //END FOR
    } //END IF
    return signValue;
} //END flipNegative

/*
Name: binaryToDecimal
Input: 
    const char * inputSequence - the char array representation of the users specified two's
    complement number
Output:
    int - the base ten representation of the inputted two's compliment number
Description:
    Method converts both positive and negative two's compliment values (in the form of char arrays)
    into base ten (decimal) numbers. Both magnitude and sign (positive or negative) are returned.
*/
int binaryToDecimal (const char * inputSequence) {
    int decimalSum = 0; //the value of the binary string in base ten
    int signValue; //represents positive or negative number
    int power = 2; //the current power of two in the buffer
    char binary[255];
    strcpy(binary, inputSequence);
    
    /*evaluates if the bits should be flipped, returns the sign value of the
    twos compliment decimal*/
    signValue = flipNegative(binary);
    
    //for the total number of elements in the binary string
    for (int i = 0; i < strlen(binary); i++) {
        //the decimal sum equals the number multiplied by its base two value
        //for example: 10001 = 2^4+1^0
        if (binary[i] == '1') { //if this digit contains a one
            if (strlen(binary)-i-1 == 0) { //if this is the first binary digit
                decimalSum += 1; //add one
            }
            else { //if the binary digit is not the first
                //run for total number of powers of 2 for this binary digit
                for (int j = 0; j < strlen(binary)-i-2; j++) {
                    power *= 2; //finds the current power of two for this binary digit
                } //END FOR
                decimalSum += power; //this power of two represents the decimal value
                power = 2; //reset the power of two buffer
            } //END FOR            
        } //END FOR        
    } //END FOR
    return decimalSum * signValue; //return the final number generated
} //END binaryStringToDecimal

/*
Name: printAsBinary
Input: 
    char bin - binary representation of the twos complement number in the form of a 8-bit char
    char * returnSequence - this is initially an empty, 9 slot char array
    int signValue - represents if the outputted array is to be positive or negative
Output:
    char * returnSequence - contains the char array representation of bin, a two's compliment number
Description:
    This method converts a twos compliment 8-bit char into a binary representation stored within a 
    9 element char array. The array contains 1's and 0's corresponding to the 8-bit char value, and
    contains one extra slot for the null terminator
*/
void printAsBinary(char bin, char * returnSequence, int signValue) {
    int currentPower = 64; //this equals the value of 2^6
    returnSequence[0] = '0'; //the last digit is zero, meaning this represents magnitude
    for (int i = 1; i < 8; i++) { //run for all but the last digit
        if (((bin) - (currentPower)) >= 0) { //a power of two can be subtracted from the total
            returnSequence[i] = '1'; //place a one at index i to represent this power of two
            bin -= currentPower; //subtract this power of two from the total binary sum
        }
        else {
            returnSequence[i] = '0'; //place a zero, as this power of two cannot be subtracted
        } //END IF
        currentPower /= 2; //halve the current power of two
    } //END FOR
    returnSequence[8] = '\0'; //place the null terminator

    if (signValue  == -1) { //if this is a negative two's complement representation
        for (int i = 0; i < 8; i++) { //for all digits within the 8-character char array
            /*subtract the current char by the char value of 0, converting a '0' to 0 and 
            a '1' to 1, then use the logical NOT operator to invert the value of 0 to 1 or
            1 to 0, and finally add the char value of '0' to convert the total value back to a
            0 (if were adding to integer 0) or 1 (if were adding to integer 1)*/
            returnSequence[i] = !(returnSequence[i] - '0') + '0';
        } //END FOR
        returnSequence[7] += 1; //add extra digit after flipping
    } //END IF

    for (int i = strlen(returnSequence)-1; i >= 0; i--) { //for the number of digits in the string
        if (returnSequence[i] == '2') { //if the number overflows
            returnSequence[i] = '0'; //set to zero
            returnSequence[i-1] += 1; //increase the next number by one
        } //END IF
    } //END FOR
    
} //END printAsBinary


int outputBitSequence(char * userInputOne, char * userInputTwo) {
    //base ten representation of these two's complement char array
    int inputOneDecimalValue = binaryToDecimal(userInputOne);
    int inputTwoDecimalValue = binaryToDecimal(userInputTwo);
    //the sum of both the two's complement values added up together
    int sum = inputOneDecimalValue + inputTwoDecimalValue;
    //this is the char container for the 8-bit sequence
    char binaryRepresentation;
    //this is the sequence containing the 8-bit sequence in char array format
    char binarySequence[9];

    //PROMPT user of summation results
    printf("\n  %s (%d)\n", userInputOne, inputOneDecimalValue);
    printf("  %s (%d)\n", userInputTwo, inputTwoDecimalValue);
    printf("----------------\n");

    //if an overflow occurs (out of range of two's compliment)
    if (sum < -128 || sum > 127) {
        printf("OVERFLOW DETECTED\n"); //PROMPT user
        return 0; //EXIT procedure
    } //END IF
    binaryRepresentation = sum; //set the binary char to equal the literal sum

    if (binaryRepresentation < 0) { //number is negative
        if (binaryRepresentation == -128) { //this is a special case, the inverse is the same value
            printf("  %s (%d)\n", "10000000", binaryRepresentation); //print out the expected value
            return 1; //RETURN 1 for success
        } //END IF

        //use binary NOT to flip, add one bit
        binaryRepresentation = (~binaryRepresentation)+1; 
        //get a char array representation for output
        printAsBinary(binaryRepresentation, binarySequence, -1); 
        //PRINT char array representation
        printf("  %s (%d)\n", binarySequence, -binaryRepresentation);
    }
    else if (binaryRepresentation == 0) { //exception case
        //PRINT char array representation
        printf("  %s (%d)\n", "00000000", -binaryRepresentation); 
    }
    else { //number is not negative
        //get a char array representation for output
        printAsBinary(binaryRepresentation, binarySequence, 1); 
        //PRINT char array representation
        printf("  %s (%d)\n", binarySequence, binaryRepresentation);
    } //END IF
    return 1; //RETURN 1 for success
} //END outputBitSequence

/*
Name: main
Input: 
    Two strings representing a two integers in two's compliment representation
Output:
    Base 10 value of the inputted two's compliment numbers
    number representing the sum of the twos compliment sum
    overflow detected if applicable to the twos compliment sum
Description:
    gets two binary digits from the user, entered in two's compliment representation
    converts these values to base 10 and outputs their base ten representation
    calculates and outputs the sum of the two's compliment summation of the two numbers
        outputs both the binary and base ten number
        prints overflow detected if the value of the sum is outside of the range of 
        the two's compliment 8-bit representation
*/
int main() {
    char userInputOne[255]; //buffer for the users first input
    char userInputTwo[255]; //buffer for the users second input
    int exitQueue; //determines if the program should exit due to an error

    //PROMPT user to enter a bit sequence
    printf("Enter 1st 8-bit sequence: ");
    //GET user input
    scanf("%s", userInputOne);

    while (getchar() != '\n') {} //clear the carriage return

    //PROMPT user to enter a bit sequence
    printf("Enter 2nd 8-bit sequence: ");
    //GET user input
    scanf("%s", userInputTwo);

    if (strlen(userInputOne) == 8) { //if the string is 8 digits long
        for (int i = 0; i < 8; i++) { //for the number of digits in the 8 digit string
            if (!(userInputOne[i] == '1' || userInputOne[i] == '0' || 
                //check for valid binary characters
                userInputOne[i] == ' ' || userInputOne[i] == '\n')) { 
                printf("Error: Bit sequence 1 has non-binary characters\n"); //PRMPT user
                exitQueue = 1; //exit after next logic block
                break; //leave loop for efficiency
            } //END IF
        } //END FOR
    }
    else {
        printf("Error: You must enter two valid 8-bit string sequences\n"); //PROMPT user
        return 0; //EXIT the program
    } //END IF

    if (strlen(userInputTwo) == 8) { //if the string is 8 digits long
        for (int i = 0; i < 8; i++) { //for the number of digits in the 8 digit string
            if (!(userInputTwo[i] == '1' || userInputTwo[i] == '0' ||
                //check for valid binary characters
                userInputTwo[i] == ' ' || userInputTwo[i] == '\n')) { 
                printf("Error: Bit sequence 2 has non-binary characters\n"); //PRMPT user
                exitQueue = 1; //exit when this block is done
                break; //leave loop for efficiency
            } //END IF
        } //END FOR
    }
    else {
        printf("Error: You must enter two valid 8-bit string sequences\n"); //PROMPT user
        return 0; //EXIT the program
    } //END IF
                
    if (exitQueue == 1) {
        return 0; //EXIT the program
    } //END IF
   
    //outputs the results to the user
    outputBitSequence(userInputOne, userInputTwo);

    return 1; //RETURN 1 for success
} //END main
