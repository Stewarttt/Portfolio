# Gabrielle Latreille (101073284)
# Connor Stewart (101041125)

import tensorflow.compat.v1 as tf
# depending on the version of tf used you may need the following line instead
# import tensorflow as tf
import tensorflow.compat.v1.keras as keras
import numpy as np
import matplotlib.pyplot as plt
import random
from copy import deepcopy

target_pattern = [
    "00100010100101010001111111000110001",
    "11110100011000111110100011000111110",
    "01110100011000010000100001000101110",
    "11110100011000110001100011000111110",
    "11111100001000011110100001000011111",
    "11111100001000011110100001000010000",
    "01110100011000010000100111000101110",
    "10001100011000111111100011000110001",
    "01110001000010000100001000010001110",
    "11111001000010000100001001010001000",
    "10001100101010011000101001001010001",
    "10000100001000010000100001000011111",
    "10001110111010110001100011000110001",
    "10001110011100110101100111001110001",
    "01110100011000110001100011000101110",
    "11110100011000111110100001000010000",
    "01110100011000110001101011001001101",
    "11110100011000111110101001001010001",
    "01110100010100000100000101000101110",
    "11111001000010000100001000010000100",
    "10001100011000110001100011000101110",
    "10001100011000110001100010101000100",
    "10001100011000110001101011101110001",
    "10001100010101000100010101000110001",
    "10001100010101000100001000010000100",
    "11111000010001000100010001000011111",
    "00010000000001000010010100101000100",
    "00100000000010000100001000010000100",
    "01110100011000110001100011000110001",
    "10001100011000101010001000100010000",
    "10010100101001010010100101001001101"
]
# target_labels = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'j', 'i', 'n', 'y', 'u' ]
target_labels = np.eye(31)
for i in range(31):
    target_pattern[i] = [float(j) for j in list(target_pattern[i])]

# distort x number of random in all images
def distortion(bits):
    distort = []
    for i in range(31):
        rd = random.sample(range(0, 35), bits)
        distorted = deepcopy(target_pattern[i])
        for j in rd:
            distorted[j] = 0 if distorted[j]==1 else 1
        distort.append(distorted)
    return distort

# compute and shape training data and labels
train_data = np.array([])
train_data = np.concatenate((target_pattern, distortion(1), distortion(2), distortion(3)))
train_labels = np.array([])
train_labels = np.concatenate((target_labels, target_labels, target_labels, target_labels))

# shuffle
# https://stackoverflow.com/questions/4601373/better-way-to-shuffle-two-numpy-arrays-in-unison
p = np.random.permutation(len(train_data))
train_data = train_data[p]
train_labels = train_labels[p]

# compute and shape testing data and labels
test_data = [np.array(target_pattern), np.array(distortion(1)), np.array(distortion(2)), np.array(distortion(3))]
test_labels = np.array(target_labels)



def train_test_nn(hidden_neurons):
    # set keras model
    model = keras.Sequential(
        [
            keras.Input(35),
            keras.layers.Dense(hidden_neurons, activation='sigmoid'),
            keras.layers.Dense(31, activation='sigmoid')
        ])

    # compile with Adam optimizer and mean_squared_error
    model.compile(optimizer=tf.train.AdamOptimizer(0.01), loss='mean_squared_error', metrics=['accuracy'])

    # train all the data at once with 100 epochs
    model.fit(train_data, train_labels, epochs=100, batch_size=1)

    # test and save results
    res = []
    for test_i in test_data:
        res.append(model.evaluate(test_i, test_labels))

    return res


# train and test data with hidden neurons [5, 10, 15, 20, 25]
res = []
for i in range(5, 26, 5):
    res.append(train_test_nn(i))

# plot results
plt.xlabel("Noise Level")
plt.ylabel("Percentage of Recognition Errors")

x = [0, 1, 2, 3]
for i in range(len(res)):
    r = [100-(res[i][j][1]*100) for j in range(len(x))]
    plt.plot(x, r, label=f"{(i+1)*5} hidden neurons")

plt.legend()
plt.show()