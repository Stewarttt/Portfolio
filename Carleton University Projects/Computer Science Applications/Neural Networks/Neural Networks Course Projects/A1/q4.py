"""
Name: Connor Raymond Stewart
ID: 101041125
Class: COMP 4107B
Term: Winter 2021
File: q4.py
Description: Solves Question Four Part C from Assignment One
"""

#import the python numpy package for the SVD, and refer to it as np as is standard
import numpy as np

#define the matrix (named A) from the problem
#Internal []-Braces represent row spaces, and commas within the []-Braces represent column separations
#The external []-Braces represents the entire matrix space
A = np.matrix([[3, 2, -1, 4], [1, 0, 2, 3], [-2, -2, 3, -1]])

#the standard matrix does not exist for a non-square matrix
#however, we still can get the psudo-inverse of the matrix
print(np.linalg.pinv(A))
