Name: Connor Stewart
Student Number: 101041125
Source Files Submitted:
    ReadMe.txt - a readme file discribing the submission
    stop.c - when run, it sends a message to stop the server it connects to
    simulator.c - communicates with robot clients to regulate a simulation
    robotClient.c - keeps track of the positions and movements of a single robot in the simulation
    Makefile - a makefile that creates/cleans everything
    simulator.h - a header file containing the main data structures and definitions for the simulation
    display.c - a file that creates a window to view the simulation visually
instructions:
    open two command prompt windows to the directory containing the source files
    type: (in window 1)
        make
    type: (in window 1)
        ./simulator
    Keep Typing until desired number of robots are added: (in window 2)
        ./robotClient &
    type: (in window 2)
        ./stop
    this will close the server and end the programs function
    type: (optional)
        make clean
Notes:
    If you close the server with a client still running, the server socket will still remain 'used' by
    the application for some time. Normally, waiting about a minute will allow for the simulation to 
    be opened again, but somtimes you will need to restart virtual box because the socket will remain
    closed.
        The professor had this problem with this as well, and no known cause was proposed. 

