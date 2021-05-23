# import sys
# np.set_printoptions(threshold=sys.maxsize)
import tensorflow.compat.v1 as tf
from tensorflow.keras.datasets import mnist
import numpy as np
import matplotlib.pyplot as plt
import random
from matplotlib.pyplot import figure
from sklearn.cluster import KMeans
from sklearn.preprocessing import OneHotEncoder
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import KFold

def filter_and_shape(x_set, y_set, set_size=-1) :
    x_set = x_set / 255
    x_set = x_set.reshape(-1, 28**2)

    return x_set, y_set
#END filter_and_shape

def sigma(centroid, cluster):
    eucl_dist = 0
    for i in cluster:
        eucl_dist += np.linalg.norm(i-centroid)
    #END FOR

    return eucl_dist / len(cluster)
#END sigma

def beta(sigma):
    return 1 / (2 * sigma ** 2)
#END beta

def beta_for_cluster(points, cluster_center, training_set):
    eucl_dist = 0
    for i in range(len(points)):
        # calculate euclidean distance between center of cluster and training point
        eucl_dist += np.linalg.norm(training_set[points[i]] - cluster_center)
    #END FOR
    return beta(eucl_dist/len(points))
#END beta_for_cluster

def gauss(beta, d):
    return np.exp(-beta*(d**2))
#END gauss

def labelVector(target_label):
    target_vector = np.zeros(10)
    target_vector[target_label] = 1
    return target_vector
#END labelVector

(x_train, y_train), (x_test, y_test) = mnist.load_data()

#filter and reshape the datasets
train_data, train_labels = filter_and_shape(x_train[:100], y_train[:100])
test_data, test_labels = filter_and_shape(x_test, y_test)

kmeans = KMeans(10, random_state=0).fit(train_data)
centroids = kmeans.cluster_centers_

outputs = np.zeros((len(train_data)))
validOutputs = np.zeros((int(len(train_data)/10)))
output_layers = np.zeros((10))
weights = np.zeros(())

cluster = [list() for i in centroids]
for image in train_data:
    min_dist = np.inf
    min_cent = 0
    for i in range(len(centroids)):
        eucl_dist = np.linalg.norm(image-centroids[i])
        if eucl_dist < min_dist:
            min_dist = eucl_dist
            min_cent = i
        #END IF
    #END FOR
    cluster[min_cent].append(image)
#END FOR

betas = []
for i in range(len(centroids)):
    b = beta(sigma(centroids[i], cluster[i]))
    betas.append(b)
#END FOR

kfold = KFold(n_splits=10)
correct = 0
largest = 0
previous = 0
iterations = 0
results = []

for train, valid in kfold.split(train_data):
    #     print(train, valid)
    train_set = train_data[train]
    train_set_labels = train_labels[train]
    valid_set = train_data[valid]
    valid_set_labels = train_labels[train]

    for i in range(100): # epochs
        centroid = centroids[int(valid[0]/10)]
        beta_val = betas[int(valid[0]/10)]
        err = 0
        for i in range(len(train_set)):
            g = gauss(beta_val, np.linalg.norm(train_set[i]- centroid))
            outputs[i] = g

            label = labelVector(train_set_labels[i])
            for j in range(10):
                err += (label[j] - output_layers[j])**2
            #END FOR
            err /= 10
            weights = weights + 0.1 * err
        #END FOR

        output = 0

        for i in range(len(valid_set)) :
            g = gauss(beta_val, np.linalg.norm(valid_set[i]- centroid))
            validOutputs[i] = g

            label = labelVector(valid_set_labels[i])
            for j in range(10):
                output += (label[j] - output_layers[j])**2
            #END FOR

            output_layers[i] = output

            argument = np.argmax(output_layers)

            correct += int(argument == valid_set_labels[i])
        #END FOR
        accuracy = correct / len(test_data)
        if accuracy > largest :
            largest = accuracy
        #END IF

        if accuracy - previous <= 0.05 :
            iterations = iterations + 1
        #END IF
        previous = accuracy
        results.append(accuracy)
    #END FOR
#END FOR
plt.plot(results)

plt.xlabel("Test Point")
plt.ylabel("Accuracy")
plt.grid()

plt.show()
