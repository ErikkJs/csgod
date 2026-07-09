# author: csgod cmps studies
# GRU - Gated Recurrent Units
# GRUs are a simpler alternative to LSTMs that combine the forget and input gates
# into a single update gate, making them computationally more efficient.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader, Dataset
import seaborn as sns

class GRUCell:
    """Numpy implementation of GRU cell for educational purposes"""
    def __init__(self, input_size, hidden_size):
        self.input_size = input_size
        self.hidden_size = hidden_size
        
        # Reset gate parameters
        self.W_r = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.b_r = np.zeros((hidden_size, 1))
        
        # Update gate parameters
        self.W_z = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.b_z = np.zeros((hidden_size, 1))
        
        # Candidate hidden state parameters
        self.W_h = np.random.randn(hidden_size, input_size + hidden_size) * 0.1
        self.b_h = np.zeros((hidden_size, 1))
    
    def sigmoid(self, x):
        return 1 / (1 + np.exp(-np.clip(x, -250, 250)))
    
    def tanh(self, x):
        return np.tanh(np.clip(x, -250, 250))
    
    def forward(self, x, h_prev):
        """Forward pass through GRU cell"""
        # Concatenate input and previous hidden state
        concat_input = np.vstack([x, h_prev])
        
        # Reset gate: decides how much past information to forget
        r = self.sigmoid(np.dot(self.W_r, concat_input) + self.b_r)
        
        # Update gate: decides how much to update
        z = self.sigmoid(np.dot(self.W_z, concat_input) + self.b_z)
        
        # Candidate hidden state (with reset gate applied)
        reset_concat = np.vstack([x, r * h_prev])
        h_tilde = self.tanh(np.dot(self.W_h, reset_concat) + self.b_h)
        
        # Final hidden state: interpolation between previous and candidate
        h = (1 - z) * h_prev + z * h_tilde
        
        # Store gate values for visualization
        self.gates = {
            'reset': r,
            'update': z,
            'candidate': h_tilde,
            'hidden': h
        }
        
        return h

