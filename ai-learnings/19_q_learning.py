# author: csgod cmps studies
# Q-Learning - Tabular reinforcement learning
# Q-Learning is a model-free reinforcement learning algorithm that learns the value
# of actions in particular states through trial and error interaction with the environment.

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from collections import defaultdict, deque
import random
from typing import Tuple, List, Dict, Optional

class GridWorld:
    """Simple grid world environment for Q-Learning demonstration"""
    def __init__(self, size: int = 5, start: Tuple[int, int] = (0, 0), 
                 goal: Tuple[int, int] = None, obstacles: List[Tuple[int, int]] = None):
        self.size = size
        self.start = start
        self.goal = goal if goal else (size-1, size-1)
        self.obstacles = obstacles if obstacles else []
        
        # Current state
        self.current_state = self.start
        self.done = False
        
        # Action space: up, down, left, right
        self.actions = [0, 1, 2, 3]  # up, down, left, right
        self.action_names = ['↑', '↓', '←', '→']
        
        # Rewards
        self.goal_reward = 100
        self.step_penalty = -1
        self.obstacle_penalty = -50
        
    def reset(self) -> Tuple[int, int]:
        """Reset environment to initial state"""
        self.current_state = self.start
        self.done = False
        return self.current_state
    
    def step(self, action: int) -> Tuple[Tuple[int, int], float, bool, dict]:
        """Take action and return next state, reward, done, info"""
        if self.done:
            return self.current_state, 0, True, {}
        
        # Get next state based on action
        next_state = self._get_next_state(self.current_state, action)
        
        # Calculate reward
        reward = self._get_reward(next_state)
        
        # Update current state
        self.current_state = next_state
        
        # Check if episode is done
        self.done = (next_state == self.goal)
        
        return next_state, reward, self.done, {}
    
    def _get_next_state(self, state: Tuple[int, int], action: int) -> Tuple[int, int]:
        """Get next state given current state and action"""
        row, col = state
        
        if action == 0:  # up
            next_state = (max(0, row - 1), col)
        elif action == 1:  # down
            next_state = (min(self.size - 1, row + 1), col)
        elif action == 2:  # left
            next_state = (row, max(0, col - 1))
        elif action == 3:  # right
            next_state = (row, min(self.size - 1, col + 1))
        else:
            next_state = state
        
        return next_state
    
    def _get_reward(self, state: Tuple[int, int]) -> float:
        """Calculate reward for reaching a state"""
        if state == self.goal:
            return self.goal_reward
        elif state in self.obstacles:
            return self.obstacle_penalty
        else:
            return self.step_penalty
    
    def render(self, q_table: Optional[Dict] = None, policy: Optional[Dict] = None):
        """Visualize the grid world"""
        grid = np.zeros((self.size, self.size))
        
        # Mark goal
        grid[self.goal] = 1
        
        # Mark obstacles
        for obs in self.obstacles:
            grid[obs] = -1
        
        # Mark current position
        if not self.done:
            grid[self.current_state] = 0.5
        
        plt.figure(figsize=(10, 8))
        
        if q_table is not None and policy is not None:
            # Show Q-values and policy
            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
            
            # Q-values heatmap
            q_values = np.zeros((self.size, self.size))
            for state, actions in q_table.items():
                if actions:
                    q_values[state] = max(actions.values())
            
            sns.heatmap(q_values, annot=True, fmt='.1f', cmap='RdYlBu_r', ax=ax1)
            ax1.set_title('Q-Values (State Values)')
            
            # Policy arrows
            ax2.imshow(grid, cmap='RdYlGn')
            for state, actions in q_table.items():
                if actions and state not in self.obstacles:
                    best_action = max(actions, key=actions.get)
                    row, col = state
                    
                    # Draw arrow for best action
                    if best_action == 0:  # up
                        ax2.arrow(col, row, 0, -0.3, head_width=0.1, head_length=0.1, fc='black', ec='black')
                    elif best_action == 1:  # down
                        ax2.arrow(col, row, 0, 0.3, head_width=0.1, head_length=0.1, fc='black', ec='black')
                    elif best_action == 2:  # left
                        ax2.arrow(col, row, -0.3, 0, head_width=0.1, head_length=0.1, fc='black', ec='black')
                    elif best_action == 3:  # right
                        ax2.arrow(col, row, 0.3, 0, head_width=0.1, head_length=0.1, fc='black', ec='black')
            
            ax2.set_title('Policy (Best Actions)')
            ax2.set_xlim(-0.5, self.size-0.5)
            ax2.set_ylim(-0.5, self.size-0.5)
        else:
            # Simple grid visualization
            plt.imshow(grid, cmap='RdYlGn')
            plt.title('Grid World')
        
        plt.colorbar()
        plt.show()

