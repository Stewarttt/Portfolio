import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
from config import gamma, device, batch_size, sequence_length

class DRQN(nn.Module):
    def __init__(self, num_inputs, num_outputs):
        super(DRQN, self).__init__()
        # The inputs are two integers giving the dimensions of the inputs and outputs respectively. 
        # The input dimension is the state dimention and the output dimension is the action dimension.
        # This constructor function initializes the network by creating the different layers. 
        # This function now only implements two fully connected layers. Modify this to include LSTM layer(s). 
        
        
        self.num_inputs = num_inputs
        self.num_outputs = num_outputs

        self.lstm = nn.LSTM(input_size = num_inputs, hidden_size = 16, batch_first=True)
        self.fc1 = nn.Linear(num_inputs * sequence_length * 2, 128)
        self.fc2 = nn.Linear(128, num_outputs)

        for m in self.modules():
            if isinstance(m, nn.Linear):
                nn.init.xavier_uniform(m.weight)
            #END IF
        #END FOR
    #END __init__

    def forward(self, x, hidden=None):
        # The variable x denotes the input to the network. 
        # The hidden variable denotes the hidden state and cell state inputs to the LSTM based network. 
        # The function returns the q value and the output hidden variable information (new cell state and new hidden state) for the given input. 
        # This function now only uses the fully connected layers. Modify this to use the LSTM layer(s).          

        if hidden == None :
            results = self.lstm(x)
            out = results[0]
            hidden = results[1]
        else :
            results = self.lstm(x, hidden)
            out = results[0]
            hidden = results[1]
        #END IF

        out = F.relu(self.fc1(out))
        qvalue = self.fc2(out)

        return qvalue, hidden
    #END forward

    @classmethod
    def train_model(cls, online_net, target_net, optimizer, batch):
        # The online_net is the variable that represents the first (current) Q network.
        # The target_net is the variable that represents the second (target) Q network.
        # The optimizer is Adam. 
        # Batch represents a mini-batch of memory. Note that the minibatch also includes the rnn state (hidden state) for the DRQN. 

        # This function takes in a mini-batch of memory, calculates the loss and trains the online network. Target network is not trained using back prop. 
        # The loss function is the mean squared TD error that takes the difference between the current q and the target q. 
        # Return the value of loss for logging purposes (optional).

        # Implement this function. Currently, temporary values to ensure that the program compiles. 

        loss = 1.0

        states = torch.stack(batch.state).view(batch_size, sequence_length, online_net.num_inputs)
        next_states = torch.stack(batch.next_state).view(batch_size, sequence_length, online_net.num_inputs)

        actions = torch.stack(batch.action).view(batch_size, sequence_length, -1).long()
        actions = actions.to(device)
        rewards = torch.stack(batch.reward).view(batch_size, sequence_length, -1)
        rewards = rewards.to(device)
        masks = torch.stack(batch.mask).view(batch_size, sequence_length, -1)
        masks = masks.to(device)

        pred = online_net(states)[0]
        next_pred = target_net(next_states)[0]

        pred = pred

        pred = pred.gather(2, actions)
        target = rewards + masks * gamma * next_pred.max(2, keepdim=True)[0]

        loss = F.mse_loss(pred, target.detach())
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        return loss

    def get_action(self, state, hidden):
        state = state.unsqueeze(0).unsqueeze(0)

        qvalue, hidden = self.forward(state, hidden)

        action = torch.max(qvalue, 2)[1]
        
        return action.cpu().numpy()[0][0], hidden
