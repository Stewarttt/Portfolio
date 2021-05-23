import tensorflow.compat.v1 as tensor
import numpy
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import math
import time
import random

tensor.disable_eager_execution()
tensor.disable_v2_behavior()

#define the function
def f(x,y) :
    return numpy.cos(x+6*0.35*y) + 2*0.35*x*y
#END f

def randValidation(dim) :
    #randomly generates a validation set
    validationSetDomain = numpy.linspace(-1, 1, 10)
    validationSetRange = numpy.linspace(-1, 1, 10)
    validationSets = numpy.meshgrid(validationSetDomain, validationSetRange)
    validationResults = f(validationSets[0], validationSets[1])
    validationResults = numpy.vstack(validationResults.ravel())
    validationCoordinates = (numpy.vstack(validationSets[0].ravel()), numpy.vstack(validationSets[1].ravel()))
    validationXY = numpy.concatenate(validationCoordinates, 1)
    validationXYZ = numpy.c_[validationXY, validationResults]

    #return the validation set in an XYZ columnization
    return validationXYZ
#END randValidation
        

def getSets() :
    #we define the grids for the training sets
    trainingSetDomain = numpy.linspace(-1, 1, 10)
    trainingSetRange = numpy.linspace(-1, 1, 10)
    trainingSets = None

    #we deine the grids for teh testing sets
    testingSetDomain = numpy.linspace(-1, 1, 9)
    testingSetRange = numpy.linspace(-1, 1, 9)
    testingSets = None

    #we create some grids with the building numpy libraries
    trainingSets = numpy.meshgrid(trainingSetDomain, trainingSetRange)
    testingSets = numpy.meshgrid(testingSetDomain, testingSetRange)

    #get the contor values
    trainingResults = f(trainingSets[0], trainingSets[1])
    testingResults = f(testingSets[0], testingSets[1])

    #flatten the arrays and columnize them
    trainingResults = numpy.vstack(trainingResults.ravel())
    testingResults = numpy.vstack(testingResults.ravel())

    #add rows for the x,y pairs in the newly created datasets
    trainingCoordinates = (numpy.vstack(trainingSets[0].ravel()), numpy.vstack(trainingSets[1].ravel()))
    testingCoordinates = (numpy.vstack(testingSets[0].ravel()), numpy.vstack(testingSets[1].ravel()))

    #gets the training and testing set data for the following
    trainingXY = numpy.concatenate(trainingCoordinates, 1)
    testingXY = numpy.concatenate(testingCoordinates, 1)

    #return the sets
    return [trainingXY, testingXY, trainingResults, testingResults]
#END getSets

def networkStruct(hiddenLayers, epochs, learningRate, dev, mode) :
    #data type the tensors are to store data in
    dataType = tensor.float32

    #defines input and output neurons ((x,y), z)
    inputNeuron = 2
    outputNeuron = 1

    #define the input and output datasets
    inputSet = tensor.placeholder(dataType)
    outputSet = tensor.placeholder(dataType)

    #defines the weights for the network
    weightOne = tensor.Variable(tensor.random_normal([inputNeuron, hiddenLayers], stddev=dev))
    weightTwo = tensor.Variable(tensor.random_normal([hiddenLayers, outputNeuron], stddev=dev))

    #define the biases for the network
    biasOne = tensor.Variable(tensor.zeros([hiddenLayers]))
    biasTwo = tensor.Variable(tensor.zeros([outputNeuron]))

    #define the hidden layer
    hiddenLayer = tensor.sigmoid(tensor.matmul(inputSet, weightOne) + biasOne)
    linearOutput = tensor.matmul(hiddenLayer, weightTwo) + biasTwo

    #gets the network error
    cost = tensor.nn.tanh(tensor.losses.mean_squared_error(outputSet, linearOutput))

    if mode == 0 :
        #optimize the network by gradient descent
        optimization = tensor.train.GradientDescentOptimizer(learningRate).minimize(cost)
    elif mode == 1 :
        optimization = tensor.train.MomentumOptimizer(learningRate, 0.9).minimize(cost)
    elif mode == 2 :
        optimization = tensor.train.RMSPropOptimizer(learningRate, 0.999).minimize(cost)
    elif mode == 3 :
        optimization = tensor.train.RMSPropOptimizer(learningRate, 0.99).minimize(cost)
    elif mode == 4 :
        optimization = tensor.train.RMSPropOptimizer(learningRate, 0.999).minimize(cost)
    #END IF

    return [inputSet, outputSet, cost, optimization, linearOutput, hiddenLayer]
