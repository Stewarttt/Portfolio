"""
Name: Connor Raymond Stewart
ID: 101041125
Class: COMP 4107B
Term: Winter 2021
File: q5.py
Description: Solves Question Five from Assignment One
"""

#import python modules
import numpy as np
import gzip
import random
import array
import matplotlib.pyplot as plt

"""
Name:
    parseData
Input:
    the name of the file to read
Output:
    A zero-matrix of the dimensions and data type specified within the files
Description:
    reads the input gzip files and uses the data to form arrays
Formating Information: (see documentation on: http://yann.lecun.com/exdb/mnist/ for more information)
    The basic format is

    magic number
    size in dimension 0
    size in dimension 1
    size in dimension 2
    .....
    size in dimension N
    data

    The magic number is an integer (MSB first). The first 2 bytes are always 0.

    The third byte codes the type of the data:
    0x08: unsigned byte
    0x09: signed byte
    0x0B: short (2 bytes)
    0x0C: int (4 bytes)
    0x0D: float (4 bytes)
    0x0E: double (8 bytes)

    The 4-th byte codes the number of dimensions of the vector/matrix: 1 for vectors, 2 for matrices....

    The sizes in each dimension are 4-byte integers (MSB first, high endian, like in most non-Intel processors).

    The data is stored like in a C array, i.e. the index in the last dimension changes the fastest.
"""
def parseData(file) :
    #open the file and store its pointer, read file as a binary gzip file
    filePointer = gzip.open(file,'rb')

    #Get the header contents from the first four bytes
    magicNumberBin = filePointer.read(2) #read the first two bytes for the magic number
    dataTypeBin = filePointer.read(1) #reads the data type at the third byte
    dimensionNumberBin = filePointer.read(1) #the fourth bytes codes for the dimension number for the matrix/vector (1=vectors, 2=matrices)

    #The magic number is an integer with MSB first, meaning it is big-endian
    #magicNumber = int.from_bytes(magicNumberBin, "big")
    #dataType = int.from_bytes(dataTypeBin, "big")
    dimensionNumber = int.from_bytes(dimensionNumberBin, "big")

    #The next dimensionNumber bytes are all big-endian and unsigned, read all three
    matrixDimensions = []

    #for all the numbers represented by the dimensionNumber variable
    for numbers in range(0, dimensionNumber) :
        matrixDimensions.append(int.from_bytes(filePointer.read(4), "big"))
    #END FOR

    #convert data to a tuple for numpy input
    matrixDimensions = tuple(matrixDimensions)

    #gets the remaining data from the dataset
    dataRaw = filePointer.read()

    #get the proper data type given the hex-value of the data type binary
    if dataTypeBin == b'\x08' :
        dataType = 'B' #unsigned byte data type
    elif dataTypeBin == b'\x09' :
        dataType = 'b' #signed byte data type
    elif dataTypeBin == b'\x0B' :
        dataType = 'h' #short data type
    elif dataTypeBin == b'\x0C' :
        dataType = 'i' #integer data type
    elif dataTypeBin == b'\x0D' :
        dataType = 'f' #float data type
    elif dataTypeBin == b'\x0E' :
        dataType = 'd' #double data type
    #END IF
    #reads the data into an array data type using the data storage type and converts it into a matrix based on the specified matrix dimension values
    return np.array(array.array(dataType, dataRaw)).reshape(matrixDimensions)
#END parseData

"""
Name:
    randomMatrix
Input:
    trainingLabelMatrix - the training labels
    trainingImageMatrix - the training images
    imageNumberBeingTested - number value currently being tested for
Output:
    A random matrix of values partaining to the desired number given the label and image matrices
Description:
    Gets a randomized matrix data set for the number being tested
"""
def randomMatrix(trainingLabelMatrix, trainingImageMatrix, imageNumberBeingTested) :
    #organize the matrix dimensions
    randMatrix = np.zeros(28**2)
    #the currently iterated index
    testIndex = random.randint(0, 59999)
    #data already used
    exclusion = []
    #number of successful iterations before termination, lower number is faster, higher is more precise
    while len(randMatrix)-1 < 1000 :
        #if the training label matdhes the number being looked for and has not been used before
        if trainingLabelMatrix[testIndex] == imageNumberBeingTested and testIndex not in exclusion :
            #stack digits into a single column
            randMatrix = np.vstack((randMatrix, columnSpace(trainingImageMatrix[testIndex])))
            #add to list of used labels
            exclusion.append(testIndex)
        else :
            #try to finda new number
            testIndex = random.randint(0, 59999)
        #END IF
    #END FOR
    return randMatrix[1:].transpose()
