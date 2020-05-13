#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Name: PersonalData
Description:
    contains information about a person in the database
*/
typedef struct {
    unsigned char      id;
    unsigned char      age;
    char               education[3];
    unsigned char      numChildren;
    char               socialDrinker;
    char               socialSmoker;
    unsigned char      numPets;
    unsigned char      bodyMassIndex;
    unsigned short int totalHoursAbsent;
} PersonalData;

/*
Name: AbsentEpisode
Description:
    Contains information reguarding an employee absence episode
*/
typedef struct {
    unsigned char      id;
    char               month;
    char               dayOfWeek;
    char               season;
    unsigned short int hoursAbsent;
} AbsentEpisode;

/*
Name: inputFormat
Description:
    Used to read information in from the absenteeism.txt
*/
typedef struct { 
    unsigned char   id;
    char            month;
    char            dayOfWeek;
    //W, P, S, or F based on 1,2,3,4 in the text file respectively
    char            season;
    unsigned char   age;
    //HS, GR, PG, or MD based on weather the text file has the number 1,2,3,4 respectively
    char            education[3];
    unsigned char   numberOfChildren;
    char            socialDrinker; //Y if 1, N if 0
    char            socialSmoker; //Y if 1, N if 0
    unsigned char   numberOfPets;
    unsigned char   bodyMassIndex;
    unsigned char   hoursAbsent;
} inputFormat; //this struct represents the way input information is formated for input

