#include <stdio.h>
#include <stdlib.h>

#include "polygonSet.h"
#include "polygonDisplay.h"

#define WIDTH  30
#define HEIGHT 30


// Trace out a single boundary from the grid, starting at the given (xs,ys) location.  Mark the boundary
// locations with a 2 in the grid.  Assume that the polygon p has been created and has a single vertex at
// location (xs, ys).  The procedure will complete the polygon by adding the remaining boundary points it
// finds in sequence.
void traceWholeBorder(char grid[HEIGHT][WIDTH], int xs, int ys, Polygon *p) {
  int  xOffset[8] = {1, 1, 0, -1, -1, -1, 0, 1};
  int  yOffset[8] = {0, 1, 1, 1, 0, -1, -1, -1};
  
  int  xc = xs, yc = ys;  // The current point is the start point
  int  d = 7;
        
  char notDone = 1;
  while(notDone) {
    int dp;
    if (d%2 == 0)
      dp = (d+7)%8;
    else
      dp = (d+6)%8;
    int i;
    for (i=0; i<8; i++) {
      char val = grid[yc+yOffset[dp]][xc+xOffset[dp]];
      if (val != '0') {
	xc = xc + xOffset[dp];
	yc = yc + yOffset[dp];
	grid[yc][xc] = '2';

    //create a single vertex in memory
	Vertex * vertex = malloc(sizeof(Vertex));
    //give the vertex its x and y corrdinates
    vertex->x = xc;
    vertex->y = yc;
    //set to NULL for now
    vertex->next = NULL;
    
    Vertex * current; //points to the currently iterated vertex
    current = p->firstVertex; //gets the first vertex in linked list
    while (current->next != NULL) { //run until the null vertex is found
        current = current->next; //move to the next vertes
    } //END WHILE

    current->next = vertex; //set the next vertex to the new vertes

    //set the last vertex to this vertex
    //this will run each time this is looped, the last vertex added here will be the 
    //the final last vertex added, and will therefore be the true last vertex
    p->lastVertex = vertex; 
    

	d = dp;
	break;
      }
      dp = (dp + 1)%8;
    }
    if (i == 8) {
      notDone = 0;
      grid[yc][xc] = '0'; // Ignore single points
    }
    if ((xc == xs) && (yc == ys))
      notDone = 0;
  }
}

// Compute the boundaries of the obstacles in the grid.  Add a polygon to pSet for each completed border.
void computeBoundaries(char grid[HEIGHT][WIDTH], PolygonSet *pSet) {
  Polygon *p;  // This will point to each polygon that you will trace
  
  for(int y=1; y<HEIGHT; y++) {
    for (int x=1; x<WIDTH; x++) {
      if ((grid[y][x] == '1') && (grid[y][x-1] == '0')) {
	
        //allocate the polygon p with memory
	    p = malloc(sizeof(Polygon));
        //allocated vertex with memory
        Vertex * vertex = malloc(sizeof(Vertex));

        //set the vertex to contain the current x and the current y
        vertex->x = x;
        vertex->y = y;
        //set the next vertex to null for now
        vertex->next = NULL;

        //set this polygons vertex to be this vertex
        p->firstVertex = vertex;
		p->lastVertex = vertex;
        //set the next polygon to null for now
        p->next = NULL;

	    grid[y][x] = '2'; // Mark it as a border
	    traceWholeBorder(grid, x, y, p);

        //if first polygon is null, then this is the first element
        if (pSet->firstPolygon == NULL) {
            pSet->firstPolygon = p; //set the first polygon in the list to this
        }
        else {
            Polygon * current; //create a pointer to a polygon
            current = pSet->firstPolygon; //set this pointer to point to the first polygon
            while (current->next != NULL) { //iterated until were at the last polygon
                current = current->next; //move to the next position in the linked list
            } //END WHILE
            current->next = p; //add the new polygon to the linked list
            //set this polygon to be the last polygon
            //the last time this code is run, this will be the final time the last polygon
            //is updated, and therefore the last polygon will be set correctly
            pSet->lastPolygon = p; 
        } //END IF
      }
    }
  }
}


/*
Name: newVertex
Input:
    int x - the x coordinate of the vertex
    int y - the y coordinate of the vertex
    Vertex * next - a pointer to the next vertes
Output:
    Vertex * newVertex - a pointer to the new dynammically allocated vertex
Discription:
    simply creates a new dynammically allocated vertex for the linked list
*/
Vertex * newVertex (int x, int y, Vertex * next) {
    Vertex * new = calloc(1, sizeof(Vertex));
    new->x = x;
    new->y = y;
    new->next = next;
    return new; 
} //END newVertex

