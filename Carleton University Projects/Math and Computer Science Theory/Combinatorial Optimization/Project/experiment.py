#imported modules
import random

"""
Name:
    getSymmetricMatrix
Input:
    size - this is the length and width of the matrix
    start - this is the starting value for city distances
    end - this is the ending value for city distances
Output:
    Outputs a symmetric matrix representing the distances between cities
Description:
    This function takes a size to represent the size of a symmetric matrix
    It also takes a range (start, end) to represent random number generation values
    It returns a symmetric matrix representation of a Graph = G(V, E) for a 2-Opt algorithm
Algorithm:
    This is based on the assumption that:
        A is symmetric if and only if A = A-transpose
"""
def getSymmetricMatrix(size, start=0, end=1) :
    #create a square matrix based on the size specified
    matrix = [[None for column in range(size)] for row in range(size)]

    #for each row in the matrix
    for row in range(len(matrix)) :
        #for each column in the matrix
        for column in range(len(matrix[row])) :
            #if the current row equals the column index, it is a diagonal entry
            if column == row :
                #assign a value of zero since a city has no distance from itself
                matrix[row][column] = 0
            elif row > column : #if we are looking at the left side of the diagonal
                #assign the left side of the diagonal a random distance value
                matrix[row][column] = int(round(random.uniform(start, end) * 100, 0))
                #assign the right side the same value such that the matrix is symmetric
                matrix[column][row] = int(matrix[row][column])
            #END IF
        #END FOR
    #END FOR

    #return the final matrix to the caller
    return matrix
#END getSymmetricMatrix

"""
Name:
    getDistance
Input:
    matrix - this is the symmetric matrix representing the cities
    tour - this is the current loop through all the cities
Output:
    totalDistance - this is the total distance between all the cities in the loop
Description:
    gets the distance for a given tour between all the cities
"""
def getDistance(matrix, tour) :
    #tracks the current distance between travelled
    totalDistance = 0

    #for each edge in the loop of cities
    for trip in range(len(tour) - 1) :
        #tally the distance between the two points
        totalDistance += matrix[tour[trip]][tour[trip + 1]]
    #END FOR

    #returns the total distance between all the traversed cities
    return totalDistance
#END getDistance

"""
Name:
    twoOptSwap
Input:
    tour - a set of cities connected as a linked list (i.e. 1->2->etc.)
    i - the first edge being swaped
    k - the second edge being swapped
Output:
    newTour - this is the new set of edges for the TSP
Description:
    changes two edges in a tour for a 2-Opt TSP algorithm and returns a the new tour
"""
def twoOptSwap(tour, i, k) :
    #contains the new tour
    reverse = []
    newTour = []
    firstPass = True

    #for each edge in the list of all edges
    for edge in range(len(tour)) :
        #if the edge is less then the current value i
        if edge < i :
            newTour.append(tour[edge])
        #add these values in reverse order to change the path
        elif edge >= i and edge <= k :
            reverse.append(tour[edge])
        #add the ramining values in order
        elif edge >= k + 1 :
            #add the reverse path if it has not been added yet
            if firstPass :
                newTour = newTour + reverse[::-1]
                firstPass = False
            #END IF
            newTour.append(tour[edge])
        #END IF
    #END FOR

    #add the reverse path if it has not been added yet
    if firstPass :
        newTour = newTour + reverse[::-1]
        firstPass = False
    #END IF

    #returns the newly generated tour
    return newTour
#END twoOptSwap

