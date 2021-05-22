"""
Name: Connor Raymond Stewart
ID: 101041125
Class: COMP 4107B
Term: Winter 2021
File: q2.py
Description: Solves Question Two from Assignment One
"""

#import the python numpy package for the SVD, and refer to it as np as is standard
import numpy as np
import math

#create a surface matrix for the problem
surface = np.tile(0.0, (1401, 1401))

for deltaX in range(1, 1402) : #for all numbers from 1 to 1401
    for deltaY in range(1, 1402) : #for all numbers from 1 to 1401
        changeInX = -0.7 + 0.001*(deltaX-1) #find the change in x
        changeInY = -0.7 + 0.001*(deltaY-1) #find the change in y
        #subtract one since the matrix is zero indexed
        surface[deltaX-1, deltaY-1] = math.sqrt(1-changeInX**2-changeInY**2) #find z and place it into the surface
    #END FOR
#END FOR

#Take the SVD of the matrix to get the matrix
#calculates the svd for the matrix. full_matrices, compute_uv are true by default, and hermitian is false by default
svdMatrix = np.linalg.svd(surface)

#build a matrix using the length of the U-matrix by the length of the V^T-matrix
matrixDim = (len(svdMatrix[0]), len(svdMatrix[2]))
A2 = np.zeros(matrixDim)

#fill in the A2 matrix
#multiply by the first rank in the diagonal
A2 = A2 + svdMatrix[1][0]*np.outer(svdMatrix[0].transpose()[0], svdMatrix[2][0].transpose())
#multiply by the second rank in the diagonal
A2 = A2 + svdMatrix[1][1]*np.outer(svdMatrix[0].transpose()[1], svdMatrix[2][1].transpose())

#finds the norm values for the ||A - A_2|| and ||A||
norm_A2 = np.linalg.norm(surface-A2) #finds ||A - A_2||
norm_A = np.linalg.norm(A2) #finds ||A||

#Output the following rank(2) matrix
print("The Rank(2) Matrix is:")
print(A2)
#Output the following norm_2
print("The norm ||A-A_2|| =", norm_A2)
#Output the differance between the norm 
print("Since ||A|| = " + str(norm_A) + ", we know that ||A-A_2|| / ||A|| =", norm_A2 / norm_A)
