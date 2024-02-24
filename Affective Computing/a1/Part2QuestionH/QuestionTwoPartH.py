import matplotlib.pyplot as plt 

results = \
"""
Training my_vgg on cuda:0
Epoch  1		Train Accuracy: 33.5898 %		Val Accuracy: 40.9028 %
Epoch  2		Train Accuracy: 47.7150 %		Val Accuracy: 53.6361 %
Epoch  3		Train Accuracy: 52.9447 %		Val Accuracy: 59.8495 %
Epoch  4		Train Accuracy: 55.2053 %		Val Accuracy: 61.6606 %
Epoch  5		Train Accuracy: 56.9755 %		Val Accuracy: 59.5152 %
Epoch  6		Train Accuracy: 58.4406 %		Val Accuracy: 63.2767 %
Epoch  7		Train Accuracy: 59.6165 %		Val Accuracy: 62.4687 %
Epoch  8		Train Accuracy: 60.7402 %		Val Accuracy: 64.3076 %
Epoch  9		Train Accuracy: 61.6134 %		Val Accuracy: 65.0878 %
Epoch 10		Train Accuracy: 62.4971 %		Val Accuracy: 62.9980 %
Epoch 11		Train Accuracy: 63.1868 %		Val Accuracy: 65.1992 %
Epoch 12		Train Accuracy: 63.8235 %		Val Accuracy: 66.1466 %
Epoch 13		Train Accuracy: 64.4133 %		Val Accuracy: 66.2301 %
Epoch 14		Train Accuracy: 65.0552 %		Val Accuracy: 66.1466 %
Epoch 15		Train Accuracy: 65.9629 %		Val Accuracy: 63.8061 %
Epoch 16		Train Accuracy: 66.4830 %		Val Accuracy: 67.6512 %
Epoch 17		Train Accuracy: 67.0734 %		Val Accuracy: 66.5924 %
Epoch 18		Train Accuracy: 67.6415 %		Val Accuracy: 68.5428 %
Epoch 19		Train Accuracy: 68.2957 %		Val Accuracy: 67.6233 %
Epoch 20		Train Accuracy: 68.9073 %		Val Accuracy: 69.4065 %
Epoch 21		Train Accuracy: 69.2048 %		Val Accuracy: 67.0382 %
Epoch 22		Train Accuracy: 69.7983 %		Val Accuracy: 68.5706 %
Epoch 23		Train Accuracy: 70.4438 %		Val Accuracy: 68.1527 %
Epoch 24		Train Accuracy: 70.9133 %		Val Accuracy: 68.5428 %
Epoch 25		Train Accuracy: 71.3626 %		Val Accuracy: 69.0722 %
Epoch    26: reducing learning rate of group 0 to 7.5000e-03.
Epoch 26		Train Accuracy: 71.9785 %		Val Accuracy: 68.9050 %
Epoch 27		Train Accuracy: 73.6525 %		Val Accuracy: 70.0474 %
Epoch 28		Train Accuracy: 74.0350 %		Val Accuracy: 69.1000 %
Epoch 29		Train Accuracy: 74.7354 %		Val Accuracy: 69.8802 %
Epoch 30		Train Accuracy: 75.1949 %		Val Accuracy: 69.2115 %
Epoch 31		Train Accuracy: 75.6049 %		Val Accuracy: 70.6604 %
""".strip()

#format data
train = [row.split('Train Accuracy: ') for row in results.split('\n')]
train = train[::-1]
train.pop()
train = train[::-1]

#contain plottable values
training = []
value = []

#convert data into raw numbers
for row in train :
    try :
        training.append(float(row[1].split(" ")[0]))
        value.append(float(row[1].split('Val Accuracy: ')[1][:-1]))
    except :
        None
    #END TRY
#END FOR

#setup matplotlib window
plt.title('Training and Value Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')

plt.plot(training, label='training')
plt.plot(value, label='value')
plt.legend()
plt.show()