"""
Name:
    twoOptLoop
Input:
    matrix - symmetric matrix representing the city
Output:
    bestTour - this is the shortest path which connect all the cities in the loop
    bestDistance - this is the distanced traversed by the bestTour path above
Description:
    Runs a 2-Opt Algorithm on the dataset in order to determine the appoximate solution for the TSP
Algorithm:
    This is an implementation of the K-Opt algorithm for the TSP, with K equal to two
"""
def twoOptLoop(matrix) :
    #holds the current tours
    bestTour = []
    bestDistance = 0

    #create a default path between the nodes in order
    for row in range(len(matrix)) :
        bestTour.append(row)
    #END FOR
    bestTour.append(0)

    #append the last connection between the ending and starting node

    #checks the distance of the current tour
    bestDistance = getDistance(matrix, bestTour)

    #checks if an improvement was made this iteration
    improvement = True

    while improvement : #run the loop for as long as an improvement is made
        improvement = False #no improvement noted this iteration
        for i in range(len(bestTour)) : #checks each row in the matrix
            for k in range(i + 1, len(bestTour)) : #checks each column in the matrix
                #gets a new tour pointer
                newTour = list(bestTour)

                #gets a new 2-Opt path
                newTour = twoOptSwap(newTour, i, k)

                #checks if the new path is valid
                error = False
                for element in range(len(newTour) - 1) :
                    if newTour[element] == newTour[element + 1] :
                        #we cannot move nowhere
                        error = True
                    #END IF
                #END FOR

                #move to the next iteration of the loop
                if error : continue

                #gets new tour distance
                newDistance = getDistance(matrix, newTour)

                #finds if the new path is better then the old one
                if bestDistance > newDistance :
                    #replaces the old path with the new path
                    bestDistance = newDistance
                    bestTour = newTour
                    improvement = True
                    #leave the current loop
                    break
                #END IF
            #END FOR

            #checks if an improvement was made this iteration
            if improvement == True :
                #go to the topmost loop
                break
            #END IF
        #END FOR
    #END WHILE
    return [bestTour, bestDistance]
#END twoOptLoop


#---Main (Entry Point Driver)---#

#gets a random symmetric matrix (of length five) and runs the 2-Opt on it
matrix = getSymmetricMatrix(5)
print("A randomly generated matrix of five elements is as follows:")
for element in matrix : print(element)
results = twoOptLoop(matrix)
#returns the results of the 2-Opt search
print("We get the following results for the 2-Opt algorithm")
print("Optimal path:")
print(results[0])
print("Optimal distance:")
print(results[1])

#gets a random symmetric matrix (of length five) and runs the 2-Opt on it
matrix = getSymmetricMatrix(25)
print("A randomly generated matrix of 25 elements is as follows:")
#--uncomment the code to get the matrix generated
#for element in matrix : print(element)
results = twoOptLoop(matrix)
#returns the results of the 2-Opt search
print("We get the following results for the 2-Opt algorithm")
print("Optimal path:")
print(results[0])
print("Optimal distance:")
print(results[1])

#gets a random symmetric matrix (of length five) and runs the 2-Opt on it
matrix = getSymmetricMatrix(100)
print("A randomly generated matrix of 100 elements is as follows:")
#--uncomment the code to get the matrix generated
#for element in matrix : print(element)
results = twoOptLoop(matrix)
#returns the results of the 2-Opt search
print("We get the following results for the 2-Opt algorithm")
print("Optimal path:")
print(results[0])
print("Optimal distance:")
print(results[1])

#gets a random symmetric matrix (of length five) and runs the 2-Opt on it
matrix = getSymmetricMatrix(500)
print("A randomly generated matrix of 500 elements is as follows:")
#--uncomment the code to get the matrix generated
#for element in matrix : print(element)
results = twoOptLoop(matrix)
#returns the results of the 2-Opt search
print("We get the following results for the 2-Opt algorithm")
print("Optimal path:")
print(results[0])
print("Optimal distance:")
print(results[1])

#gets a random symmetric matrix (of length one thousand) and runs the 2-Opt on it
matrix = getSymmetricMatrix(1000)
#--uncomment the code to get the matrix generated
#print("A randomly generated matrix of one thousand elements is as follows:")
#output = ""
#for element in matrix : output += str(element) + "\n"
#print(output)
results = twoOptLoop(matrix)
#returns the results of the 2-Opt search
print("We get the following results for the 2-Opt algorithm")
print("Optimal path:")
print(results[0])
print("Optimal distance:")
print(results[1])
