#imported libraries
import numpy as np

class MDP:
    '''A simple MDP class.  It includes the following members'''

    def __init__(self,T,R,discount):
        '''Constructor for the MDP class

        Inputs:
        T -- Transition function: |A| x |S| x |S'| array
        R -- Reward function: |A| x |S| array
        discount -- discount factor: scalar in [0,1)

        The constructor verifies that the inputs are valid and sets
        corresponding variables in a MDP object'''

        assert T.ndim == 3, "Invalid transition function: it should have 3 dimensions"
        self.nActions = T.shape[0]
        self.nStates = T.shape[1]
        assert T.shape == (self.nActions,self.nStates,self.nStates), "Invalid transition function: it has dimensionality " + repr(T.shape) + ", but it should be (nActions,nStates,nStates)"
        assert (abs(T.sum(2)-1) < 1e-5).all(), "Invalid transition function: some transition probability does not equal 1"
        self.T = T
        assert R.ndim == 2, "Invalid reward function: it should have 2 dimensions" 
        assert R.shape == (self.nActions,self.nStates), "Invalid reward function: it has dimensionality " + repr(R.shape) + ", but it should be (nActions,nStates)"
        self.R = R
        assert 0 <= discount < 1, "Invalid discount factor: it should be in [0,1)"
        self.discount = discount
    #END __init__
        
    def valueIteration(self,initialV,nIterations=np.inf,tolerance=0.01):
        '''Value iteration procedure
        V <-- max_a R^a + gamma T^a V

        Inputs:
        initialV -- Initial value function: array of |S| entries
        nIterations -- limit on the # of iterations: scalar (default: infinity)
        tolerance -- threshold on ||V^n-V^n+1||_inf: scalar (default: 0.01)

        Outputs: 
        V -- Value function: array of |S| entries
        iterId -- # of iterations performed: scalar
        epsilon -- ||V^n-V^n+1||_inf: scalar'''

        #initialize the function values
        V = initialV
        iterId = 0
        epsilon = max(np.absolute(V))

        #while the current iteration is less then the number of iterations
        while iterId < nIterations :
            #get a blank array
            actions = np.zeros((self.nActions, self.nStates))

            #for each action in the transition function
            for action in range(self.nActions):
                #for each state in the transition function 
                for state in range(self.nStates):
                    #set the current action for the value function
                    actions[action][state] = (self.T[action][state]).dot(self.R[action][state] + V * self.discount)
                #END FOR
            #END FOR
            #gets the difference between the old and new value functions
            epsilon = (np.absolute(actions.max(axis=0)-V)).max()
            #get the new value function
            V = actions.max(axis=0)

            #iterate the counter
            iterId = iterId + 1

            #check if the difference between the old and new value functions is greater then the tolerance
            if epsilon <= tolerance :
                #exit the current loop
                break
            #END IF
        return [V,iterId,epsilon]
    #END valueIteration

    def extractPolicy(self,V):
        '''Procedure to extract a policy from a value function
        pi <-- argmax_a R^a + gamma T^a V

        Inputs:
        V -- Value function: array of |S| entries

        Output:
        policy -- Policy: array of |S| entries'''

        #get a blank numpy array of the following dimensions
        actions = np.zeros((self.nActions, self.nStates))

        #for each action in the transition function
        for action in range(self.nActions):
            #for each state in the transition function
            for state in range(self.nStates):
                #set the current action for the value function
                actions[action][state] = (self.T[action][state]).dot(self.R[action][state] + V * self.discount)
            #END FOR
        #END FOR

        #get the new policy
        policy = np.argmax(actions, 0)

        #return the calcualted policy structure
        return policy
    #END extractPolicy

    def evaluatePolicy(self, policy):
        '''Evaluate a policy by solving a system of linear equations
        V^pi = R^pi + gamma T^pi V^pi

        Input:
        policy -- Policy: array of |S| entries

        Ouput:
        V -- Value function: array of |S| entries'''

        #create a numpy array of the following dimensions
        V = np.zeros(self.nStates)

        #create empty lists as containers for dimensions
        listR = []
        listT = []

        #for each state being considered
        for state in range(self.nStates) :
            #get the current policy state
            policyState = policy[state]

            #get the R state
            RUnit = self.R[policyState][state]
            #get the T state
            TUnit = self.T[policyState][state]

            #append the state rows to the lists
            listR.append(RUnit)
            listT.append(TUnit)
        #END FOR

        #convert the lists into arrays
        Rpi = np.array(listR)
        Tpi = np.array(listT)

        #preform operations on the array
        transformed = np.identity(self.nStates) - (Tpi * self.discount)

        #solve the array
        V = np.linalg.solve(transformed, Rpi)

        #return the value function
        return V
    #END evaluatePolicy
        
    def policyIteration(self,initialPolicy,nIterations=np.inf):
        '''Policy iteration procedure: alternate between policy
        evaluation (solve V^pi = R^pi + gamma T^pi V^pi) and policy
        improvement (pi <-- argmax_a R^a + gamma T^a V^pi).

        Inputs:
        initialPolicy -- Initial policy: array of |S| entries
        nIterations -- limit on # of iterations: scalar (default: inf)

        Outputs: 
        policy -- Policy: array of |S| entries
        V -- Value function: array of |S| entries
        iterId -- # of iterations peformed by modified policy iteration: scalar'''

        #set the current iteration value to zero
        iterId = 0
        policy = initialPolicy

        #while we have not reached the iteration limit
        while nIterations >= iterId :
            #set the new value function by solving the system of linear equations
            V = self.evaluatePolicy(policy)
            #extract the policy from the given value function
            policyNew = self.extractPolicy(V)
            #iterate the counter by one
            iterId = iterId + 1

            #check if the policies have converged
            if np.array_equal(policy, policyNew) :
                #exit the loop
                break
            else :
                #set the new policy
                policy = policyNew
            #END IF
        #END WHILE

        #return the policy iteration results
        return [policy,V,iterId]
    #END policyIteration
            
    def evaluatePolicyPartially(self,policy,initialV,nIterations=np.inf,tolerance=0.01):
        '''Partial policy evaluation:
        Repeat V^pi <-- R^pi + gamma T^pi V^pi

        Inputs:
        policy -- Policy: array of |S| entries
        initialV -- Initial value function: array of |S| entries
        nIterations -- limit on the # of iterations: scalar (default: infinity)
        tolerance -- threshold on ||V^n-V^n+1||_inf: scalar (default: 0.01)

        Outputs: 
        V -- Value function: array of |S| entries
        iterId -- # of iterations performed: scalar
        epsilon -- ||V^n-V^n+1||_inf: scalar'''

        #Initialize the following function parameters
        V = initialV
        iterId = 0
        epsilon = float('inf')

        #set empty lists to contain array dimensions
        listR = []
        listT = []

        #for each state in the range of possible states
        for state in range(self.nStates) :
            #get the current policy state
            policyState = int(policy[state])

            #get the R and S states
            RUnit = self.R[policyState][state]
            TUnit = self.T[policyState][state]

            #append the states rows to the following lists
            listR.append(RUnit)
            listT.append(TUnit)
        #END FOR

        #convert the lists into arrays
        Rpi = np.array(listR)
        Tpi = np.array(listT)

        #while the tolerance is not surpassed by the difference between the old and new value function
        while tolerance < epsilon :
            #determine the new value function
            Vnew = Rpi + self.discount * Tpi.dot(V)
            #check the difference between the old and new value functions
            epsilon = (np.absolute(Vnew-V)).max()
            #set the new value function
            V = Vnew

            #iterate the counter by one
            iterId = iterId + 1

            #if the iteration surpasses the number of iterations allowed
            if iterId >= nIterations :
                #exit the loop
                break
            #END IF
        #END WHILE

        return [V,iterId,epsilon]
    #END evaluatePolicyPartially

    def modifiedPolicyIteration(self,initialPolicy,initialV,nEvalIterations=5,nIterations=np.inf,tolerance=0.01):
        '''Modified policy iteration procedure: alternate between
        partial policy evaluation (repeat a few times V^pi <-- R^pi + gamma T^pi V^pi)
        and policy improvement (pi <-- argmax_a R^a + gamma T^a V^pi)

        Inputs:
        initialPolicy -- Initial policy: array of |S| entries
        initialV -- Initial value function: array of |S| entries
        nEvalIterations -- limit on # of iterations to be performed in each partial policy evaluation: scalar (default: 5)
        nIterations -- limit on # of iterations to be performed in modified policy iteration: scalar (default: inf)
        tolerance -- threshold on ||V^n-V^n+1||_inf: scalar (default: 0.01)

        Outputs: 
        policy -- Policy: array of |S| entries
        V -- Value function: array of |S| entries
        iterId -- # of iterations peformed by modified policy iteration: scalar
        epsilon -- ||V^n-V^n+1||_inf: scalar'''

        #Initialize the following function parameters
        policy = np.zeros(self.nStates)
        V = np.zeros(self.nStates)
        iterId = 0
        epsilon = float('inf')

        #while the tolerance is less then the difference between the old and new value functions
        while tolerance < epsilon :
            #calcualte the partial policy evaluation
            V = self.evaluatePolicyPartially(policy, V, nEvalIterations, tolerance)[0]
            #extract the policy from the new partial policy evaluated value function
            policy = self.extractPolicy(V)

            #get a blank array
            actions = np.zeros((self.nActions, self.nStates))

            #for each action in the transition function
            for action in range(self.nActions):
                #for each state in the transition function 
                for state in range(self.nStates):
                    #set the current action for the value function
                    actions[action][state] = (self.T[action][state]).dot(self.R[action][state] + V * self.discount)
                #END FOR
            #END FOR
            VPartialTest = actions.max(0)

            #get the difference between the old and the new value function
            epsilon = (np.absolute(V-VPartialTest)).max()

            #iterate the counter by one
            iterId = iterId + 1

            #check if the number of iterations has surpassed the limit
            if iterId >= nIterations :
                #exit the while loop
                break
            #END IF
        #END WHILE

        #get the partial policy
        V = self.evaluatePolicyPartially(policy, V, float('inf'), tolerance)[0]

        #return the new policy iteration value and value function
        return [policy,V,iterId,epsilon]
    #END modifiedPolicyIteration
#END mdp
