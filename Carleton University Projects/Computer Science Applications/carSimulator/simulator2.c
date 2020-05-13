/*
Name: simulator2.c
Author: Connor Raymond Stewart
ID: 101041125
cmail: connorrstewart@cmail.carleton.ca
Input: 
    None
Output: 
    Test case results that are hard-coded in main
Description:
    simulates a simple parking lot
    keeps track of weather or not a car is in a lot at a given moment
    keeps track of how much time passes between a car entering and leaving a lot
    determines how much money the lot makes, based on how long cars are in the lot and the lots
        hourly rate
    keeps track of how many cars are in the lot, weather the lots full, and if a car has a permit to
        allow it to not pay a toll
    outputs the results of the hard-coded test cases in main to the shell
*/

//imported modules
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
Name:
    Time
Description: 
    stores an hour and a minute as an integer
*/
typedef struct {
    int hour;
    int minute;
} Time; //END Time

/*
Name:
    Car
Description: 
    stores a plate number for a car as a char pointer
    stores the permits existence as a char
    stores entering time as a time struct
    stores the lot number the car is parked in as an integer
*/
typedef struct {
    char * plateNumber;
    char permit;
    Time enteringTime;
    int lotParkedIn;
} Car; //END Car

/*
Name: 
    ParkingLot
Description:
    stores the lot number as an integer
    stores the lots hourly rate as a double
    stores the maximum toll charge as a double
    stores the car capacity as an integer
    stores the current car count as an integer
    stores the revenue as a double
*/
typedef struct {
    int lotNumber;
    double hourlyRate;
    double maxCharge;
    int capacity;
    int currentCarCount;
    double revenue;
} ParkingLot; //END ParkingLot

// Sets the hours and minutes amount for the given time t based 
// on the specified hours h.  (e.g., 1.25 hours would be 1 hour 
// and 15 minutes)
void setHours(Time *t, double h) {
    int truncatedTime = h; //the decimal portion is truncated
    h -= truncatedTime; //removes the hour portion of the time, leaving just the minutes
    h *= 60; //converts decimal hours to minutes
    t->minute = h;
    t->hour = truncatedTime; 
} //END setHours

// Takes two Time objects (not pointers) and computes the difference 
// in time from t1 to t2 and then stores that difference in the diff 
// Time (which must be a pointer)
void difference(Time t1, Time t2, Time *diff) {
    //time in minutes
    int timeOne = t1.hour*60 + t1.minute;
    int timeTwo = t2.hour*60 + t2.minute;
    int changeInTime = timeTwo - timeOne;
    //find the amount of time thats passed
    setHours(diff, changeInTime/60.0);
} //END difference


// Initialize the car pointed to by c to have the given plate and 
// hasPermit status.  The car should have it’s lotParkedIn set to 
// 0 and enteringTime to be -1 hours and -1 minutes.
void initializeCar(Car *c, char *plate, char hasPermit) {
    //initialize
    c->plateNumber = plate;
    c->permit = hasPermit;
    //set default
    c->lotParkedIn = 0;
    c->enteringTime.minute = -1;
    c->enteringTime.hour = -1;
} //END initializeCar


// Initialize the lot pointed to by p to have the given number, 
// capacity, hourly rate and max charge values.  The currentCarCount 
// and revenue should be at 0.
void initializeLot(ParkingLot *p, int num, int cap, double rate, double max) {
    //initialize
    p->lotNumber = num;
    p->capacity = cap;
    p->hourlyRate = rate;
    p->maxCharge = max;
    //set default
    p->currentCarCount = 0;
    p->revenue = 0;
} //END initializeLot

// Print out the parking lot parameters so that is displays as 
// follows:   Parking Lot #2 - rate = $3.00, capacity 6, current cars 5
void printLotInfo(ParkingLot p) {
    printf("Parking Lot #%d - rate = $%.2f, capacity %d, current cars %d\n",
        p.lotNumber, p.hourlyRate, p.capacity, p.currentCarCount);
} //END printLotInfo


// Simulate a car entering the parking lot
// ...

