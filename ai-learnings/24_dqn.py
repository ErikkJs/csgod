# author: csgod cmps studies
# DQN - Deep Q-Network
# DQN combines deep learning with Q-learning to handle high-dimensional state spaces
# using experience replay and target networks for stable training.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import numpy as np
import matplotlib.pyplot as plt
import random
from collections import deque, namedtuple
import gym
import time

# Experience tuple for replay buffer
Experience = namedtuple('Experience', ['state', 'action', 'reward', 'next_state', 'done'])

class DQNetwork(nn.Module):
    """Deep Q-Network architecture"""
    def __init__(self, state_size, action_size, hidden_sizes=[512, 256], dueling=False):
        super(DQNetwork, self).__init__()
        
        self.state_size = state_size
        self.action_size = action_size
        self.dueling = dueling
        
        # Input layer
        self.input_layer = nn.Linear(state_size, hidden_sizes[0])
        
        # Hidden layers
        self.hidden_layers = nn.ModuleList()
        for i in range(len(hidden_sizes) - 1):
            self.hidden_layers.append(nn.Linear(hidden_sizes[i], hidden_sizes[i + 1]))
        
        if dueling:
            # Dueling DQN: separate value and advantage streams
            self.value_stream = nn.Linear(hidden_sizes[-1], 1)
            self.advantage_stream = nn.Linear(hidden_sizes[-1], action_size)
        else:
            # Standard DQN
            self.output_layer = nn.Linear(hidden_sizes[-1], action_size)
    
    def forward(self, x):
        # Input and hidden layers
        x = F.relu(self.input_layer(x))
        for hidden_layer in self.hidden_layers:
            x = F.relu(hidden_layer(x))
        
        if self.dueling:
            # Dueling architecture: Q(s,a) = V(s) + A(s,a) - mean(A(s,:))
            value = self.value_stream(x)
            advantage = self.advantage_stream(x)
            
            # Combine value and advantage
            q_values = value + advantage - advantage.mean(dim=1, keepdim=True)
            return q_values
        else:
            # Standard Q-values
            return self.output_layer(x)

class ConvDQNetwork(nn.Module):
    """Convolutional DQN for image-based environments"""
    def __init__(self, input_shape, action_size, dueling=False):
        super(ConvDQNetwork, self).__init__()
        
        self.input_shape = input_shape
        self.action_size = action_size
        self.dueling = dueling
        
        # Convolutional layers
        self.conv_layers = nn.Sequential(
            nn.Conv2d(input_shape[0], 32, kernel_size=8, stride=4),
            nn.ReLU(),
            nn.Conv2d(32, 64, kernel_size=4, stride=2),
            nn.ReLU(),
            nn.Conv2d(64, 64, kernel_size=3, stride=1),
            nn.ReLU(),
        )
        
        # Calculate size after convolutions
        conv_out_size = self._get_conv_out_size(input_shape)
        
        if dueling:
            # Dueling streams
            self.value_stream = nn.Sequential(
                nn.Linear(conv_out_size, 512),
                nn.ReLU(),
                nn.Linear(512, 1)
            )
            
            self.advantage_stream = nn.Sequential(
                nn.Linear(conv_out_size, 512),
                nn.ReLU(),
                nn.Linear(512, action_size)
            )
        else:
            # Standard fully connected layers
            self.fc_layers = nn.Sequential(
                nn.Linear(conv_out_size, 512),
                nn.ReLU(),
                nn.Linear(512, action_size)
            )
    
    def _get_conv_out_size(self, shape):
        """Calculate the output size of convolutional layers"""
        dummy_input = torch.zeros(1, *shape)
        conv_out = self.conv_layers(dummy_input)
        return int(np.prod(conv_out.size()))
    
    def forward(self, x):
        # Convolutional feature extraction
        conv_out = self.conv_layers(x)
        conv_out = conv_out.view(conv_out.size(0), -1)  # Flatten
        
        if self.dueling:
            # Dueling architecture
            value = self.value_stream(conv_out)
            advantage = self.advantage_stream(conv_out)
            
            q_values = value + advantage - advantage.mean(dim=1, keepdim=True)
            return q_values
        else:
            return self.fc_layers(conv_out)

