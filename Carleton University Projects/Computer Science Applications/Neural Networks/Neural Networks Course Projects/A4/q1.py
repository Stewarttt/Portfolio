"""
Name:
    q1.py
Input:
    Uses the MNIST dataset, which contains images of numbers with labels matching their true number value
Output:
    Outputs a Hopfield network run on the dataset with predefined internal parameters
Description:
    This provides a code based solution to question one of assignment four in COMP 4107 Neural Networks at Carleton University
Group Members:
    Gabrielle Latreille (101073284)
    Connor Stewart (101041125)
"""

#import statements
import tensorflow.compat.v1 as tf
from tensorflow.keras.datasets import mnist
import numpy as np
import matplotlib.pyplot as plt

"""
Name:
    Hopfield
Input:
    self - this is the name of the object being called
    num_train_data - this is the number of training data
Output:
    outputs a hopfield network for the dataset
Description:
    takes in a dataset and produces a hopfield data set
"""
class Hopfield:
    # num_train_data: the number of imaged from each label that will be included in the training_dataset
    def __init__(self, num_train_data):
        # get training and testing data and labels
        (x_train, y_train), (x_test, y_test) = mnist.load_data()
        self.train_data, self.train_labels = self.filter_and_shape(x_train, y_train, num_train_data)
        self.test_data, self.test_labels = self.filter_and_shape(x_test, y_test)

        # initialize weights
        self.weights = np.zeros((784, 784))
    #END __init__

    #filters dataset into a training and testing set and resizes it to correctly be used by the rest of the application
    def filter_and_shape(self, x_set, y_set, set_size=-1):
        # get indices of ones and fives
        set_filter1 = np.where(y_set==1)[0]
        set_filter5 = np.where(y_set==5)[0]

        # shuffle indices to randomly sample
        np.random.shuffle(set_filter1)
        np.random.shuffle(set_filter5)

        # combine lists indices of ones and fives and shuffle
        set_filter = np.concatenate((set_filter1[0:set_size], set_filter5[0:set_size]))
        np.random.shuffle(set_filter)

        # get only selected data and matching labels
        x_set = x_set[set_filter] /255  # normalize
        y_set = y_set[set_filter]

        # polarize data
        x_set[x_set == 0] = -1
        x_set[x_set > 0] = 1

        # reshape 28x28 grid of pixels to one row of 784 pixels
        x_set = x_set.reshape(-1, 28*28)

        return x_set, y_set
    #END filter_and_shape

    #tests to determine the accuracy of the images in the label set
    def image_accuracy(self, image, image_label):
        # set the minimum norm to a very high number
        min_norm = np.inf

        #for each element of data in the set of the training data points
        for i in range(len(self.train_data)):
            # https://python-forum.io/Thread-Get-closest-value-array-in-array-of-arrays-follow-up-help

            #calculate the norm for the dataset
            norm = np.linalg.norm(image - self.train_data[i])

            #if the norm is less then the designated minimum norm
            if norm < min_norm:
                #flag this norm as the new smallest minimum norm
                min_norm = norm
                #set this label as the new minimum label
                min_label = self.train_labels[i]
            #END IF
        # True if classified correctly, else False
        return min_label == image_label
    #END image_accuracy

    #flages the images as active based on the results of the image multiplied by the weight value
    def activation_and_stabilization(self, test_image):
        #while we are still activating the sets
        while True:
            #checks if we have activated this iteration
            changed = False

            #for each image in the set of all test images
            for n in range(len(test_image)):

                #multiply the weight vector by the activation vector
                activation = self.weights[n].dot(test_image.T)
                activation = 1 if activation > 0 else -1

                #checks if a test image is not the same as the activation
                if activation != test_image[n]:
                    #activate the image and flag it as changed
                    test_image[n] = activation
                    changed = True
                #END IF
            #END FOR

            #if the test image is not changed yet, return it
            if not changed:
                return test_image
            #END IF
        #END WHILE
    #END activation_and_stabilization

    #defines the entry point function for the algorithm
    def run(self):
        # train weights
        self.weights = self.train_data.T.dot(self.train_data) - len(self.train_data)*np.identity(len(self.train_data[0]))

        # run activation function and test accuracy
        correct = 0

        #for each test data in the range of all test datas
        for i in range(len(self.test_data)):
            test_image = self.activation_and_stabilization(self.test_data[i])
            accurate = self.image_accuracy(test_image, self.test_labels[i])
            correct += 1 if accurate else 0
        #END FOR

        # return accuracy
        return correct/len(self.test_data)*100
    #END run
#END Hopfield

#---PROGRAM-DRIVER_(MAIN)---#

# test Hopfield's algorithm with 1 to 10 images of each label
num_images = np.arange(1, 11)
accuracies = []

#for each element in the set of all image number-value
for i in num_images:
    accs = []
    hopfield = Hopfield(i)
    accuracy = hopfield.run()
    accuracies.append(accuracy)
#END FOR

#plot accuracy via the matplotlib library
plt.plot(num_images, accuracies)
plt.ylabel("Accuracy (%)")
plt.xlabel("Number of Images for Each Label")
plt.xticks(num_images)
plt.grid()
plt.show()
