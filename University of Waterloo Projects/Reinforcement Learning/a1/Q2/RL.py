import numpy as np
import MDP as mdp

class RL:
    def __init__(self,mdp,sampleReward):
        '''Constructor for the RL class

        Inputs:
        mdp -- Markov decision process (T, R, discount)
        sampleReward -- Function to sample rewards (e.g., bernoulli, Gaussian).
        This function takes one argument: the mean of the distributon and 
        returns a sample from the distribution.
        '''

        self.mdp = mdp
        self.sampleReward = sampleReward

    def sampleRewardAndNextState(self,state,action):
        '''Procedure to sample a reward and the next state
        reward ~ Pr(r)
        nextState ~ Pr(s'|s,a)

        Inputs:
        state -- current state
        action -- action to be executed

        Outputs: 
        reward -- sampled reward
        nextState -- sampled next state
        '''

        reward = self.sampleReward(self.mdp.R[action,state])
        cumProb = np.cumsum(self.mdp.T[action,state,:])
        nextState = np.where(cumProb >= np.random.rand(1))[0][0]
        return [reward,nextState]

    def qLearning(self,s0,initialQ,nEpisodes,nSteps,epsilon=0,temperature=0):
        '''qLearning algorithm.  Epsilon exploration and Boltzmann exploration
        are combined in one procedure by sampling a random action with 
        probabilty epsilon and performing Boltzmann exploration otherwise.  
        When epsilon and temperature are set to 0, there is no exploration.

        Inputs:
        s0 -- initial state
        initialQ -- initial Q function (|A|x|S| array)
        nEpisodes -- # of episodes (one episode consists of a trajectory of nSteps that starts in s0
        nSteps -- # of steps per episode
        epsilon -- probability with which an action is chosen at random
        temperature -- parameter that regulates Boltzmann exploration

        Outputs: 
        Q -- final Q function (|A|x|S| array)
        policy -- final policy
        '''

        #set the initial function state
        Q = initialQ

        #set the initial policy to be a blank array with the following dimensions
        policy = np.zeros((self.mdp.nStates, self.mdp.nActions))

        #set the reward state to be a blank array with the following dimensions
        reward = np.zeros(nEpisodes)

        #for each episode in the range of all valid episodes
        for episode in range(nEpisodes) :

            #initialize the state as the initial state
            state = s0

            #used to count state iterations
            counter = np.zeros((self.mdp.nActions, self.mdp.nStates))

            #for each step in the range of all valid steps
            for step in range(nSteps) :

                #by default, set action to be the max of the final Q function state spaces
                action = np.argmax(Q[:,state])

                #if the epsilon value is within the below range
                if 0 < epsilon < np.random.rand(1) :
                    #set the action to be a random integer from zero up to the number of actions value
                    action = np.random.randint(0, self.mdp.nActions)
                #if the above check fails, and the temperature is greater then zero
                elif 0 < temperature :
                    #set the probability to be 
                    probability = np.exp(Q[:,state] / temperature)
                    boltzmannValue = probability / probability.sum()
                    action = np.random.choice(self.mdp.nActions, None, True, probability)                    
                #END IF

                #get the reward and the next state values for the current state and action
                rewardState = self.sampleRewardAndNextState(state, action)

                #multiply the reward value by the discount value to the power of the step
                reward[episode] += rewardState[0] * (self.mdp.discount ** step)

                #iterate the counter by one
                counter[action][state] += 1

                #finds the difference between the reward states
                difference = rewardState[0] + self.mdp.discount * np.max(Q[:,rewardState[1]]) - Q[action][state]

                #determines the new learning rate
                learningRate = 1.0 / counter[action][state]

                #set the new state for the action
                Q[action][state] = Q[action][state] + learningRate * difference

                #set the next state as the current rewards next state
                state = rewardState[1]
            #END FOR
        #END FOR

        #set the new policy
        policy = np.argmax(Q, 0)

        #return the output function and final policy
        return [Q,policy, reward]
    #END qLearning
#END RL