class ReplayBuffer:
    """Experience replay buffer for DQN"""
    def __init__(self, capacity):
        self.buffer = deque(maxlen=capacity)
    
    def push(self, state, action, reward, next_state, done):
        """Add experience to buffer"""
        experience = Experience(state, action, reward, next_state, done)
        self.buffer.append(experience)
    
    def sample(self, batch_size):
        """Sample a batch of experiences"""
        experiences = random.sample(self.buffer, batch_size)
        
        states = torch.FloatTensor(np.array([e.state for e in experiences]))
        actions = torch.LongTensor([e.action for e in experiences])
        rewards = torch.FloatTensor([e.reward for e in experiences])
        next_states = torch.FloatTensor(np.array([e.next_state for e in experiences]))
        dones = torch.BoolTensor([e.done for e in experiences])
        
        return states, actions, rewards, next_states, dones
    
    def __len__(self):
        return len(self.buffer)

class DQNAgent:
    """DQN Agent with experience replay and target network"""
    def __init__(self, state_size, action_size, lr=1e-3, gamma=0.99, epsilon=1.0, 
                 epsilon_decay=0.995, epsilon_min=0.01, buffer_size=10000, 
                 batch_size=32, target_update=100, device=None, dueling=False):
        
        self.state_size = state_size
        self.action_size = action_size
        self.lr = lr
        self.gamma = gamma
        self.epsilon = epsilon
        self.epsilon_decay = epsilon_decay
        self.epsilon_min = epsilon_min
        self.batch_size = batch_size
        self.target_update = target_update
        self.device = device or torch.device("cuda" if torch.cuda.is_available() else "cpu")
        
        # Q-Networks
        self.q_network = DQNetwork(state_size, action_size, dueling=dueling).to(self.device)
        self.target_network = DQNetwork(state_size, action_size, dueling=dueling).to(self.device)
        
        # Copy weights to target network
        self.target_network.load_state_dict(self.q_network.state_dict())
        
        # Optimizer and replay buffer
        self.optimizer = optim.Adam(self.q_network.parameters(), lr=lr)
        self.replay_buffer = ReplayBuffer(buffer_size)
        
        # Training statistics
        self.losses = []
        self.episode_rewards = []
        self.epsilons = []
        self.update_count = 0
    
    def act(self, state, training=True):
        """Choose action using epsilon-greedy policy"""
        if training and random.random() < self.epsilon:
            return random.randrange(self.action_size)
        
        state_tensor = torch.FloatTensor(state).unsqueeze(0).to(self.device)
        q_values = self.q_network(state_tensor)
        return q_values.argmax().item()
    
    def remember(self, state, action, reward, next_state, done):
        """Store experience in replay buffer"""
        self.replay_buffer.push(state, action, reward, next_state, done)
    
    def replay(self):
        """Train the network on a batch of experiences"""
        if len(self.replay_buffer) < self.batch_size:
            return
        
        # Sample batch from replay buffer
        states, actions, rewards, next_states, dones = self.replay_buffer.sample(self.batch_size)
        states = states.to(self.device)
        actions = actions.to(self.device)
        rewards = rewards.to(self.device)
        next_states = next_states.to(self.device)
        dones = dones.to(self.device)
        
        # Current Q-values
        current_q_values = self.q_network(states).gather(1, actions.unsqueeze(1))
        
        # Next Q-values from target network
        next_q_values = self.target_network(next_states).max(1)[0].detach()
        target_q_values = rewards + (self.gamma * next_q_values * ~dones)
        
        # Compute loss
        loss = F.mse_loss(current_q_values.squeeze(), target_q_values)
        
        # Optimize
        self.optimizer.zero_grad()
        loss.backward()
        
        # Gradient clipping for stability
        torch.nn.utils.clip_grad_norm_(self.q_network.parameters(), max_norm=1.0)
        
        self.optimizer.step()
        
        # Store loss
        self.losses.append(loss.item())
        
        # Update target network
        self.update_count += 1
        if self.update_count % self.target_update == 0:
            self.target_network.load_state_dict(self.q_network.state_dict())
        
        # Decay epsilon
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

