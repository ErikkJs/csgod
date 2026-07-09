# author: csgod cmps studies
# Attention Mechanism - Bahdanau/Luong attention
# Attention mechanisms allow models to focus on relevant parts of input sequences
# when making predictions, solving the information bottleneck problem in seq2seq models.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset
import seaborn as sns

class BahdanauAttention(nn.Module):
    """Bahdanau (Additive) Attention Mechanism"""
    def __init__(self, hidden_size):
        super(BahdanauAttention, self).__init__()
        self.hidden_size = hidden_size
        
        # Attention weights
        self.W_a = nn.Linear(hidden_size, hidden_size, bias=False)  # For encoder hidden states
        self.U_a = nn.Linear(hidden_size, hidden_size, bias=False)  # For decoder hidden state
        self.v_a = nn.Linear(hidden_size, 1, bias=False)            # For attention scores
    
    def forward(self, decoder_hidden, encoder_outputs):
        """
        decoder_hidden: [batch_size, hidden_size]
        encoder_outputs: [batch_size, seq_len, hidden_size]
        """
        batch_size, seq_len, hidden_size = encoder_outputs.size()
        
        # Expand decoder hidden state to match encoder outputs
        decoder_hidden_expanded = decoder_hidden.unsqueeze(1).expand(batch_size, seq_len, hidden_size)
        
        # Compute attention scores
        # e_ij = v_a^T * tanh(W_a * h_j + U_a * s_{i-1})
        energy = torch.tanh(self.W_a(encoder_outputs) + self.U_a(decoder_hidden_expanded))
        attention_scores = self.v_a(energy).squeeze(2)  # [batch_size, seq_len]
        
        # Apply softmax to get attention weights
        attention_weights = F.softmax(attention_scores, dim=1)  # [batch_size, seq_len]
        
        # Compute context vector
        context_vector = torch.bmm(attention_weights.unsqueeze(1), encoder_outputs)
        context_vector = context_vector.squeeze(1)  # [batch_size, hidden_size]
        
        return context_vector, attention_weights

class LuongAttention(nn.Module):
    """Luong (Multiplicative) Attention Mechanism"""
    def __init__(self, hidden_size, attn_type='dot'):
        super(LuongAttention, self).__init__()
        self.hidden_size = hidden_size
        self.attn_type = attn_type
        
        if attn_type == 'general':
            self.W_a = nn.Linear(hidden_size, hidden_size, bias=False)
        elif attn_type == 'concat':
            self.W_a = nn.Linear(hidden_size * 2, hidden_size, bias=False)
            self.v_a = nn.Linear(hidden_size, 1, bias=False)
    
    def forward(self, decoder_hidden, encoder_outputs):
        """
        decoder_hidden: [batch_size, hidden_size]
        encoder_outputs: [batch_size, seq_len, hidden_size]
        """
        batch_size, seq_len, hidden_size = encoder_outputs.size()
        
        if self.attn_type == 'dot':
            # Dot product attention: score = h_t^T * h_s
            attention_scores = torch.bmm(decoder_hidden.unsqueeze(1), encoder_outputs.transpose(1, 2))
            attention_scores = attention_scores.squeeze(1)  # [batch_size, seq_len]
            
        elif self.attn_type == 'general':
            # General attention: score = h_t^T * W_a * h_s
            decoder_hidden_transformed = self.W_a(decoder_hidden)  # [batch_size, hidden_size]
            attention_scores = torch.bmm(decoder_hidden_transformed.unsqueeze(1), encoder_outputs.transpose(1, 2))
            attention_scores = attention_scores.squeeze(1)  # [batch_size, seq_len]
            
        elif self.attn_type == 'concat':
            # Concatenation attention: score = v_a^T * tanh(W_a * [h_t; h_s])
            decoder_hidden_expanded = decoder_hidden.unsqueeze(1).expand(batch_size, seq_len, hidden_size)
            concat_hidden = torch.cat([decoder_hidden_expanded, encoder_outputs], dim=2)
            energy = torch.tanh(self.W_a(concat_hidden))
            attention_scores = self.v_a(energy).squeeze(2)  # [batch_size, seq_len]
        
        # Apply softmax to get attention weights
        attention_weights = F.softmax(attention_scores, dim=1)
        
        # Compute context vector
        context_vector = torch.bmm(attention_weights.unsqueeze(1), encoder_outputs)
        context_vector = context_vector.squeeze(1)  # [batch_size, hidden_size]
        
        return context_vector, attention_weights