/*
Name: main
Input:
    None
Output:
    String representation in a table showing information from the bin file
Description:
    Reads in information from the bin file
    Formats the information into two separate structs
    Calculates sums based on employee absence hours
    Displays (to the shell) information reguarding employees health in table representation
*/
int main() {
    FILE * fileInputID; //stores the file pointer
    fileInputID = fopen("absenteeism.bin", "rb"); //open the file in binary mode for reading
    int records; //represents the number of records in the file
    inputFormat inputBuffer; //holds the raw information for a line of information read
    unsigned char buffer[36][2]; //holds into a list of user id's to check for duplicates
    int exclusionList[36]; //list of indexes to exclude from the output table
    int found = -1; //index of the duplicate person entery
    int numberOfPeople = 0; //the number of people found in the database so far
    int totalHoursAbsent = 0; //the total hours absent of all employees summed together
    int absentEpisodeNumber = 0; //counts the current absent episode
    int nextIndex = 0; //next index of an array to add somthing to
    int largestTimeFound = -1; //largest amount of hours absent for an employee
    int isExcluded = 0; //if this employees information has already been printed
    int weeklyAbsence[7]; //total amount of time employees have been absent per day of week
    int seasonalAbsence[4]; //total amount of time employees have been absent per season
    int monthlyAbsence[12]; //total amount of time employees have been absent per month
    char bmiRange[12]; //string representation of an emplplyees BMI

    memset(exclusionList, -1, 36*sizeof(int)); //clears the exclution list

    if (fileInputID == NULL) { //if the bin file does not exist
        printf("ERROR: absenteeism.bin does not exist, exiting\n"); //PROMPT user
        exit(1); //exit the application
    } //END IF

    for (int i = 0; i < 36; i++) {
        memset(buffer[i], 0, 2); //clears the buffer
        buffer[i][1] = '\0'; //null terminates the buffer
    } //END FOR

    PersonalData * personalDataPointers[36]; //points to 36 employee records
    for (int i = 0; i < 36; i++) {
        personalDataPointers[i] = calloc(1, sizeof(PersonalData)); //allocate memory for each employee
    } //END FOR

    AbsentEpisode * absentEpisodeArray[1000]; //contains room for 1000 absence records

    /*reads in the first unsigned short int of memory from the file, this line, this contains
    the total number of records in the file*/
    fread(&records, sizeof(unsigned short int), 1, fileInputID);

    if (records > 1000) { //PROMPT user if there are more then the specified number of records
        printf("ERROR: more then 1000 records\n");
        printf("The array only needs to hold up to 1000 pointers (assignment requirments\n");
        printf("Program is now exiting\n");
        exit(1); //exit the program
    } //END IF

    for (int i = 0; i < records; i++) {
        absentEpisodeArray[i] = calloc(1, sizeof(AbsentEpisode)); //allocate memory for each record
    } //END FOR

    for (int i = 0; i < records; i++) {
        found = -1; //false
        fread(&inputBuffer, sizeof(inputFormat), 1, fileInputID); //read in a record of information

        //set the current absent episodes information
        absentEpisodeArray[absentEpisodeNumber]->id = inputBuffer.id; //set the employee id
        absentEpisodeArray[absentEpisodeNumber]->month = inputBuffer.month; //set the current month
        absentEpisodeArray[absentEpisodeNumber]->dayOfWeek = inputBuffer.dayOfWeek; //set the day
        absentEpisodeArray[absentEpisodeNumber]->season = inputBuffer.season; //set the season
        absentEpisodeArray[absentEpisodeNumber++]->hoursAbsent = inputBuffer.hoursAbsent; //set hours

        for (int j = 0; j < 36; j++) { //for the number of employees
            if (buffer[j][0] == inputBuffer.id) { //if this employee has already been found
                found = j; //take this employees index
            } //END IF
        } //END FOR
        if (found != -1) { //if the employee already exists in the struct database
            personalDataPointers[found]->id = inputBuffer.id; //set id
            personalDataPointers[found]->age = inputBuffer.age; //set age
            //set education
            personalDataPointers[found]->education[0] = inputBuffer.education[0];
            personalDataPointers[found]->education[1] = inputBuffer.education[1];
            personalDataPointers[found]->education[2] = inputBuffer.education[2];

            //set number children
            personalDataPointers[found]->numChildren = inputBuffer.numberOfChildren;
            //set social drinker
            personalDataPointers[found]->socialDrinker = inputBuffer.socialDrinker;
            //set social smoker
            personalDataPointers[found]->socialSmoker = inputBuffer.socialSmoker;
            //set number of pets
            personalDataPointers[found]->numPets = inputBuffer.numberOfPets;
            //set the BMI value
            personalDataPointers[found]->bodyMassIndex = inputBuffer.bodyMassIndex;
            //set the total number of hours absent
            personalDataPointers[found]->totalHoursAbsent += inputBuffer.hoursAbsent;
        }
        else { //if the user is not yet added to the struct database, add it, and fill this record
            buffer[numberOfPeople][0] = inputBuffer.id; //tracks this users id
            buffer[numberOfPeople][1] = '\0'; //terminate the string
            personalDataPointers[numberOfPeople]->id = inputBuffer.id; //set the id
            personalDataPointers[numberOfPeople]->age = inputBuffer.age; //set the age
            //set the education
            personalDataPointers[numberOfPeople]->education[0] = inputBuffer.education[0];
            personalDataPointers[numberOfPeople]->education[1] = inputBuffer.education[1];
            personalDataPointers[numberOfPeople]->education[2] = inputBuffer.education[2];
            //set the number of children
            personalDataPointers[numberOfPeople]->numChildren = inputBuffer.numberOfChildren;
            //set the number of children
            personalDataPointers[numberOfPeople]->socialDrinker = inputBuffer.socialDrinker;
            //set the social smoker value
            personalDataPointers[numberOfPeople]->socialSmoker = inputBuffer.socialSmoker;
            //set the social drinker value
            personalDataPointers[numberOfPeople]->numPets = inputBuffer.numberOfPets;
            //set the BMI value
            personalDataPointers[numberOfPeople]->bodyMassIndex = inputBuffer.bodyMassIndex;
            //set the hours absent value
            personalDataPointers[numberOfPeople]->totalHoursAbsent += inputBuffer.hoursAbsent;
            numberOfPeople++; //increment the number of people found by one
        } //END IF
        totalHoursAbsent += inputBuffer.hoursAbsent; //count the total number of hours absent

        //count the total number of hours absent by season
        if (inputBuffer.season == 'W') { //winter
            seasonalAbsence[0] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.season == 'P') { //spring
            seasonalAbsence[1] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.season == 'S') { //summer
            seasonalAbsence[2] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.season == 'F') { //fall
            seasonalAbsence[3] += inputBuffer.hoursAbsent; //count hours absent
        } //END IF

        //counts the total number of hours absent by day of the week
        if (inputBuffer.dayOfWeek == 0) { //sunday
            weeklyAbsence[0] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.dayOfWeek == 1) { //monday
            weeklyAbsence[1] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.dayOfWeek == 2) { //tuesday
            weeklyAbsence[2] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.dayOfWeek == 3) { //wendsday
            weeklyAbsence[3] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.dayOfWeek == 4) { //thursday
            weeklyAbsence[4] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.dayOfWeek == 5) { //friday
            weeklyAbsence[5] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.dayOfWeek == 6) { //saturday
            weeklyAbsence[6] += inputBuffer.hoursAbsent; //count hours absent
        } //END IF

        //count the total number of hours missed by the month
        if (inputBuffer.month == 1) { //jan
            monthlyAbsence[0] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 2) { //feb
            monthlyAbsence[1] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 3) { //mar
            monthlyAbsence[2] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 4) { //apr
            monthlyAbsence[3] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 5) { //may
            monthlyAbsence[4] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 6) { //jun
            monthlyAbsence[5] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 7) { //jul
            monthlyAbsence[6] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 8) { //aug
            monthlyAbsence[7] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 9) { //sep
            monthlyAbsence[8] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 10) { //oct
            monthlyAbsence[9] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 11) { //nov
            monthlyAbsence[10] += inputBuffer.hoursAbsent; //count hours absent
        }
        else if (inputBuffer.month == 12) { //dec
            monthlyAbsence[11] += inputBuffer.hoursAbsent; //count hours absent
        } //END IF
    } //END FOR

    printf("The file contains %d records\n\n", records); //PRINT total number of records
    //print table of employee information
    printf("Individual Employee Data:\n");
    printf("-------------------------\n\n");
    printf("ID   ABSENT   AGE   SMOKER   DRINKER   #CH   #PT   CATEGORY    EDU\n");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < numberOfPeople; i++) { //for the number of employees
        for (int j = 0; j < numberOfPeople; j++) { //for the number of employees
            isExcluded = 0; //set to false by default
            //find the employee who has missed the most hours
            if (personalDataPointers[j]->totalHoursAbsent >= largestTimeFound) {
                for (int k = 0; k < numberOfPeople; k++) { //for the number of employees
                    if (exclusionList[k] == j) { //if this employee has alrady been printed
                        isExcluded = 1; //dont print this again
                    } //END IF
                } //END FOR
                if (isExcluded == 1) { //if were to skip this record
                    continue; //move to the next for loop iteration
                } //END IF
                //set this record to be the largest hours missed record found so far
                largestTimeFound = personalDataPointers[j]->totalHoursAbsent;
                found = j; //record this records index
            } //END IF
        } //END FOR
        exclusionList[nextIndex++] = found; //remember to not print this record again
        largestTimeFound = -1; //reset this value to default
    } //END FOR
    for (int i = 0; i < numberOfPeople; i++) { //for the number of employees
        if (personalDataPointers[exclusionList[i]]->bodyMassIndex < 18.5) { //BMI value is underweight
            strcpy(bmiRange, "Underweight"); //set BMI output
        }
        else if (personalDataPointers[exclusionList[i]]->bodyMassIndex > 25 //BMI value overweight
                && personalDataPointers[exclusionList[i]]->bodyMassIndex <= 30) {
            strcpy(bmiRange, "Overweight"); //set BMI output
        }
        else if (personalDataPointers[exclusionList[i]]->bodyMassIndex > 30) { //BMI value Obese
            strcpy(bmiRange, "Obese"); //set BMI output
        }
        else { //BMI value standard
            strcpy(bmiRange, ""); //set BMI output
        } //END IF

        //print out the current record of information to the shell
        printf("%2d   %3d      %2d    %c        %c         %d     %d     %-12s%s\n",
            personalDataPointers[exclusionList[i]]->id,
            personalDataPointers[exclusionList[i]]->totalHoursAbsent,
            personalDataPointers[exclusionList[i]]->age,
            (personalDataPointers[exclusionList[i]]->socialSmoker == 'N') ? ' ' : 'Y',
            (personalDataPointers[exclusionList[i]]->socialDrinker == 'N') ? ' ' : 'Y',
            personalDataPointers[exclusionList[i]]->numChildren,
            personalDataPointers[exclusionList[i]]->numPets,
            bmiRange,
            personalDataPointers[exclusionList[i]]->education);
        memset(bmiRange, 0, 12); //clear the bmi output string
    } //END FOR

    //output the total hours absent of all employees combined
    printf("\nGrand Total Hours Absent: %d\n\n", totalHoursAbsent);

    //output the total hours absent by the day of the week
    printf("Hours absent based on weekday:\n");
    printf("------------------------------\n");
    printf("  Sat: %d\n", weeklyAbsence[0]);
    printf("  Sun: %d\n", weeklyAbsence[1]);
    printf("  Mon: %d\n", weeklyAbsence[2]);
    printf("  Tue: %d\n", weeklyAbsence[3]);
    printf("  Wed: %d\n", weeklyAbsence[4]);
    printf("  Thu: %d\n", weeklyAbsence[5]);
    printf("  Fri: %d\n\n", weeklyAbsence[6]);

    //output the total hours absent by the season
    printf("Hours absent based on season:\n");
    printf("-----------------------------\n");
    printf("  Winter: %d\n", seasonalAbsence[0]);
    printf("  Spring: %d\n", seasonalAbsence[1]);
    printf("  Summer: %d\n", seasonalAbsence[2]);
    printf("  Fall  : %d\n\n", seasonalAbsence[3]);

    //output the total hours absent by the month
    printf("Hours absent based on month:\n");
    printf("----------------------------\n");
    printf("  Jan: %d\n", monthlyAbsence[0]);
    printf("  Feb: %d\n", monthlyAbsence[1]);
    printf("  Mar: %d\n", monthlyAbsence[2]);
    printf("  Apr: %d\n", monthlyAbsence[3]);
    printf("  May: %d\n", monthlyAbsence[4]);
    printf("  Jun: %d\n", monthlyAbsence[5]);
    printf("  Jul: %d\n", monthlyAbsence[6]);
    printf("  Aug: %d\n", monthlyAbsence[7]);
    printf("  Sep: %d\n", monthlyAbsence[8]);
    printf("  Oct: %d\n", monthlyAbsence[9]);
    printf("  Nov: %d\n", monthlyAbsence[10]);
    printf("  Dec: %d\n", monthlyAbsence[11]);

    for (int i = 0; i < numberOfPeople; i++) { //for the total number of employees
        free(personalDataPointers[i]); //free the allocated memory
    } //END FOR
    for (int i = 0; i < records; i++) { //for the total number of employees
        free(absentEpisodeArray[i]); //free the allocated memory
    } //END FOR

    fclose(fileInputID); //close the opened binary file
} //END main
