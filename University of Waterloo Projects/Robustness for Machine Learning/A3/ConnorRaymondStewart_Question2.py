#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
from tqdm import tqdm

class SVM:
    def __init__(self, learningRate = 0.001, epsilon = 0.01, epochs = 1000, norm = 2) :
        #sets constraints for SVM
        self.weight = None
        self.intercept = None
        self.learningRate = learningRate
        self.epsilon = epsilon
        self.epochs = epochs
        self.norm = norm
    #END __init__

    def fit(self, x, y):
        #aligns values of negative one and zero to number zero, and one to number one
        aligned = np.where(y <= 0, -1, 1)

        #initialize the weight vector
        self.weight = np.ones(x.shape[1])
        self.intercept = 0

        for epoch in range(self.epochs):
            #counts the image iteration
            imageIter = 0
            for image in x:
                if self.norm == 1 :
                    weight = np.divide(self.weight, np.abs(self.weight))
                    np.nan_to_num(weight)
                elif self.norm == 2 :
                    weight = np.divide(self.weight, np.linalg.norm(self.weight, 2))
                    np.nan_to_num(weight)
                elif self.norm == 3 :
                    weight = np.divide(np.matmul(self.weight, np.abs(self.weight)), np.power(np.linalg.norm(self.weight, 3), 2))
                    np.nan_to_num(weight)
                #END IF

                if 1 > aligned[imageIter] * (np.dot(image, self.weight) - self.intercept) :
                    self.weight -= self.learningRate * (self.epsilon * weight - np.dot(image, aligned[imageIter]))
                    self.intercept -= self.learningRate * aligned[imageIter]
                else : 
                    self.weight -= self.learningRate * (self.epsilon * weight)
                #END IF

                #iterate the index image iterator by one
                imageIter += 1
            #END FOR
        #END FOR
    #END fit

    def predict(self, x):
        return np.sign(np.dot(x, self.weight) - self.intercept)
    #END predict
    
    def saveData(self, fileName) :
        np.save(fileName, self.weight)
#END SVM


# In[2]:


def get_subset_of_MNIST(X, Y):
	new_X, new_Y = [], []

	for i in range(len(Y)):
		if Y[i] in [0, 1]:
			new_X.append(X[i].flatten())
			new_Y.append(Y[i])
	X, Y = np.array(new_X), np.array(new_Y)
	Y = Y * 2 - 1
	return X, Y


# In[3]:


def get_natural_acc(pred, Y):
	acc = pred * Y > 0
	return acc.sum() / Y.shape[0]


# In[4]:


#obtain variable data
np.random.seed(0)
epsilon = 0.01
epoch = 1000
learning_rate = 0.001
norms = [1, 2, 3]

#load the datasets
MNIST_X = np.load('./mnist_X.npy')
MNIST_Y = np.load('./mnist_Y.npy')

#get data subsets
x, y = get_subset_of_MNIST(MNIST_X, MNIST_Y)

#for each norm from one to three


# In[5]:


#for the 1-norm
norm = norms[0]

#calculate the output prediction
decent = SVM(learning_rate, epsilon, epoch, norm)
decent.fit(x, y)
output = decent.predict(x)

#save array data
decent.saveData("ckpt_1.py")

#obtain the accuracy
acc = output * y > 0
acc.sum() / y.shape[0]

#print the accuracy values
print("Accuracy for %d-norm" % norm)
print(acc.sum() / y.shape[0])


# In[6]:


#for the 2-norm
norm = norms[1]

#calculate the output prediction
decent = SVM(learning_rate, epsilon, epoch, norm)
decent.fit(x, y)
output = decent.predict(x)

#save array data
decent.saveData("ckpt_2.py")

#obtain the accuracy
acc = output * y > 0
acc.sum() / y.shape[0]

#print the accuracy values
print("Accuracy for %d-norm" % norm)
print(acc.sum() / y.shape[0])


# In[7]:


#for the 3-norm
norm = norms[2]

#calculate the output prediction
decent = SVM(learning_rate, epsilon, epoch, norm)
decent.fit(x, y)
output = decent.predict(x)

#save array data
decent.saveData("ckpt_3.py")

#obtain the accuracy
acc = output * y > 0
acc.sum() / y.shape[0]

#print the accuracy values
print("Accuracy for %d-norm" % norm)
print(acc.sum() / y.shape[0])


# In[ ]:




