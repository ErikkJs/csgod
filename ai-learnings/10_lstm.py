# author: csgod cmps studies
# LSTM - Long Short-Term Memory networks
# LSTMs solve the vanishing gradient problem of vanilla RNNs through gating mechanisms
# that control information flow, enabling learning of long-term dependencies.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset
import seaborn as sns

class LSTMCell:
    """Numpy implementation of LSTM cell for educational purposes"""
    def __init__(self, input_size, hidden_size):
        self.input_size = input_size
        self.hidden_size = hidden_size
        
        # Initialize weights for forget gate
        self.Wf = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.bf = np.zeros((hidden_size, 1))
        
        # Initialize weights for input gate
        self.Wi = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.bi = np.zeros((hidden_size, 1))
        
        # Initialize weights for candidate values
        self.Wc = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.bc = np.zeros((hidden_size, 1))
        
        # Initialize weights for output gate
        self.Wo = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.bo = np.zeros((hidden_size, 1))
    
    def sigmoid(self, x):
        return 1 / (1 + np.exp(-np.clip(x, -250, 250)))
    
    def tanh(self, x):
        return np.tanh(np.clip(x, -250, 250))
    
    def forward(self, x, h_prev, c_prev):
        """Forward pass through LSTM cell"""
        # Concatenate input and previous hidden state
        concat = np.vstack([x, h_prev])
        
        # Forget gate: decides what information to discard
        f = self.sigmoid(np.dot(self.Wf, concat) + self.bf)
        
        # Input gate: decides what new information to store
        i = self.sigmoid(np.dot(self.Wi, concat) + self.bi)
        
        # Candidate values: new information that could be stored
        c_tilde = self.tanh(np.dot(self.Wc, concat) + self.bc)
        
        # Update cell state
        c = f * c_prev + i * c_tilde
        
        # Output gate: decides what parts of cell state to output
        o = self.sigmoid(np.dot(self.Wo, concat) + self.bo)
        
        # Compute hidden state
        h = o * self.tanh(c)
        
        # Store intermediate values for visualization
        self.gates = {
            'forget': f,
            'input': i,
            'output': o,
            'candidate': c_tilde,
            'cell_state': c,
            'hidden_state': h
        }
        
        return h, c

