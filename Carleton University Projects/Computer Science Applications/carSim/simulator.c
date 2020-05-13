#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "simulator.h"

#include "display.c"

// The environment that contains all the robots
Environment    environment;  


// Initialize the server by creating the server socket, 
// setting up the server address, binding the server socket 
// and setting up the server to listen for the MAX_ROBOTS 
// number of robot clients
void initializeServer(int *serverSocket, 
                      struct sockaddr_in  *serverAddress) {
    //create the server socket
    *serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //get socket id
    if (serverSocket < 0) { //if the socket id is invalid, dispaly this error message
        printf("*** SERVER ERROR: Could not open socket.\n");
        exit(-1); //exit the program
    } //END IF

    //setup the server address
    memset(serverAddress, 0, sizeof(*serverAddress)); //zero the struct
    serverAddress->sin_family = AF_INET; //set server information
    serverAddress->sin_addr.s_addr = htonl(INADDR_ANY); //set address for server
    serverAddress->sin_port = htons((unsigned short) SERVER_PORT); //set port for server

    //Bind the server socket
    int status = bind(*serverSocket, (struct sockaddr *)serverAddress, sizeof(*serverAddress));
    if (status < 0) { //if the status is out of range
        printf("*** SERVER ERROR: Could not bind socket.\n");
        exit(-1); //exitthe program
    } //END IF

    //Set up the line-up to handle up to MAX_ROBOTS clients in line
    status = listen(*serverSocket, MAX_ROBOTS);
    if (status < 0) { //if the status is out of range
        printf("*** SERVER ERROR: Could not listen on socket.\n");
        exit(-1); //exit the program
    } //END IF   
} //END initializeServer

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


// Determine whether the registered robot with id i is able 
// to move to the location specified by (newX, newY). Return 
// OK if it is able.  Return NOT_OK_COLLIDE if moving to that 
// location would cause the robot to collide with another robot.
// Return NOT_OK_BOUNDARY if moving to that location would cause 
// the robot to go beyond the environmental boundary.
char canMoveTo(char * buffer) { //check if a robot can move to a position
    //parse the input string to get the robots ID, X, and Y values
    int currentRobot = parseInformation(buffer, 1); //ID
    int proposedX = parseInformation(buffer, 2); //X value
    int proposedY = parseInformation(buffer, 3); //Y value

    //check if the robot is not in valid position around the window
    if (!((proposedX > ROBOT_RADIUS && proposedX < (ENV_SIZE-   ROBOT_RADIUS)) 
        && proposedY > ROBOT_RADIUS && proposedY < (ENV_SIZE-ROBOT_RADIUS)))  {
        return NOT_OK_BOUNDARY; //return the robot is out of position
    } //END IF

    for (int i = 0; i < environment.numRobots; i++) { //for all the robots in the array
        if (currentRobot == i) { //check if the robot is currently being iterated over
            continue; //skip this iteration
        } //END IF
        if (hypot(environment.robots[i].x-proposedX, environment.robots[i].y-proposedY) 
            <= (ROBOT_RADIUS*2)) {
            return NOT_OK_COLLIDE; //return the robot is colliding
        } //END IF
    } //END FOR

    // delay to slow things down
    usleep(10000/(1 + environment.numRobots));
    return OK; //if all went well, return OK
}

