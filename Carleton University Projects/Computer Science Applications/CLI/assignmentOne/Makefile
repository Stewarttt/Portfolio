#Name: Connor Raymond Stewart 
#*Student Number: 101041125 

OBJ = main.o input.o reader.o logging.o UI.o parser.o

workshop: $(OBJ)
	g++ -o command $(OBJ)
	
main.o: main.cpp
	g++ -c main.cpp
	
input.o: input.cpp input.h
	g++ -c -std=c++11 input.cpp
    
reader.o: reader.cpp reader.h
	g++ -c -std=c++11 reader.cpp
	
logging.o: logging.cpp logging.h
	g++ -c -std=c++11 logging.cpp
	
parser.o: parser.cpp parser.h
	g++ -c -std=c++11 parser.cpp
	
UI.o: UI.cpp UI.h
	g++ -c -std=c++11 UI.cpp

clean:
	rm -f $(OBJ) command