class Seq2SeqWithAttention(nn.Module):
    """Sequence-to-Sequence model with attention mechanism"""
    def __init__(self, input_size, output_size, hidden_size, attention_type='bahdanau'):
        super(Seq2SeqWithAttention, self).__init__()
        self.hidden_size = hidden_size
        self.output_size = output_size
        
        # Encoder
        self.encoder = nn.LSTM(input_size, hidden_size, batch_first=True, bidirectional=True)
        
        # Attention mechanism
        encoder_hidden_size = hidden_size * 2  # Bidirectional
        if attention_type == 'bahdanau':
            self.attention = BahdanauAttention(encoder_hidden_size)
        elif attention_type == 'luong':
            self.attention = LuongAttention(encoder_hidden_size, 'dot')
        
        # Decoder
        self.decoder_cell = nn.LSTMCell(input_size + encoder_hidden_size, hidden_size)
        self.output_projection = nn.Linear(hidden_size, output_size)
        
        # Hidden state transformation (bidirectional to unidirectional)
        self.hidden_transform = nn.Linear(encoder_hidden_size, hidden_size)
        self.cell_transform = nn.Linear(encoder_hidden_size, hidden_size)
    
    def encode(self, input_seq):
        """Encode input sequence"""
        encoder_outputs, (hidden, cell) = self.encoder(input_seq)
        
        # Combine bidirectional hidden states
        # hidden: [2, batch_size, hidden_size] -> [batch_size, hidden_size]
        hidden = torch.cat([hidden[0], hidden[1]], dim=1)  # Concatenate forward and backward
        cell = torch.cat([cell[0], cell[1]], dim=1)
        
        # Transform to decoder hidden size
        hidden = self.hidden_transform(hidden)
        cell = self.cell_transform(cell)
        
        return encoder_outputs, hidden, cell
    
    def decode_step(self, input_token, decoder_hidden, decoder_cell, encoder_outputs):
        """Single decoder step with attention"""
        # Compute attention
        context_vector, attention_weights = self.attention(decoder_hidden, encoder_outputs)
        
        # Combine input and context
        decoder_input = torch.cat([input_token, context_vector], dim=1)
        
        # LSTM step
        new_hidden, new_cell = self.decoder_cell(decoder_input, (decoder_hidden, decoder_cell))
        
        # Output projection
        output = self.output_projection(new_hidden)
        
        return output, new_hidden, new_cell, attention_weights
    
    def forward(self, input_seq, target_seq=None, max_length=20):
        """Forward pass with teacher forcing during training"""
        batch_size = input_seq.size(0)
        
        # Encode
        encoder_outputs, decoder_hidden, decoder_cell = self.encode(input_seq)
        
        # Decode
        outputs = []
        attention_weights_list = []
        
        # Start token (zeros)
        decoder_input = torch.zeros(batch_size, input_seq.size(2)).to(input_seq.device)
        
        if target_seq is not None:  # Training mode with teacher forcing
            for t in range(target_seq.size(1)):
                output, decoder_hidden, decoder_cell, attention_weights = self.decode_step(
                    decoder_input, decoder_hidden, decoder_cell, encoder_outputs
                )
                outputs.append(output.unsqueeze(1))
                attention_weights_list.append(attention_weights.unsqueeze(1))
                
                # Teacher forcing: use target as next input
                if t < target_seq.size(1) - 1:
                    decoder_input = target_seq[:, t]
        else:  # Inference mode
            for t in range(max_length):
                output, decoder_hidden, decoder_cell, attention_weights = self.decode_step(
                    decoder_input, decoder_hidden, decoder_cell, encoder_outputs
                )
                outputs.append(output.unsqueeze(1))
                attention_weights_list.append(attention_weights.unsqueeze(1))
                
                # Use predicted output as next input
                decoder_input = output
        
        outputs = torch.cat(outputs, dim=1)
        attention_weights_all = torch.cat(attention_weights_list, dim=1)
        
        return outputs, attention_weights_all