// This procedure must clean up the polygons in the initialPolygons set by removing sequences of vertices
// that have the same x values or the same y values.
void cleanUpPolygons(PolygonSet *inputSet, PolygonSet *outputSet) {
	Polygon * previousPolygon = inputSet->firstPolygon; //previosly iterated polygon
	Vertex * previousVertex = previousPolygon->firstVertex; //previously iterated vertes
	Polygon * currentPolygon = inputSet->firstPolygon; //currently iterated polygon
	Vertex * currentVertex; //currently iterated vertex
    Vertex * currentVertexPoint; //currently iterated vertex, used for new set

    int polygonNumber = 0; //the number of polygons in the input set
    int vertexNumber = 0; //the number of vertexes in the input set

    int xDepth = 0; //the number of iterated polygons in the set horizontal
    int yDepth = 0; //the number of iterated polygons in the set vertical

    //format: polygon->vertex
    Vertex * vertexArray[255][255]; //an array consisting of all the vertex pointers

    //format: polygon->vertex->coordinate
    float valueArray[255][255][2]; //an array consisting of all the vertex coordinents
    int polygonDepth[255]; //an array containing the number of vertexes for each polygon

	//iterate through the polygones in the 
	while (currentPolygon != NULL) {        
		currentVertex = currentPolygon->firstVertex;
		//iterate through the verticies in the current polygon
		while (currentVertex != NULL) { 
			previousVertex = currentVertex; //set the previous vertex
			currentVertex = currentVertex->next; //iterate one vertex
            //increase the number of vertexes, also add this vertex to the vertex array    
            vertexArray[polygonNumber][vertexNumber++] = previousVertex;
		} //END WHILE
        //set the polygon depth value to the number of vertexes iterated through
        polygonDepth[polygonNumber] = vertexNumber; 
        vertexNumber = 0; //reset the number of vertexes
        polygonNumber++; //there is one more polygon to count
		previousPolygon = currentPolygon; //set the previously iterated polygon
		currentPolygon = currentPolygon->next; //set the next polygon
    } //END WHILE

    for (int i = 0; i < polygonNumber; i++) { //for the number of polygons
        for (int k = 0; k < polygonDepth[i]; k++) { //for the depth of the current polygon
            //set the current vertexes x and y coordinates
            valueArray[i][k][0] = vertexArray[i][k]->x; 
            valueArray[i][k][1] = vertexArray[i][k]->y;
        } //END FOR
    } //END FOR

    for (int i = 0; i < polygonNumber; i++) { //for the number of polygons
        for (int k = 0; k < polygonDepth[i]-1; k++) { //for the depth of the current polygon
            for (int j = k+1; j < polygonDepth[i]-1; j++) { //for the depth of the current polygon
                if (valueArray[i][k][0] == valueArray[i][j][0] &&
                    valueArray[i][k][1] == valueArray[i][j][1]) { //if the values are the same
                    if (valueArray[i][k][0] == 1) { //change to -1.1
                        valueArray[i][k][0] = -1.1; //set to -1.1 to avoid deletion
                    }
                    else {
                        valueArray[i][k][0] = -valueArray[i][k][0]; //negate value
                    } //END IF
                    if (valueArray[i][k][1] == 1) {
                        valueArray[i][k][1] = -1.1; //set to -1.1 to avoid deletion
                    }
                    else {
                        valueArray[i][k][1] = -valueArray[i][k][1]; //negate value
                    } //END IF
                } //END IF
            } //END FOR
        } //END FOR
    } //END FOR
        
    for (int i = 0; i < polygonNumber; i++) { //for the number of polygons
        for (int k = 0; k < polygonDepth[i]-1; k++) { //for the depth of the current polygon
            if (polygonDepth[i] < 3) { //if the polygon is less then 3 units long
                break; //exit, the polygon cannot be shortened
            }
            else if (polygonDepth[i] == 5) { //if the polygon is exactly 3 units long
                break; //exit this loop
            } //END IF
            if (abs(valueArray[i][0][1]) != abs(valueArray[i][k][1])) { //if the polygon is not a line
                break; //if its not a line exit
            } //END IF
            if (k == polygonDepth[i]-2) { //if the polygon is a stright line
                //for the depth of the current polygon, remove the palindrome within it
                for (int j = polygonDepth[i]/2+(polygonDepth[i]%2); j < polygonDepth[i]-1; j++) { 
                    valueArray[i][j][0] = valueArray[i][j][1] = -1; //set the second half to be empty
                } //END FOR
                for (int j = 0; j < polygonDepth[i]/2+(polygonDepth[i]%2); j++) { 
                    valueArray[i][j][0] = abs(valueArray[i][j][0]); //change to positive
                    valueArray[i][j][1] = abs(valueArray[i][j][1]); //change to positive
                } //END FOR
                break; //exit the loop
            } //END IF
        } //END FOR

        for (int k = 0; k < polygonDepth[i]-1; k++) { //for the depth of the current polygon
            //the first and last polygons are 'duplicate' pointers
            if (polygonDepth[i] < 4) { //if the polygon is less then 3 units long
                break; //exit, the polygon cannot be shortened
            }
            else if (polygonDepth[i] == 5) { //if the polygon is exactly 3 units long
                valueArray[i][1][0] = valueArray[i][1][1] = -1; //set the middle to be empty
            } //END IF

            //if two vertexes are lieing on the same x plane
            if (k+1<polygonDepth[i] && valueArray[i][k][0] == valueArray[i][k+1][0]) {
                xDepth += 1; //increment the x depth by one
            } //END IF

            //if the chain of vertexes is broken
            if (k+1<polygonDepth[i] && valueArray[i][k][0] != valueArray[i][k+1][0]) {
                xDepth = 0; //reset the counter
            }
            else if (xDepth >= 2) { //if there is another vertex in the chain
                //if the next element is not the same
                if (valueArray[i][k][0] != valueArray[i][k+1][0]) { 
                    break; //exit
                } //END IF
                valueArray[i][k][0] = valueArray[i][k][1] = -1; //set the x and y to -1
            } //END IF

            //if two vertexes are lieing on the same y plane
            if (k+1 < polygonDepth[i] && valueArray[i][k][1] == valueArray[i][k+1][1]) {
                yDepth += 1; //increment the y depth
            } //END IF

            //if the chain of vertexes is broken
            if (k+1<polygonDepth[i] && valueArray[i][k][1] != valueArray[i][k+1][1]) {
                yDepth = 0; //reset the counter
            }
            else if (yDepth >= 2) { //if there is another vertex in the chain
                valueArray[i][k][0] = valueArray[i][k][1] = -1; //set the x and y to -1
            } //END IF
        } //END FOR
        xDepth = 0; //reset the counter
        yDepth = 0; //reset the counter
    } //END FOR

    for (int i = 0; i < polygonNumber; i++) { //for the number of polygons
        for (int k = 0; k < polygonDepth[i]-1; k++) { //for the depth of the current polygon
            if (valueArray[i][k][0] < -1 &&  valueArray[i][k][1] < -1) {
                if (valueArray[i][k][0] == -1.1) { //change -1.1 back to 1
                    valueArray[i][k][0] = 1; //set back to one
                }
                else {
                    valueArray[i][k][0] = -valueArray[i][k][0]; //set back to positive
                } //END IF
                if (valueArray[i][k][1] == -1.1) { //change -1.1 back to one
                    valueArray[i][k][1] = 1; //set back to positive
                }
                else {
                    valueArray[i][k][1] = -valueArray[i][k][1]; //set back to positive
                } //END IF
            } //END IF
        } //END FOR
    } //END FOR
    
    //allocate and set the first polygon to null
    outputSet->firstPolygon = malloc(sizeof(Polygon));
    outputSet->firstPolygon->firstVertex = NULL;
    outputSet->firstPolygon->lastVertex = NULL;
    outputSet->firstPolygon->next = NULL;

    //set the current polygon to be equal the first one in the set
    currentPolygon = outputSet->firstPolygon;
    
    for (int i = 0; i < polygonNumber; i++) { //for the number of polygons
        for (int k = 0; k < polygonDepth[i]; k++) { //for the depth of the current polygon
            if (valueArray[i][k][0] != -1 || valueArray[i][k][1] != -1) { //if the polygons valid
                if (currentPolygon->firstVertex == NULL) { //if this is the first vertex in the poly
                    currentPolygon->firstVertex = newVertex(
                        valueArray[i][k][0], valueArray[i][k][1], NULL); //set the vertex
                    currentVertexPoint = currentPolygon->firstVertex; //set the current vertex
                }
                else {
                    
                    currentVertexPoint->next = newVertex(
                        valueArray[i][k][0], valueArray[i][k][1], NULL); //set the vertex
                    currentPolygon->lastVertex = currentVertexPoint->next;
                    currentVertexPoint = currentVertexPoint->next; //set the current vertex
                } //END IF
            } //END IF
        } //END FOR
        //set the current polygons next to a new polygon
        currentPolygon->next = malloc(sizeof(Polygon));
        //initialize new polygon to null
        currentPolygon->next->firstVertex = NULL;
        currentPolygon->next->lastVertex = NULL;
        currentPolygon->next->next = NULL;
        //set the last polygon to be this polygon
        //the last time this is updated, the last polygon will be set
        outputSet->lastPolygon = currentPolygon;
        //move to the next polygon
        currentPolygon = currentPolygon->next;
    } //END FOR       
} //END cleanUpPolygons



