#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "simulator.h"


// Set up a client socket and connect to the server.  
// Return -1 if there was an error.
int connectToServer(int *sock,  struct sockaddr_in *address) {
    int status; //stores the status of the server connection
    *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //connect to the server socket
    if (*sock < 0) { //check if the socket is valid
        printf("*** CLIENT ERROR: Could not open socket.\n"); //PROMPT user
        return -1; //RETURN that an error occured
    } //END IF

    // Setup address
    memset(address, 0, sizeof(*address)); //set memory to zero
    address->sin_family = AF_INET; //set information to address variable
    address->sin_addr.s_addr = inet_addr(SERVER_IP); //set address to address variable
    address->sin_port = htons((unsigned short) SERVER_PORT); //set port to address variable

    //connect to server, and check the status of the connection
    status = connect(*sock, (struct sockaddr *) address, sizeof(*address));
    if (status < 0) { //if the status is not within the success range
        printf("*** CLIENT ERROR: Could not connect.\n"); //PROMPT user
        return -1; //RETURN that an error occured
    } //END IF
    return 1; //exit with success code
} //END connectToServer

int parseInformation(char * buffer, int section) { //parsed a string containing input information
    int length = strlen(buffer); //finds the length of the string
    char substring[10]; //holds a substring containing a peace of info from the main string
    int counter = 0; //counts the current chuck of information being analized
    int arrayPosition = 0; //the next empty position in the substring array

    for (int i = 0; i < length; i++) { //for all letters in the input string
        if (buffer[i] == ':') { //check if a new peace of information is being checked
            counter++; //were now iterating over a new peace of information
        }
        else if (buffer[i] == '\0') { //if we reach the null terminator
            break; //exit the loop
        }
        else if (section == counter) { //check if were currently anylizing the correct peace of info
            substring[arrayPosition++] = buffer[i]; //copy the information from this segment
        } //END IF
    } //END FOR
    substring[arrayPosition] = '\0'; //add the null terminator to the end of the substring
    return atof(substring); //convert the substring into a float representation
} //END parseInformation

int main() {
    //initialize server socket and address variables
    int clientSocket;
    struct sockaddr_in clientAddress;

    int bytesRcv; //stores the number of bytes receaved from the server
    char buffer[30]; //stores the message receaved from the server
    char message[30]; //stores a second server message
    int turningDirection = 0; //tracks if the robot is currently turing left or right
    char commandMessage = REGISTER; //set message to join request

    float x, y; //tracks the x and y position of the robot
    int pos, ID; //tracks the direction and the ID of the robot
    float newX, newY; //stores the temp x and y positions for the robot
  
    // Set up the random seed
    srand(time(NULL));
  
    // Register with the server
    //check if the connection was successful
    if (connectToServer(&clientSocket, &clientAddress) == -1) { //-1 means an error occured
        printf("Exiting...\n"); //PROMPT user
        exit(-1); //exits the program
    } //END IF

    // Send register command to server.  Get back response data
    // and store it.   If denied registration, then quit.
    printf("Client: sending join request to server.\n");
    send(clientSocket, &commandMessage, 1, 0); //send 1 byte representation of register

    // Get response from server, should be "OK"
    bytesRcv = recv(clientSocket, buffer, 80, 0);
    buffer[bytesRcv] = 0; // put a 0 at the end so we can display the string
    printf("CLIENT: Got back response from server.\n");

    if (*buffer == NOT_OK) { //if the server refuses connection
        printf("The server is full, exiting...\n");
        close(clientSocket); //close the client socket
        exit(-1); //EXIT the program
    } //END IF

    //get the robots id from the server
    printf("Client: requesting the robots information from server.\n");
    commandMessage = OK;
    send(clientSocket, &commandMessage, 1, 0); //send OK message
    // Get response from server, should be "OK"
    bytesRcv = recv(clientSocket, buffer, 80, 0);
    buffer[bytesRcv] = 0; // put a 0 at the end so we can display the string
    printf("CLIENT: Got back response from server.\n");

    ID = parseInformation(buffer, 0); //get the id from the message
    x = parseInformation(buffer, 1); //get the x coordinate from the message
    y = parseInformation(buffer, 2); //get the y coordinate from the message
    pos = parseInformation(buffer, 3); //get the robots direction from the message

    //PROMPT user of the servers information
    printf("CLIENT: Got back response of ID:%d, X:%lf, Y:%lf, Pos:%d from the server\n"
        , ID, x, y, pos);
    close(clientSocket); //close the client socket
  
    // Go into an infinite loop exhibiting the robot behavior
    while (1) {
        // Connect to the server
        if (connectToServer(&clientSocket, &clientAddress) == -1) { //-1 means an error occured
            printf("Exiting...\n"); //PROMPT user
            exit(-1); //exits the program
        } //END IF
        
        // Compute a forward location and check if it is ok
        newX = x + ROBOT_SPEED*cos(pos);
        newY = y + ROBOT_SPEED*sin(pos);
        sprintf(message, "%d:%d:%d:%d:%d:", MOVE_TO, ID, (int)roundf(newX), (int)roundf(newY), 1);
            
        // Send MOVE_TO request to server
        send(clientSocket, message, sizeof(message), 0);
        
        // Get response from server.
        memset(buffer, 0, sizeof(buffer)); //zero the buffer
        bytesRcv = recv(clientSocket, buffer, 80, 0); //put the message into the buffer

        // If response is OK then move forward
        if (*buffer == OK) { //if the operation was a success, 4 = OK
            x = newX; //set the current x coordinate to be the proposed one
            y = newY; //set the current y coordinate to be the proposed one
            turningDirection = 0; //set the turining direction to unknown
        } //END IF

        // Otherwise, we could not move forward, so make a turn.
        // If we were turning from the last time we collided, keep
        // turning in the same direction, otherwise choose a random 
        // direction to start turning.
        else {
            if (turningDirection == 0) {
                //get a random number from 1 to 2
                turningDirection = (int)(rand()/(double)RAND_MAX*2) + 1;
                if (turningDirection == 2) { //if the number is randomly generated to two
                    turningDirection = -1; //set the turining direction to left
                } //END IF
            } //END IF
            memset(buffer, 0, sizeof(buffer)); //zero the buffer
            pos += ROBOT_TURN_ANGLE*turningDirection; //change the robot position by left or right

            //check if the robot position is within valid range
            if (pos > 180) { //if the position is greater then 180
                pos = (-180)+(pos - 180); //set the negative position
            }
            else if (pos < -180) { //if the robots position is less then 180
                pos = 180+(pos + 180); //set the positive position
            } //END IF

            sprintf(message, "%d:%d:", pos, ID); //format reply as position:ID:
            send(clientSocket, message, sizeof(message), 0); //send information back to server
        } //END IF
        
        // Uncomment line below to slow things down a bit 
        close(clientSocket); //close the client socket
        usleep(1000); //uncomments the line as instructed
    } //END WHILE
    return 1; //exit with success code
} //END main