class QLearningAgent:
    """Q-Learning agent implementation"""
    def __init__(self, learning_rate: float = 0.1, discount_factor: float = 0.95, 
                 epsilon: float = 1.0, epsilon_decay: float = 0.995, min_epsilon: float = 0.01):
        self.learning_rate = learning_rate
        self.discount_factor = discount_factor
        self.epsilon = epsilon
        self.epsilon_decay = epsilon_decay
        self.min_epsilon = min_epsilon
        
        # Q-table: state -> {action: q_value}
        self.q_table = defaultdict(lambda: defaultdict(float))
        
        # Training statistics
        self.episode_rewards = []
        self.episode_lengths = []
        self.epsilons = []
    
    def get_action(self, state: Tuple[int, int], available_actions: List[int]) -> int:
        """Choose action using epsilon-greedy policy"""
        if random.random() < self.epsilon:
            # Exploration: random action
            return random.choice(available_actions)
        else:
            # Exploitation: best known action
            q_values = {action: self.q_table[state][action] for action in available_actions}
            if not q_values or all(v == 0 for v in q_values.values()):
                return random.choice(available_actions)
            return max(q_values, key=q_values.get)
    
    def update_q_value(self, state: Tuple[int, int], action: int, reward: float, 
                      next_state: Tuple[int, int], available_next_actions: List[int]):
        """Update Q-value using Q-learning update rule"""
        # Current Q-value
        current_q = self.q_table[state][action]
        
        # Maximum Q-value for next state
        if available_next_actions:
            max_next_q = max(self.q_table[next_state][a] for a in available_next_actions)
        else:
            max_next_q = 0
        
        # Q-learning update: Q(s,a) = Q(s,a) + α[r + γ*max_Q(s',a') - Q(s,a)]
        new_q = current_q + self.learning_rate * (reward + self.discount_factor * max_next_q - current_q)
        self.q_table[state][action] = new_q
    
    def decay_epsilon(self):
        """Decay exploration rate"""
        self.epsilon = max(self.min_epsilon, self.epsilon * self.epsilon_decay)
    
    def get_policy(self) -> Dict[Tuple[int, int], int]:
        """Extract policy from Q-table"""
        policy = {}
        for state, actions in self.q_table.items():
            if actions:
                policy[state] = max(actions, key=actions.get)
        return policy

def train_q_learning(env: GridWorld, agent: QLearningAgent, num_episodes: int = 1000):
    """Train Q-Learning agent"""
    print(f"Training Q-Learning agent for {num_episodes} episodes...")
    
    for episode in range(num_episodes):
        state = env.reset()
        total_reward = 0
        steps = 0
        
        while not env.done and steps < 200:  # Max steps per episode
            # Get available actions
            available_actions = env.actions
            
            # Choose action
            action = agent.get_action(state, available_actions)
            
            # Take action
            next_state, reward, done, _ = env.step(action)
            
            # Update Q-value
            agent.update_q_value(state, action, reward, next_state, available_actions)
            
            # Update state and stats
            state = next_state
            total_reward += reward
            steps += 1
        
        # Store episode statistics
        agent.episode_rewards.append(total_reward)
        agent.episode_lengths.append(steps)
        agent.epsilons.append(agent.epsilon)
        
        # Decay epsilon
        agent.decay_epsilon()
        
        # Print progress
        if (episode + 1) % 100 == 0:
            avg_reward = np.mean(agent.episode_rewards[-100:])
            avg_length = np.mean(agent.episode_lengths[-100:])
            print(f"Episode {episode + 1}: Avg Reward = {avg_reward:.2f}, "
                  f"Avg Steps = {avg_length:.2f}, Epsilon = {agent.epsilon:.3f}")
    
    print("Training completed!")