// This procedure must free up all of the memory that was allocated to create the vertices, polygons and polygon set
void freePolygonSet(PolygonSet *pSet) {
    //set pointer variables

    //points to the previously iterated polygon, set to the first for now
	Polygon * previousPolygon = pSet->firstPolygon;
    //points to the previously iterated vertex, set to the first for now
	Vertex * previousVertex = previousPolygon->firstVertex;
    //set to the currently iterated polygon
	Polygon * currentPolygon = pSet->firstPolygon;
    //contains the currently iterated vertes
	Vertex * currentVertex;

	//iterate through the polygones in the 
	while (currentPolygon != NULL) {
        //this sets the currently iterated vertex
		currentVertex = currentPolygon->firstVertex;
		//iterate through the verticies in the current polygon
		while (currentVertex != NULL) {
            //take the previously iterated vertes
			previousVertex = currentVertex;
            //set the newly iterated vertex
			currentVertex = currentVertex->next;
            //free the previously iterated vertex
			free(previousVertex);
		} //END WHILE
        //sets the previously iterated polygon
		previousPolygon = currentPolygon;
        //sets the newly iterated polygon
		currentPolygon = currentPolygon->next;
        //frees the previously iterated polygon
		free(previousPolygon);
    } //END WHILE
} //END freePolygonSet


