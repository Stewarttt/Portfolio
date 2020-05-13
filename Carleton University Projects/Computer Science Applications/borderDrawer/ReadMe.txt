Name: Connor Stewart
Student Number: 101041125

Instructions:
    open the terminal to the current directory
    type:
        make all
    OR type:
        make
    this will compile the code into executables
    Type:
        ./boundaries
    this will start the program
    when the program has started:
        click ENTER inside the terminal shell to continue allong with the disply sequences
    Type:
        make clean
    this will remove all object files and executables from the current directory

    to test for memory leaks:
    type:
        valgrind --leak-check=full ./boundaries
        
Errors:
    the clean up polygon function sometimes gets the borders for single chain polygons wrong, and 
    leaves too many extra blocks

