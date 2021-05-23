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
train_data = [np.array(target_pattern), np.concatenate((distortion(1), distortion(2), distortion(3)))]
train_labels = [np.array(target_labels), np.concatenate((target_labels, target_labels, target_labels))]

# shuffle
# https://stackoverflow.com/questions/4601373/better-way-to-shuffle-two-numpy-arrays-in-unison
for i in range(2):
    p = np.random.permutation(len(train_data[i]))
    train_data[i] = train_data[i][p]
    train_labels[i] = train_labels[i][p]


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

    # save loss result for graphs
    fig_a = []
    fig_b = []

    # train with original until zero decision errors
    accuracy = 0.0
    while accuracy != 1.0:
        r = model.fit(train_data[0], train_labels[0], epochs=1, batch_size=10)
        accuracy = r.history['accuracy'][0]
        fig_a.append(r.history['loss'][0])

    # train with noisy data for 10 passes
    r = model.fit(train_data[1], train_labels[1], epochs=10, batch_size=10)
    fig_b = r.history['loss']

    # train with original until zero decision errors again
    accuracy = 0.0
    while accuracy != 1.0:
        r = model.fit(train_data[0], train_labels[0], epochs=1, batch_size=10)
        accuracy = r.history['accuracy'][0]
        fig_b.append(r.history['loss'][0])

    return fig_a, fig_b

# train data with 20 hidden neurons
fig_a, fig_b = train_test_nn(20)

# plot results
# fig a
plt.xlabel("Epochs")
plt.ylabel("Training Error")

fig_a_x = np.arange(len(fig_a))
fig_a_y = fig_a

plt.plot(fig_a_x, fig_a_y)
plt.show()

# fig b
plt.xlabel("Epochs")
plt.ylabel("Training Error")

fig_b_x = np.arange(len(fig_b))
fig_b_y = fig_b

plt.plot(fig_b_x, fig_b_y)
plt.show()