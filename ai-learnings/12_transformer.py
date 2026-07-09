# author: csgod cmps studies
# Transformer - Full transformer architecture
# Transformers revolutionized NLP by replacing recurrence with self-attention,
# enabling parallel processing and better modeling of long-range dependencies.

import math
import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset

class MultiHeadAttention(nn.Module):
    """Multi-Head Self-Attention mechanism"""
    def __init__(self, d_model, n_heads):
        super(MultiHeadAttention, self).__init__()
        self.d_model = d_model
        self.n_heads = n_heads
        self.d_k = d_model // n_heads
        
        assert d_model % n_heads == 0, "d_model must be divisible by n_heads"
        
        # Linear transformations for Q, K, V
        self.W_q = nn.Linear(d_model, d_model)
        self.W_k = nn.Linear(d_model, d_model)
        self.W_v = nn.Linear(d_model, d_model)
        self.W_o = nn.Linear(d_model, d_model)
        
        self.dropout = nn.Dropout(0.1)
    
    def scaled_dot_product_attention(self, Q, K, V, mask=None):
        """Compute scaled dot-product attention"""
        d_k = Q.size(-1)
        
        # Compute attention scores
        scores = torch.matmul(Q, K.transpose(-2, -1)) / math.sqrt(d_k)
        
        # Apply mask if provided
        if mask is not None:
            scores = scores.masked_fill(mask == 0, -1e9)
        
        # Apply softmax
        attention_weights = F.softmax(scores, dim=-1)
        attention_weights = self.dropout(attention_weights)
        
        # Apply attention to values
        output = torch.matmul(attention_weights, V)
        
        return output, attention_weights
    
    def forward(self, query, key, value, mask=None):
        batch_size = query.size(0)
        
        # Linear transformations and split into heads
        Q = self.W_q(query).view(batch_size, -1, self.n_heads, self.d_k).transpose(1, 2)
        K = self.W_k(key).view(batch_size, -1, self.n_heads, self.d_k).transpose(1, 2)
        V = self.W_v(value).view(batch_size, -1, self.n_heads, self.d_k).transpose(1, 2)
        
        # Apply scaled dot-product attention
        attn_output, attention_weights = self.scaled_dot_product_attention(Q, K, V, mask)
        
        # Concatenate heads
        attn_output = attn_output.transpose(1, 2).contiguous().view(
            batch_size, -1, self.d_model
        )
        
        # Apply final linear transformation
        output = self.W_o(attn_output)
        
        return output, attention_weights

class PositionalEncoding(nn.Module):
    """Positional encoding for transformer"""
    def __init__(self, d_model, max_length=5000):
        super(PositionalEncoding, self).__init__()
        
        pe = torch.zeros(max_length, d_model)
        position = torch.arange(0, max_length, dtype=torch.float).unsqueeze(1)
        
        # Create positional encoding using sine and cosine functions
        div_term = torch.exp(torch.arange(0, d_model, 2).float() * 
                           (-math.log(10000.0) / d_model))
        
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        
        pe = pe.unsqueeze(0).transpose(0, 1)
        self.register_buffer('pe', pe)
    
    def forward(self, x):
        return x + self.pe[:x.size(0), :]

class FeedForward(nn.Module):
    """Position-wise feed-forward network"""
    def __init__(self, d_model, d_ff, dropout=0.1):
        super(FeedForward, self).__init__()
        self.linear1 = nn.Linear(d_model, d_ff)
        self.linear2 = nn.Linear(d_ff, d_model)
        self.dropout = nn.Dropout(dropout)
    
    def forward(self, x):
        return self.linear2(self.dropout(F.relu(self.linear1(x))))

class TransformerBlock(nn.Module):
    """Transformer encoder block"""
    def __init__(self, d_model, n_heads, d_ff, dropout=0.1):
        super(TransformerBlock, self).__init__()
        self.attention = MultiHeadAttention(d_model, n_heads)
        self.norm1 = nn.LayerNorm(d_model)
        self.norm2 = nn.LayerNorm(d_model)
        self.feed_forward = FeedForward(d_model, d_ff, dropout)
        self.dropout = nn.Dropout(dropout)
    
    def forward(self, x, mask=None):
        # Self-attention with residual connection and layer norm
        attn_output, attention_weights = self.attention(x, x, x, mask)
        x = self.norm1(x + self.dropout(attn_output))
        
        # Feed-forward with residual connection and layer norm
        ff_output = self.feed_forward(x)
        x = self.norm2(x + self.dropout(ff_output))
        
        return x, attention_weights

