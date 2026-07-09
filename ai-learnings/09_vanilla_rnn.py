# author: csgod cmps studies
# Vanilla RNN - Basic sequential processing
# Vanilla RNNs process sequences by maintaining hidden state that gets updated at each time step,
# enabling them to capture temporal dependencies in sequential data.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset
import string
import random

class VanillaRNN:
    """Numpy implementation of Vanilla RNN for educational purposes"""
    def __init__(self, input_size, hidden_size, output_size, learning_rate=0.001):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.learning_rate = learning_rate
        
        # Initialize weights
        # Input to hidden weights
        self.Wxh = np.random.randn(hidden_size, input_size) * 0.01
        # Hidden to hidden weights
        self.Whh = np.random.randn(hidden_size, hidden_size) * 0.01
        # Hidden to output weights
        self.Why = np.random.randn(output_size, hidden_size) * 0.01
        # Biases
        self.bh = np.zeros((hidden_size, 1))
        self.by = np.zeros((output_size, 1))
    
    def forward(self, inputs, h_prev):
        """Forward pass through RNN"""
        xs, hs, ys, ps = {}, {}, {}, {}
        hs[-1] = np.copy(h_prev)
        
        for t in range(len(inputs)):
            xs[t] = np.zeros((self.input_size, 1))
            xs[t][inputs[t]] = 1  # One-hot encoding
            
            # Hidden state: h_t = tanh(W_xh * x_t + W_hh * h_{t-1} + b_h)
            hs[t] = np.tanh(np.dot(self.Wxh, xs[t]) + np.dot(self.Whh, hs[t-1]) + self.bh)
            
            # Output: y_t = W_hy * h_t + b_y
            ys[t] = np.dot(self.Why, hs[t]) + self.by
            
            # Softmax probabilities
            ps[t] = np.exp(ys[t]) / np.sum(np.exp(ys[t]))
        
        return xs, hs, ys, ps
    
    def backward(self, xs, hs, ps, targets):
        """Backward pass - compute gradients"""
        dWxh = np.zeros_like(self.Wxh)
        dWhh = np.zeros_like(self.Whh)
        dWhy = np.zeros_like(self.Why)
        dbh = np.zeros_like(self.bh)
        dby = np.zeros_like(self.by)
        dhnext = np.zeros_like(hs[0])
        
        for t in reversed(range(len(targets))):
            # Output layer gradients
            dy = np.copy(ps[t])
            dy[targets[t]] -= 1  # Cross-entropy gradient
            
            dWhy += np.dot(dy, hs[t].T)
            dby += dy
            
            # Hidden layer gradients
            dh = np.dot(self.Why.T, dy) + dhnext
            dhraw = (1 - hs[t] * hs[t]) * dh  # tanh derivative
            
            dbh += dhraw
            dWxh += np.dot(dhraw, xs[t].T)
            dWhh += np.dot(dhraw, hs[t-1].T)
            dhnext = np.dot(self.Whh.T, dhraw)
        
        # Clip gradients to prevent exploding gradients
        for dparam in [dWxh, dWhh, dWhy, dbh, dby]:
            np.clip(dparam, -5, 5, out=dparam)
        
        return dWxh, dWhh, dWhy, dbh, dby
    
    def update_parameters(self, dWxh, dWhh, dWhy, dbh, dby):
        """Update parameters using gradients"""
        self.Wxh -= self.learning_rate * dWxh
        self.Whh -= self.learning_rate * dWhh
        self.Why -= self.learning_rate * dWhy
        self.bh -= self.learning_rate * dbh
        self.by -= self.learning_rate * dby
    
    def sample(self, seed_ix, n):
        """Generate a sequence of characters"""
        h = np.zeros((self.hidden_size, 1))
        x = np.zeros((self.input_size, 1))
        x[seed_ix] = 1
        
        ixes = []
        for t in range(n):
            h = np.tanh(np.dot(self.Wxh, x) + np.dot(self.Whh, h) + self.bh)
            y = np.dot(self.Why, h) + self.by
            p = np.exp(y) / np.sum(np.exp(y))
            ix = np.random.choice(range(self.input_size), p=p.ravel())
            x = np.zeros((self.input_size, 1))
            x[ix] = 1
            ixes.append(ix)
        
        return ixes

class TorchVanillaRNN(nn.Module):
    """PyTorch implementation of Vanilla RNN"""
    def __init__(self, input_size, hidden_size, output_size, num_layers=1):
        super(TorchVanillaRNN, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        
        self.rnn = nn.RNN(input_size, hidden_size, num_layers, batch_first=True)
        self.fc = nn.Linear(hidden_size, output_size)
        
    def forward(self, x, h0=None):
        if h0 is None:
            h0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)
        
        out, hidden = self.rnn(x, h0)
        out = self.fc(out)
        return out, hidden

class CharDataset(Dataset):
    """Dataset for character-level language modeling"""
    def __init__(self, text, seq_length=25):
        self.seq_length = seq_length
        
        # Create character mappings
        self.chars = sorted(list(set(text)))
        self.char_to_idx = {ch: i for i, ch in enumerate(self.chars)}
        self.idx_to_char = {i: ch for i, ch in enumerate(self.chars)}
        
        # Convert text to indices
        self.data = [self.char_to_idx[ch] for ch in text]
        
    def __len__(self):
        return len(self.data) - self.seq_length
    
    def __getitem__(self, idx):
        seq = self.data[idx:idx + self.seq_length]
        target = self.data[idx + 1:idx + self.seq_length + 1]
        return torch.tensor(seq, dtype=torch.long), torch.tensor(target, dtype=torch.long)

def train_numpy_rnn():
    """Train the numpy RNN on a simple character prediction task"""
    print("=== Training Numpy RNN ===")
    
    # Simple text data
    text = "hello world this is a simple example for rnn training with repeating patterns hello world"
    chars = list(set(text))
    data_size, vocab_size = len(text), len(chars)
    
    print(f'Data size: {data_size}, Vocabulary size: {vocab_size}')
    
    char_to_ix = {ch: i for i, ch in enumerate(chars)}
    ix_to_char = {i: ch for i, ch in enumerate(chars)}
    
    # Hyperparameters
    hidden_size = 50
    seq_length = 10
    learning_rate = 0.1
    
    # Initialize RNN
    rnn = VanillaRNN(vocab_size, hidden_size, vocab_size, learning_rate)
    
    # Training parameters
    n_epochs = 1000
    losses = []
    
    # Training loop
    for epoch in range(n_epochs):
        # Prepare inputs and targets
        p = random.randint(0, len(text) - seq_length - 1)
        inputs = [char_to_ix[ch] for ch in text[p:p + seq_length]]
        targets = [char_to_ix[ch] for ch in text[p + 1:p + seq_length + 1]]
        
        # Initialize hidden state
        h_prev = np.zeros((hidden_size, 1))
        
        # Forward pass
        xs, hs, ys, ps = rnn.forward(inputs, h_prev)
        
        # Compute loss
        loss = 0
        for t in range(len(targets)):
            loss += -np.log(ps[t][targets[t], 0])
        
        losses.append(loss)
        
        # Backward pass
        dWxh, dWhh, dWhy, dbh, dby = rnn.backward(xs, hs, ps, targets)
        
        # Update parameters
        rnn.update_parameters(dWxh, dWhh, dWhy, dbh, dby)
        
        # Print progress
        if epoch % 100 == 0:
            print(f'Epoch {epoch}, Loss: {loss:.4f}')
            
            # Sample some text
            sample_ix = rnn.sample(char_to_ix['h'], 20)
            sample_text = ''.join(ix_to_char[ix] for ix in sample_ix)
            print(f'Sample: "h{sample_text}"')
    
    return rnn, losses, char_to_ix, ix_to_char