def create_copy_task_data(seq_length=10, vocab_size=10, n_samples=1000):
    """Create copy task data for testing attention"""
    data = []
    targets = []
    
    for _ in range(n_samples):
        # Random sequence
        sequence = np.random.randint(0, vocab_size, seq_length)
        
        # Convert to one-hot
        seq_onehot = np.eye(vocab_size)[sequence]
        target_onehot = seq_onehot.copy()
        
        data.append(seq_onehot)
        targets.append(target_onehot)
    
    return np.array(data), np.array(targets)

class AttentionDataset(Dataset):
    """Dataset for attention experiments"""
    def __init__(self, sequences, targets):
        self.sequences = torch.FloatTensor(sequences)
        self.targets = torch.FloatTensor(targets)
    
    def __len__(self):
        return len(self.sequences)
    
    def __getitem__(self, idx):
        return self.sequences[idx], self.targets[idx]

def train_attention_model():
    """Train seq2seq model with attention on copy task"""
    print("=== Training Attention Model ===")
    
    # Create data
    seq_length = 8
    vocab_size = 10
    X, y = create_copy_task_data(seq_length, vocab_size, 2000)
    
    # Split data
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]
    
    # Create datasets
    train_dataset = AttentionDataset(X_train, y_train)
    test_dataset = AttentionDataset(X_test, y_test)
    
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)
    
    # Create model
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = Seq2SeqWithAttention(vocab_size, vocab_size, 64, 'bahdanau').to(device)
    
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    
    # Training
    train_losses = []
    
    for epoch in range(50):
        model.train()
        epoch_loss = 0
        
        for batch_x, batch_y in train_loader:
            batch_x, batch_y = batch_x.to(device), batch_y.to(device)
            
            optimizer.zero_grad()
            outputs, attention_weights = model(batch_x, batch_y)
            loss = criterion(outputs, batch_y)
            loss.backward()
            
            # Gradient clipping
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
            
            optimizer.step()
            epoch_loss += loss.item()
        
        avg_loss = epoch_loss / len(train_loader)
        train_losses.append(avg_loss)
        
        if epoch % 10 == 0:
            print(f'Epoch [{epoch+1}/50], Loss: {avg_loss:.4f}')
    
    # Test
    model.eval()
    test_loss = 0
    correct_sequences = 0
    
    with torch.no_grad():
        for batch_x, batch_y in test_loader:
            batch_x, batch_y = batch_x.to(device), batch_y.to(device)
            outputs, attention_weights = model(batch_x, batch_y)
            test_loss += criterion(outputs, batch_y).item()
            
            # Check if entire sequences match
            pred_sequences = torch.argmax(outputs, dim=2)
            true_sequences = torch.argmax(batch_y, dim=2)
            correct_sequences += (pred_sequences == true_sequences).all(dim=1).sum().item()
    
    avg_test_loss = test_loss / len(test_loader)
    sequence_accuracy = 100 * correct_sequences / len(test_dataset)
    
    print(f'Test Loss: {avg_test_loss:.4f}')
    print(f'Sequence Accuracy: {sequence_accuracy:.2f}%')
    
    return model, train_losses, test_loader