void setupRobot (char * inputArray) { //sets up a new robot in the robots struct
    srand(time(NULL)); //seeds the random number generator

    char returnString[30]; //this string contains the information for the new robot
    char noOverlappingRobots = 'F'; //represents if overlapping robots have been found
    int ID, direction; //holds the ID and direction of the robots
    float xPosition, yPosition; //holds the x and y position of the robots
    ID = environment.numRobots++; //get the current robot ID, increment the total number of bots

    while (noOverlappingRobots == 'F') { //while robots are overlapping
        //get a new x,y position and a new direction
        xPosition = (int)(rand()/(double)RAND_MAX*(ENV_SIZE-ROBOT_RADIUS*3))+ROBOT_RADIUS;
        yPosition = (int)(rand()/(double)RAND_MAX*(ENV_SIZE-ROBOT_RADIUS*3))+ROBOT_RADIUS;
        direction =  (int)(rand()/(double)RAND_MAX*360);
        noOverlappingRobots = 'T'; //assume the robots are not overlapping

        for (int i = 0; i < environment.numRobots; i++) { //for all of the robots in the struct
            //check if the newly created bot is overlapping with any of the existing bots
            if (hypot(environment.robots[i].x-xPosition, environment.robots[i].y-yPosition) 
                <= (ROBOT_RADIUS*2)) {
                noOverlappingRobots = 'F'; //say the robots are still overlapping
            } //END IF
        } //END FOR
    } //END WHILE

    //once a unique position is found, set the x,y, and direction values in the struct
    environment.robots[ID].x = xPosition;
    environment.robots[ID].y = yPosition;
    environment.robots[ID].direction = direction;

    //set the return string array to be zero'ed
    memset(returnString, 0, sizeof(returnString));
    
    if (direction > 180) { //if the direction value is over 180, convert it to a negative value
        direction = (direction - 180) * -1; //get new direction value for the robot
    } //END IF

    //format the robot return string: ID:X:Y:direction:
    sprintf(returnString, "%d:%f:%f:%d:", ID, xPosition, yPosition, direction);
    
    returnString[strlen(returnString)] = '\0'; //add the null terminator
    
    strcpy(inputArray, returnString); //copy the newly created string into the old array
} //END setupRobot

