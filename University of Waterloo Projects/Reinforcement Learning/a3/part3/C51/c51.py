import copy
import random
import numpy as np 
import gym
import torch
import gym_cartpolemod
import memory
from memory import Transition
import networks
import matplotlib.pyplot as plt

def extract(transitions):
    """Extract tensors of s, a, r, s' from a batch of transitions.

    Args:
        transitions (list): List of Transition named tuples where next_state is None if episode
            ended.

    Returns:
        (states, actions, rewards, next_states, mask) that are all (batch_size, *shape) tensors
        containing the extracted data. next_states does not contain elements for episode that
        ended. mask is a boolean tensor that specifies which transitions have a next state.
    """
    states = torch.cat([t.state for t in transitions])
    actions = torch.cat([t.action for t in transitions])
    rewards = torch.cat([t.reward for t in transitions])
    mask = torch.tensor([t.next_state is not None for t in transitions])
    next_states = torch.cat([t.next_state for t in transitions if t.next_state is not None])
    return states, actions, rewards, next_states, mask



def check_state(state):

    for i in range(len(state[0])):
        if isinstance(state[0][i], np.ndarray):
            state[0][i] = state[0][i][0]

    state = np.vstack(state[:, :]).astype(np.float)
    state = state[0]

    return state




def select_argmax_action(z, atoms):
    # Take state-action distribution z, which is a (batch_size, action_size, n_atoms) and
    # returns a tensor of shape (batch_size, 1) with the greedy actions for each state
    q_values = (z * atoms[:, None, :]).sum(dim=-1)
    return q_values.argmax(dim=-1).unsqueeze(1)