class TransformerEncoder(nn.Module):
    """Transformer encoder"""
    def __init__(self, vocab_size, d_model, n_heads, n_layers, d_ff, max_length, dropout=0.1):
        super(TransformerEncoder, self).__init__()
        self.d_model = d_model
        self.embedding = nn.Embedding(vocab_size, d_model)
        self.positional_encoding = PositionalEncoding(d_model, max_length)
        
        self.transformer_blocks = nn.ModuleList([
            TransformerBlock(d_model, n_heads, d_ff, dropout)
            for _ in range(n_layers)
        ])
        
        self.dropout = nn.Dropout(dropout)
    
    def forward(self, x, mask=None):
        # Embedding and positional encoding
        x = self.embedding(x) * math.sqrt(self.d_model)
        x = self.positional_encoding(x)
        x = self.dropout(x)
        
        attention_weights = []
        
        # Pass through transformer blocks
        for transformer_block in self.transformer_blocks:
            x, attn_weights = transformer_block(x, mask)
            attention_weights.append(attn_weights)
        
        return x, attention_weights

class TransformerClassifier(nn.Module):
    """Transformer for classification tasks"""
    def __init__(self, vocab_size, d_model, n_heads, n_layers, n_classes, max_length, d_ff=None, dropout=0.1):
        super(TransformerClassifier, self).__init__()
        if d_ff is None:
            d_ff = 4 * d_model
        
        self.encoder = TransformerEncoder(vocab_size, d_model, n_heads, n_layers, d_ff, max_length, dropout)
        self.classifier = nn.Linear(d_model, n_classes)
        self.dropout = nn.Dropout(dropout)
    
    def forward(self, x, mask=None):
        # Encode sequence
        encoded, attention_weights = self.encoder(x, mask)
        
        # Global average pooling
        if mask is not None:
            mask_expanded = mask.unsqueeze(-1).expand_as(encoded)
            encoded = encoded * mask_expanded
            lengths = mask.sum(dim=1, keepdim=True).float()
            pooled = encoded.sum(dim=1) / lengths
        else:
            pooled = encoded.mean(dim=1)
        
        # Classification
        pooled = self.dropout(pooled)
        output = self.classifier(pooled)
        
        return output, attention_weights

class TextDataset(Dataset):
    """Simple text dataset for demonstration"""
    def __init__(self, texts, labels, vocab, max_length):
        self.texts = texts
        self.labels = labels
        self.vocab = vocab
        self.max_length = max_length
        
    def text_to_indices(self, text):
        """Convert text to indices"""
        words = text.lower().split()
        indices = [self.vocab.get(word, 0) for word in words]  # 0 for unknown
        
        # Pad or truncate
        if len(indices) < self.max_length:
            indices.extend([0] * (self.max_length - len(indices)))
        else:
            indices = indices[:self.max_length]
        
        return indices
    
    def __len__(self):
        return len(self.texts)
    
    def __getitem__(self, idx):
        text_indices = self.text_to_indices(self.texts[idx])
        label = self.labels[idx]
        
        return torch.tensor(text_indices, dtype=torch.long), torch.tensor(label, dtype=torch.long)

def create_synthetic_text_data():
    """Create synthetic text classification data"""
    # Simple sentiment-like data
    positive_texts = [
        "this is great and amazing",
        "wonderful excellent fantastic",
        "love this so much",
        "absolutely brilliant and perfect",
        "incredible outstanding superb",
        "amazing wonderful great excellent",
        "fantastic love this",
        "perfect brilliant incredible"
    ] * 50  # Repeat for more data
    
    negative_texts = [
        "this is terrible and awful",
        "horrible bad disappointing",
        "hate this so much",
        "absolutely terrible and disappointing",
        "awful horrible bad",
        "terrible disappointing bad horrible",
        "awful hate this",
        "disappointing terrible horrible"
    ] * 50
    
    # Create labels
    texts = positive_texts + negative_texts
    labels = [1] * len(positive_texts) + [0] * len(negative_texts)
    
    # Create vocabulary
    all_words = set()
    for text in texts:
        all_words.update(text.lower().split())
    
    vocab = {word: idx + 1 for idx, word in enumerate(sorted(all_words))}
    vocab['<UNK>'] = 0  # Unknown token
    
    return texts, labels, vocab

