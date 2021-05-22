"""
Name: Connor Raymond Stewart
ID: 101041125
Class: COMP 4107B
Term: Winter 2021
File: q3.py
Description: Solves Question One from Assignment One
"""

#import the python numpy package for the SVD, and refer to it as np as is standard
import numpy as np


"""
Name: leastSquares
Input:
    epsilon - this is a weighting to change the x value by per iteration
    randomX - this is a initial x-vector for the program
Output:
    Returns a least-squares estimation via the gradient descent method
    Returns the number of iterations the algorithmic loop ran for
    Return a False if divergance occured, and a True otherwise
"""
def leastSquares(epsilon, randomX) :
    #define the matrix (named A) from the problem
    #Internal []-Braces represent row spaces, and commas within the []-Braces represent column separations
    #The external []-Braces represents the entire matrix space
    A = np.matrix([[1, 2, 3], [2, 3, 4], [4, 5, 6], [1, 1, 1]])
    b = np.matrix([1,1,1,1]).transpose() #take the transpose to get the vector

    #delta value for the algorithm
    delta = 0.01

    #counts the number of iterations in the loop
    iterations = 0

    #initialize the x vector to be a three unit random vector
    x = randomX

    #note that .norm gives the Euclidean distance, which is norm type 2
    #run the least squares algorithm 
    while np.linalg.norm((np.dot(np.dot((A.transpose()), A),x) - np.dot(A.transpose(),b)), 2) > delta :
        #set the new value of x to be this
        x = x - epsilon*(np.dot(np.dot((A.transpose()), A),x) - np.dot(A.transpose(),b))
        #tally an iteration
        iterations += 1

        #test for overfitting using boundary-range
        if max(x[0], x[1], x[2]) >= 120000 :
            return [x, iterations, False]
        #END IF
    #END WHILE

    #returns the result of the least squares algorithm
    return [x, iterations, True]
#END leastSquares

#define a list containing all the epsilon values
epsilonList = [0.01,0.05,0.1,0.15,0.2,0.25,0.5]
#initialize the x vector to be a three unit random vector
randomX = np.random.rand(3, 1)

#for each epsilon value in the list of epsilon values
for epsilonValue in epsilonList :
    #run the algorithm on the dataset to get the results
    print("Using an epsilon of", epsilonValue, "we see that we get the following least-squares vector: ")
    results = leastSquares(epsilonValue, randomX) #store the results

    #output results based on model divergance or convergance
    if results[2] == True : #if the model does not diverge
        print("---Success---(results below)")
        print(results[0])
        print("After", results[1], "iterations in the loop")
    else : #if the model diverges
        print("---Divergence---(last estimate below)")
        print(results[0])
        print("After", results[1], "iterations in the loop")
    #END IF
#END FOR
