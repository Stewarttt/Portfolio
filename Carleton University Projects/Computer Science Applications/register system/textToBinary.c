//imported headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Name: formatedInformation
Discription:
    contains the formated information for a single record from the text file
    if stores in a dynammically allocated array, this can be used to represent all the records
*/
typedef struct { 
    unsigned char id;
    char month;
    char dayOfWeek;
    //W, P, S, or F based on 1,2,3,4 in the text file respectively
    char season;
    unsigned char age;
    //HS, GR, PG, or MD based on weather the text file has the number 1,2,3,4 respectively
    char education[3];
    unsigned char numberOfChildren;
    char socialDrinker; //Y if 1, N if 0
    char socialSmoker; //Y if 1, N if 0
    unsigned char numberOfPets;
    unsigned char bodyMassIndex;
    unsigned char hoursAbsent;
} formatedInformation;

/*
Name: main
Input:
    None
Output:
    bin file representation of absenteeism.txt
Discription:
    takes the number of records from absenteeism.txt and records it to the top of a bin file with the
        same name.
    formates information from absenteeism.txt and writes it into a new bin file with the same name.
*/
int main() {
    FILE * fileInputID; //stores the file pointer
    FILE * fileOutputID; //stores the file pointer

    unsigned short int sizeOfFile; //stores the number of records in the file
    char buffer[355]; //stores the current line of text
    formatedInformation formatedInfoArray;
    int line = 0; //the currently iterated line of the input file
    int word = 0; //the currently iterated word of a line in the input file
    int index = 0; //the currently iterated letter within a word of a line from the input file
    char *** currentWord; //pointer to a 2D array containing a line of text from the input file

    fileInputID = fopen("absenteeism.txt", "r"); //open the file

    if (fileInputID == NULL) { //check if the file was opened correctly
        printf("ERROR: Could not open file\n"); //PROMPT user
        exit(1); //exit the application
    } //END IF

    remove("absenteeism.bin");

    fileOutputID = fopen("absenteeism.bin","ab+");

    if (fileOutputID == NULL) { //open the file
        printf("ERROR: Could not make bin file\n"); //PROMPT user
        exit(1); //exit the application
    } //END IF

    //ignores the first line of the file, which contains titles, not records
    //removes off by 1, as fgets reads one more line then is needed
    sizeOfFile = -2;

    //count the number of lines within the file
    while (fgets(buffer, 255, fileInputID) != NULL) { //run untill the end of the lines reached
        sizeOfFile++; //iterate by one for each line in the file read
    } //END WHILE
    

    memset(buffer, 0, 255); //clear the current line of text in the buffer
    fseek(fileInputID, 0, SEEK_SET); //move back to the start of the file

    currentWord = calloc(sizeOfFile, sizeof(char**)); //holds a pointer to the word array

    for (int i = 0; i < sizeOfFile; i++) {
        currentWord[i] = calloc(21, sizeof(char*)); //for each peace of input in each line
        for (int j = 0; j < 21; j++) {
            currentWord[i][j] = calloc(10, sizeof(char)); //for each character per entry
        } //END FOR
    } //END FOR

    while (fgets(buffer, 355, fileInputID) != NULL) { //run until the end of the lines reached
        if (line == 0) { //if this is the first line in the text file
            line++; //iterate to the next line
            memset(buffer, 0, 355); //clear the buffer
            continue; //skips the first line, which is only titles
        } //END IF
        for (int i = 0; i < strlen(buffer); i++) {
            if (buffer[i] == ';') {
                word++; //iterate to the next peace of data on the line
                currentWord[line-1][word][index] = '\0'; //terminate the end of the current data point
                index = 0; //reset the current character for the next data entry iterated over
            }
            else if (buffer[i] == '\n' || buffer[i] == '\0') {
                memset(buffer, 0, 355); //clear the buffers memory
                word = 0; //when reading a new line, set the data entry to the first
                index = 0; //reset the current character to iterate over the next data entry
                line++; //move to the next line
                break; //exit the inner for loop
            }
            else {
                //add a character to this specific word
                currentWord[line-1][word][index++] = buffer[i];
            } //END IF
        } //END FOR
    } //END WHILE

    memset(buffer, 0, 355); //clear the buffer
    index = 0; //reset the index variable

    /*write to the binary file a short integer representing the total number of records in the 
    database*/
    fwrite(&sizeOfFile, sizeof(unsigned short int), 1, fileOutputID);

    for (int i = 0; i < sizeOfFile; i++) {
        if (currentWord[i][0] == NULL) { //if this word is not yet initialized
            continue; //move to the next iteration in the for loop
        } //END IF
        for (int j = 0; j < 21; j++) { //for each data point in a line from the text file            
            for (int k = 0; k < 10; k++) { //for each character in a data point
                if (currentWord[i][j][k] == '\0') { //if were at the end of the data string
                    buffer[k] = currentWord[i][j][k]; //add the terminator to the buffer
                    break; //exit the inner loop
                } //END IF
                buffer[k] = currentWord[i][j][k]; //add the current character to the buffer
            } //END FOR
            if (j == 0) { //if this is the first iteration of the inner for loop
                formatedInfoArray.id = (unsigned int) atoi(buffer); //set the id value
            }
            else if (j == 2) { //if this is the second iteration
                formatedInfoArray.month = atoi(buffer); //set the month value
            }
            else if (j == 3) { //third iteration...
                formatedInfoArray.dayOfWeek = atoi(buffer); //set the day of week value
            }
            else if (j == 4) { //fourth
                if (buffer[0] == '1') { //winter month
                    formatedInfoArray.season = 'W';
                }
                else if (buffer[0] == '2') { //spring month
                    formatedInfoArray.season = 'P';
                }
                else if (buffer[0] == '3') { //summer month
                    formatedInfoArray.season = 'S';
                }
                else if (buffer[0] == '4') { //fall month
                    formatedInfoArray.season = 'F';
                }
                else { //error entry
                    printf("ERROR: season number not in the range of 1 to 4.\n"); //PROMPT user
                } //END IF
            }
            else if (j == 8) { //set the age
                formatedInfoArray.age = atoi(buffer);
            }
            else if (j == 12) { //set the education level
                if (buffer[0] == '1') { //highschool education
                    formatedInfoArray.education[0] = 'H';
                    formatedInfoArray.education[1] = 'S';
                    formatedInfoArray.education[2] = '\0';
                }
                else if (buffer[0] == '2') { //undergraduate education
                    formatedInfoArray.education[0] = 'G';
                    formatedInfoArray.education[1] = 'R';
                    formatedInfoArray.education[2] = '\0';
                }
                else if (buffer[0] == '3') { //postgraduate education
                    formatedInfoArray.education[0] = 'P';
                    formatedInfoArray.education[1] = 'G';
                    formatedInfoArray.education[2] = '\0';
                }
                else if (buffer[0] == '4') { //masters or doctorate education
                    formatedInfoArray.education[0] = 'M';
                    formatedInfoArray.education[1] = 'D';
                    formatedInfoArray.education[2] = '\0';
                }
                else { //error entry
                    printf("ERROR: education number not in the range of 1 to 4.\n");
                } //END IF
            }
            else if (j == 13) { //set the number of children
                formatedInfoArray.numberOfChildren = atoi(buffer);
            }
            else if (j == 14) { //set the social drinker value
                if (buffer[0] == '0') { //no
                    formatedInfoArray.socialDrinker = 'N';
                }
                else if (buffer[0] == '1') { //yes
                    formatedInfoArray.socialDrinker = 'Y';
                }
                else { //error entry
                    printf("ERROR: social drinker number not in the range of 0 to 1.\n");
                } //END IF
            }
            else if (j == 15) { //set the social smoker value
                if (buffer[0] == '0') { //no
                    formatedInfoArray.socialSmoker = 'N';
                }
                else if (buffer[0] == '1') { //yes
                    formatedInfoArray.socialSmoker = 'Y';
                }
                else { //error entry
                    printf("ERROR: social smoker number not in the range of 0 to 1.\n");
                } //END IF
            }
            else if (j == 16) { //set the number of pets value
                formatedInfoArray.numberOfPets = atoi(buffer);
            }
            else if (j == 19) { //set the body mass index value
                formatedInfoArray.bodyMassIndex = atoi(buffer);
            }
            else if (j == 20) { //set the hours absent value
                formatedInfoArray.hoursAbsent = atoi(buffer);
            } //END IF
            
            memset(buffer, 0, 355); //clear the buffer
            index++; //iterate the current index
        } //END FOR
        //write the information into the bin file using the struct
        fwrite(&formatedInfoArray, sizeof(formatedInfoArray), 1, fileOutputID);
    } //END FOR

    for (int i = 0; i < sizeOfFile; i++) { //run for the number of lines in the file
        for (int j = 0; j < 21; j++) { //run for the number of data entry points per line
            if (currentWord[i][j] == NULL) { //if the current data point is not initialized
                continue; //move to the next for loop iteration
            } //END IF
            free(currentWord[i][j]); //free the allocated memory
        } //END FOR
        free(currentWord[i]); //free the allocated memory
    } //END FOR
    free(currentWord); //free the allocated memory

    fclose(fileInputID); //close the file
    fclose(fileOutputID); //close the file
} //END main