/*
Name:
    carEnters
Input:
    ParkingLot * lot - the parking lot being modified
    Car * car - the car being added to the lot
    int hour - the hour the car arrived to the parking lot
    int minute - the minute the car arrived to the parking lot
Output:
    ParkingLot * lot - the modified parking lot containing the car
    Car * car - the modified car containing the entering time
Description:
    takes a pointer to a parking lot struct, then modifies it using information from 
    an imputed car struct. the hour and minute the car is added are also imputed to the function
    and added to the cars information.
*/
void carEnters(ParkingLot * lot, Car * car, int hour, int minute) {
    if (car->lotParkedIn != 0) { //if the car is in another lot, intended as error PROMPT for user
        printf("Error, car %s is already parked in a lot\n", car->plateNumber); //PROMPT user
        return; //exit the function
    }
    else if (lot->currentCarCount + 1 > lot->capacity) { //if the lot is full
        printf("Car %s arrives at Lot %d at %d:%.2d, but the lot is full.\n", 
            car->plateNumber, lot->lotNumber, hour, minute); //PROMPT user
        printf("Car %s cannot get in.\n", car->plateNumber); //PROMPT user
        return; //exit the function
    } //END IF

    //PROMPT user
    printf("Car %s enters Lot %d at %d:%.2d.\n", car->plateNumber, lot->lotNumber, hour, minute);
    //initialize the time values
    car->enteringTime.hour = hour;
    car->enteringTime.minute = minute;
    //initialize the lot id
    car->lotParkedIn = lot->lotNumber;
    //increment the number of people in the parking lot
    lot->currentCarCount += 1;
} //END carEnters

// Simulate a car leaving the parking lot
// ...

/*
Name:
    carLeaves
Input:
    ParkingLot * lot - the parking lot being modified
    Car * car - the car being added to the lot
    int hour - the hour the car left the parking lot
    int minute - the minute the car left the parking lot
Output:
    ParkingLot * lot - the modified parking lot without the car
    Car * car - the reset car
Description:
    takes a pointer to a parking lot struct, then modifies it using information from 
    an imputed car struct. the hour and minute the car leaves is also imputed to the function
    and the cars information is reset.
*/
void carLeaves(ParkingLot * lot, Car * car, int hour, int minute) {
    double revenue = 0; //the amount of money to pay

    //time values for the current time (time the cars leaving), and the change in time from the
    //car parking in the lot to leaving the log
    Time currentTime; //leaving time
    Time changeInTime; //change from starting and leaving time

    //initialize the time
    currentTime.hour = hour;
    currentTime.minute = minute;

    //error message for user
    if (car->lotParkedIn != lot->lotNumber) { //if the car is not in this lot
        printf("ERROR: Car %s is in lot %d, not lot %d\n", 
            car->plateNumber, car->lotParkedIn, lot->lotNumber); //PROMPT user of error
        return; //exit function
    } //END IF

    if (!car->permit) { //car has no permit
        difference(car->enteringTime, currentTime, &changeInTime); //find the difference in time
        //check if a minute has passed since the last hour spent in the lot
        if (changeInTime.minute > 0) {
            revenue = lot->hourlyRate; //add one hour to the lots revenue
        } //END IF

        //check how many whole hours have passed in the lot
        revenue += lot->hourlyRate*changeInTime.hour;

        //if the toll is greater then the max charge, set the lots revenue to be the max charge
        if (revenue > lot->maxCharge) {
            revenue = lot->maxCharge;
        } //END IF

        lot->revenue += revenue; //add the revenue to the total lot revenue

        printf("Car %s leaves Lot %d at %d:%.2d paid $%.2f.\n", 
        car->plateNumber, lot->lotNumber, hour, minute, revenue); //PROMPT user
    }
    else { //car has a permit
        //PROMPT user
        printf("Car %s leaves Lot %d at %d:%.2d.\n", car->plateNumber, lot->lotNumber, hour, minute);
    } //END IF

    
    lot->currentCarCount -= 1; //remove a car from the lot
    car->lotParkedIn = 0; //reset the cars lot location
    //reset to cars default time in lot
    car->enteringTime.minute = -1;
    car->enteringTime.hour = -1;

   
} //END carLeaves

/*
Name:
    randomPlate
Input:
    None
Output:
    char * - a pointer to the dynamically allocated string generated within this function
Description:
    generates a random license plate with the format: XXX ###
        x is a random alphabetic, capitalized character and # is a base ten digit
    the random string is dynamically allocated 
*/
char * randomPlate() {    
    //pointer to the random string
    char * randomCar;

    /*set memory for the string, use calloc because valgrind is picky about 
    uninitialized memory blocks during printf requests*/
    randomCar = calloc (8, sizeof(char)); //7 chars (3 X's, 3#'s, and 1 ' '), plus \0 (terminator)

    //check if the memory was allocated successfully
    if (randomCar == NULL) {
        printf("ERROR: application out of memory, exiting\n");
        exit(0); //exit the program to prevent more errors
    }

    //add random char's to the string
    for (int i = 0; i < 3; i++) {        
        /*get a random number between 0 and 25, and add it 
        to the ASCII integer representation of the letter A
        this ensures we get a random letter between A and Z*/
        randomCar[i] = 'A' + (int) (rand()/(double)RAND_MAX*26);
    } //END FOR

    //add the space between the XXX and the ###
    randomCar[3] = ' ';

    for (int i = 4; i < 7; i++) {
        /*get a random number between 0 and 8, and add it to the
        ASCII integer representation of the char representation 
        of 0; this ensures we get a random char between 0 and 9*/
        randomCar[i] = '0' + (int) (rand()/(double)RAND_MAX*9);
    } //END FOR

    //returns the random character array
    return randomCar; 
} //END randomPlate

