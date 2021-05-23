import tensorflow.compat.v1 as tf
from tensorflow.keras.datasets import mnist
import numpy as np
import matplotlib.pyplot as plt
import random
from matplotlib.pyplot import figure
from sklearn.cluster import KMeans
from sklearn.preprocessing import OneHotEncoder
from sklearn.neural_network import MLPClassifier

#reshapes the matrix into the correct dimensions
def filter_and_shape(x_set, y_set, set_size=-1) :
    x_set = x_set.reshape(-1, 28**2)

    return x_set, y_set
#END filter_and_shape

#this is the solution for the K-Clustering
def questionOne(train_data, clusters=[10,40], change=1000000) :

    #get a random segment of the mnist dataset
    randomSample = random.sample(range(len(train_data)), 1000)
    distance = np.array([])

    #get the k means dataset
    kRange = np.arange(clusters[0], clusters[1], 4)

    #iterate through the k mean values
    for k in kRange :
        means = KMeans(k, random_state=0).fit(train_data[randomSample])
        results = means.inertia_ + change*k
        distance = np.append(distance, results)
    #END FOR

    #plot a figure using matplotlib
    plt.figure()
    plt.plot(kRange, distance)
    plt.title("K Mean Cluster Data")
    plt.xlabel("K-Data")
    plt.ylabel("Distance")

    #plot the data onto matplotlib
    plt.grid()
    plt.show()
#END questionOne

#get the datasets
(x_train, y_train), (x_test, y_test) = mnist.load_data()

#filter and reshape the datasets
train_data, train_labels = filter_and_shape(x_train, y_train)
test_data, test_labels = filter_and_shape(x_test, y_test)

#find the solution to question one
questionOne(train_data, [10,40], 1000000)