def train_transformer():
    """Train transformer on text classification"""
    print("=== Training Transformer ===")
    
    # Create data
    texts, labels, vocab = create_synthetic_text_data()
    
    # Shuffle data
    indices = np.random.permutation(len(texts))
    texts = [texts[i] for i in indices]
    labels = [labels[i] for i in indices]
    
    # Split
    split = int(0.8 * len(texts))
    train_texts, test_texts = texts[:split], texts[split:]
    train_labels, test_labels = labels[:split], labels[split:]
    
    # Create datasets
    max_length = 8
    train_dataset = TextDataset(train_texts, train_labels, vocab, max_length)
    test_dataset = TextDataset(test_texts, test_labels, vocab, max_length)
    
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)
    
    print(f"Vocabulary size: {len(vocab)}")
    print(f"Training samples: {len(train_dataset)}")
    print(f"Test samples: {len(test_dataset)}")
    
    # Create model
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = TransformerClassifier(
        vocab_size=len(vocab),
        d_model=128,
        n_heads=8,
        n_layers=2,
        n_classes=2,
        max_length=max_length
    ).to(device)
    
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
        
        for batch_texts, batch_labels in train_loader:
            batch_texts, batch_labels = batch_texts.to(device), batch_labels.to(device)
            
            # Create padding mask
            mask = (batch_texts != 0).float()
            
            optimizer.zero_grad()
            outputs, _ = model(batch_texts, mask)
            loss = criterion(outputs, batch_labels)
            loss.backward()
            
            # Gradient clipping
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
            
            optimizer.step()
            
            epoch_loss += loss.item()
            _, predicted = torch.max(outputs, 1)
            total += batch_labels.size(0)
            correct += (predicted == batch_labels).sum().item()
        
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
    
    with torch.no_grad():
        for batch_texts, batch_labels in test_loader:
            batch_texts, batch_labels = batch_texts.to(device), batch_labels.to(device)
            mask = (batch_texts != 0).float()
            outputs, _ = model(batch_texts, mask)
            _, predicted = torch.max(outputs, 1)
            total += batch_labels.size(0)
            correct += (predicted == batch_labels).sum().item()
    
    test_accuracy = 100 * correct / total
    print(f'Test Accuracy: {test_accuracy:.2f}%')
    
    return model, train_losses, train_accuracies, test_loader, vocab

def visualize_attention_patterns(model, test_loader, vocab):
    """Visualize attention patterns in transformer"""
    print("=== Visualizing Attention Patterns ===")
    
    model.eval()
    device = next(model.parameters()).device
    
    # Get reverse vocabulary for visualization
    idx_to_word = {idx: word for word, idx in vocab.items()}
    
    # Get a sample
    sample_batch = next(iter(test_loader))
    sample_texts, sample_labels = sample_batch[0][:1], sample_batch[1][:1]
    sample_texts, sample_labels = sample_texts.to(device), sample_labels.to(device)
    
    mask = (sample_texts != 0).float()
    
    with torch.no_grad():
        outputs, attention_weights = model(sample_texts, mask)
    
    # Get the text
    text_indices = sample_texts[0].cpu().numpy()
    words = [idx_to_word.get(idx, '<UNK>') for idx in text_indices if idx != 0]
    
    print(f"Sample text: {' '.join(words)}")
    print(f"Predicted label: {torch.argmax(outputs[0]).item()}")
    print(f"True label: {sample_labels[0].item()}")
    
    # Visualize attention from last layer, first head
    last_layer_attention = attention_weights[-1][0, 0].cpu().numpy()  # [seq_len, seq_len]
    
    # Truncate to actual sequence length
    seq_len = len(words)
    attention_matrix = last_layer_attention[:seq_len, :seq_len]
    
    # Plot attention heatmap
    plt.figure(figsize=(10, 8))
    plt.imshow(attention_matrix, cmap='Blues', aspect='auto')
    plt.colorbar()
    
    # Add labels
    plt.xticks(range(seq_len), words, rotation=45)
    plt.yticks(range(seq_len), words)
    plt.xlabel('Key Position')
    plt.ylabel('Query Position')
    plt.title('Self-Attention Pattern (Last Layer, Head 1)')
    
    # Add attention values as text
    for i in range(seq_len):
        for j in range(seq_len):
            plt.text(j, i, f'{attention_matrix[i, j]:.2f}', 
                    ha='center', va='center', fontsize=8)
    
    plt.tight_layout()
    plt.show()

