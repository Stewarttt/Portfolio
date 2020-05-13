#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"


// Set up a client socket and connect to the server.  
// Return -1 if there was an error.
int connectToServer(int *sock,  struct sockaddr_in *address) {
    int status;

    *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

//initialized the socket variables, sends message to server, and closes socket for shutdown
int main() {
    //initialize server socket and address variables
    int clientSocket;
    struct sockaddr_in clientAddress;
    char message = STOP; //set the message to be stop

    //check if the connection was successful
    if (connectToServer(&clientSocket, &clientAddress) == -1) { //-1 means an error occured
        printf("Exiting...\n"); //PROMPT user
        exit(-1); //exits the program
    } //END IF
    
    send(clientSocket, &message, 1, 0); //2 is a 1 byte representation of stop

    close(clientSocket); //close the client socket
    printf("CLIENT: Shutting down.\n"); //PROMPT user
    return 1; //exit with success code
} //END main