def visualize_attention_weights(model, test_loader):
    """Visualize attention weights for sample sequences"""
    print("=== Visualizing Attention Weights ===")
    
    model.eval()
    device = next(model.parameters()).device
    
    # Get a sample batch
    sample_batch = next(iter(test_loader))
    sample_x, sample_y = sample_batch[0][:4], sample_batch[1][:4]  # Take 4 samples
    sample_x, sample_y = sample_x.to(device), sample_y.to(device)
    
    with torch.no_grad():
        outputs, attention_weights = model(sample_x, sample_y)
    
    # Convert to numpy
    attention_weights = attention_weights.cpu().numpy()
    sample_x_np = torch.argmax(sample_x, dim=2).cpu().numpy()
    sample_y_np = torch.argmax(sample_y, dim=2).cpu().numpy()
    outputs_np = torch.argmax(outputs, dim=2).cpu().numpy()
    
    # Plot attention heatmaps
    fig, axes = plt.subplots(2, 2, figsize=(12, 10))
    axes = axes.flatten()
    
    for i in range(4):
        # Create heatmap
        sns.heatmap(attention_weights[i].T, 
                   xticklabels=[f't{j}' for j in range(attention_weights.shape[1])],
                   yticklabels=[f's{j}' for j in range(attention_weights.shape[2])],
                   cmap='Blues', ax=axes[i], cbar=True)
        
        axes[i].set_title(f'Sample {i+1}: Input {sample_x_np[i]} → Output {outputs_np[i]}')
        axes[i].set_xlabel('Decoder Time Step')
        axes[i].set_ylabel('Encoder Position')
    
    plt.tight_layout()
    plt.show()

def compare_attention_types():
    """Compare different attention mechanisms"""
    print("\n=== Comparing Attention Mechanisms ===")
    
    seq_length = 6
    vocab_size = 8
    X, y = create_copy_task_data(seq_length, vocab_size, 1000)
    
    # Split data
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]
    
    train_dataset = AttentionDataset(X_train, y_train)
    test_dataset = AttentionDataset(X_test, y_test)
    
    train_loader = DataLoader(train_dataset, batch_size=16, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=16, shuffle=False)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    attention_types = ['bahdanau', 'luong']
    results = {}
    
    for attn_type in attention_types:
        print(f"\nTraining with {attn_type} attention...")
        
        model = Seq2SeqWithAttention(vocab_size, vocab_size, 32, attn_type).to(device)
        criterion = nn.MSELoss()
        optimizer = optim.Adam(model.parameters(), lr=0.002)
        
        train_losses = []
        
        for epoch in range(30):
            model.train()
            epoch_loss = 0
            
            for batch_x, batch_y in train_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                
                optimizer.zero_grad()
                outputs, _ = model(batch_x, batch_y)
                loss = criterion(outputs, batch_y)
                loss.backward()
                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                optimizer.step()
                
                epoch_loss += loss.item()
            
            avg_loss = epoch_loss / len(train_loader)
            train_losses.append(avg_loss)
        
        # Test
        model.eval()
        correct_sequences = 0
        
        with torch.no_grad():
            for batch_x, batch_y in test_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                outputs, _ = model(batch_x, batch_y)
                
                pred_sequences = torch.argmax(outputs, dim=2)
                true_sequences = torch.argmax(batch_y, dim=2)
                correct_sequences += (pred_sequences == true_sequences).all(dim=1).sum().item()
        
        accuracy = 100 * correct_sequences / len(test_dataset)
        print(f'{attn_type} attention - Final accuracy: {accuracy:.2f}%')
        
        results[attn_type] = {
            'train_losses': train_losses,
            'accuracy': accuracy
        }
    
    # Plot comparison
    plt.figure(figsize=(10, 6))
    for attn_type, data in results.items():
        plt.plot(data['train_losses'], label=f'{attn_type} (acc: {data["accuracy"]:.1f}%)', linewidth=2)
    
    plt.title('Attention Mechanisms Comparison')
    plt.xlabel('Epoch')
    plt.ylabel('Training Loss')
    plt.legend()
    plt.grid(True)
    plt.show()
    
    return results