def compare_model_sizes():
    """Compare different transformer configurations"""
    print("\n=== Comparing Transformer Configurations ===")
    
    configs = [
        {'d_model': 64, 'n_heads': 4, 'n_layers': 1, 'name': 'Small'},
        {'d_model': 128, 'n_heads': 8, 'n_layers': 2, 'name': 'Medium'},
        {'d_model': 256, 'n_heads': 8, 'n_layers': 3, 'name': 'Large'},
    ]
    
    vocab_size = 1000
    max_length = 20
    
    results = []
    
    for config in configs:
        model = TransformerClassifier(
            vocab_size=vocab_size,
            d_model=config['d_model'],
            n_heads=config['n_heads'],
            n_layers=config['n_layers'],
            n_classes=2,
            max_length=max_length
        )
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        
        # Estimate memory (rough)
        memory_mb = total_params * 4 / (1024 * 1024)  # 4 bytes per float32
        
        results.append({
            'name': config['name'],
            'params': total_params,
            'memory_mb': memory_mb,
            'd_model': config['d_model'],
            'n_heads': config['n_heads'],
            'n_layers': config['n_layers']
        })
        
        print(f"{config['name']} model:")
        print(f"  Parameters: {total_params:,}")
        print(f"  Memory: ~{memory_mb:.1f} MB")
        print(f"  d_model: {config['d_model']}, heads: {config['n_heads']}, layers: {config['n_layers']}")
        print()
    
    # Plot comparison
    names = [r['name'] for r in results]
    params = [r['params'] for r in results]
    
    plt.figure(figsize=(10, 6))
    plt.bar(names, params)
    plt.title('Transformer Model Sizes')
    plt.xlabel('Model Configuration')
    plt.ylabel('Number of Parameters')
    plt.yscale('log')
    
    # Add value labels on bars
    for i, (name, param_count) in enumerate(zip(names, params)):
        plt.text(i, param_count, f'{param_count:,}', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.show()

def demonstrate_positional_encoding():
    """Demonstrate positional encoding patterns"""
    print("\n=== Positional Encoding Visualization ===")
    
    d_model = 128
    max_length = 100
    
    pe = PositionalEncoding(d_model, max_length)
    
    # Get positional encodings
    positions = torch.arange(0, max_length).unsqueeze(1)
    dummy_input = torch.zeros(max_length, 1, d_model)
    pos_encodings = pe(dummy_input).squeeze(1).numpy()  # [max_length, d_model]
    
    # Plot positional encodings
    plt.figure(figsize=(12, 8))
    
    # Heatmap of positional encodings
    plt.subplot(2, 1, 1)
    plt.imshow(pos_encodings.T, aspect='auto', cmap='RdBu_r')
    plt.colorbar()
    plt.title('Positional Encodings Heatmap')
    plt.xlabel('Position')
    plt.ylabel('Dimension')
    
    # Individual encoding patterns
    plt.subplot(2, 1, 2)
    for dim in [0, 1, 4, 8, 16, 32]:
        plt.plot(pos_encodings[:50, dim], label=f'dim {dim}')
    plt.title('Positional Encoding Patterns (first 50 positions)')
    plt.xlabel('Position')
    plt.ylabel('Encoding Value')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Transformer Architecture Implementation ===")
    
    # Train transformer
    model, losses, accuracies, test_loader, vocab = train_transformer()
    
    # Plot training curves
    plt.figure(figsize=(12, 5))
    
    plt.subplot(1, 2, 1)
    plt.plot(losses, linewidth=2)
    plt.title('Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.subplot(1, 2, 2)
    plt.plot(accuracies, linewidth=2)
    plt.title('Training Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy (%)')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Visualize attention patterns
    visualize_attention_patterns(model, test_loader, vocab)
    
    # Compare model sizes
    compare_model_sizes()
    
    # Demonstrate positional encoding
    demonstrate_positional_encoding()
    
    print("\n=== Transformer Key Innovations ===")
    print("1. SELF-ATTENTION: Replaces recurrence with attention")
    print("   - Each position can attend to all positions")
    print("   - Enables parallelization during training")
    print("   - Better long-range dependency modeling")
    
    print("\n2. MULTI-HEAD ATTENTION: Multiple attention mechanisms")
    print("   - Different heads learn different types of relationships")
    print("   - Increased model expressiveness")
    print("   - Parallel computation of attention")
    
    print("\n3. POSITIONAL ENCODING: Inject sequence order information")
    print("   - Sine/cosine functions of different frequencies")
    print("   - Allows model to understand position")
    print("   - No learned position embeddings needed")
    
    print("\n4. LAYER NORMALIZATION & RESIDUAL CONNECTIONS:")
    print("   - Stabilizes training of deep networks")
    print("   - Enables gradient flow")
    print("   - Applied before rather than after sub-layers")
    
    print("\n5. FEED-FORWARD NETWORKS:")
    print("   - Position-wise processing")
    print("   - Adds non-linearity and capacity")
    print("   - Typically 4x hidden size")
    
    print("\n=== Transformer Advantages ===")
    print("✓ Parallelizable training (vs RNNs)")
    print("✓ Better long-range dependencies")
    print("✓ More interpretable attention patterns")
    print("✓ Transfer learning capabilities")
    print("✓ Scalable to very large models")
    
    print("\n=== Applications ===")
    print("• BERT: Bidirectional encoder representations")
    print("• GPT: Generative pre-trained transformer")
    print("• T5: Text-to-text transfer transformer")
    print("• Vision Transformer (ViT): Images as sequences")
    print("• DETR: Object detection with transformers")