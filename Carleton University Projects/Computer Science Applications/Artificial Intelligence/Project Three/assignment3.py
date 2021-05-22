# COMP 4106 Assignment 3
# Gabrielle Latreille (101073284)
# Connor Stewart (101041125)
# Abdaljabbar Hersi (100919600)

import numpy as np


class td_qlearning:
    alpha = 0.1
    gamma = 0.5

    def __init__(self, trajectory_filepath):
        # trajectory_filepath is the path to a file containing a trajectory through state space
        # Return nothing
        # initialize variables
        trajectory = self.open_file(trajectory_filepath)
        self.actions = ['C', 'L', 'R', 'U', 'D']
        self.q_function = {}
        
        # go through trajectory and create q function
        for i in range(len(trajectory) - 1):
            self.q_function_learning(*trajectory[i], trajectory[i + 1][0])
        # END FOR
    # END __init__

    # open trajectory file and format data
    def open_file(self, file_name):
        trajectory = []
        with open(file_name) as f:
            for line in f:
                # array of [state, action]
                trajectory.append(line.strip('\n').split(','))
            # END FOR
        # END WITH
        return trajectory
    # END open_file

    # reward function
    def reward(self, state):
        # -1 * number of dirty squares
        return -1 * sum([int(elem) for elem in list(state)][1:])
    # END reward

    # get q value for the state and action, else create it and initialize it to 0
    def get_q_value(self, state, action):
        # try to find state in q function
        try:
            self.q_function[state]
        # if it doesn't exist, create it and initialize to 0
        except:
            self.q_function[state] = [0, 0, 0, 0, 0]
        # return the state-pair from q function
        return self.q_function[state][self.get_action_index(action)]
    # END get_q_value

    # get index corresponding to action
    # [C, L, R, U, D] -> [0, 1, 2, 3, 4]
    def get_action_index(self, action):
        return self.actions.index(action)
    # END get_action_index

    # get max value for state and corresponding state
    def get_max_state_value(self, state):
        # get position of robot during the state
        pos = int(state[0])
        # set initial values corresponding to actions for the state: [C, L, R, U, D]
        values = [-np.inf, -np.inf, -np.inf, -np.inf, -np.inf]

        values[0] = self.get_q_value(state, 'C')
        if pos in [3, 4]:
            values[1] = self.get_q_value(state, 'L')
        if pos in [2, 3]:
            values[2] = self.get_q_value(state, 'R')
        if pos in [3, 5]:
            values[3] = self.get_q_value(state, 'U')
        if pos in [1, 3]:
            values[4] = self.get_q_value(state, 'D')

        # get index of max value
        i = values.index(max(values))

        # return max value, max action
        return values[i], self.actions[i]
    # END get_max_state_value

    # apply q function and update q values
    def q_function_learning(self, state, action, next_state):
        # current q value for the state-action pair
        state_q_val = self.get_q_value(state, action)

        # max value for the next state-action' pair
        max_q_val = self.get_max_state_value(next_state)[0]

        # apply q learning function
        # Q(st, at) = Q(st, at) + alpha * ( r(st) + gamma * ( max_a' Q(st+1, a') ) - Q(st, at) )
        q = state_q_val + self.alpha * (self.reward(state) + self.gamma * max_q_val - state_q_val)

        # update state-action pair and round to 6 decimal points
        self.q_function[state][self.get_action_index(action)] = round(q, 6)
    # END q_function_learning

    # get and return q value for state-action pair
    def qvalue(self, state, action):
        # state is a string representation of a state
        # action is a string representation of an action
        # Return the q-value for the state-action pair
        return self.get_q_value(state, action)
    # END qvalue

    # return policy for state
    def policy(self, state):
        # state is a string representation of a state
        # Return the optimal action under the learned policy
        return self.get_max_state_value(state)[1]
    # END policy