#END randomMatrix

#returns a matrix sqeezed into a column space
def columnSpace(matrix) :
    #transpose the current matrix
    matrixTranspose = matrix.transpose()
    #create a temporary array for the program
    tempArray = np.array([])
    #for each element in the array
    for i in range(len(matrixTranspose)):
        #add to the temp array
        tempArray = np.append(tempArray, matrixTranspose[i])
    #END FOR
    #return the temp array
    return tempArray
#END columnSpace

#returns the unit vector based on the SVD method
def unitVector(matrix) :
    return np.linalg.svd(matrix, False)[0]
#END unitVector

"""
Name:
    correctness
Input:
    shuffle_array - an array of shuffed data values for the number
    imageNumber - the number currently being tested for
Output:
    A percentage value representing the number of correctly matched images
Description:
    Validates the matching accuracy
"""
def correctness(shuffle_array, imageNumber) :
    correctImages = 0
    testedImages = 200
    print("---Testing image number:" + str(imageNumber))
    for iteration in range(testedImages):
        index = random.randint(0, 9999)

        #gets the information
        image = testingImageMatrix[index]
        label = testingLabelMatrix[index]

        #store the residuals in this array
        residuals = np.array([])

        #converts the data into a single column space        
        vectorSpace = columnSpace(image)

        
        #compare image against the unit vector
        for rowSpace in shuffle_array:
            #gets the residual value for the row and column vectors
            delta = np.linalg.norm(np.dot(np.identity(784) - np.dot(rowSpace,np.transpose(rowSpace)),vectorSpace), 2)
            residuals = np.append(residuals,delta)
        #END FOR

        if label == residuals.argmin() :
            correctImages += 1
        #END IF
    #END FOR

    #output the percentage of correctly matched images
    print(100*(correctImages/testedImages))

    #converts the ratio of correct images to tested images to a percentage
    return (correctImages/testedImages)
#END correctness

"""
Name:
    plotResults
Input:
    percents - the percent accuracy for respective hyperparameter values
Output:
    Outputs a matplotlib representation of the dataset
Description:
    Uses the matplotlib API to plot the results of the program
"""
def plotResults(percents) :
    #settings for the graph figure
    figure = plt.figure(figsize=(16,9))

    #configure plot settings
    plt.title("Hyperparameter (K) vs. Average Classification Accuracy (Percentage)")
    plt.xlabel("# of Basis Images") #x axis label
    plt.ylabel("Classification Percentage") #y axis label
    plt.grid(True) #turn grids on

    #formatted x axis for matplotlib output
    outputList = [integer for integer in range(len(percents))]

    #plot the results onto the plot
    plt.plot(outputList, percents, marker = "o")

    #display the results of the plot
    plt.show()
#END plotResults

#---Main Program Entry Point---#

#import datasets from the current working directory
trainingImagePath = "train-images-idx3-ubyte.gz"
testingImagePath = "t10k-images-idx3-ubyte.gz"
trainingLabelPath = "train-labels-idx1-ubyte.gz"
testingLabelPath = "t10k-labels-idx1-ubyte.gz"

#get matrices of the dimensions specified within the files
trainingImageMatrix = parseData(trainingImagePath)
testingImageMatrix = parseData(testingImagePath)
trainingLabelMatrix = parseData(trainingLabelPath)
testingLabelMatrix = parseData(testingLabelPath)

#create an array to store ten testing sets
A_arrays = []
U_arrays = []
shuffle_array = []
correctnessRate = []

#for each number photo from 0 to 9
for imageNumber in range(10) :
    A_arrays.append(randomMatrix(trainingLabelMatrix, trainingImageMatrix, imageNumber))
    U_arrays.append(unitVector(A_arrays[imageNumber]))
#END FOR

#for the hyperparameters being tested
for hyperparameter in range(51) :
    for unitVector in range(10) :
        shuffle_array.append(U_arrays[unitVector][:,0:hyperparameter+1])
    #END FOR
    correctnessRate.append(correctness(np.asarray(shuffle_array), hyperparameter)*100)
    #END FOR
    shuffle_array = []
#END FOR

#print the results to the command-line interface
for result in range(len(correctnessRate)) :
    print("Basis Image #" + str(result) + " = " + str(correctnessRate[result]) + " percent")
#END FOR

#plots the results using matplotlibs API
plotResults(correctnessRate)