def evaluate_agent(env: GridWorld, agent: QLearningAgent, num_episodes: int = 100) -> Dict:
    """Evaluate trained agent"""
    print(f"Evaluating agent for {num_episodes} episodes...")
    
    # Save original epsilon and set to 0 (no exploration)
    original_epsilon = agent.epsilon
    agent.epsilon = 0
    
    rewards = []
    steps_to_goal = []
    success_count = 0
    
    for episode in range(num_episodes):
        state = env.reset()
        total_reward = 0
        steps = 0
        
        while not env.done and steps < 200:
            action = agent.get_action(state, env.actions)
            state, reward, done, _ = env.step(action)
            total_reward += reward
            steps += 1
        
        rewards.append(total_reward)
        steps_to_goal.append(steps)
        
        if env.done:
            success_count += 1
    
    # Restore original epsilon
    agent.epsilon = original_epsilon
    
    results = {
        'success_rate': success_count / num_episodes,
        'avg_reward': np.mean(rewards),
        'avg_steps': np.mean(steps_to_goal),
        'std_reward': np.std(rewards),
        'std_steps': np.std(steps_to_goal)
    }
    
    print(f"Evaluation Results:")
    print(f"  Success Rate: {results['success_rate']:.2%}")
    print(f"  Average Reward: {results['avg_reward']:.2f} ± {results['std_reward']:.2f}")
    print(f"  Average Steps to Goal: {results['avg_steps']:.2f} ± {results['std_steps']:.2f}")
    
    return results

def plot_training_progress(agent: QLearningAgent):
    """Plot training progress"""
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # Reward per episode
    axes[0, 0].plot(agent.episode_rewards, alpha=0.7, linewidth=0.5)
    # Moving average
    window_size = 50
    if len(agent.episode_rewards) > window_size:
        moving_avg = np.convolve(agent.episode_rewards, np.ones(window_size)/window_size, mode='valid')
        axes[0, 0].plot(range(window_size-1, len(agent.episode_rewards)), moving_avg, 'r-', linewidth=2, label='Moving Average')
        axes[0, 0].legend()
    
    axes[0, 0].set_title('Episode Rewards')
    axes[0, 0].set_xlabel('Episode')
    axes[0, 0].set_ylabel('Total Reward')
    axes[0, 0].grid(True)
    
    # Episode length
    axes[0, 1].plot(agent.episode_lengths, alpha=0.7, linewidth=0.5)
    if len(agent.episode_lengths) > window_size:
        moving_avg = np.convolve(agent.episode_lengths, np.ones(window_size)/window_size, mode='valid')
        axes[0, 1].plot(range(window_size-1, len(agent.episode_lengths)), moving_avg, 'r-', linewidth=2, label='Moving Average')
        axes[0, 1].legend()
    
    axes[0, 1].set_title('Episode Lengths')
    axes[0, 1].set_xlabel('Episode')
    axes[0, 1].set_ylabel('Steps to Goal')
    axes[0, 1].grid(True)
    
    # Epsilon decay
    axes[1, 0].plot(agent.epsilons, 'g-', linewidth=2)
    axes[1, 0].set_title('Exploration Rate (Epsilon)')
    axes[1, 0].set_xlabel('Episode')
    axes[1, 0].set_ylabel('Epsilon')
    axes[1, 0].grid(True)
    
    # Learning curve (last 200 episodes)
    if len(agent.episode_rewards) > 200:
        recent_rewards = agent.episode_rewards[-200:]
        axes[1, 1].plot(recent_rewards, 'b-', alpha=0.7, linewidth=1)
        moving_avg = np.convolve(recent_rewards, np.ones(20)/20, mode='valid')
        axes[1, 1].plot(range(19, len(recent_rewards)), moving_avg, 'r-', linewidth=2)
        axes[1, 1].set_title('Recent Performance (Last 200 Episodes)')
    else:
        axes[1, 1].plot(agent.episode_rewards, 'b-', alpha=0.7)
        axes[1, 1].set_title('All Episodes Performance')
    
    axes[1, 1].set_xlabel('Episode')
    axes[1, 1].set_ylabel('Total Reward')
    axes[1, 1].grid(True)
    
    plt.tight_layout()
    plt.show()