def train_pytorch_rnn():
    """Train PyTorch RNN on character prediction"""
    print("\n=== Training PyTorch RNN ===")
    
    # Load text data
    text = """
    The quick brown fox jumps over the lazy dog. 
    This is a longer text to train our RNN model.
    RNNs are good at sequential data processing.
    They can learn patterns in text and generate new sequences.
    However, they suffer from vanishing gradient problem.
    """ * 10  # Repeat for more data
    
    # Create dataset
    dataset = CharDataset(text, seq_length=30)
    dataloader = DataLoader(dataset, batch_size=32, shuffle=True)
    
    print(f'Vocabulary size: {len(dataset.chars)}')
    print(f'Dataset size: {len(dataset)}')
    
    # Model parameters
    vocab_size = len(dataset.chars)
    hidden_size = 128
    num_layers = 2
    
    # Create model
    model = TorchVanillaRNN(vocab_size, hidden_size, vocab_size, num_layers)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=0.002)
    
    # Training
    num_epochs = 20
    losses = []
    
    model.train()
    for epoch in range(num_epochs):
        epoch_loss = 0
        for batch_idx, (data, targets) in enumerate(dataloader):
            # One-hot encode input
            data_onehot = torch.zeros(data.size(0), data.size(1), vocab_size)
            data_onehot.scatter_(2, data.unsqueeze(2), 1)
            
            optimizer.zero_grad()
            
            # Forward pass
            outputs, _ = model(data_onehot)
            loss = criterion(outputs.reshape(-1, vocab_size), targets.reshape(-1))
            
            # Backward pass
            loss.backward()
            
            # Clip gradients to prevent exploding gradients
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
            
            optimizer.step()
            
            epoch_loss += loss.item()
        
        avg_loss = epoch_loss / len(dataloader)
        losses.append(avg_loss)
        print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {avg_loss:.4f}')
    
    return model, losses, dataset

def generate_text(model, dataset, seed_text="The", length=100):
    """Generate text using trained model"""
    model.eval()
    
    # Convert seed text to indices
    current_seq = [dataset.char_to_idx.get(ch, 0) for ch in seed_text]
    generated_text = seed_text
    
    with torch.no_grad():
        for _ in range(length):
            # Prepare input
            seq_tensor = torch.tensor([current_seq], dtype=torch.long)
            seq_onehot = torch.zeros(1, len(current_seq), len(dataset.chars))
            seq_onehot.scatter_(2, seq_tensor.unsqueeze(2), 1)
            
            # Forward pass
            outputs, _ = model(seq_onehot)
            
            # Get probabilities for next character
            probs = torch.softmax(outputs[0, -1], dim=0).numpy()
            
            # Sample next character
            next_idx = np.random.choice(len(dataset.chars), p=probs)
            next_char = dataset.idx_to_char[next_idx]
            
            generated_text += next_char
            
            # Update sequence (keep last 29 characters + new one)
            current_seq = current_seq[1:] + [next_idx]
    
    return generated_text

