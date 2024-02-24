import numpy as np
import MDP

class RL2:
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

    def sampleSoftmaxPolicy(self,policyParams,state):
        '''Procedure to sample an action from stochastic policy
        pi(a|s) = exp(policyParams(a,s))/[sum_a' exp(policyParams(a',s))])
        This function should be called by reinforce() to selection actions

        Inputs:
        policyParams -- parameters of a softmax policy (|A|x|S| array)
        state -- current state

        Outputs: 
        action -- sampled action
        '''

        #get the exponential probability of the given row
        exponentialProbability = np.exp(policyParams[:, state])

        #divide by the total probability sum
        exponentialProbability /= np.sum(exponentialProbability)

        #get the culminative probability
        culminativeProbability = np.cumsum(exponentialProbability)

        #return the action generated
        return np.where(np.random.rand(1) <= culminativeProbability)[0][0]

    def modelBasedRL(self,s0,defaultT,initialR,nEpisodes,nSteps,epsilon=0):
        '''Model-based Reinforcement Learning with epsilon greedy 
        exploration.  This function should use value iteration,
        policy iteration or modified policy iteration to update the policy at each step

        Inputs:
        s0 -- initial state
        defaultT -- default transition function when a state-action pair has not been vsited
        initialR -- initial estimate of the reward function
        nEpisodes -- # of episodes (one episode consists of a trajectory of nSteps that starts in s0
        nSteps -- # of steps per episode
        epsilon -- probability with which an action is chosen at random

        Outputs: 
        V -- final value function
        policy -- final policy
        '''

        #Create container variables
        V = np.zeros(self.mdp.nStates)
        policy = np.zeros(self.mdp.nStates,int)
        nsa = np.zeros((self.mdp.nActions, self.mdp.nStates))
        nsaNext = np.zeros((self.mdp.nActions, self.mdp.nStates, self.mdp.nStates))
        rewardTotal = np.zeros(nEpisodes)

        #runtime variables
        transition = defaultT
        reward = initialR
        epsilonLimit = 0.05 #Q1 Point 1

        #for each episode
        for episode in range(nEpisodes) :
            #set the state to the initial state
            state = s0

            #for each step within the number of steps
            for step in range(nSteps) :

                #if a random value is bigger then the epsilon
                if epsilon <= np.random.rand(1) :
                    #set the action to be the current policy
                    action = policy[state]
                else :
                    #choose a random action from the number of actions
                    action = np.random.choice(self.mdp.nActions)
                #END IF

                #get the same rewards and next state for this code
                sample = self.sampleRewardAndNextState(state, action)

                #iterate the counter variables
                nsa[action][state] += 1
                nsaNext[action][state][sample[1]] += 1

                #find the reward generated for this iteration
                reward[action][state] = ((nsa[action][state] - 1) * reward[action][state] + sample[0]) / nsa[action][state]

                #for each sub-state within the range of all possible states
                for substate in range(self.mdp.nStates) :
                    #calculate the transition value
                    transition[action][state][substate] = nsaNext[action][state][substate] / nsa[action][state]
                #END FOR

                #finds the max of the reward
                V = np.amax(reward, 0)
                #finds the new epsilon value from the max of the output function
                epsilon = np.absolute(V).max()

                #iterate through until a small enough difference is reached
                while epsilonLimit < epsilon :
                    #change variables to learn
                    Vnext = np.amax(transition.dot(V) * self.mdp.discount + reward, 0)
                    #check error
                    epsilon = np.absolute(Vnext - V).max()
                    #set new output function
                    V = Vnext
                #END WHILE

                #finds the total reward from this iteration
                rewardTotal[episode] += (self.mdp.discount ** step) * sample[0]

                #initialize a new state
                state = sample[1]

                #set the new policy
                policy = np.argmax(transition.dot(V) * self.mdp.discount + reward, 0)
            #END FOR
        #END FOR

        #return the output function, policy, and total rewards
        return [V, policy, rewardTotal]

    def epsilonGreedyBandit(self,nIterations):
        '''Epsilon greedy algorithm for bandits (assume no discount factor).  Use epsilon = 1 / # of iterations.

        Inputs:
        nIterations -- # of arms that are pulled

        Outputs: 
        empiricalMeans -- empirical average of rewards for each arm (array of |A| entries)
        '''

        #set the container variables
        empiricalMeans = np.zeros(self.mdp.nActions)
        rewards = np.zeros(nIterations)
        actionNumber = np.zeros(self.mdp.nActions)

        #set the default state
        state = 0

        for iteration in range(nIterations) :
            #sets the epsilon value per iterations (increment by one from zero indexing)
            epsilon = 1 / (iteration + 1) #Q1 Point 4.5

            #if the epsilon value is less then a random number
            if epsilon <= np.random.rand(1) :
                #set the new action to be the maximum of the empiricalMeans
                action = np.argmax(empiricalMeans)
            else :
                #choose a random action from the set of actions
                action = np.random.choice(self.mdp.nActions)
            #END IF

            #gets the sample reward and next state for this iteration
            sample = self.sampleRewardAndNextState(state, action)

            #finds the updated value for the empiricalMeans
            empiricalMeans[action] = (actionNumber[action]*empiricalMeans[action] + sample[0]) / (actionNumber[action] + 1)

            #iterate the action number by one
            actionNumber[action] += 1

            #gets the new reward from the sample
            rewards[iteration] = sample[0]

            #gets the new state from the sample
            state = sample[1]
        #END FOR

        #returns the empircal means and the rewards from the function
        return empiricalMeans, rewards

    def thompsonSamplingBandit(self,prior,nIterations,k=1):
        '''Thompson sampling algorithm for Bernoulli bandits (assume no discount factor)

        Inputs:
        prior -- initial beta distribution over the average reward of each arm (|A|x2 matrix such that prior[a,0] is the alpha hyperparameter for arm a and prior[a,1] is the beta hyperparameter for arm a)  
        nIterations -- # of arms that are pulled
        k -- # of sampled average rewards

        Outputs: 
        empiricalMeans -- empirical average of rewards for each arm (array of |A| entries)
        '''

        #define container variables
        empiricalMeans = np.zeros(self.mdp.nActions)
        rewards = np.zeros(nIterations)
        actionCounter = np.zeros(self.mdp.nActions)

        #define the runtime variables
        state = 0
        V = 0

        #for each possible iteration
        for iteration in range(nIterations) :

            #define the samples tested
            samples = np.zeros(self.mdp.nActions)

            #for each valid action that can be made
            for action in range(self.mdp.nActions) :
                #for each sampled reward
                for rewardNumber in range(k) :
                    #determine the samples result
                    samples[action] += np.random.beta(prior[action][0], prior[action][1]) #Q1 Point 5
                #END FOR

                #divide by the number of sampled average rewards
                samples[action] /= k
            #END FOR

            #run the sample data
            action = np.argmax(samples)
            sampleRewards = self.sampleRewardAndNextState(state, action)

            #get the rewards for the iteration
            rewards[iteration] = sampleRewards[0]

            #check if the reward is given
            if sampleRewards[0] == True :
                prior[action][0] += 1
            else :
                prior[action][1] += 1
            #END IF

            #add to the current state and output function
            V += sampleRewards[0]
            state = sampleRewards[1]

            #learn from the data
            empiricalMeans[action] = (sampleRewards[0] + actionCounter[action] * empiricalMeans[action]) / (1 + actionCounter[action])
            actionCounter[action] += 1
        #END FOR

        #return the results
        return empiricalMeans, rewards
        
    def UCBbandit(self,nIterations):
        '''Upper confidence bound algorithm for bandits (assume no discount factor)

        Inputs:
        nIterations -- # of arms that are pulled

        Outputs: 
        empiricalMeans -- empirical average of rewards for each arm (array of |A| entries)
        '''

        #define container variables
        empiricalMeans = np.zeros(self.mdp.nActions)
        confidence = np.zeros(self.mdp.nActions)
        actions = np.ones(self.mdp.nActions)
        rewards = np.zeros(nIterations)

        #define runtime variables
        state = 0

        #for each valid iteration
        for iteration in range(nIterations) :
            #determine the upper confidence bound
            confidence = np.power(np.log(iteration + 1)/((1/2)*actions), 0.5) + empiricalMeans #Q1 point 4

            #find the action
            action = np.argmax(confidence)

            #get the results of the sample reward and next state for the current iteration
            rewards[iteration], state = self.sampleRewardAndNextState(state, action)

            #update the empirical means and learn
            empiricalMeans[action] = (empiricalMeans[action] * actions[action] + rewards[iteration]) / (actions[action] + 1)
            actions[action] += 1
        #END FOR

        #return the empirical means and the rewards
        return empiricalMeans, rewards

    def reinforce(self,s0,initialPolicyParams,nEpisodes,nSteps):
        '''reinforce algorithm.  Learn a stochastic policy of the form
        pi(a|s) = exp(policyParams(a,s))/[sum_a' exp(policyParams(a',s))]).
        This function should call the function sampleSoftmaxPolicy(policyParams,state) to select actions

        Inputs:
        s0 -- initial state
        initialPolicyParams -- parameters of the initial policy (array of |A|x|S| entries)
        nEpisodes -- # of episodes (one episode consists of a trajectory of nSteps that starts in s0)
        nSteps -- # of steps per episode

        Outputs: 
        policyParams -- parameters of the final policy (array of |A|x|S| entries)
        '''

        #define the container variables
        rewardTotal = np.zeros(nEpisodes)
        numActions = np.zeros((self.mdp.nActions,self.mdp.nStates))
        rewards = np.zeros(nSteps)
        actions = np.zeros(nSteps, int)
        states = np.zeros(nSteps, int)

        #define the runtime variables
        learningRate = 0.01 #Q1 point 1
        policyParams = initialPolicyParams

        #for each episode in the set of all possible episodes
        for episode in range(nEpisodes):

            #set the current state to be the initial state
            state = s0

            #for each possible step attempt to create an episode
            for step in range(nSteps):
                #for the current action use the sample policy
                action = self.sampleSoftmaxPolicy(policyParams, state)

                #get the result of the reward and next state for the current step
                sample = self.sampleRewardAndNextState(state, action)

                #get the reward value sum
                rewardTotal[episode] += sample[0] * self.mdp.discount ** step

                #iterate the counter
                numActions[action][state] += 1

                #set the steps action and state containers using the results produced
                actions[step] = action
                states[step] = state

                #set the steps rewards and state
                rewards[step] = sample[0]
                state = sample[1]
            #END FOR

            #for each value of n we attempt to update the policy
            for n in range(nSteps):
                #get the total rewards and discount products
                GTotal = 0
                for i in range(nSteps - n):
                    GTotal += rewards[n + i] * self.mdp.discount ** i
                #END FOR

                #get the exponential probability of the given row
                gradient = np.exp(policyParams[:, states[n]])

                #divide by the total probability sum
                gradient /= -np.sum(gradient)

                #iterate the gradient by one
                gradient[actions[n]] += 1

                #learn based on the data and add results to the policyParams
                policyParams[:, states[n]] += learningRate * GTotal * gradient * self.mdp.discount ** n
            #END FOR

        #return the policyParams and the rewards
        return policyParams, rewardTotal
