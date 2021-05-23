"""
Name: Connor Raymond Stewart
ID: 101041125
Class: COMP 4107B
Term: Winter 2021
File: q1.py
Description: Solves Question One from Assignment One
"""

#import the python numpy package for the SVD, and refer to it as np as is standard
import numpy as np

#define the matrix (named A) from the problem
#Internal []-Braces represent row spaces, and commas within the []-Braces represent column separations
#The external []-Braces represents the entire matrix space
A = np.matrix([[1, 2, 3], [2, 3, 4], [4, 5, 6], [1, 1, 1]])

#calculates the svd for the matrix. full_matrices, compute_uv are true by default, and hermitian is false by default
resultingMatrixSVD = np.linalg.svd(A)

#print the resulting matrixes to the terminal for review
for matrix in resultingMatrixSVD :
    #round to the eight decimal place for clearity
    print(matrix.round(decimals=8))
#END FOR