// Reset the set by freeing its polygons and starting as an empty set again
void resetPolygonSet(PolygonSet *pSet) {
  freePolygonSet(pSet);
  pSet->firstPolygon = NULL;
  pSet->lastPolygon = NULL;
}

// This is where it all begins
int main() {
  // Allocate a PolygonSet to store the original traced border points
  PolygonSet *initPolygons = malloc(sizeof(PolygonSet));
  initPolygons->firstPolygon = NULL;
  initPolygons->lastPolygon = NULL;
   
  // Allocate a PolygonSet to store the simplified/processed polygons
  PolygonSet *finalPolygons = malloc(sizeof(PolygonSet));
  finalPolygons->firstPolygon = NULL;
  finalPolygons->lastPolygon = NULL;


  char grids[5][HEIGHT][WIDTH] = {
    {"000000000000000000000000000000",
     "000111111111111111111111110000",
     "000111111111111111111111110000",
     "000111111110000000111111110000",
     "000111111110000000111111110000",
     "000111111110000000111111110000",
     "000111111110000000111111110000",
     "000111111110000000111111110000",
     "000111111110000000111111110000",
     "000111111111111111111111110000",
     "000111111111111111111111110000",
     "000011111111111111111111100000",
     "000001111111111111111111000000",
     "000000111111111111111110000000",
     "000000011111111111111100000000",
     "000000001111111111111000000000",
     "000000000000111110000000000000",
     "000000000000111110000000000000",
     "000000000000111110000000000000",
     "001111111111111111111111111100",
     "001111111111111111111111111100",
     "001111111111000000111111111100",
     "001111111111000000111111111100",
     "001111111111000000111111111100",
     "001111111111000000111111111100",
     "000011111000000000000111000000",
     "000011111000000000000111000000",
     "000011111000000000000111000000",
     "000000000000000000000000000000",
     "000000000000000000000000000000"},
     
    {"000000000000000000000000000000",
     "000000000000000000000000000000",
     "000000000111111000000000000000",
     "000000011111111111110000000000",
     "000001111111111000000000110000",
     "000001111111000000000001110000",
     "000001111111000000000000111000",
     "000000011111000000000000000000",
     "000000011111111000000000000000",
     "000000000111111110000000000000",
     "000000111111111111111100000000",
     "000111111111111111111111000000",
     "001111111111111111111000000000",
     "000111111100011111111110000000",
     "011111100000111111111000000000",
     "001111110000000011111111000000",
     "001111110000000011111100000000",
     "000011110000000111111000000000",
     "000001110000000111000000000000",
     "000000010000000110000000000000",
     "000000000000000000000000110000",
     "000000000000000000000000000000",
     "000000011111111110000000000000",
     "001000000011111111110000001000",
     "001000000000011111111000000000",
     "001000000000001111111100000000",
     "001000000001111110000000001000",
     "001111000000011110000000011000",
     "000000000000000110000000000000",
     "000000000000000000000000000000"},
      
    {"000000000000000000000000000000",
     "001110001110000110001000100100",
     "001110001110000110001000100000",
     "001110001110000110001000000000",
     "000000000000000000000000000000",
     "001111111111111111111111111100",
     "000000000000000000000000000000",
     "000000000000000000000000000000",
     "000000011111111111111100000000",
     "000000010101010101010100000000",
     "000000010101010101010100000000",
     "000000010101010101010100000000",
     "000000000000000000000000000000",
     "000000100000000000000010000000",
     "000000010000011100000100000000",
     "000000001000001000001000000000",
     "000000000100001000010000000000",
     "000000000010000000100000000000",
     "000000000001000001000000000000",
     "000000000000100010000000000000",
     "000000000000000000000000000000",
     "000000000000000000000000000000",
     "000001111111111111111111100000",
     "000001111110000000000000000000",
     "000000111111111100000000000000",
     "000000001111111111110000000000",
     "000000000001111111111110000000",
     "000000000000000111111111100000",
     "000000000000000000001111110000",
     "000000000000000000000000000000"},
      
    {"000000000000000000000000000000",
     "001111111111111111111111111100",
     "001111111111111111111111111100",
     "001111111111111111111111111100",
     "001110000000000000000000000000",
     "001110111111111111111111111110",
     "001110111111111111111111111110",
     "001110111111111111111111111110",
     "001110111000000000000000001110",
     "001110111011111111111111101110",
     "001110111011111111111111101110",
     "001110111011111111111111101110",
     "001110111011100000000011101110",
     "001110111011101111111011101110",
     "001110111011101111111011101110",
     "001110111011101111111011101110",
     "001110111011101110111011101110",
     "001110111011100000111011101110",
     "001110111011111111111011101110",
     "001110111011111111111011101110",
     "001110111011111111111011101110",
     "001110111000000000000011101110",
     "001110111111111111111111101110",
     "001110111111111111111111101110",
     "001110111111111111111111101110",
     "001110000000000000000000001110",
     "001111111111111111111111111110",
     "001111111111111111111111111110",
     "001111111111111111111111111110",
     "000000000000000000000000000000"},
	
    {"000000000000000000000000000000",
     "000001100000000000000000111000",
     "000011110000000000000011110000",
     "000111111000000000011111100000",
     "000111111000000111111110000000",
     "000001111000001111111000000000",
     "000000011000111111000000000000",
     "000000001000001111000000000000",
     "000000000000000011000000000000",
     "000111100000000000000000000000",
     "000111000000000000000000000000",
     "000111000001111111111111110000",
     "000111000000111111111111110000",
     "000011000001111111111111110000",
     "000011000000001111111111110000",
     "000011000001111111111111110000",
     "000010000001111100000000000000",
     "000010000001111100000000000000",
     "000010000000111110000000000000", 
     "000011000000011111111111100000",
     "000011100000000011111111100000",
     "000001100000000000111111100000",
     "000001111000000000000111100000",
     "000000111000000000000111000000",
     "000000011000000000000111000000",
     "000000000000011111111111000000",
     "000000000000011111111100000000",
     "000000000000000000111000000000",
     "000000000000000000000000000000",
     "000000000000000000000000000000"}};

  // Open a display window
  openDisplayWindow();

  // Run the tests on the 5 grids
  for (int i=0; i<5; i++) {
    // Draw the original grid
    drawGrid(grids[i]);

    // Compute the boundaries and redraw the grid
    computeBoundaries(grids[i], initPolygons);
    drawGrid(grids[i]);

    // Now draw the boundaries as polygons
    drawPolygonSet(grids[i], initPolygons);

    // Clean up the polygons and redraw them
    cleanUpPolygons(initPolygons, finalPolygons);
    drawPolygonSet(grids[i], finalPolygons);
    
    // Reset to get reday for the next grid test
    resetPolygonSet(initPolygons);
    resetPolygonSet(finalPolygons);
  }

  // Free up the allocated memory
  free(initPolygons);
  free(finalPolygons);

  // Close the display window
  closeDisplayWindow();
}