class CartPoleEnvironment:
    """Simplified CartPole environment for demonstration"""
    def __init__(self):
        self.gravity = 9.8
        self.masscart = 1.0
        self.masspole = 0.1
        self.total_mass = (self.masspole + self.masscart)
        self.length = 0.5
        self.polemass_length = (self.masspole * self.length)
        self.force_mag = 10.0
        self.tau = 0.02
        
        # Thresholds
        self.theta_threshold_radians = 12 * 2 * np.pi / 360
        self.x_threshold = 2.4
        
        self.reset()
    
    def reset(self):
        """Reset environment to initial state"""
        self.state = np.random.uniform(low=-0.05, high=0.05, size=(4,))
        self.steps = 0
        return self.state.copy()
    
    def step(self, action):
        """Take action and return next state, reward, done"""
        x, x_dot, theta, theta_dot = self.state
        
        force = self.force_mag if action == 1 else -self.force_mag
        
        costheta = np.cos(theta)
        sintheta = np.sin(theta)
        
        temp = (force + self.polemass_length * theta_dot * theta_dot * sintheta) / self.total_mass
        thetaacc = (self.gravity * sintheta - costheta * temp) / \
                   (self.length * (4.0/3.0 - self.masspole * costheta * costheta / self.total_mass))
        xacc = temp - self.polemass_length * thetaacc * costheta / self.total_mass
        
        x = x + self.tau * x_dot
        x_dot = x_dot + self.tau * xacc
        theta = theta + self.tau * theta_dot
        theta_dot = theta_dot + self.tau * thetaacc
        
        self.state = np.array([x, x_dot, theta, theta_dot])
        
        done = bool(x < -self.x_threshold or x > self.x_threshold or 
                   theta < -self.theta_threshold_radians or theta > self.theta_threshold_radians)
        
        reward = 1.0 if not done else 0.0
        self.steps += 1
        
        # Episode terminates after 500 steps in standard CartPole
        if self.steps >= 500:
            done = True
        
        return self.state.copy(), reward, done, {}

def train_dqn_agent(agent, env, num_episodes=1000, max_steps=500):
    """Train DQN agent on environment"""
    print(f"Training DQN agent for {num_episodes} episodes...")
    
    episode_rewards = []
    episode_lengths = []
    
    for episode in range(num_episodes):
        state = env.reset()
        total_reward = 0
        steps = 0
        
        for step in range(max_steps):
            action = agent.act(state)
            next_state, reward, done, _ = env.step(action)
            
            agent.remember(state, action, reward, next_state, done)
            state = next_state
            total_reward += reward
            steps += 1
            
            # Train the agent
            agent.replay()
            
            if done:
                break
        
        episode_rewards.append(total_reward)
        episode_lengths.append(steps)
        agent.episode_rewards.append(total_reward)
        agent.epsilons.append(agent.epsilon)
        
        if (episode + 1) % 100 == 0:
            avg_reward = np.mean(episode_rewards[-100:])
            print(f"Episode {episode + 1}, Average Reward: {avg_reward:.2f}, Epsilon: {agent.epsilon:.3f}")
    
    print("Training completed!")
    return episode_rewards, episode_lengths

def evaluate_agent(agent, env, num_episodes=100):
    """Evaluate trained agent"""
    print(f"Evaluating agent for {num_episodes} episodes...")
    
    episode_rewards = []
    original_epsilon = agent.epsilon
    agent.epsilon = 0  # No exploration during evaluation
    
    for episode in range(num_episodes):
        state = env.reset()
        total_reward = 0
        
        for step in range(500):
            action = agent.act(state, training=False)
            state, reward, done, _ = env.step(action)
            total_reward += reward
            
            if done:
                break
        
        episode_rewards.append(total_reward)
    
    agent.epsilon = original_epsilon  # Restore epsilon
    
    avg_reward = np.mean(episode_rewards)
    std_reward = np.std(episode_rewards)
    
    print(f"Evaluation Results:")
    print(f"  Average Reward: {avg_reward:.2f} ± {std_reward:.2f}")
    print(f"  Max Reward: {np.max(episode_rewards):.2f}")
    print(f"  Min Reward: {np.min(episode_rewards):.2f}")
    
    return episode_rewards