def visualize_hidden_states(model, dataset, text="hello world"):
    """Visualize how hidden states evolve during processing"""
    model.eval()
    
    # Convert text to indices and one-hot
    indices = [dataset.char_to_idx.get(ch, 0) for ch in text]
    seq_tensor = torch.tensor([indices], dtype=torch.long)
    seq_onehot = torch.zeros(1, len(indices), len(dataset.chars))
    seq_onehot.scatter_(2, seq_tensor.unsqueeze(2), 1)
    
    # Get hidden states
    with torch.no_grad():
        outputs, hidden_states = model.rnn(seq_onehot)
        hidden_states = hidden_states.squeeze().numpy()  # Remove batch dimension
    
    # Plot hidden state evolution
    plt.figure(figsize=(12, 8))
    
    # Plot hidden state values over time
    plt.subplot(2, 1, 1)
    for i in range(min(10, hidden_states.shape[1])):  # Plot first 10 hidden units
        plt.plot(hidden_states[:, i], label=f'Unit {i}')
    plt.title('Hidden State Evolution')
    plt.xlabel('Time Step')
    plt.ylabel('Activation')
    plt.legend()
    plt.grid(True)
    
    # Plot heatmap of all hidden states
    plt.subplot(2, 1, 2)
    plt.imshow(hidden_states.T, aspect='auto', cmap='RdBu_r')
    plt.title('Hidden States Heatmap')
    plt.xlabel('Time Step')
    plt.ylabel('Hidden Unit')
    plt.colorbar()
    
    # Add character labels
    char_positions = range(len(text))
    plt.xticks(char_positions, list(text))
    
    plt.tight_layout()
    plt.show()

def demonstrate_vanishing_gradients():
    """Demonstrate vanishing gradient problem in vanilla RNNs"""
    print("\n=== Vanishing Gradient Demonstration ===")
    
    # Create a simple RNN and show how gradients diminish
    seq_length = 20
    hidden_size = 10
    
    # Initialize weights
    W = np.random.randn(hidden_size, hidden_size) * 0.1
    
    # Simulate gradient backpropagation through time
    gradients = []
    current_grad = np.ones(hidden_size)
    
    for t in reversed(range(seq_length)):
        # Simulate tanh derivative (max value is 1)
        tanh_derivative = np.random.uniform(0, 1, hidden_size)
        
        # Gradient flows back: grad = W^T @ grad * tanh'
        current_grad = np.dot(W.T, current_grad) * tanh_derivative
        gradient_norm = np.linalg.norm(current_grad)
        gradients.append(gradient_norm)
    
    gradients.reverse()
    
    plt.figure(figsize=(10, 6))
    plt.plot(range(seq_length), gradients, 'b-o', linewidth=2)
    plt.title('Gradient Magnitude Through Time (Vanilla RNN)')
    plt.xlabel('Time Step (from beginning)')
    plt.ylabel('Gradient Norm')
    plt.yscale('log')
    plt.grid(True)
    plt.show()
    
    print(f"Initial gradient norm: {gradients[0]:.6f}")
    print(f"Final gradient norm: {gradients[-1]:.6f}")
    print(f"Gradient decay factor: {gradients[-1]/gradients[0]:.6f}")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Vanilla RNN Implementation and Analysis ===")
    
    # Train numpy implementation
    numpy_rnn, numpy_losses, char_to_ix, ix_to_char = train_numpy_rnn()
    
    # Train PyTorch implementation
    torch_model, torch_losses, dataset = train_pytorch_rnn()
    
    # Plot training curves
    plt.figure(figsize=(12, 5))
    
    plt.subplot(1, 2, 1)
    plt.plot(numpy_losses)
    plt.title('Numpy RNN Training Loss')
    plt.xlabel('Iteration')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.subplot(1, 2, 2)
    plt.plot(torch_losses)
    plt.title('PyTorch RNN Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Generate text
    print("\n=== Text Generation ===")
    generated = generate_text(torch_model, dataset, "The quick", 100)
    print(f"Generated text: {generated}")
    
    # Visualize hidden states
    print("\n=== Hidden State Visualization ===")
    visualize_hidden_states(torch_model, dataset, "hello world")
    
    # Demonstrate vanishing gradients
    demonstrate_vanishing_gradients()
    
    print("\n=== Key Properties of Vanilla RNNs ===")
    print("1. Process sequences one element at a time")
    print("2. Maintain hidden state that captures past information")
    print("3. Share parameters across time steps")
    print("4. Suffer from vanishing gradient problem")
    print("5. Difficulty learning long-term dependencies")
    print("6. Simple architecture but limited practical use")
    print("7. Foundation for more advanced RNN variants (LSTM, GRU)")