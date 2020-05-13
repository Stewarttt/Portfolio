all: simulation stop robotClient
simulation: simulator.o
	gcc -g -Wall -o simulator simulator.o -lX11 -lm -pthread
simulator.o: simulator.c display.c simulator.h
	gcc -c -Wall simulator.c
stop.o: stop.c simulator.h
	gcc -c -Wall stop.c
robotClient: robotClient.c simulator.h
	gcc -g -Wall -o robotClient robotClient.c -lm
display.o: display.c
	gcc -c -Wall display.c
clean:
	rm *.o stop simulator robotClient