def demonstrate_attention_benefits():
    """Demonstrate the benefits of attention mechanism"""
    print("\n=== Demonstrating Attention Benefits ===")
    
    # Create sequences of different lengths
    lengths = [5, 10, 15, 20]
    results = []
    
    for length in lengths:
        print(f"Testing sequence length: {length}")
        
        # Create data
        X, y = create_copy_task_data(length, 10, 500)
        split = int(0.8 * len(X))
        X_train, X_test = X[:split], X[split:]
        y_train, y_test = y[:split], y[split:]
        
        train_dataset = AttentionDataset(X_train, y_train)
        test_dataset = AttentionDataset(X_test, y_test)
        
        train_loader = DataLoader(train_dataset, batch_size=16, shuffle=True)
        test_loader = DataLoader(test_dataset, batch_size=16, shuffle=False)
        
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        
        # Train model with attention
        model = Seq2SeqWithAttention(10, 10, 64, 'bahdanau').to(device)
        criterion = nn.MSELoss()
        optimizer = optim.Adam(model.parameters(), lr=0.001)
        
        # Quick training
        for epoch in range(20):
            model.train()
            for batch_x, batch_y in train_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                optimizer.zero_grad()
                outputs, _ = model(batch_x, batch_y)
                loss = criterion(outputs, batch_y)
                loss.backward()
                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                optimizer.step()
        
        # Test
        model.eval()
        correct = 0
        with torch.no_grad():
            for batch_x, batch_y in test_loader:
                batch_x, batch_y = batch_x.to(device), batch_y.to(device)
                outputs, _ = model(batch_x, batch_y)
                pred = torch.argmax(outputs, dim=2)
                true = torch.argmax(batch_y, dim=2)
                correct += (pred == true).all(dim=1).sum().item()
        
        accuracy = 100 * correct / len(test_dataset)
        results.append((length, accuracy))
        print(f"Length {length}: {accuracy:.1f}% accuracy")
    
    # Plot results
    lengths, accuracies = zip(*results)
    plt.figure(figsize=(10, 6))
    plt.plot(lengths, accuracies, 'bo-', linewidth=2, markersize=8)
    plt.title('Attention Model Performance vs Sequence Length')
    plt.xlabel('Sequence Length')
    plt.ylabel('Accuracy (%)')
    plt.grid(True)
    plt.ylim(0, 100)
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Attention Mechanisms Implementation and Analysis ===")
    
    # Train attention model
    model, losses, test_loader = train_attention_model()
    
    # Visualize attention weights
    visualize_attention_weights(model, test_loader)
    
    # Compare attention types
    comparison_results = compare_attention_types()
    
    # Demonstrate benefits
    demonstrate_attention_benefits()
    
    print("\n=== Key Concepts of Attention ===")
    print("1. PROBLEM: Information bottleneck in seq2seq models")
    print("   - Fixed-size context vector loses information")
    print("   - Performance degrades with longer sequences")
    
    print("\n2. SOLUTION: Attention mechanism")
    print("   - Allow decoder to access all encoder states")
    print("   - Learn which parts of input to focus on")
    print("   - Dynamic context vector for each time step")
    
    print("\n3. BAHDANAU (Additive) ATTENTION:")
    print("   - e_ij = v^T * tanh(W_a*h_j + U_a*s_i)")
    print("   - Concatenates encoder and decoder states")
    print("   - Uses feed-forward network for scoring")
    
    print("\n4. LUONG (Multiplicative) ATTENTION:")
    print("   - Dot: score = h_t^T * h_s")
    print("   - General: score = h_t^T * W_a * h_s")
    print("   - Simpler and more efficient")
    
    print("\n5. BENEFITS:")
    print("   ✓ Better performance on long sequences")
    print("   ✓ Helps with vanishing gradient problem")
    print("   ✓ Provides interpretability")
    print("   ✓ Foundation for Transformer architecture")
    
    print("\n6. APPLICATIONS:")
    print("   • Neural machine translation")
    print("   • Text summarization")
    print("   • Image captioning")
    print("   • Speech recognition")
    print("   • Any sequence-to-sequence task")