def compare_hyperparameters():
    """Compare different hyperparameter settings"""
    print("=== Hyperparameter Comparison ===")
    
    # Create environment
    obstacles = [(1, 1), (1, 2), (2, 1), (3, 3)]
    env = GridWorld(size=5, obstacles=obstacles)
    
    # Different hyperparameter configurations
    configs = {
        'Standard': {'learning_rate': 0.1, 'discount_factor': 0.95, 'epsilon_decay': 0.995},
        'High LR': {'learning_rate': 0.5, 'discount_factor': 0.95, 'epsilon_decay': 0.995},
        'Low Discount': {'learning_rate': 0.1, 'discount_factor': 0.8, 'epsilon_decay': 0.995},
        'Fast Decay': {'learning_rate': 0.1, 'discount_factor': 0.95, 'epsilon_decay': 0.99},
        'Slow Decay': {'learning_rate': 0.1, 'discount_factor': 0.95, 'epsilon_decay': 0.999},
    }
    
    results = {}
    
    for name, config in configs.items():
        print(f"\nTraining with {name} configuration...")
        
        # Create agent
        agent = QLearningAgent(**config)
        
        # Train
        train_q_learning(env, agent, num_episodes=500)
        
        # Evaluate
        eval_results = evaluate_agent(env, agent, num_episodes=100)
        
        results[name] = {
            'agent': agent,
            'eval_results': eval_results,
            'config': config
        }
    
    # Plot comparison
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # Success rates
    names = list(results.keys())
    success_rates = [results[name]['eval_results']['success_rate'] for name in names]
    
    axes[0, 0].bar(names, success_rates)
    axes[0, 0].set_title('Success Rate Comparison')
    axes[0, 0].set_ylabel('Success Rate')
    axes[0, 0].tick_params(axis='x', rotation=45)
    
    # Average rewards
    avg_rewards = [results[name]['eval_results']['avg_reward'] for name in names]
    axes[0, 1].bar(names, avg_rewards)
    axes[0, 1].set_title('Average Reward Comparison')
    axes[0, 1].set_ylabel('Average Reward')
    axes[0, 1].tick_params(axis='x', rotation=45)
    
    # Learning curves
    for name, data in results.items():
        agent = data['agent']
        if len(agent.episode_rewards) > 50:
            moving_avg = np.convolve(agent.episode_rewards, np.ones(50)/50, mode='valid')
            axes[1, 0].plot(moving_avg, label=name, linewidth=2)
    
    axes[1, 0].set_title('Learning Curves')
    axes[1, 0].set_xlabel('Episode')
    axes[1, 0].set_ylabel('Average Reward')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Steps to goal
    avg_steps = [results[name]['eval_results']['avg_steps'] for name in names]
    axes[1, 1].bar(names, avg_steps)
    axes[1, 1].set_title('Average Steps to Goal')
    axes[1, 1].set_ylabel('Steps')
    axes[1, 1].tick_params(axis='x', rotation=45)
    
    plt.tight_layout()
    plt.show()
    
    return results

