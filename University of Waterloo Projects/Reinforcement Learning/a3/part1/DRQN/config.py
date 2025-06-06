import torch

env_name = 'CartPole-v1'
gamma = 0.99
batch_size = 32
lr = 0.0001
initial_exploration = 1000
goal_score = 200
log_interval = 10
update_target = 100
replay_memory_capacity = 100
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
max_epsilon = 0.1
sequence_length = 4