#END networkStruct

def runSession(networkStructure, epochs, trainingXY, outputShuffle, testType, extraData=None) :
    #initialize the tensor environment
    init = tensor.global_variables_initializer()
    #display progress every 500 steps
    displayStep = 500

    #contains a list of elements for the results
    resultsList = []
    testing = []
    training = []
    goalList = []
    validationList = []
    #counts number of runs where MSE increased
    failedRuns = 0
    totalIter = -1
    threshold = -1
    #generate a validation set
    validations = randValidation(epochs)

    #run the session
    with tensor.Session() as session :
        #instantiate the session run envionment
        session.run(init)
        #for each iteration of the program
        for iteration in range(epochs) :
            #get the results of the network
            results = session.run([networkStructure[3], networkStructure[2]], feed_dict = {networkStructure[0] : trainingXY, networkStructure[1]: outputShuffle})
            #display progress to the user
            if iteration % displayStep == 0:
                print("COST at step " + str(iteration) + ": " + str(results[1]))
            #END IF
            resultsList.append(results[1])

            #if extra information is listed
            if extraData != None :
                #arrange the testing values
                testingValues = numpy.c_[extraData[1], extraData[3]]
                #get the testing results
                predRes = session.run([networkStructure[4]], feed_dict = {networkStructure[0] : testingValues[:,:2]})[0]
                predRes = [element[0] for element in predRes]
                testing.append(numpy.square(predRes - testingValues[:,2]).mean())

                #get the validation data predictions
                validRes = session.run([networkStructure[4]], feed_dict = {networkStructure[0] : validations[:,:2]})[0]
                validRes = [element[0] for element in validRes]
                validationList.append(numpy.square(validRes - validations[:,2]).mean())

                #get the goal data predictions
                goalRes = session.run([networkStructure[4]], feed_dict = {networkStructure[0] : extraData[4][:,:2]})[0]
                goalRes = [element[0] for element in goalRes]
                goalList.append(numpy.square(goalRes - extraData[4][:,2]).mean())
            #END IF

            #if we are stopping upon reaching the error threashold
            if testType[0] == 1 and results[1] <= testType[1] :
                totalIter = iteration
                break
            elif testType[0] != 1 and results[1] <= testType[1] and threshold == -1 :
                threshold = iteration
            #END IF
            totalIter = iteration                

            #check if MSE is decreasing
            if iteration != 0 and resultsList[-1] - resultsList[-2] > 0 :
                failedRuns += 1
            else :
                failedRuns = 0
            #END IF

            #exit if we exceed the maximum number of error runs
            if testType[0] == 2 and testType[1] <= failedRuns :
                break
            #END IF
        #END FOR
        #predict the results
        prediction = session.run([networkStructure[4]], feed_dict = {networkStructure[0]: trainingXY})
    #END WITH

    #outputs the prediction results for the following network
    predictedResults = [element[0] for element in prediction[0]]
    meanSquareError = numpy.square(predictedResults - outputShuffle).mean()

    # Reshape prediction into format of testData for comparison [[x, y, z]]
    predictedResults = numpy.concatenate((trainingXY, numpy.array(predictedResults).reshape(len(predictedResults), 1)),axis=1)

    #return the results to the caller
    return [predictedResults, meanSquareError, resultsList, totalIter, threshold, testing, validationList, goalList]
#END runSession

#used to plot the contor graphs
def pltData(data, colour):
    dimensions = int(math.sqrt(len(data)))
    plt.contour(
        data[:, 0].reshape((dimensions, dimensions)), data[:, 1].reshape((dimensions, dimensions)), data[:, 2].reshape((dimensions, dimensions)), 10, colors=colour)
#END pltData

#runs the experiment
def runExperiment(neuronArrays, epochs, learningRate, testMode):
    #define runtime variables
    results = []
    trainingXY, testingXY, trainingResults, testingResults = getSets()

    #inform the user the models paramters
    print("Learning rate is: " + str(learningRate))
    print("Number of epochs is: " + str(epochs))

    #for question a
    if testMode == 0 :
        # Run experiments and get results for each number of neurons
        for neurons in neuronArrays:
            #inform the user the number of neurons being used for the test
            print("---Testing with " + str(neurons) + " neurons---")
            #get the network structure results
            network = networkStruct(neurons, epochs, learningRate, 0.65, 0)

            #assign the results of the model
            sessionResults = runSession(network, epochs, trainingXY, trainingResults, [0,0])
            predictedResults = sessionResults[0]
            meanSquareError = sessionResults[1]
            costs = sessionResults[2]

            #store the results in memory
            results.append((predictedResults, meanSquareError, costs))
        #END FOR

        #convert the results to a contor (XYZ) matrix
        testData = numpy.c_[testingXY, testingResults]

        #return the formatted data    
        return [results, testData, neuronArrays, epochs]
    elif testMode == 1 :
        neurons = 8
        times = []
        #run experiments for each optimizer on an eight neuron network
        for optimizer in [0,1,2] :
            #inform the user the number of neurons being used for the test
            print("---Testing with " + str(neurons) + " neurons---")

            #start timing
            start_time = time.time()

            #get the network structure results
            network = networkStruct(neurons, epochs, learningRate, 0.65, optimizer)

            #get total time in seconds per epoch on average
            totalTime = (time.time() - start_time) / epochs
            times.append(totalTime)

            #assign the results of the model
            sessionResults = runSession(network, epochs, trainingXY, trainingResults, [0,0])
            predictedResults = sessionResults[0]
            meanSquareError = sessionResults[1]
            costs = sessionResults[2]

            #store the results in memory
            results.append((predictedResults, meanSquareError, costs))
        #END FOR

        #convert the results to a contor (XYZ) matrix
        testData = numpy.c_[testingXY, testingResults]

        #return the formatted data    
        return [results, testData, neuronArrays, epochs, times]
    elif testMode == 2 :
        neurons = 8
        times = []
        #run experiments for each optimizer on an eight neuron network
        for optimizer in [0,1,2] :
            #inform the user the number of neurons being used for the test
            print("---Testing with " + str(neurons) + " neurons---")

            #start timing
            start_time = time.time()

            #get the network structure results
            network = networkStruct(neurons, epochs, learningRate, 0.65, optimizer)

            #get total time in seconds per epoch on average
            totalTime = (time.time() - start_time) / epochs
            times.append(totalTime)

            #assign the results of the model
            sessionResults = runSession(network, epochs, trainingXY, trainingResults, [1,0.02])
            predictedResults = sessionResults[0]
            meanSquareError = sessionResults[1]
            costs = sessionResults[2]
            iterationExit = sessionResults[3]

            #store the results in memory
            results.append((predictedResults, meanSquareError, costs, iterationExit))
        #END FOR

        #convert the results to a contor (XYZ) matrix
        testData = numpy.c_[testingXY, testingResults]

        #return the formatted data    
        return [results, testData, neuronArrays, epochs, times]
    elif testMode == 3 :
        # Run experiments and get results for each number of neurons
        for neurons in neuronArrays:
            #inform the user the number of neurons being used for the test
            print("---Testing with " + str(neurons) + " neurons---")
            #get the network structure results
            network = networkStruct(neurons, epochs, learningRate, 0.65, 4)

            #assign the results of the model
            sessionResults = runSession(network, epochs, trainingXY, trainingResults, [0,0.02])
            predictedResults = sessionResults[0]
            meanSquareError = sessionResults[1]
            costs = sessionResults[2]
            firstPoint = sessionResults[4]

            #store the results in memory
            results.append((predictedResults, meanSquareError, costs))
        #END FOR

        #convert the results to a contor (XYZ) matrix
        testData = numpy.c_[testingXY, testingResults]

        #return the formatted data    
        return [results, testData, neuronArrays, epochs, firstPoint]
    elif testMode == 4 :
        # Run experiments and get results for each number of neurons
        for stopping in [0,1]:
            #get the network structure results
            network = networkStruct(8, epochs, learningRate, 0.85, 3)

            #assign the results of the model
            sessionResults = runSession(network, epochs, trainingXY, trainingResults, [stopping,0.02])
            predictedResults = sessionResults[0]
            meanSquareError = sessionResults[1]
            costs = sessionResults[2]

            #store the results in memory
            results.append((predictedResults, meanSquareError, costs))
        #END FOR

        #convert the results to a contor (XYZ) matrix
        testData = numpy.c_[testingXY, testingResults]

        #return the formatted data    
        return [results, testData, neuronArrays, epochs]
    elif testMode == 5 :
        #get the network structure results
        network = networkStruct(8, epochs, learningRate, 0.85, 3)

        #assign the results of the model
        sessionResults = runSession(network, epochs, trainingXY, trainingResults, [0, 0.02], [trainingXY, testingXY, trainingResults, testingResults, numpy.c_[testingXY, testingResults]])
        predictedResults = sessionResults[0]
        meanSquareError = sessionResults[1]
        costs = sessionResults[2]
        threshold = sessionResults[4]
        testing = sessionResults[5]
        validation = sessionResults[6]
        goal = sessionResults[7]

        #store the results in memory
        results.append((predictedResults, meanSquareError, costs))

        #convert the results to a contor (XYZ) matrix
        testData = numpy.c_[testingXY, testingResults]

        #return the formatted data    
        return [results, testData, neuronArrays, epochs, threshold, testing, validation, goal]
    #END IF