class TorchLSTM(nn.Module):
    """PyTorch LSTM for sequence modeling"""
    def __init__(self, input_size, hidden_size, output_size, num_layers=1, dropout=0.2):
        super(TorchLSTM, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        
        self.lstm = nn.LSTM(input_size, hidden_size, num_layers, 
                           batch_first=True, dropout=dropout)
        self.fc = nn.Linear(hidden_size, output_size)
        self.dropout = nn.Dropout(dropout)
        
    def forward(self, x, hidden=None):
        lstm_out, hidden = self.lstm(x, hidden)
        lstm_out = self.dropout(lstm_out)
        output = self.fc(lstm_out)
        return output, hidden
    
    def init_hidden(self, batch_size, device):
        h0 = torch.zeros(self.num_layers, batch_size, self.hidden_size).to(device)
        c0 = torch.zeros(self.num_layers, batch_size, self.hidden_size).to(device)
        return (h0, c0)

class SequenceDataset(Dataset):
    """Dataset for sequence prediction tasks"""
    def __init__(self, sequences, targets):
        self.sequences = sequences
        self.targets = targets
    
    def __len__(self):
        return len(self.sequences)
    
    def __getitem__(self, idx):
        return torch.tensor(self.sequences[idx], dtype=torch.float32), \
               torch.tensor(self.targets[idx], dtype=torch.long)

def visualize_lstm_gates():
    """Visualize LSTM gates behavior"""
    print("=== LSTM Gates Visualization ===")
    
    # Create simple LSTM cell
    lstm_cell = LSTMCell(input_size=1, hidden_size=3)
    
    # Create a simple sequence
    sequence = [0.5, 0.8, 0.2, 0.9, 0.1, 0.7]
    
    # Initialize states
    h = np.zeros((3, 1))
    c = np.zeros((3, 1))
    
    # Store gate values
    forget_gates = []
    input_gates = []
    output_gates = []
    cell_states = []
    hidden_states = []
    
    # Process sequence
    for x_val in sequence:
        x = np.array([[x_val]])
        h, c = lstm_cell.forward(x, h, c)
        
        # Store gate values
        forget_gates.append(lstm_cell.gates['forget'].flatten())
        input_gates.append(lstm_cell.gates['input'].flatten())
        output_gates.append(lstm_cell.gates['output'].flatten())
        cell_states.append(lstm_cell.gates['cell_state'].flatten())
        hidden_states.append(lstm_cell.gates['hidden_state'].flatten())
    
    # Convert to arrays
    forget_gates = np.array(forget_gates)
    input_gates = np.array(input_gates)
    output_gates = np.array(output_gates)
    cell_states = np.array(cell_states)
    hidden_states = np.array(hidden_states)
    
    # Plot gates behavior
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    
    # Input sequence
    axes[0, 0].plot(sequence, 'ko-', linewidth=2, markersize=8)
    axes[0, 0].set_title('Input Sequence')
    axes[0, 0].set_ylabel('Input Value')
    axes[0, 0].grid(True)
    
    # Forget gate
    for i in range(3):
        axes[0, 1].plot(forget_gates[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[0, 1].set_title('Forget Gate')
    axes[0, 1].set_ylabel('Gate Value')
    axes[0, 1].legend()
    axes[0, 1].grid(True)
    
    # Input gate
    for i in range(3):
        axes[0, 2].plot(input_gates[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[0, 2].set_title('Input Gate')
    axes[0, 2].set_ylabel('Gate Value')
    axes[0, 2].legend()
    axes[0, 2].grid(True)
    
    # Output gate
    for i in range(3):
        axes[1, 0].plot(output_gates[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[1, 0].set_title('Output Gate')
    axes[1, 0].set_xlabel('Time Step')
    axes[1, 0].set_ylabel('Gate Value')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Cell state
    for i in range(3):
        axes[1, 1].plot(cell_states[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[1, 1].set_title('Cell State')
    axes[1, 1].set_xlabel('Time Step')
    axes[1, 1].set_ylabel('State Value')
    axes[1, 1].legend()
    axes[1, 1].grid(True)
    
    # Hidden state
    for i in range(3):
        axes[1, 2].plot(hidden_states[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[1, 2].set_title('Hidden State')
    axes[1, 2].set_xlabel('Time Step')
    axes[1, 2].set_ylabel('State Value')
    axes[1, 2].legend()
    axes[1, 2].grid(True)
    
    plt.tight_layout()
    plt.show()

def create_sequence_data(seq_type='sine'):
    """Create different types of sequence data for testing"""
    if seq_type == 'sine':
        # Sine wave sequence
        t = np.linspace(0, 4*np.pi, 1000)
        data = np.sin(t) + 0.1 * np.random.randn(1000)
        
    elif seq_type == 'binary_addition':
        # Binary addition sequences
        data = []
        targets = []
        
        for _ in range(1000):
            # Generate two random binary numbers
            a = np.random.randint(0, 16)  # 4-bit numbers
            b = np.random.randint(0, 16)
            
            # Convert to binary sequences (4 bits each)
            seq_a = [(a >> i) & 1 for i in range(4)]
            seq_b = [(b >> i) & 1 for i in range(4)]
            
            # Combine sequences: [a0, b0, a1, b1, a2, b2, a3, b3]
            sequence = []
            for i in range(4):
                sequence.extend([seq_a[i], seq_b[i]])
            
            # Target is the binary representation of a + b
            target = a + b
            
            data.append(sequence)
            targets.append(target)
        
        return np.array(data), np.array(targets)
    
    elif seq_type == 'copy':
        # Copy task: copy a sequence after seeing a delimiter
        data = []
        targets = []
        seq_length = 10
        
        for _ in range(1000):
            # Random sequence
            sequence = np.random.randint(0, 8, seq_length)
            # Add delimiter (8) and zeros
            full_sequence = np.concatenate([sequence, [8], np.zeros(seq_length)])
            # Target: zeros followed by original sequence
            target = np.concatenate([np.zeros(seq_length + 1), sequence])
            
            data.append(full_sequence)
            targets.append(target)
        
        return np.array(data), np.array(targets)
    
    # Default: sine wave for sequence prediction
    seq_length = 50
    sequences = []
    targets = []
    
    for i in range(len(data) - seq_length):
        sequences.append(data[i:i + seq_length])
        targets.append(data[i + seq_length])
    
    return np.array(sequences).reshape(-1, seq_length, 1), np.array(targets)

def train_lstm_on_sequences():
    """Train LSTM on different sequence tasks"""
    print("=== Training LSTM on Sequence Tasks ===")
    
    # Test different tasks
    tasks = ['sine', 'binary_addition', 'copy']
    results = {}
    
    for task in tasks:
        print(f"\nTraining on {task} task...")
        
        # Create data
        if task == 'sine':
            X, y = create_sequence_data('sine')
            input_size = 1
            output_size = 1
            task_type = 'regression'
        elif task == 'binary_addition':
            X, y = create_sequence_data('binary_addition')
            X = X.reshape(X.shape[0], X.shape[1], 1)  # Add feature dimension
            input_size = 1
            output_size = 32  # Max sum of two 4-bit numbers
            task_type = 'classification'
        elif task == 'copy':
            X, y = create_sequence_data('copy')
            X = X.reshape(X.shape[0], X.shape[1], 1)
            input_size = 1
            output_size = 9  # 0-8 (including delimiter)
            task_type = 'sequence_classification'
        
        # Split data
        split = int(0.8 * len(X))
        X_train, X_test = X[:split], X[split:]
        y_train, y_test = y[:split], y[split:]
        
        # Create datasets and loaders
        if task_type == 'sequence_classification':
            # For copy task, we need sequence-to-sequence
            continue  # Skip for now as it's more complex
        
        train_dataset = SequenceDataset(X_train, y_train)
        test_dataset = SequenceDataset(X_test, y_test)
        
        train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
        test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)
        
        # Create model
        model = TorchLSTM(input_size=input_size, hidden_size=64, 
                         output_size=output_size, num_layers=2)
        
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        model = model.to(device)
        
        # Loss and optimizer
        if task_type == 'regression':
            criterion = nn.MSELoss()
        else:
            criterion = nn.CrossEntropyLoss()
        
        optimizer = optim.Adam(model.parameters(), lr=0.001)
        
        # Training
        num_epochs = 20
        train_losses = []
        
        for epoch in range(num_epochs):
            model.train()
            epoch_loss = 0
            
            for batch_x, batch_y in train_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                
                optimizer.zero_grad()
                outputs, _ = model(batch_x)
                
                if task_type == 'regression':
                    loss = criterion(outputs.squeeze(), batch_y.float())
                else:
                    loss = criterion(outputs.squeeze(), batch_y)
                
                loss.backward()
                
                # Gradient clipping
                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                
                optimizer.step()
                epoch_loss += loss.item()
            
            avg_loss = epoch_loss / len(train_loader)
            train_losses.append(avg_loss)
            
            if epoch % 5 == 0:
                print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {avg_loss:.4f}')
        
        # Evaluation
        model.eval()
        test_loss = 0
        correct = 0
        total = 0
        
        with torch.no_grad():
            for batch_x, batch_y in test_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                outputs, _ = model(batch_x)
                
                if task_type == 'regression':
                    test_loss += criterion(outputs.squeeze(), batch_y.float()).item()
                else:
                    test_loss += criterion(outputs.squeeze(), batch_y).item()
                    _, predicted = torch.max(outputs.data, 1)
                    total += batch_y.size(0)
                    correct += (predicted == batch_y).sum().item()
        
        avg_test_loss = test_loss / len(test_loader)
        
        if task_type == 'regression':
            print(f'Test MSE: {avg_test_loss:.4f}')
            results[task] = {'losses': train_losses, 'test_mse': avg_test_loss}
        else:
            accuracy = 100 * correct / total
            print(f'Test Accuracy: {accuracy:.2f}%')
            results[task] = {'losses': train_losses, 'test_accuracy': accuracy}
    
    # Plot results
    fig, axes = plt.subplots(1, len(results), figsize=(15, 5))
    if len(results) == 1:
        axes = [axes]
    
    for i, (task, data) in enumerate(results.items()):
        axes[i].plot(data['losses'], linewidth=2)
        axes[i].set_title(f'{task.capitalize()} Task')
        axes[i].set_xlabel('Epoch')
        axes[i].set_ylabel('Loss')
        axes[i].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    return results

def compare_lstm_vs_rnn():
    """Compare LSTM vs Vanilla RNN on long sequences"""
    print("\n=== LSTM vs RNN Comparison ===")
    
    # Create a task that requires long-term memory
    def create_long_dependency_data(seq_length=100, n_samples=1000):
        """Create data where early input affects late output"""
        data = []
        targets = []
        
        for _ in range(n_samples):
            # Random sequence
            sequence = np.random.randn(seq_length)
            
            # Target depends on first element's sign and last element's magnitude
            if sequence[0] > 0:
                target = 1 if abs(sequence[-1]) > 0.5 else 0
            else:
                target = 1 if abs(sequence[-1]) < 0.5 else 0
            
            data.append(sequence)
            targets.append(target)
        
        return np.array(data).reshape(-1, seq_length, 1), np.array(targets)
    
    X, y = create_long_dependency_data(seq_length=50, n_samples=2000)
    
    # Split data
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]
    
    # Create datasets
    train_dataset = SequenceDataset(X_train, y_train)
    test_dataset = SequenceDataset(X_test, y_test)
    
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    # Test both RNN and LSTM
    models = {
        'RNN': nn.RNN(1, 64, 2, batch_first=True),
        'LSTM': nn.LSTM(1, 64, 2, batch_first=True)
    }
    
    results = {}
    
    for model_name, rnn_layer in models.items():
        print(f"\nTraining {model_name}...")
        
        # Create full model
        class SequenceClassifier(nn.Module):
            def __init__(self, rnn_layer):
                super().__init__()
                self.rnn = rnn_layer
                self.fc = nn.Linear(64, 2)
            
            def forward(self, x):
                out, _ = self.rnn(x)
                out = self.fc(out[:, -1, :])  # Use last output
                return out
        
        model = SequenceClassifier(rnn_layer).to(device)
        criterion = nn.CrossEntropyLoss()
        optimizer = optim.Adam(model.parameters(), lr=0.001)
        
        # Training
        train_losses = []
        train_accuracies = []
        
        for epoch in range(30):
            model.train()
            epoch_loss = 0
            correct = 0
            total = 0
            
            for batch_x, batch_y in train_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                
                optimizer.zero_grad()
                outputs = model(batch_x)
                loss = criterion(outputs, batch_y)
                loss.backward()
                
                # Gradient clipping
                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                
                optimizer.step()
                
                epoch_loss += loss.item()
                _, predicted = torch.max(outputs, 1)
                total += batch_y.size(0)
                correct += (predicted == batch_y).sum().item()
            
            avg_loss = epoch_loss / len(train_loader)
            accuracy = 100 * correct / total
            
            train_losses.append(avg_loss)
            train_accuracies.append(accuracy)
            
            if epoch % 10 == 0:
                print(f'Epoch [{epoch+1}/30], Loss: {avg_loss:.4f}, Acc: {accuracy:.2f}%')
        
        # Test
        model.eval()
        correct = 0
        total = 0
        
        with torch.no_grad():
            for batch_x, batch_y in test_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                outputs = model(batch_x)
                _, predicted = torch.max(outputs, 1)
                total += batch_y.size(0)
                correct += (predicted == batch_y).sum().item()
        
        test_accuracy = 100 * correct / total
        print(f'{model_name} Test Accuracy: {test_accuracy:.2f}%')
        
        results[model_name] = {
            'train_losses': train_losses,
            'train_accuracies': train_accuracies,
            'test_accuracy': test_accuracy
        }
    
    # Plot comparison
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    
    # Training loss
    for model_name, data in results.items():
        axes[0].plot(data['train_losses'], label=model_name, linewidth=2)
    axes[0].set_title('Training Loss Comparison')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # Training accuracy
    for model_name, data in results.items():
        axes[1].plot(data['train_accuracies'], label=model_name, linewidth=2)
    axes[1].set_title('Training Accuracy Comparison')
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('Accuracy (%)')
    axes[1].legend()
    axes[1].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    return results

# Example usage and demonstration
if __name__ == "__main__":
    print("=== LSTM Implementation and Analysis ===")
    
    # Visualize LSTM gates
    visualize_lstm_gates()
    
    # Train on different sequence tasks
    sequence_results = train_lstm_on_sequences()
    
    # Compare LSTM vs RNN
    comparison_results = compare_lstm_vs_rnn()
    
    print("\n=== LSTM Key Advantages ===")
    print("1. Solves vanishing gradient problem through gating mechanisms")
    print("2. Can learn long-term dependencies effectively")
    print("3. Forget gate allows selective memory retention")
    print("4. Input gate controls what new information to store")
    print("5. Output gate controls what to output from cell state")
    print("6. Cell state provides highway for gradient flow")
    
    print("\n=== LSTM Architecture Components ===")
    print("• Forget Gate: f_t = σ(W_f · [h_{t-1}, x_t] + b_f)")
    print("• Input Gate: i_t = σ(W_i · [h_{t-1}, x_t] + b_i)")
    print("• Candidate: C̃_t = tanh(W_C · [h_{t-1}, x_t] + b_C)")
    print("• Cell State: C_t = f_t * C_{t-1} + i_t * C̃_t")
    print("• Output Gate: o_t = σ(W_o · [h_{t-1}, x_t] + b_o)")
    print("• Hidden State: h_t = o_t * tanh(C_t)")
    
    print("\n=== When to Use LSTMs ===")
    print("✓ Long sequences with long-term dependencies")
    print("✓ Time series forecasting")
    print("✓ Natural language processing")
    print("✓ Speech recognition")
    print("✓ Machine translation")
    print("✗ Very long sequences (consider Transformers)")
    print("✗ Simple sequential patterns (vanilla RNN might suffice)")