// Handle client requests coming in through the server socket.  
// This code should run indefinitiely.  It should wait for a 
// client to send a request, process it, and then close the 
// client connection and wait for another client.  The requests 
// that may be handlesd are REGISTER and MOVE_TO.   Upon 
// receiving a REGISTER request, the server should choose a 
// random location and direction within the environment (which 
// MUST be within the boundaries and must NOT be overlapping 
// another robot).  Upon receiving a MOVE_TO request, the 
// code should send a single byte to the client which is either 
// OK or NOT_OK_BOUNDARY (if the robot would collide with the 
// boundary at the requested location) or NOT_OK_COLLIDE (if the 
// robot would collide with another robot at the requested 
// location).  Parameter *e is a pointer to the environment.
void * handleIncomingRequests(void * e) {
    //initialize server variables
    int serverSocket; //stores the server socket value
    int clientSocket; //stores the client socket value
    unsigned int addressSize; //stores the address size value
    int bytesRcv; //stores the number of bytes in the clients message
    char buffer[30]; //buffer to hold the clients message
    char message[15]; //buffer to hold output message
    char * response = "OK"; //holds a message for a client
    char commandMessage; //single commands sent to server
    struct sockaddr_in serverAddress, clientAddress; //stores the server and client address info

    //start the server using the predefined method
    initializeServer(&serverSocket, &serverAddress);
    while (environment.shutDown == 0) { //wait for clients to connect untill the server shuts down
        addressSize = sizeof(clientAddress);
        //accept the client connection and track its socket number
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &addressSize);
        if (clientSocket < 0) { //if the socket is outside of the valid range
            printf("*** SERVER ERROR: Could accept incoming client connection.\n"); //PROMPT user
            exit(-1); //exit the program if this error occurs
        } //END IF
        printf("SERVER: Received client connection\n"); //PROMPT user
        // Go into infinite loop to talk to client
        while (1) { //communicate with this client until loop is broken
            memset(buffer, 0, sizeof(buffer)); //zero the memory array
            // Get the message from the client, and check the # if bytes received
            bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0); //get response from client
            if (bytesRcv == 0) { //if the message was empty, exit as there no way to proceed
                break; //exit the inner loop
            } //END IF
            buffer[bytesRcv] = 0; // put a 0 at the end so we can display the string
            printf("SERVER: Received client request\n"); //PROMPT user
            
            //STOP is a 1 byte message representing a request to STOP the server
            if (*buffer == STOP) { //2 = request to stop the server
                printf("SERVER: Client has requested the server to stop.\n"); //PROMPT user
                // Respond with an "OK" message
                printf("SERVER: Sending OK to client\n"); //PROMPT user
                send(clientSocket, response, strlen(response), 0); //send response
                break; //exit the inner loop
            } 
            else if (*buffer == REGISTER) { //request to join server, check for space
                printf("SERVER: Client has requested the server to join.\n");
                if (environment.numRobots < MAX_ROBOTS) { //if there are less then max robots
                    commandMessage = OK; // Respond with an "OK" message
                    printf("SERVER: Sending OK to client\n"); //PROMPT user
                    send(clientSocket, &commandMessage, 1, 0);
                }
                else { //if the max robot number is reached
                    commandMessage = NOT_OK; // Respond with a "NOT_OK" message
                    printf("SERVER: Sending NOT_OK to client\n"); //PROMPT user
                    send(clientSocket, &commandMessage, 1, 0); //send message
                    break; //exit the inner loop
                } //END IF
            }
            else if (*buffer == OK) { //4 = setup new robot
                setupRobot(message); //fill message into the message array
                send(clientSocket, message, strlen(message), 0); //send the message
                break; //exit the inner loop
            }
            else if (parseInformation(buffer, 0) == MOVE_TO) { //3 = move to new location
                if (canMoveTo(buffer) == OK) { //if four, then the robot can move
                    environment.robots[(int) parseInformation(buffer, 1)].x
                        = parseInformation(buffer, 2); //set new x location
                    environment.robots[(int) parseInformation(buffer, 1)].y
                        = parseInformation(buffer, 3); //set new y location
                    memset(buffer, 0, sizeof(buffer)); //zero the memory array
                    commandMessage = OK; //set the response to OK
                    // Respond with an "OK" message
                    printf("SERVER: Sending OK code to client\n"); //PROMPT user
                    send(clientSocket, &commandMessage, 1, 0); //send message
                    break; //exit the inner loop
                }
                else { //otherwise there is a collision with somthing
                    response = "NOT_OK"; //set message to not ok
                    // Respond with a "NOT_OK" message
                    printf("SERVER: Sending \"%s\" to client.\n", response); //PROMPT user
                    send(clientSocket, response, strlen(response), 0); //send the message
                    memset(buffer, 0, sizeof(buffer)); //reset the buffer array
                    bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0); //get response
                    environment.robots[(int) parseInformation(buffer, 1)].direction
                        = (int) parseInformation(buffer, 0); //use response to set new direction
                } //END IF
            } //END IF
        } //END WHILE
        printf("SERVER: Closing client connection.\n"); //PROMPT user
        close(clientSocket); // Close this client's socket
        // If the client said to stop, then I'll stop myself
        if (*buffer == STOP) { //2 is a single byte representation of stop
            environment.shutDown = 1; //signal the application to shut down
            break; //exit the outer loop
        } //END IF
    } //END WHILE
    close(serverSocket); //close the server socket
    return NULL; //return nothing upon success
} //END handleIncomingRequests

int main() {
  // So far, the environment is NOT shut down
  environment.shutDown = 0;
  
  // Set up the random seed
  srand(time(NULL));

  // Spawn threads to handle incoming requests and 
  // update the display
  pthread_t threads[2]; //set two threads
  pthread_create(&threads[0], NULL, redraw, &environment); //initialize the first thread
  pthread_create(&threads[1], NULL, handleIncomingRequests, &environment); //initialize thread two
  //join the first and second thread together
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL); 
  
  // Wait for the threads to complete  
  printf("SERVER: Shutting down.\n"); //PROMPT user of server closing
  return 1; //exit with success code
} //END main