#END run

def plot(data) :
    #gets results for the tests
    dataSets = data[0]
    control = data[1]
    neuronArrays = data[2]
    epochs = data[3]

    #runtime parameters for the legend
    legendParameters = []
    legendParameters.append(mpatches.Patch(label='target', color='black'))
    legendParameters.append(mpatches.Patch(label=str(neuronArrays[0]) + ' Neurons', color='red'))
    legendParameters.append(mpatches.Patch(label=str(neuronArrays[1]) + ' Neurons', color='green'))
    legendParameters.append(mpatches.Patch(label=str(neuronArrays[2]) + ' Neurons', color='blue'))
    #instantiate the legends parameters
    plt.legend(handles=legendParameters)

    #plot the data with the correctly coloured lines
    pltData(control, 'black')
    pltData(dataSets[0][0], 'red')
    pltData(dataSets[1][0], 'green')
    pltData(dataSets[2][0], 'blue')

    #show the results of the neural network
    plt.show()

    #show the MSE results for each network
    plt.figure(1)
    plt.subplot(311)
    plt.plot(range(0, epochs), dataSets[0][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')

    plt.subplot(312)
    plt.plot(range(0, epochs), dataSets[1][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')

    plt.subplot(313)
    plt.plot(range(0, epochs), dataSets[2][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')
    plt.show()
#END plot

def plotTwo(data, dataTwo) :
    #gets results for the tests
    dataSets = data[0]
    control = data[1]
    neuronArrays = data[2]
    epochs = data[3]
    times = data[4]

    #show the MSE results for each network
    plt.figure(1)
    plt.subplot(311)
    plt.title('GradientDescentOptimizer', y=0.8)
    plt.plot(range(0, epochs), dataSets[0][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')

    plt.subplot(312)
    plt.title('MomentumOptimizer', y=0.8)
    plt.plot(range(0, epochs), dataSets[1][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')

    plt.subplot(313)
    plt.title('RMSPropOptimizer', y=0.8)
    plt.plot(range(0, epochs), dataSets[2][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')
    plt.show()

    plt.bar([0, 1, 2], times)
    plt.xticks([0, 1, 2], ('GradientDescentOptimizer', 'MomentumOptimizer', 'RMSPropOptimizer'))
    plt.ylabel('CPU time per epoch (s)')
    plt.show()

    #prints the cost for the optimizers at the final step
    print("MSE for optimizers with a learningRate of 0.02 and 100 epochs:")
    print("Cost for GradientDescentOptimizer:", dataSets[0][2][-1])
    print("Cost for MomentumOptimizer:", dataSets[1][2][-1])
    print("Cost for RMSPropOptimizer:", dataSets[2][2][-1])

    #prints the epoch upon which the error threashold is reached
    print("MSE and iteration for learningRate of 0.02 when stopping on training error less then 0.02:")
    print("Cost for GradientDescentOptimizer:", dataTwo[0][0][2][-1], "after", dataTwo[0][0][3])
    print("Cost for MomentumOptimizer:", dataTwo[0][1][2][-1], "after", dataTwo[0][1][3])
    print("Cost for RMSPropOptimizer:", dataTwo[0][2][2][-1], "after", dataTwo[0][2][3])
#END plotTwo

def plotThree(data) :
    #gets results for the tests
    dataSets = data[0]
    control = data[1]
    neuronArrays = data[2]
    epochs = data[3]
    thresh = data[4]

    #runtime parameters for the legend
    legendParameters = []
    legendParameters.append(mpatches.Patch(label='target', color='black'))
    legendParameters.append(mpatches.Patch(label=str(neuronArrays[0]) + ' Neurons', color='red'))
    legendParameters.append(mpatches.Patch(label=str(neuronArrays[1]) + ' Neurons', color='green'))
    legendParameters.append(mpatches.Patch(label=str(neuronArrays[2]) + ' Neurons', color='blue'))
    #instantiate the legends parameters
    plt.legend(handles=legendParameters)

    #show the MSE results for each network
    plt.figure(1)
    plt.subplot(311)
    plt.title('Two Neuron Model', y=0.8)
    plt.plot(range(0, epochs), dataSets[0][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')

    plt.subplot(312)
    plt.title('Eight Neuron Model', y=0.8)
    plt.plot(range(0, epochs), dataSets[1][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')

    plt.subplot(313)
    plt.title('Fifty Neuron Model', y=0.8)
    plt.plot(range(0, epochs), dataSets[2][2])
    #set labels
    plt.ylabel('MSE')
    plt.xlabel('Epochs')
    plt.show()

#END plotThree

def plotFour(data) :
    #gets results for the tests
    dataSets = data[0]
    control = data[1]
    neuronArrays = data[2]
    epochs = data[3]

    #runtime parameters for the legend
    legendParameters = []
    legendParameters.append(mpatches.Patch(label='target', color='black'))
    legendParameters.append(mpatches.Patch(label='without early stopping', color='red'))
    legendParameters.append(mpatches.Patch(label='with early stopping', color='green'))

    #instantiate the legends parameters
    plt.legend(handles=legendParameters)

    #show the mse of the goal region
    plt.ylabel('MSE')
    plt.xlabel('Epochs')
    plt.title('Contor diagram')

    pltData(control, 'black')
    pltData(dataSets[0][0], 'red')
    pltData(dataSets[1][0], 'green')

    plt.show()
#END plotFour

def plotFive(data) :
    #gets results for the tests
    dataSets = data[0]
    control = data[1]
    neuronArrays = data[2]
    epochs = data[3]
    thresh = data[4]

    #runtime parameters for the legend
    legendParameters = []
    legendParameters.append(mpatches.Patch(label='goal', color='black'))
    legendParameters.append(mpatches.Patch(label='test', color='red'))
    legendParameters.append(mpatches.Patch(label='validation', color='green'))
    legendParameters.append(mpatches.Patch(label='training', color='blue'))

    #instantiate the legends parameters
    plt.legend(handles=legendParameters)

    #show the mse of the goal region
    plt.ylabel('MSE')
    plt.xlabel('Epochs')
    plt.title('MSE evolution')

    plt.plot([thresh-1, thresh+1], dataSets[0][2][thresh-1 : thresh+1], color='blue')
    plt.plot([thresh-1, thresh+1], data[5][thresh-1 : thresh+1], color='red')
    plt.plot([thresh-1, thresh+1], data[6][thresh-1 : thresh+1], color='green')
    plt.hlines(0.02, thresh-1, thresh+1, color='black')
    plt.show()
#END plotFour

#question a
trying = True
while trying :
    try :
        plot(runExperiment([2, 8, 50], 50000, 0.1, 0))
        trying = False
    except :
        None
#END WHILE

#question b
trying = True
while trying :
    try :
        plotTwo(runExperiment([2, 8, 50], 100, 0.2, 1), runExperiment([2, 8, 50], 100000, 0.2, 2))
        trying = False
    except :
        None
#END WHILE

#question c
#part 1
trying = True
while trying :
    try :
        plotThree(runExperiment([2, 8, 50], 50000, 0.02, 3))
        trying = False
    except :
        None
#END WHILE

#part 2
trying = True
while trying :
    try :
        plotFive(runExperiment([8], 5000, 0.02, 5))
        trying = False
    except :
        None
#END WHILE

#part 3
trying = True
while trying :
    try :
        plotFour(runExperiment([8], 5000, 0.02, 4))
        trying = False
    except :
        None
#END WHILE
