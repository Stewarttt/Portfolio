"""
Name:
    q3.py
Input:
    Uses the MNIST dataset, which contains images of numbers with labels matching their true number value
Output:
    A computed SOM and 3D scatterplot of a 3D self-organizing map to compare the clusters when compared to K-means for the MNIST data
Description:
    This provides a code based solution to question three of assignment four in COMP 4107 Neural Networks at Carleton University
Group Members:
    Gabrielle Latreille (101073284)
    Connor Stewart (101041125)
"""

#import statements
import tensorflow.compat.v1 as tf
from tensorflow.keras.datasets import mnist
import numpy as np
import matplotlib.pyplot as plt
import random
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA


"""
Name:
    SOM
Input:
    self - this is the instantiated object being refered to
    train_size - this is the size of the training data being generated
    epochs - this is the number of epochs being executed
Output:
    Outputs a SOM network and 2D scatterplot of the data
Description:
    This provides a code based solution to question three of assignment four in COMP 4107 Neural Networks at Carleton University
Bibliography:
    https://www.novaims.unl.pt/docentes/vlobo/Publicacoes/1_5_lobo05_SOM_kmeans.pdf
Group Members:
    Gabrielle Latreille (101073284)
    Connor Stewart (101041125)
"""
class SOM :
    #initialize the dataset and
    def __init__(self, train_size=6000, epochs=1):
        # get training data
        (x_train, y_train), (_, _) = mnist.load_data()
        set_filter = np.where((y_train==1) | (y_train==5))
        train_data = x_train[set_filter] /255
        train_data = train_data.reshape(-1, 28*28)
        np.random.shuffle(train_data)
        self.train_data = train_data[:train_size]

        # set weights
        # map dimensions as explained here: http://www.giscience2010.org/pdfs/paper_230.pdf
        som_dim = int(np.ceil(np.sqrt(5 * np.sqrt(train_size))))
        self.weights = np.random.random((som_dim, som_dim, 28*28))

        # set variables
        self.init_learning_rate = 0.02
        self.init_radius = 10
        self.epochs = epochs
    #END __init__

    #gets the minimum index of the best matching
    def get_best_matching_unit(self, sample) :
        #sets the wieght shape to be that of the objects weight shape
        weights_shape = self.weights.shape

        # set minimim euclidean distance to a very large number
        min_eucl_dist = np.inf
        min_index = (0, 0)

        #for each row in the set of all the weights
        for i in range(weights_shape[0]):
            #for each column in the set of all the weights
            for j in range(weights_shape[1]):
                # https://www.geeksforgeeks.org/calculate-the-euclidean-distance-using-numpy/
                #excludes the distribution based on the results of the algorithm thus far
                eucl_dist = np.linalg.norm(sample - self.weights[i, j])
                #checks if a new minimum distribution point is found
                if eucl_dist < min_eucl_dist:
                    min_eucl_dist = eucl_dist
                    min_index = (i, j)
                #END IF
            #END FOR
        #END FOR

        # return index of the best matching unit weight
        return min_index
    #END get_best_matching_unit

    #defines the gaussian distribtion
    def gaussian(self, weight, possible_neighbour, radius):
        #excludes the distribution based on the results of the algorithm thus far
        eucl_dist = np.linalg.norm(possible_neighbour - weight)
        # if the possible neighbour is within the radius of weight
        if eucl_dist <= radius:
            # gaussian function
            return np.exp(-( eucl_dist / (2 * radius ** 2)))
        else:
            # return 0 so the weight doesn't change if not within radius
            return 0
        #END IF
    #END gaussian

    #scales the neighbour
    def scale_neighbour(self, sample, bmu_weight, learning_rate, radius):
        #sets the wieght shape to be that of the objects weight shape
        weights_shape = self.weights.shape

        #for each row in the set of all weights
        for i in range(weights_shape[0]) :
            #for each column in the set of all weights
            for j in range(weights_shape[1]) :
                #defines the gaussian distribution for the next step
                g = self.gaussian(bmu_weight, np.array([i, j]), radius)
                # if gaussian = 0, then weights[i, j] = weights[i, j] else weights[i, j] update
                #defines the weight vector for the neighbours
                self.weights[i, j] += (learning_rate * g * (sample - self.weights[i, j]))
            #END FOR
        #END FOR
    #END scale_neighbour

    # https://medium.com/machine-learning-researcher/self-organizing-map-som-c296561e2117
    #gets the decay rate for the function
    def decay(self, step, time_const) :
        #runs the expenential function for the decay steps
        return np.exp(- step / time_const)
    #END decay

    #this function is used to train the dataset
    def train(self):
        #gets the learning rate and radius for training
        learning_rate = self.init_learning_rate
        radius = self.init_radius

        #for each element in the set of all epochs
        for j in range(self.epochs) :
            #train weights on set of images
            for train_image in self.train_data :
                bmu_weight = self.get_best_matching_unit(train_image)
                self.scale_neighbour(train_image, bmu_weight, learning_rate, radius)
            #END FOR

            # decay learning_rate and radius
            learning_rate = self.init_learning_rate * self.decay(j, len(self.train_data))
            radius = self.init_radius * self.decay(j, len(self.train_data))
        #END FOR
    #END train

    def display_weights(self) :
        #gets the dimensions of the weight vector and the defines the matplotlib window
        weights_shape = self.weights.shape
        fig, axs = plt.subplots(weights_shape[0], weights_shape[1], figsize=(28, 28))

        #for each row in the set of all weights
        for i in range(weights_shape[0]):
            #for each column in the set of all weights
            for j in range(weights_shape[1]):
                axs[i, j].imshow(self.weights[i, j].reshape((28, 28))*255)
                axs[i, j].axis('off')
            #END FOR
        #END FOR
        plt.show()
    #END display_weights

    #gets the kmeans for the dataset
    def kmeans(self) :
        kmeans = KMeans(2, random_state=0).fit(self.train_data)
        labels = kmeans.labels_

        pca = PCA(2)
        pca.fit(self.train_data)
        pca_2d = pca.transform(self.train_data)

        plt.scatter(pca_2d[:, 0], pca_2d[:, 1], c=labels, cmap='jet', s=1)
        plt.title("K Mean Cluster Data")
        plt.xlabel("K-Data")
        plt.ylabel("Distance")
        plt.show()

    #END kmeans
#END SOM

#---PROGRAM-DRIVER_(MAIN)---#

#define the SOM data class
som = SOM()
#train the data within the SOM class
som.train()
#output the results of the SOM
som.display_weights()

#display a scatterplot for the data
som.kmeans()