def plot_training_results(agent, episode_rewards):
    """Plot training progress and statistics"""
    fig, axes = plt.subplots(2, 3, figsize=(18, 10))
    
    # Episode rewards
    axes[0, 0].plot(episode_rewards, alpha=0.7, linewidth=0.5)
    if len(episode_rewards) > 50:
        moving_avg = np.convolve(episode_rewards, np.ones(50)/50, mode='valid')
        axes[0, 0].plot(range(49, len(episode_rewards)), moving_avg, 'r-', linewidth=2)
    
    axes[0, 0].set_title('Episode Rewards')
    axes[0, 0].set_xlabel('Episode')
    axes[0, 0].set_ylabel('Total Reward')
    axes[0, 0].grid(True)
    
    # Training losses
    if agent.losses:
        axes[0, 1].plot(agent.losses, alpha=0.7, linewidth=0.5)
        if len(agent.losses) > 50:
            moving_avg = np.convolve(agent.losses, np.ones(50)/50, mode='valid')
            axes[0, 1].plot(range(49, len(agent.losses)), moving_avg, 'r-', linewidth=2)
    
    axes[0, 1].set_title('Training Loss')
    axes[0, 1].set_xlabel('Training Step')
    axes[0, 1].set_ylabel('Loss')
    axes[0, 1].grid(True)
    
    # Epsilon decay
    axes[0, 2].plot(agent.epsilons, 'g-', linewidth=2)
    axes[0, 2].set_title('Exploration Rate (Epsilon)')
    axes[0, 2].set_xlabel('Episode')
    axes[0, 2].set_ylabel('Epsilon')
    axes[0, 2].grid(True)
    
    # Q-value distribution
    state_samples = np.random.randn(100, agent.state_size)
    state_tensor = torch.FloatTensor(state_samples).to(agent.device)
    with torch.no_grad():
        q_values = agent.q_network(state_tensor)
        q_max = q_values.max(dim=1)[0].cpu().numpy()
        q_mean = q_values.mean(dim=1).cpu().numpy()
    
    axes[1, 0].hist(q_max, bins=20, alpha=0.7, label='Max Q-values')
    axes[1, 0].hist(q_mean, bins=20, alpha=0.7, label='Mean Q-values')
    axes[1, 0].set_title('Q-value Distribution')
    axes[1, 0].set_xlabel('Q-value')
    axes[1, 0].set_ylabel('Frequency')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Recent performance
    if len(episode_rewards) > 200:
        recent_rewards = episode_rewards[-200:]
        axes[1, 1].plot(recent_rewards, 'b-', alpha=0.7)
        moving_avg = np.convolve(recent_rewards, np.ones(20)/20, mode='valid')
        axes[1, 1].plot(range(19, len(recent_rewards)), moving_avg, 'r-', linewidth=2)
        axes[1, 1].set_title('Recent Performance (Last 200 Episodes)')
    else:
        axes[1, 1].plot(episode_rewards, 'b-', alpha=0.7)
        axes[1, 1].set_title('Performance')
    
    axes[1, 1].set_xlabel('Episode')
    axes[1, 1].set_ylabel('Total Reward')
    axes[1, 1].grid(True)
    
    # Learning curve analysis
    window_size = 100
    if len(episode_rewards) > window_size:
        learning_curve = []
        for i in range(window_size, len(episode_rewards) + 1, window_size//4):
            window_avg = np.mean(episode_rewards[i-window_size:i])
            learning_curve.append(window_avg)
        
        axes[1, 2].plot(learning_curve, 'bo-', linewidth=2, markersize=4)
        axes[1, 2].set_title(f'Learning Progress ({window_size}-Episode Windows)')
        axes[1, 2].set_xlabel('Window')
        axes[1, 2].set_ylabel('Average Reward')
        axes[1, 2].grid(True)
    else:
        axes[1, 2].text(0.5, 0.5, 'Insufficient data\nfor learning curve', 
                       ha='center', va='center', transform=axes[1, 2].transAxes)
        axes[1, 2].set_title('Learning Progress')
    
    plt.tight_layout()
    plt.show()

def compare_dqn_variants():
    """Compare standard DQN vs Dueling DQN"""
    print("=== DQN Variants Comparison ===")
    
    env = CartPoleEnvironment()
    state_size = 4
    action_size = 2
    
    # Create agents
    agents = {
        'Standard DQN': DQNAgent(state_size, action_size, dueling=False, lr=1e-3),
        'Dueling DQN': DQNAgent(state_size, action_size, dueling=True, lr=1e-3)
    }
    
    results = {}
    
    for name, agent in agents.items():
        print(f"\nTraining {name}...")
        episode_rewards, episode_lengths = train_dqn_agent(agent, env, num_episodes=800)
        
        # Evaluate trained agent
        eval_rewards = evaluate_agent(agent, env, num_episodes=50)
        
        results[name] = {
            'agent': agent,
            'training_rewards': episode_rewards,
            'evaluation_rewards': eval_rewards,
            'final_performance': np.mean(eval_rewards)
        }
    
    # Plot comparison
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # Training curves
    for name, data in results.items():
        rewards = data['training_rewards']
        if len(rewards) > 50:
            moving_avg = np.convolve(rewards, np.ones(50)/50, mode='valid')
            axes[0, 0].plot(range(49, len(rewards)), moving_avg, linewidth=2, label=name)
        else:
            axes[0, 0].plot(rewards, linewidth=2, label=name)
    
    axes[0, 0].set_title('Training Progress')
    axes[0, 0].set_xlabel('Episode')
    axes[0, 0].set_ylabel('Average Reward (50-episode window)')
    axes[0, 0].legend()
    axes[0, 0].grid(True)
    
    # Final performance comparison
    names = list(results.keys())
    performances = [results[name]['final_performance'] for name in names]
    
    bars = axes[0, 1].bar(names, performances, color=['lightblue', 'lightgreen'])
    axes[0, 1].set_title('Final Performance (50 Test Episodes)')
    axes[0, 1].set_ylabel('Average Reward')
    
    for bar, perf in zip(bars, performances):
        height = bar.get_height()
        axes[0, 1].text(bar.get_x() + bar.get_width()/2., height + 5,
                       f'{perf:.1f}', ha='center', va='bottom')
    
    # Learning stability (variance over time)
    window_size = 100
    for name, data in results.items():
        rewards = data['training_rewards']
        if len(rewards) > window_size:
            stds = []
            for i in range(window_size, len(rewards), 25):
                window_std = np.std(rewards[i-window_size:i])
                stds.append(window_std)
            
            axes[1, 0].plot(stds, linewidth=2, label=f'{name} Std')
    
    axes[1, 0].set_title('Learning Stability (Standard Deviation)')
    axes[1, 0].set_xlabel('Window')
    axes[1, 0].set_ylabel('Reward Standard Deviation')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Evaluation distribution
    for name, data in results.items():
        eval_rewards = data['evaluation_rewards']
        axes[1, 1].hist(eval_rewards, bins=15, alpha=0.7, label=name, density=True)
    
    axes[1, 1].set_title('Evaluation Performance Distribution')
    axes[1, 1].set_xlabel('Episode Reward')
    axes[1, 1].set_ylabel('Density')
    axes[1, 1].legend()
    axes[1, 1].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    return results

def analyze_dqn_components():
    """Analyze key components of DQN"""
    print("\n=== DQN Component Analysis ===")
    
    # Visualize the impact of different components
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # Experience Replay effectiveness
    buffer_sizes = [1000, 5000, 10000, 50000]
    performance_improvement = [15, 35, 45, 50]  # Simulated data
    
    axes[0, 0].plot(buffer_sizes, performance_improvement, 'bo-', linewidth=2, markersize=8)
    axes[0, 0].set_title('Experience Replay Buffer Size Impact')
    axes[0, 0].set_xlabel('Buffer Size')
    axes[0, 0].set_ylabel('Performance Improvement (%)')
    axes[0, 0].grid(True)
    axes[0, 0].set_xscale('log')
    
    # Target Network Update Frequency
    update_freqs = [10, 50, 100, 500, 1000]
    stability_scores = [60, 75, 85, 80, 70]  # Simulated stability scores
    
    axes[0, 1].plot(update_freqs, stability_scores, 'ro-', linewidth=2, markersize=8)
    axes[0, 1].set_title('Target Network Update Frequency')
    axes[0, 1].set_xlabel('Update Frequency (steps)')
    axes[0, 1].set_ylabel('Training Stability Score')
    axes[0, 1].grid(True)
    axes[0, 1].set_xscale('log')
    
    # Epsilon decay strategies
    episodes = np.arange(1000)
    
    # Different decay strategies
    linear_decay = np.maximum(0.01, 1.0 - episodes * 0.001)
    exponential_decay = np.maximum(0.01, 1.0 * (0.995 ** episodes))
    step_decay = np.maximum(0.01, 1.0 * (0.5 ** (episodes // 200)))
    
    axes[1, 0].plot(episodes, linear_decay, label='Linear Decay', linewidth=2)
    axes[1, 0].plot(episodes, exponential_decay, label='Exponential Decay', linewidth=2)
    axes[1, 0].plot(episodes, step_decay, label='Step Decay', linewidth=2)
    
    axes[1, 0].set_title('Epsilon Decay Strategies')
    axes[1, 0].set_xlabel('Episode')
    axes[1, 0].set_ylabel('Epsilon')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # DQN vs Q-Learning comparison
    categories = ['Sample\nEfficiency', 'Convergence\nSpeed', 'Memory\nUsage', 'Scalability']
    dqn_scores = [8, 9, 6, 9]
    q_learning_scores = [6, 4, 9, 3]
    
    x = np.arange(len(categories))
    width = 0.35
    
    axes[1, 1].bar(x - width/2, dqn_scores, width, label='DQN', color='lightblue')
    axes[1, 1].bar(x + width/2, q_learning_scores, width, label='Q-Learning', color='lightcoral')
    
    axes[1, 1].set_title('DQN vs Q-Learning Comparison')
    axes[1, 1].set_ylabel('Score (1-10)')
    axes[1, 1].set_xticks(x)
    axes[1, 1].set_xticklabels(categories)
    axes[1, 1].legend()
    axes[1, 1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Deep Q-Network (DQN) Implementation and Analysis ===")
    
    # Create environment and agent
    env = CartPoleEnvironment()
    state_size = 4  # [x, x_dot, theta, theta_dot]
    action_size = 2  # [left, right]
    
    print(f"Environment: CartPole")
    print(f"State size: {state_size}")
    print(f"Action size: {action_size}")
    
    # Create and train DQN agent
    print("\n=== Training Standard DQN ===")
    agent = DQNAgent(
        state_size=state_size,
        action_size=action_size,
        lr=1e-3,
        gamma=0.99,
        epsilon=1.0,
        epsilon_decay=0.995,
        epsilon_min=0.01,
        buffer_size=10000,
        batch_size=32,
        target_update=100
    )
    
    print(f"Agent parameters: {sum(p.numel() for p in agent.q_network.parameters()):,}")
    
    # Train the agent
    episode_rewards, episode_lengths = train_dqn_agent(agent, env, num_episodes=1000)
    
    # Plot training results
    plot_training_results(agent, episode_rewards)
    
    # Evaluate trained agent
    print("\n=== Agent Evaluation ===")
    evaluation_rewards = evaluate_agent(agent, env, num_episodes=100)
    
    # Compare DQN variants
    comparison_results = compare_dqn_variants()
    
    # Analyze DQN components
    analyze_dqn_components()
    
    print("\n=== DQN Key Innovations ===")
    print("1. EXPERIENCE REPLAY:")
    print("   - Store experiences in replay buffer")
    print("   - Sample random minibatches for training")
    print("   - Breaks temporal correlations")
    print("   - Improves sample efficiency")
    
    print("\n2. TARGET NETWORK:")
    print("   - Separate network for target Q-values")
    print("   - Periodically updated from main network")
    print("   - Provides stable training targets")
    print("   - Prevents divergence and oscillations")
    
    print("\n3. DEEP NEURAL NETWORKS:")
    print("   - Handle high-dimensional state spaces")
    print("   - Automatic feature extraction")
    print("   - Non-linear function approximation")
    print("   - Scalable to complex environments")
    
    print("\n4. EPSILON-GREEDY EXPLORATION:")
    print("   - Balance exploration vs exploitation")
    print("   - Decay epsilon over time")
    print("   - Ensures adequate exploration early")
    print("   - Converges to greedy policy")
    
    print("\n=== DQN Algorithm ===")
    print("1. Initialize replay buffer D and Q-network")
    print("2. For each episode:")
    print("   a. Observe initial state s₀")
    print("   b. For each step:")
    print("      - Select action: a = argmax Q(s,a) with prob 1-ε, random otherwise")
    print("      - Execute action, observe reward r and next state s'")
    print("      - Store (s,a,r,s') in replay buffer")
    print("      - Sample random batch from buffer")
    print("      - Compute target: y = r + γ max Q_target(s',a')")
    print("      - Update Q-network: minimize (y - Q(s,a))²")
    print("      - Periodically update target network")
    
    print("\n=== DQN Improvements ===")
    print("DOUBLE DQN:")
    print("• Use main network to select actions")
    print("• Use target network to evaluate actions")
    print("• Reduces overestimation bias")
    
    print("\nDUELING DQN:")
    print("• Separate value and advantage streams")
    print("• Q(s,a) = V(s) + A(s,a) - mean(A(s,:))")
    print("• Better learning of state values")
    
    print("\nPRIORITIZED EXPERIENCE REPLAY:")
    print("• Sample important transitions more frequently")
    print("• Based on TD error magnitude")
    print("• Improves sample efficiency")
    
    print("\nRAINBOW DQN:")
    print("• Combines multiple improvements")
    print("• Double DQN + Dueling + Prioritized Replay")
    print("• Noisy networks + Multi-step + Distributional")
    
    print("\n=== Advantages over Q-Learning ===")
    print("✓ SCALABILITY: Handles high-dimensional states")
    print("✓ GENERALIZATION: Neural networks generalize")
    print("✓ FEATURE LEARNING: Automatic feature extraction")
    print("✓ CONTINUOUS STATES: No discretization needed")
    
    print("\n=== Limitations ===")
    print("❌ SAMPLE COMPLEXITY: Requires many samples")
    print("❌ HYPERPARAMETER SENSITIVITY: Many hyperparameters")
    print("❌ TRAINING INSTABILITY: Can be unstable")
    print("❌ OVERESTIMATION BIAS: Q-values tend to be overestimated")
    
    print("\n=== Applications ===")
    print("• Atari games (human-level performance)")
    print("• Robotics control")
    print("• Autonomous driving")
    print("• Game playing (Go, Chess)")
    print("• Resource allocation")
    print("• Trading and finance")
    
    print("\n=== DQN Impact ===")
    print("✓ First deep RL breakthrough")
    print("✓ Achieved human-level Atari performance")
    print("✓ Sparked deep RL revolution")
    print("✓ Enabled RL for complex environments")
    print("✓ Foundation for modern deep RL")
    
    print("\n=== Training Tips ===")
    print("• Start with smaller networks and simple environments")
    print("• Use appropriate replay buffer size")
    print("• Tune target network update frequency")
    print("• Monitor Q-value magnitudes")
    print("• Use gradient clipping for stability")
    print("• Implement proper exploration strategy")