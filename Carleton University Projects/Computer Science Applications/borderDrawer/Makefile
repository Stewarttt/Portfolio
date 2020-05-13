all: boundaries.o polygonDisplay.o
	gcc -g -o boundaries boundaries.o polygonDisplay.o -lX11

boundaries.o: boundaries.c polygonSet.h polygonDisplay.h
	gcc -g -Wall -c boundaries.c

polygonDisplay.o: polygonDisplay.c polygonDisplay.h
	gcc -g -Wall -c polygonDisplay.c

clean:
	-rm *.o boundaries