/*
Name:
    randomCar
Input:
    None
Output:
    Car * - points to the new dynamically allocated car
Description:
    initializes a new car struct and returns a pointer to it
    the new car struct is dynamically allocated
    the new car struct contains a randomly generated license plate value
    the new car struct automatically has its permit value set to '0'
*/
Car * randomCar() {
    //random number in order to initialize the permit value
    int random = (int) (rand()/(double)RAND_MAX*2);

    //create a car pointer
    Car * newCar;

    //set memory for the car type
    newCar = malloc (sizeof(Car));

    //check if the memory was allocated successfully
    if (newCar == NULL) {
        printf("ERROR: application out of memory, exiting\n");
        exit(0); //exit the program to prevent more errors
    }

    //initializes a new car
    initializeCar(newCar, randomPlate(), random);

    //returns the pointer to the new dynamically allocated car type
    return newCar;
} //END randomCar

int main() {
    int lotNumber;
    int hour = 6;
    int minute = 0;

    //seed the random number generator with the current time
    //the random number generator will be used in various procedures later
    srand(time(NULL));

    //array to hold pointers to 50 cars
    Car * carArray[50];

    //array to hold pointers to 5 parking lots with the following paramters
    ParkingLot * parkingLotArray[5];

    for (int i = 0; i < 50; i++) {        
        carArray[i] = randomCar();
        printf("Car %s with permit %d\n", carArray[i]->plateNumber, carArray[i]->permit);
    } //END FOR

    for (int i = 0; i < 5; i++) {
        //parking lot buffer
        parkingLotArray[i] = malloc(sizeof(ParkingLot));
    }

    //initialize the lots
    initializeLot(parkingLotArray[0], 1, 5, 4, 12);
    initializeLot(parkingLotArray[1], 2, 10, 5, 14);
    initializeLot(parkingLotArray[2], 3, 15, 6, 16);
    initializeLot(parkingLotArray[3], 4, 20, 7, 18);
    initializeLot(parkingLotArray[4], 5, 25, 8, 20);

    printf("\n");

    for (int i = 0; i < 5; i++) {
        printLotInfo(*parkingLotArray[i]);
    } //END IF

    printf("\n");

    for (int i = 0; i < 50; i++) {
        lotNumber = (int) (rand()/(double)RAND_MAX*5);
        carEnters(parkingLotArray[lotNumber], carArray[i], hour, minute);
        minute += 5;
        if (minute >= 60) {
            minute = 0;
            hour += 1;
        } //END IF
    } //END FOR

    printf("\n");

    for (int i = 0; i < 5; i++) {
        printLotInfo(*parkingLotArray[i]);
    } //END IF

    printf("\n");

    minute = 0;
    hour = 11;

    for (int i = 0; i < 50; i++) {
        if (carArray[i]->lotParkedIn != 0) {
            carLeaves(parkingLotArray[carArray[i]->lotParkedIn-1], carArray[i], hour, minute);
            minute += 5;
        } //END IF
        if (minute >= 60) {
            minute = 0;
            hour += 1;
        } //END IF
    } //END FOR

    printf("\n");

    for (int i = 0; i < 5; i++) {
        printLotInfo(*parkingLotArray[i]);
    } //END IF

    printf("\n");

    for (int i = 0; i < 5; i++) {
        printf("Total revenue of Lot %d is $%.2f\n", parkingLotArray[i]->lotNumber, parkingLotArray[i]->revenue);
    } //END IF

    printf("\n");

    //free all the memory that was dynamically allocated
    for (int i = 0; i < 50; i++) {
        free(carArray[i]->plateNumber);
        free(carArray[i]);
    } //END FOR

    for (int i = 0; i < 5; i++) {
        free(parkingLotArray[i]);
    } //END FOR
    
} //END main