class TorchGRU(nn.Module):
    """PyTorch GRU for sequence modeling"""
    def __init__(self, input_size, hidden_size, output_size, num_layers=1, dropout=0.2):
        super(TorchGRU, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        
        self.gru = nn.GRU(input_size, hidden_size, num_layers, 
                         batch_first=True, dropout=dropout if num_layers > 1 else 0)
        self.fc = nn.Linear(hidden_size, output_size)
        self.dropout = nn.Dropout(dropout)
        
    def forward(self, x, hidden=None):
        gru_out, hidden = self.gru(x, hidden)
        gru_out = self.dropout(gru_out)
        output = self.fc(gru_out)
        return output, hidden
    
    def init_hidden(self, batch_size, device):
        return torch.zeros(self.num_layers, batch_size, self.hidden_size).to(device)

def visualize_gru_gates():
    """Visualize GRU gates behavior"""
    print("=== GRU Gates Visualization ===")
    
    # Create GRU cell
    gru_cell = GRUCell(input_size=1, hidden_size=4)
    
    # Create a sequence
    sequence = [0.1, 0.8, 0.3, 0.9, 0.2, 0.7, 0.5]
    
    # Initialize hidden state
    h = np.zeros((4, 1))
    
    # Store gate values
    reset_gates = []
    update_gates = []
    candidates = []
    hidden_states = []
    
    # Process sequence
    for x_val in sequence:
        x = np.array([[x_val]])
        h = gru_cell.forward(x, h)
        
        # Store gate values
        reset_gates.append(gru_cell.gates['reset'].flatten())
        update_gates.append(gru_cell.gates['update'].flatten())
        candidates.append(gru_cell.gates['candidate'].flatten())
        hidden_states.append(gru_cell.gates['hidden'].flatten())
    
    # Convert to arrays
    reset_gates = np.array(reset_gates)
    update_gates = np.array(update_gates)
    candidates = np.array(candidates)
    hidden_states = np.array(hidden_states)
    
    # Plot gates behavior
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    
    # Input sequence
    axes[0, 0].plot(sequence, 'ko-', linewidth=2, markersize=8)
    axes[0, 0].set_title('Input Sequence')
    axes[0, 0].set_ylabel('Input Value')
    axes[0, 0].grid(True)
    
    # Reset gate
    for i in range(4):
        axes[0, 1].plot(reset_gates[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[0, 1].set_title('Reset Gate (r_t)')
    axes[0, 1].set_ylabel('Gate Value')
    axes[0, 1].legend()
    axes[0, 1].grid(True)
    axes[0, 1].set_ylim(0, 1)
    
    # Update gate
    for i in range(4):
        axes[0, 2].plot(update_gates[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[0, 2].set_title('Update Gate (z_t)')
    axes[0, 2].set_ylabel('Gate Value')
    axes[0, 2].legend()
    axes[0, 2].grid(True)
    axes[0, 2].set_ylim(0, 1)
    
    # Candidate hidden state
    for i in range(4):
        axes[1, 0].plot(candidates[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[1, 0].set_title('Candidate Hidden State (h̃_t)')
    axes[1, 0].set_xlabel('Time Step')
    axes[1, 0].set_ylabel('State Value')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Final hidden state
    for i in range(4):
        axes[1, 1].plot(hidden_states[:, i], 'o-', label=f'Unit {i}', linewidth=2)
    axes[1, 1].set_title('Hidden State (h_t)')
    axes[1, 1].set_xlabel('Time Step')
    axes[1, 1].set_ylabel('State Value')
    axes[1, 1].legend()
    axes[1, 1].grid(True)
    
    # Gate interaction visualization
    axes[1, 2].bar(range(len(sequence)), np.mean(update_gates, axis=1), 
                  alpha=0.7, label='Update Gate', color='blue')
    axes[1, 2].bar(range(len(sequence)), np.mean(reset_gates, axis=1), 
                  alpha=0.7, label='Reset Gate', color='red')
    axes[1, 2].set_title('Average Gate Values')
    axes[1, 2].set_xlabel('Time Step')
    axes[1, 2].set_ylabel('Average Gate Value')
    axes[1, 2].legend()
    axes[1, 2].grid(True)
    
    plt.tight_layout()
    plt.show()

class SequenceDataset(Dataset):
    """Dataset for sequence tasks"""
    def __init__(self, sequences, targets):
        self.sequences = torch.FloatTensor(sequences)
        self.targets = torch.LongTensor(targets)
    
    def __len__(self):
        return len(self.sequences)
    
    def __getitem__(self, idx):
        return self.sequences[idx], self.targets[idx]

def create_pattern_recognition_data():
    """Create sequence pattern recognition data"""
    sequences = []
    labels = []
    
    # Pattern 1: Increasing sequence
    for _ in range(500):
        base = np.random.uniform(0, 0.5)
        seq = base + np.cumsum(np.random.uniform(0, 0.1, 10))
        sequences.append(seq.reshape(-1, 1))
        labels.append(0)
    
    # Pattern 2: Decreasing sequence
    for _ in range(500):
        base = np.random.uniform(0.5, 1.0)
        seq = base - np.cumsum(np.random.uniform(0, 0.1, 10))
        sequences.append(seq.reshape(-1, 1))
        labels.append(1)
    
    # Pattern 3: Oscillating sequence
    for _ in range(500):
        t = np.linspace(0, 4*np.pi, 10)
        seq = np.sin(t) + np.random.normal(0, 0.1, 10)
        sequences.append(seq.reshape(-1, 1))
        labels.append(2)
    
    return np.array(sequences), np.array(labels)

def train_gru_classifier():
    """Train GRU on sequence classification"""
    print("=== Training GRU Classifier ===")
    
    # Create data
    X, y = create_pattern_recognition_data()
    
    # Shuffle data
    indices = np.random.permutation(len(X))
    X, y = X[indices], y[indices]
    
    # Split data
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]
    
    # Create datasets
    train_dataset = SequenceDataset(X_train, y_train)
    test_dataset = SequenceDataset(X_test, y_test)
    
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)
    
    print(f"Training samples: {len(train_dataset)}")
    print(f"Test samples: {len(test_dataset)}")
    print(f"Sequence length: {X.shape[1]}")
    print(f"Number of classes: {len(np.unique(y))}")
    
    # Create model
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = TorchGRU(input_size=1, hidden_size=64, output_size=3, num_layers=2).to(device)
    
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    
    # Training
    train_losses = []
    train_accuracies = []
    
    for epoch in range(50):
        model.train()
        epoch_loss = 0
        correct = 0
        total = 0
        
        for batch_x, batch_y in train_loader:
            batch_x, batch_y = batch_x.to(device), batch_y.to(device)
            
            optimizer.zero_grad()
            outputs, _ = model(batch_x)
            
            # Use last time step for classification
            outputs = outputs[:, -1, :]
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
            print(f'Epoch [{epoch+1}/50], Loss: {avg_loss:.4f}, Accuracy: {accuracy:.2f}%')
    
    # Test
    model.eval()
    correct = 0
    total = 0
    class_correct = [0, 0, 0]
    class_total = [0, 0, 0]
    
    with torch.no_grad():
        for batch_x, batch_y in test_loader:
            batch_x, batch_y = batch_x.to(device), batch_y.to(device)
            outputs, _ = model(batch_x)
            outputs = outputs[:, -1, :]
            _, predicted = torch.max(outputs, 1)
            
            total += batch_y.size(0)
            correct += (predicted == batch_y).sum().item()
            
            # Per-class accuracy
            for i in range(batch_y.size(0)):
                label = batch_y[i].item()
                class_correct[label] += (predicted[i] == batch_y[i]).item()
                class_total[label] += 1
    
    test_accuracy = 100 * correct / total
    print(f'\nTest Accuracy: {test_accuracy:.2f}%')
    
    # Per-class accuracy
    class_names = ['Increasing', 'Decreasing', 'Oscillating']
    for i, name in enumerate(class_names):
        if class_total[i] > 0:
            acc = 100 * class_correct[i] / class_total[i]
            print(f'{name}: {acc:.2f}%')
    
    return model, train_losses, train_accuracies, test_loader

def compare_gru_vs_lstm():
    """Compare GRU vs LSTM performance"""
    print("\n=== GRU vs LSTM Comparison ===")
    
    # Create challenging sequence data
    def create_long_dependency_data():
        sequences = []
        targets = []
        
        for _ in range(1000):
            # Create sequence where early elements affect late classification
            seq_length = 30
            sequence = np.random.randn(seq_length)
            
            # If first element is positive and last element is large, class 1
            # Otherwise class 0
            if sequence[0] > 0 and abs(sequence[-1]) > 1.0:
                target = 1
            else:
                target = 0
            
            sequences.append(sequence.reshape(-1, 1))
            targets.append(target)
        
        return np.array(sequences), np.array(targets)
    
    X, y = create_long_dependency_data()
    
    # Split data
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]
    
    train_dataset = SequenceDataset(X_train, y_train)
    test_dataset = SequenceDataset(X_test, y_test)
    
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    # Models to compare
    models = {
        'GRU': TorchGRU(input_size=1, hidden_size=64, output_size=2, num_layers=2),
        'LSTM': nn.Module()  # We'll create a simple LSTM wrapper
    }
    
    # Create LSTM model
    class SimpleLSTM(nn.Module):
        def __init__(self, input_size, hidden_size, output_size, num_layers=1):
            super(SimpleLSTM, self).__init__()
            self.lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first=True)
            self.fc = nn.Linear(hidden_size, output_size)
            
        def forward(self, x):
            lstm_out, _ = self.lstm(x)
            output = self.fc(lstm_out[:, -1, :])  # Use last time step
            return output
    
    models['LSTM'] = SimpleLSTM(input_size=1, hidden_size=64, output_size=2, num_layers=2)
    
    results = {}
    
    for model_name, model in models.items():
        print(f"\nTraining {model_name}...")
        model = model.to(device)
        
        criterion = nn.CrossEntropyLoss()
        optimizer = optim.Adam(model.parameters(), lr=0.001)
        
        # Training
        train_losses = []
        
        for epoch in range(30):
            model.train()
            epoch_loss = 0
            
            for batch_x, batch_y in train_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                
                optimizer.zero_grad()
                
                if model_name == 'GRU':
                    outputs, _ = model(batch_x)
                    outputs = outputs[:, -1, :]
                else:
                    outputs = model(batch_x)
                
                loss = criterion(outputs, batch_y)
                loss.backward()
                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                optimizer.step()
                
                epoch_loss += loss.item()
            
            train_losses.append(epoch_loss / len(train_loader))
        
        # Test
        model.eval()
        correct = 0
        total = 0
        
        with torch.no_grad():
            for batch_x, batch_y in test_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                
                if model_name == 'GRU':
                    outputs, _ = model(batch_x)
                    outputs = outputs[:, -1, :]
                else:
                    outputs = model(batch_x)
                
                _, predicted = torch.max(outputs, 1)
                total += batch_y.size(0)
                correct += (predicted == batch_y).sum().item()
        
        accuracy = 100 * correct / total
        print(f'{model_name} Test Accuracy: {accuracy:.2f}%')
        
        results[model_name] = {
            'train_losses': train_losses,
            'accuracy': accuracy
        }
    
    # Plot comparison
    plt.figure(figsize=(12, 5))
    
    # Training loss
    plt.subplot(1, 2, 1)
    for model_name, data in results.items():
        plt.plot(data['train_losses'], label=f'{model_name}', linewidth=2)
    plt.title('Training Loss Comparison')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    
    # Final accuracy comparison
    plt.subplot(1, 2, 2)
    models_list = list(results.keys())
    accuracies = [results[model]['accuracy'] for model in models_list]
    plt.bar(models_list, accuracies)
    plt.title('Final Test Accuracy')
    plt.ylabel('Accuracy (%)')
    plt.ylim(0, 100)
    
    # Add value labels on bars
    for i, accuracy in enumerate(accuracies):
        plt.text(i, accuracy + 1, f'{accuracy:.1f}%', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.show()
    
    return results

def demonstrate_gru_efficiency():
    """Demonstrate computational efficiency of GRU vs LSTM"""
    print("\n=== GRU vs LSTM Efficiency Comparison ===")
    
    import time
    
    input_size = 100
    hidden_size = 256
    batch_size = 32
    seq_length = 50
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    # Create models
    gru_model = nn.GRU(input_size, hidden_size, batch_first=True).to(device)
    lstm_model = nn.LSTM(input_size, hidden_size, batch_first=True).to(device)
    
    # Create random input
    x = torch.randn(batch_size, seq_length, input_size).to(device)
    
    # Count parameters
    gru_params = sum(p.numel() for p in gru_model.parameters())
    lstm_params = sum(p.numel() for p in lstm_model.parameters())
    
    print(f"GRU Parameters: {gru_params:,}")
    print(f"LSTM Parameters: {lstm_params:,}")
    print(f"Parameter Ratio (LSTM/GRU): {lstm_params/gru_params:.2f}x")
    
    # Timing comparison
    num_runs = 100
    
    # GRU timing
    torch.cuda.synchronize() if device.type == 'cuda' else None
    start_time = time.time()
    
    for _ in range(num_runs):
        with torch.no_grad():
            _ = gru_model(x)
    
    torch.cuda.synchronize() if device.type == 'cuda' else None
    gru_time = (time.time() - start_time) / num_runs
    
    # LSTM timing
    torch.cuda.synchronize() if device.type == 'cuda' else None
    start_time = time.time()
    
    for _ in range(num_runs):
        with torch.no_grad():
            _ = lstm_model(x)
    
    torch.cuda.synchronize() if device.type == 'cuda' else None
    lstm_time = (time.time() - start_time) / num_runs
    
    print(f"\nInference Time (average over {num_runs} runs):")
    print(f"GRU: {gru_time*1000:.2f} ms")
    print(f"LSTM: {lstm_time*1000:.2f} ms")
    print(f"Speed Ratio (LSTM/GRU): {lstm_time/gru_time:.2f}x")
    
    # Memory usage comparison (approximate)
    gru_memory = gru_params * 4 / (1024**2)  # 4 bytes per float32, convert to MB
    lstm_memory = lstm_params * 4 / (1024**2)
    
    print(f"\nMemory Usage (approximate):")
    print(f"GRU: {gru_memory:.1f} MB")
    print(f"LSTM: {lstm_memory:.1f} MB")

def visualize_gru_vs_lstm_architecture():
    """Visualize the architectural differences between GRU and LSTM"""
    print("\n=== GRU vs LSTM Architecture Comparison ===")
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 8))
    
    # GRU Architecture Diagram (simplified)
    ax1.text(0.5, 0.9, 'GRU Cell', ha='center', va='center', fontsize=16, fontweight='bold')
    ax1.text(0.1, 0.7, 'Reset Gate', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightcoral"))
    ax1.text(0.9, 0.7, 'Update Gate', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue"))
    ax1.text(0.5, 0.5, 'Candidate\nHidden State', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgreen"))
    ax1.text(0.5, 0.2, 'Hidden State', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow"))
    
    # Add arrows (simplified)
    ax1.annotate('', xy=(0.5, 0.4), xytext=(0.1, 0.6), arrowprops=dict(arrowstyle='->', lw=2))
    ax1.annotate('', xy=(0.5, 0.4), xytext=(0.9, 0.6), arrowprops=dict(arrowstyle='->', lw=2))
    ax1.annotate('', xy=(0.5, 0.3), xytext=(0.5, 0.4), arrowprops=dict(arrowstyle='->', lw=2))
    
    ax1.set_xlim(0, 1)
    ax1.set_ylim(0, 1)
    ax1.set_title('GRU Architecture (Simpler)')
    ax1.axis('off')
    
    # LSTM Architecture Diagram (simplified)
    ax2.text(0.5, 0.9, 'LSTM Cell', ha='center', va='center', fontsize=16, fontweight='bold')
    ax2.text(0.15, 0.7, 'Forget\nGate', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightcoral"))
    ax2.text(0.5, 0.7, 'Input\nGate', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue"))
    ax2.text(0.85, 0.7, 'Output\nGate', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightpink"))
    ax2.text(0.3, 0.4, 'Cell State', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgreen"))
    ax2.text(0.7, 0.4, 'Hidden State', ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow"))
    
    ax2.set_xlim(0, 1)
    ax2.set_ylim(0, 1)
    ax2.set_title('LSTM Architecture (More Complex)')
    ax2.axis('off')
    
    plt.tight_layout()
    plt.show()
    
    # Comparison table
    print("\n=== GRU vs LSTM Comparison Table ===")
    comparison = {
        'Aspect': ['Number of Gates', 'Parameters', 'Computational Cost', 'Memory Usage', 'Training Speed', 'Performance'],
        'GRU': ['2 (Reset, Update)', 'Fewer', 'Lower', 'Less', 'Faster', 'Similar to LSTM'],
        'LSTM': ['3 (Forget, Input, Output)', 'More', 'Higher', 'More', 'Slower', 'Similar to GRU']
    }
    
    print(f"{'Aspect':<20} {'GRU':<25} {'LSTM'}")
    print("-" * 70)
    for i in range(len(comparison['Aspect'])):
        print(f"{comparison['Aspect'][i]:<20} {comparison['GRU'][i]:<25} {comparison['LSTM'][i]}")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== GRU Implementation and Analysis ===")
    
    # Visualize GRU gates
    visualize_gru_gates()
    
    # Train GRU classifier
    model, losses, accuracies, test_loader = train_gru_classifier()
    
    # Plot training curves
    plt.figure(figsize=(12, 5))
    
    plt.subplot(1, 2, 1)
    plt.plot(losses, linewidth=2, color='blue')
    plt.title('GRU Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.subplot(1, 2, 2)
    plt.plot(accuracies, linewidth=2, color='green')
    plt.title('GRU Training Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy (%)')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Compare GRU vs LSTM
    comparison_results = compare_gru_vs_lstm()
    
    # Demonstrate efficiency
    demonstrate_gru_efficiency()
    
    # Visualize architecture differences
    visualize_gru_vs_lstm_architecture()
    
    print("\n=== GRU Key Concepts ===")
    print("1. SIMPLIFIED ARCHITECTURE: Combines forget and input gates into update gate")
    print("2. RESET GATE: Controls how much past information to forget")
    print("   - r_t = σ(W_r * [h_{t-1}, x_t])")
    print("3. UPDATE GATE: Controls how much to update hidden state")
    print("   - z_t = σ(W_z * [h_{t-1}, x_t])")
    print("4. CANDIDATE HIDDEN STATE: New information to be stored")
    print("   - h̃_t = tanh(W_h * [r_t ⊙ h_{t-1}, x_t])")
    print("5. FINAL HIDDEN STATE: Interpolation between old and new")
    print("   - h_t = (1 - z_t) ⊙ h_{t-1} + z_t ⊙ h̃_t")
    
    print("\n=== GRU Advantages ===")
    print("✓ Fewer parameters than LSTM (about 25% fewer)")
    print("✓ Faster training and inference")
    print("✓ Less memory usage")
    print("✓ Simpler architecture, easier to understand")
    print("✓ Often performs as well as LSTM")
    print("✓ Less prone to overfitting on small datasets")
    
    print("\n=== When to Use GRU vs LSTM ===")
    print("USE GRU WHEN:")
    print("• Limited computational resources")
    print("• Smaller datasets")
    print("• Simpler sequential patterns")
    print("• Need faster training/inference")
    
    print("\nUSE LSTM WHEN:")
    print("• Need maximum performance")
    print("• Very complex long-term dependencies")
    print("• Large datasets available")
    print("• Computational cost is not a concern")