def demonstrate_exploration_exploitation():
    """Demonstrate exploration vs exploitation tradeoff"""
    print("\n=== Exploration vs Exploitation Demo ===")
    
    # Create simple environment
    env = GridWorld(size=4, obstacles=[(1, 1)])
    
    # Different exploration strategies
    agents = {
        'High Exploration': QLearningAgent(epsilon=0.9, epsilon_decay=0.999),
        'Balanced': QLearningAgent(epsilon=0.1, epsilon_decay=0.995),
        'Low Exploration': QLearningAgent(epsilon=0.05, epsilon_decay=0.99),
        'Greedy': QLearningAgent(epsilon=0.01, epsilon_decay=1.0)
    }
    
    # Train all agents
    for name, agent in agents.items():
        print(f"Training {name} agent...")
        train_q_learning(env, agent, num_episodes=300)
    
    # Plot comparison
    plt.figure(figsize=(15, 10))
    
    # Learning curves
    plt.subplot(2, 2, 1)
    for name, agent in agents.items():
        window_size = 30
        moving_avg = np.convolve(agent.episode_rewards, np.ones(window_size)/window_size, mode='valid')
        plt.plot(moving_avg, label=name, linewidth=2)
    
    plt.title('Learning Curves (30-episode moving average)')
    plt.xlabel('Episode')
    plt.ylabel('Average Reward')
    plt.legend()
    plt.grid(True)
    
    # Epsilon decay
    plt.subplot(2, 2, 2)
    for name, agent in agents.items():
        plt.plot(agent.epsilons, label=name, linewidth=2)
    
    plt.title('Exploration Rate Over Time')
    plt.xlabel('Episode')
    plt.ylabel('Epsilon')
    plt.legend()
    plt.grid(True)
    
    # Final performance evaluation
    plt.subplot(2, 2, 3)
    final_performance = {}
    for name, agent in agents.items():
        eval_results = evaluate_agent(env, agent, num_episodes=100)
        final_performance[name] = eval_results['success_rate']
    
    names = list(final_performance.keys())
    success_rates = list(final_performance.values())
    plt.bar(names, success_rates)
    plt.title('Final Success Rate')
    plt.ylabel('Success Rate')
    plt.tick_params(axis='x', rotation=45)
    
    # Q-table size (exploration indicator)
    plt.subplot(2, 2, 4)
    q_table_sizes = {name: len(agent.q_table) for name, agent in agents.items()}
    names = list(q_table_sizes.keys())
    sizes = list(q_table_sizes.values())
    plt.bar(names, sizes)
    plt.title('Q-table Size (States Explored)')
    plt.ylabel('Number of States')
    plt.tick_params(axis='x', rotation=45)
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Q-Learning Implementation and Analysis ===")
    
    # Create environment with obstacles
    obstacles = [(1, 2), (2, 1), (2, 2), (3, 3)]
    env = GridWorld(size=6, start=(0, 0), goal=(5, 5), obstacles=obstacles)
    
    print("Environment Setup:")
    print(f"Grid size: {env.size}x{env.size}")
    print(f"Start: {env.start}")
    print(f"Goal: {env.goal}")
    print(f"Obstacles: {env.obstacles}")
    
    # Create and train Q-Learning agent
    print("\n=== Training Q-Learning Agent ===")
    agent = QLearningAgent(
        learning_rate=0.1,
        discount_factor=0.95,
        epsilon=1.0,
        epsilon_decay=0.995,
        min_epsilon=0.01
    )
    
    # Train the agent
    train_q_learning(env, agent, num_episodes=1000)
    
    # Plot training progress
    plot_training_progress(agent)
    
    # Evaluate trained agent
    print("\n=== Agent Evaluation ===")
    evaluation_results = evaluate_agent(env, agent, num_episodes=100)
    
    # Visualize learned policy
    print("\n=== Learned Policy Visualization ===")
    env.render(agent.q_table, agent.get_policy())
    
    # Compare different hyperparameters
    hyperparameter_results = compare_hyperparameters()
    
    # Demonstrate exploration vs exploitation
    demonstrate_exploration_exploitation()
    
    print("\n=== Q-Learning Key Concepts ===")
    print("1. TEMPORAL DIFFERENCE LEARNING:")
    print("   - Learn from difference between predicted and actual rewards")
    print("   - Update estimates based on subsequent estimates")
    print("   - No need for complete episodes")
    
    print("\n2. Q-LEARNING UPDATE RULE:")
    print("   Q(s,a) = Q(s,a) + α[r + γ*max_Q(s',a') - Q(s,a)]")
    print("   - α: Learning rate (how much to update)")
    print("   - γ: Discount factor (importance of future rewards)")
    print("   - Off-policy: learns optimal policy regardless of behavior policy")
    
    print("\n3. EXPLORATION vs EXPLOITATION:")
    print("   - Exploration: Try new actions to discover better strategies")
    print("   - Exploitation: Use known good actions")
    print("   - ε-greedy: Balance with epsilon parameter")
    print("   - Epsilon decay: Reduce exploration over time")
    
    print("\n4. Q-TABLE:")
    print("   - Tabular representation: Q(state, action)")
    print("   - Works for discrete, small state spaces")
    print("   - Each entry represents expected future reward")
    print("   - Policy extracted as argmax_a Q(s,a)")
    
    print("\n=== Q-Learning Properties ===")
    print("✓ MODEL-FREE: No need to know environment dynamics")
    print("✓ OFF-POLICY: Can learn optimal policy from any behavior policy")
    print("✓ GUARANTEED CONVERGENCE: Under certain conditions")
    print("✓ SIMPLE IMPLEMENTATION: Easy to understand and code")
    
    print("\n=== Limitations ===")
    print("❌ TABULAR ONLY: Doesn't scale to large state spaces")
    print("❌ DISCRETE SPACES: Requires discretization of continuous spaces")
    print("❌ SLOW CONVERGENCE: May require many episodes")
    print("❌ MEMORY INTENSIVE: Q-table grows with state-action pairs")
    
    print("\n=== Extensions & Improvements ===")
    print("• Function Approximation: Neural networks for large state spaces")
    print("• Deep Q-Networks (DQN): Combine Q-learning with deep learning")
    print("• Double Q-Learning: Reduce overestimation bias")
    print("• Prioritized Experience Replay: Learn from important transitions")
    print("• Dueling Networks: Separate state value and advantage functions")
    
    print("\n=== Applications ===")
    print("• Game playing (board games, video games)")
    print("• Robot navigation and control")
    print("• Resource allocation")
    print("• Trading and finance")
    print("• Recommendation systems")
    print("• Network routing")
    
    print("\n=== Hyperparameter Tuning Tips ===")
    print("• Learning Rate α: Start with 0.1, decrease for stability")
    print("• Discount Factor γ: 0.9-0.99 for most problems")
    print("• Epsilon: Start high (1.0), decay to 0.01-0.1")
    print("• Epsilon Decay: 0.995-0.999 for gradual reduction")
    print("• Balance exploration and convergence speed")