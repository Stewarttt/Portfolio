import numpy as np
import MDP
import RL2
import matplotlib.pyplot as plt

def sampleBernoulli(mean):
    ''' function to obtain a sample from a Bernoulli distribution
    Input:
    mean -- mean of the Bernoulli
    
    Output:
    sample -- sample (0 or 1)
    '''

    if np.random.rand(1) < mean: return 1
    else: return 0


# Multi-arm bandit problems (3 arms with probabilities 0.3, 0.5 and 0.7)
T = np.array([[[1]],[[1]],[[1]]])
R = np.array([[0.3],[0.5],[0.7]])
discount = 0.999
mdp = MDP.MDP(T,R,discount)
banditProblem = RL2.RL2(mdp,sampleBernoulli)

#run 200 iterations for the UCB, epsilon-greedy, and Thompson sampling functions
reward = np.zeros((3,200))

#for each of the 1000 trials being preformed
for step in range(0, 1000):
    #run the UCBbandit
    empiricalMeans, results = banditProblem.UCBbandit(nIterations=200)
    reward[0,:] = (reward[0,:] * step + results) / (step + 1)

    #run the epsilonGreedyBandit
    empiricalMeans, results = banditProblem.epsilonGreedyBandit(nIterations=200)
    reward[1,:] = (reward[1,:] * step + results) / (step + 1)
    
    #run the thompsonSamplingBandit
    empiricalMeans, results = banditProblem.thompsonSamplingBandit(prior=np.ones([mdp.nActions,2]),nIterations=200)
    reward[2,:] = (reward[2,:] * step + results) / (step + 1)
#END FOR

#define the matplotlib graphs
fig, ax = plt.subplots()

#for each algorithm type, make a plot
ax.plot(np.linspace(1,200,200), reward[0,:], label = "UCB")
ax.plot(np.linspace(1,200,200), reward[1,:], label = "EpsilonGreedy")
ax.plot(np.linspace(1,200,200), reward[2,:], label = "ThompsonSampling")

#set the legend to appear
legend = ax.legend(loc='best')

#set the x and y axis labels
plt.xlabel("Iteration (Number)")
plt.ylabel("Average Reward")

#show the matplotlib window
plt.show()