class CategoricalDQN:

    def __init__(self, z_net, n_atoms, v_min, v_max, df=0.99, buffer_len=1e6, batch_size=32,
                 lr=0.5e-3, update_mode='hard', update_every=5, tau=0.05, epsilon=0.1,
                 start_train_at=4000, state_size = None, results_dir=None):
        self.z_net = z_net
        self.n_atoms = n_atoms
        self.v_min = v_min
        self.v_max = v_max
        self.delta = (v_max - v_min) / n_atoms
        self.df = df
        self.buffer_len = buffer_len
        self.batch_size = batch_size
        self.update_mode = update_mode
        self.update_target_every = update_every
        self.tau = tau
        self.state_size = state_size
        self.epsilon = epsilon
        self.start_train_at = start_train_at
        self.replay_buffer = memory.TransitionReplayBuffer(maxlen=buffer_len)
        self._target_net = copy.deepcopy(z_net)
        self.optimizer = torch.optim.Adam(self.z_net.parameters(), lr=lr)
        self.atoms = torch.arange(self.v_min, self.v_max, self.delta).unsqueeze(0)

    def train(self, env: gym.Env, n_episodes):
        rewards = []
        episode_list = []
        episode_rewards = 0
        state = env.reset()
        episode = 0
        step = 0
        state = np.reshape(state, [1, self.state_size])
        state = check_state(state)
        
        state = torch.from_numpy(state).float().unsqueeze(0) 
        
        while True:
            step = step + 1
            with torch.no_grad():
                z = self.z_net(state)
            if random.random() < self.epsilon:  # Random action
                action = torch.LongTensor([[env.action_space.sample()]])
            else:
                action = select_argmax_action(z, self.atoms)
            next_state, reward, done, info = env.step(action.squeeze().numpy())
            next_state = np.reshape(next_state, [1, self.state_size])
            next_state = check_state(next_state) 
            next_state = torch.from_numpy(next_state).float().unsqueeze(0) if not done else None
            
            self.replay_buffer.remember(
                Transition(state, action, torch.tensor([[reward]]), next_state))
            state = next_state


            self.train_single_step(step) 
            
            episode_rewards = episode_rewards + reward
            if done:
                state = env.reset()
                state = np.reshape(state, [1, self.state_size])
                state = check_state(state)
                state = torch.from_numpy(state).float().unsqueeze(0) 
                episode = episode + 1
                print("We are in episode", episode)
                print("The reward is", episode_rewards)
                rewards.append(episode_rewards)
                episode_list.append(episode)
                episode_rewards = 0

            if episode >= n_episodes: 
                break

        self.plot(episode_list, rewards)    


    def plot(self, episodes, rewards): 
        plt.figure(figsize=(20,5))
        plt.plot(episodes, rewards)
        plt.xlabel('Episodes')
        plt.ylabel('Rewards')
        plt.savefig('C51.png')
        print('plotted')


    def train_single_step(self, step):
        
        if step > self.start_train_at and self.replay_buffer.size() > self.batch_size:
            batch = self.replay_buffer.sample(self.batch_size)
            states, actions, rewards, next_states, mask = extract(batch)
            targets = self.compute_targets(rewards, next_states, mask)
            self.train_networks(states, actions, targets, update=(step % self.update_target_every) == 0)


    def train_networks(self, states, actions, targets, update):
        """Train both the evaluation and target nets  
            Evalution net is trained using a cross-entropy loss
            Target net is trained using a hard update (copy weights from evaluation net)
            Implement this function
        """

        #initialize optimizer
        self.optimizer.zero_grad()

        #get the z net data
        zNetData = self.z_net(states)

        #container for actions by index
        actionListing = []

        #for each zNet row
        for index in range(zNetData.shape[0]) :
            actionListing.append(zNetData[index, actions[index]])
        #END FOR

        #convert to a torch tensor
        zNetData = torch.cat(actionListing)

        # Compute cross-entropy loss
        loss = -(targets * zNetData.log()).sum(dim = -1).mean()
        loss.backward()

        #run the optimizer step
        self.optimizer.step()

        if update == True :
            #check which mode type is selected
            if self.update_mode == 'soft' :
                #for each parameter in the list
                for parameter in self.z_net.parameters() :
                    for target_parameter in self._target_net.parameters() :
                        target_parameter.copy_(target_parameter * (1 - self.tau) + parameter * self.tau)
                    #END FOR
                #END FOR
            elif self.update_mode == 'hard' :
                self._target_net.load_state_dict(self.z_net.state_dict())
            #END IF
        #END IF

        return loss
    #END train_networks

    def compute_targets(self, rewards, next_states, mask):
        """Compute the target distributions for the given transitions.
           The function returns the target distribution after distribution of the new probabilities 
            Implement this function
            For now returns a placeholder distribution. 
        """

        #define atom tensor
        atomTensor = (rewards + self.df * mask[:, None] * torch.arange(self.v_min, self.v_max, self.delta))\
                     .clamp(max = self.v_max, min = self.v_min)

        #define formula variables
        b = (atomTensor - self.v_min) / self.delta
        lowerB = torch.floor(b).long()
        upperB = torch.ceil(b).long().clamp(max = self.n_atoms - 1)

        #get distribution for action
        with torch.no_grad() :
            zDist = self._target_net(next_states)
        #END WITH

        #get the target actions
        target_actions = select_argmax_action(zDist, atomTensor[mask])

        #a list containing all z prime rows
        zContainer = []        

        #for every z_prime row
        for index in range(zDist.shape[0]) :
            zContainer.append(zDist[index][target_actions[index]])
        #END FOR

        #convert the container into a tensor
        zDist = torch.cat(zContainer)

        #set a uniform distribution since the atoms are all equal
        probabilities = torch.ones((self.batch_size, self.n_atoms))
        probabilities /= self.n_atoms
        probabilities[mask] = zDist

        #find probabilities
        probabilityDist = torch.zeros_like(probabilities)

        #add probabilities to scatter
        probabilityDist.scatter_add_(1, lowerB, (upperB - b) * probabilities)
        probabilityDist.scatter_add_(1, upperB, (b - lowerB) * probabilities)

        #return probabilities
        return probabilityDist
    #END compute_targets


if __name__ == '__main__':
    
    
    env = gym.make('CartPoleMod-v1')
    state_dim = env.observation_space.shape[0]
    act_dim = env.action_space.n
    n_atoms = 51
    n_units = 64 
    n_layers = 2 
    z_net = networks.DistributionalNetwork(inputs=state_dim, n_actions=act_dim, n_atoms=n_atoms,
                                           n_hidden_units=n_units, n_hidden_layers=n_layers)
    v_min, v_max = [0, 200] 
    start_train_at = 32
    update_target_net_every = 5 
    epsilon = 0.1
    n_episodes = 200
    C51 = CategoricalDQN(z_net=z_net, n_atoms=n_atoms, v_min=v_min, v_max=v_max,
                          start_train_at=start_train_at,
                          update_every=update_target_net_every, epsilon=epsilon, state_size = state_dim)
    C51.train(env=env, n_episodes=n_episodes)

