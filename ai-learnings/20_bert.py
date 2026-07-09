# author: csgod cmps studies
# BERT - Bidirectional Encoder Representations from Transformers
# BERT uses bidirectional self-attention to learn contextualized word representations
# through masked language modeling and next sentence prediction pretraining tasks.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader, Dataset
import numpy as np
import matplotlib.pyplot as plt
import math
import random
from typing import Optional, Tuple, List

class BERTEmbedding(nn.Module):
    """BERT embeddings: token + position + segment embeddings"""
    def __init__(self, vocab_size: int, d_model: int, max_len: int = 512, dropout: float = 0.1):
        super(BERTEmbedding, self).__init__()
        
        self.token_embedding = nn.Embedding(vocab_size, d_model)
        self.position_embedding = nn.Embedding(max_len, d_model)
        self.segment_embedding = nn.Embedding(2, d_model)  # 2 for sentence A and B
        
        self.layer_norm = nn.LayerNorm(d_model)
        self.dropout = nn.Dropout(dropout)
        
    def forward(self, x: torch.Tensor, segment_ids: Optional[torch.Tensor] = None) -> torch.Tensor:
        batch_size, seq_len = x.size()
        
        # Token embeddings
        token_emb = self.token_embedding(x)
        
        # Position embeddings
        positions = torch.arange(seq_len, device=x.device).unsqueeze(0).expand(batch_size, -1)
        position_emb = self.position_embedding(positions)
        
        # Segment embeddings
        if segment_ids is None:
            segment_ids = torch.zeros_like(x)
        segment_emb = self.segment_embedding(segment_ids)
        
        # Sum embeddings
        embeddings = token_emb + position_emb + segment_emb
        embeddings = self.layer_norm(embeddings)
        embeddings = self.dropout(embeddings)
        
        return embeddings

class MultiHeadSelfAttention(nn.Module):
    """Multi-head self-attention for BERT"""
    def __init__(self, d_model: int, num_heads: int, dropout: float = 0.1):
        super(MultiHeadSelfAttention, self).__init__()
        assert d_model % num_heads == 0
        
        self.d_model = d_model
        self.num_heads = num_heads
        self.d_k = d_model // num_heads
        
        self.w_q = nn.Linear(d_model, d_model)
        self.w_k = nn.Linear(d_model, d_model)
        self.w_v = nn.Linear(d_model, d_model)
        self.w_o = nn.Linear(d_model, d_model)
        
        self.dropout = nn.Dropout(dropout)
        
    def scaled_dot_product_attention(self, Q: torch.Tensor, K: torch.Tensor, V: torch.Tensor,
                                   mask: Optional[torch.Tensor] = None) -> Tuple[torch.Tensor, torch.Tensor]:
        # Calculate attention scores
        scores = torch.matmul(Q, K.transpose(-2, -1)) / math.sqrt(self.d_k)
        
        # Apply mask
        if mask is not None:
            scores = scores.masked_fill(mask == 0, -1e9)
        
        # Apply softmax
        attention_weights = F.softmax(scores, dim=-1)
        attention_weights = self.dropout(attention_weights)
        
        # Apply attention to values
        output = torch.matmul(attention_weights, V)
        return output, attention_weights
    
    def forward(self, x: torch.Tensor, mask: Optional[torch.Tensor] = None) -> Tuple[torch.Tensor, torch.Tensor]:
        batch_size, seq_len, d_model = x.size()
        
        # Linear transformations and split into heads
        Q = self.w_q(x).view(batch_size, seq_len, self.num_heads, self.d_k).transpose(1, 2)
        K = self.w_k(x).view(batch_size, seq_len, self.num_heads, self.d_k).transpose(1, 2)
        V = self.w_v(x).view(batch_size, seq_len, self.num_heads, self.d_k).transpose(1, 2)
        
        # Apply attention
        attention_output, attention_weights = self.scaled_dot_product_attention(Q, K, V, mask)
        
        # Concatenate heads
        attention_output = attention_output.transpose(1, 2).contiguous().view(
            batch_size, seq_len, d_model
        )
        
        # Final linear transformation
        output = self.w_o(attention_output)
        
        return output, attention_weights

class FeedForward(nn.Module):
    """Position-wise feed-forward network"""
    def __init__(self, d_model: int, d_ff: int, dropout: float = 0.1):
        super(FeedForward, self).__init__()
        self.linear1 = nn.Linear(d_model, d_ff)
        self.linear2 = nn.Linear(d_ff, d_model)
        self.dropout = nn.Dropout(dropout)
        
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.linear2(self.dropout(F.gelu(self.linear1(x))))

class BERTLayer(nn.Module):
    """Single BERT transformer layer"""
    def __init__(self, d_model: int, num_heads: int, d_ff: int, dropout: float = 0.1):
        super(BERTLayer, self).__init__()
        
        self.attention = MultiHeadSelfAttention(d_model, num_heads, dropout)
        self.feed_forward = FeedForward(d_model, d_ff, dropout)
        
        self.layer_norm1 = nn.LayerNorm(d_model)
        self.layer_norm2 = nn.LayerNorm(d_model)
        
        self.dropout = nn.Dropout(dropout)
    
    def forward(self, x: torch.Tensor, mask: Optional[torch.Tensor] = None) -> Tuple[torch.Tensor, torch.Tensor]:
        # Self-attention with residual connection and layer norm
        attention_output, attention_weights = self.attention(x, mask)
        x = self.layer_norm1(x + self.dropout(attention_output))
        
        # Feed-forward with residual connection and layer norm
        ff_output = self.feed_forward(x)
        x = self.layer_norm2(x + self.dropout(ff_output))
        
        return x, attention_weights

class BERTEncoder(nn.Module):
    """BERT encoder with multiple transformer layers"""
    def __init__(self, vocab_size: int, d_model: int, num_heads: int, d_ff: int,
                 num_layers: int, max_len: int = 512, dropout: float = 0.1):
        super(BERTEncoder, self).__init__()
        
        self.embedding = BERTEmbedding(vocab_size, d_model, max_len, dropout)
        
        self.layers = nn.ModuleList([
            BERTLayer(d_model, num_heads, d_ff, dropout)
            for _ in range(num_layers)
        ])
    
    def forward(self, x: torch.Tensor, segment_ids: Optional[torch.Tensor] = None,
                mask: Optional[torch.Tensor] = None) -> Tuple[torch.Tensor, List[torch.Tensor]]:
        # Embeddings
        x = self.embedding(x, segment_ids)
        
        # Pass through transformer layers
        attention_weights = []
        for layer in self.layers:
            x, weights = layer(x, mask)
            attention_weights.append(weights)
        
        return x, attention_weights

class BERTForPreTraining(nn.Module):
    """BERT model for pretraining with MLM and NSP heads"""
    def __init__(self, vocab_size: int, d_model: int = 768, num_heads: int = 12,
                 d_ff: int = 3072, num_layers: int = 12, max_len: int = 512, dropout: float = 0.1):
        super(BERTForPreTraining, self).__init__()
        
        self.encoder = BERTEncoder(vocab_size, d_model, num_heads, d_ff, num_layers, max_len, dropout)
        
        # Masked Language Model head
        self.mlm_head = nn.Sequential(
            nn.Linear(d_model, d_model),
            nn.GELU(),
            nn.LayerNorm(d_model),
            nn.Linear(d_model, vocab_size)
        )
        
        # Next Sentence Prediction head
        self.nsp_head = nn.Sequential(
            nn.Linear(d_model, d_model),
            nn.Tanh(),
            nn.Linear(d_model, 2)  # Binary classification
        )
        
    def forward(self, input_ids: torch.Tensor, segment_ids: Optional[torch.Tensor] = None,
                attention_mask: Optional[torch.Tensor] = None) -> Tuple[torch.Tensor, torch.Tensor]:
        # Get encoder outputs
        sequence_output, attention_weights = self.encoder(input_ids, segment_ids, attention_mask)
        
        # MLM predictions for all tokens
        mlm_logits = self.mlm_head(sequence_output)
        
        # NSP prediction using [CLS] token (first token)
        cls_output = sequence_output[:, 0]  # [CLS] token
        nsp_logits = self.nsp_head(cls_output)
        
        return mlm_logits, nsp_logits

class SimpleBERTDataset(Dataset):
    """Simple dataset for BERT pretraining demonstration"""
    def __init__(self, sentences: List[str], vocab: dict, max_len: int = 128, mask_prob: float = 0.15):
        self.sentences = sentences
        self.vocab = vocab
        self.max_len = max_len
        self.mask_prob = mask_prob
        
        # Special tokens
        self.cls_token = vocab['[CLS]']
        self.sep_token = vocab['[SEP]']
        self.mask_token = vocab['[MASK]']
        self.pad_token = vocab['[PAD]']
        self.unk_token = vocab['[UNK]']
    
    def __len__(self):
        return len(self.sentences) // 2  # Pairs of sentences
    
    def __getitem__(self, idx):
        # Get sentence pair
        sent_a = self.sentences[idx * 2]
        sent_b = self.sentences[idx * 2 + 1] if idx * 2 + 1 < len(self.sentences) else ""
        
        # Create NSP label (50% chance of random pair)
        is_next = 1
        if random.random() < 0.5:
            # Replace sent_b with random sentence
            random_idx = random.randint(0, len(self.sentences) - 1)
            sent_b = self.sentences[random_idx]
            is_next = 0
        
        # Tokenize (simple word-level tokenization)
        tokens_a = sent_a.lower().split()[:self.max_len//2 - 2]
        tokens_b = sent_b.lower().split()[:self.max_len//2 - 1] if sent_b else []
        
        # Create input sequence: [CLS] sent_a [SEP] sent_b [SEP]
        tokens = ['[CLS]'] + tokens_a + ['[SEP]']
        segment_ids = [0] * len(tokens)
        
        if tokens_b:
            tokens += tokens_b + ['[SEP]']
            segment_ids += [1] * (len(tokens_b) + 1)
        
        # Convert to ids
        input_ids = [self.vocab.get(token, self.unk_token) for token in tokens]
        
        # Create MLM labels and mask tokens
        mlm_labels = [-100] * len(input_ids)  # -100 means ignore in loss
        
        for i in range(1, len(input_ids) - 1):  # Skip [CLS] and [SEP]
            if random.random() < self.mask_prob:
                mlm_labels[i] = input_ids[i]  # Store original token
                
                # 80% mask, 10% random, 10% keep original
                prob = random.random()
                if prob < 0.8:
                    input_ids[i] = self.mask_token
                elif prob < 0.9:
                    input_ids[i] = random.randint(0, len(self.vocab) - 1)
        
        # Pad sequences
        while len(input_ids) < self.max_len:
            input_ids.append(self.pad_token)
            segment_ids.append(0)
            mlm_labels.append(-100)
        
        # Create attention mask
        attention_mask = [1 if token != self.pad_token else 0 for token in input_ids]
        
        return {
            'input_ids': torch.tensor(input_ids, dtype=torch.long),
            'segment_ids': torch.tensor(segment_ids, dtype=torch.long),
            'attention_mask': torch.tensor(attention_mask, dtype=torch.long),
            'mlm_labels': torch.tensor(mlm_labels, dtype=torch.long),
            'nsp_label': torch.tensor(is_next, dtype=torch.long)
        }

def create_simple_vocab_and_data():
    """Create simple vocabulary and dataset for demonstration"""
    # Simple sentences for demonstration
    sentences = [
        "the cat sat on the mat",
        "dogs are loyal animals",
        "birds can fly in the sky",
        "fish swim in the water",
        "the sun shines brightly today",
        "rain falls from dark clouds",
        "flowers bloom in spring season",
        "winter brings snow and cold",
        "students study in the library",
        "teachers help students learn",
        "computers process information quickly",
        "books contain knowledge and stories",
        "music makes people feel happy",
        "art expresses human creativity",
        "food provides energy for body",
        "exercise keeps people healthy"
    ] * 10  # Repeat for more training data
    
    # Create vocabulary
    vocab = {'[PAD]': 0, '[CLS]': 1, '[SEP]': 2, '[MASK]': 3, '[UNK]': 4}
    
    for sentence in sentences:
        for word in sentence.lower().split():
            if word not in vocab:
                vocab[word] = len(vocab)
    
    return sentences, vocab

def train_bert(model, dataloader, num_epochs=5, learning_rate=1e-4):
    """Train BERT model"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    
    optimizer = optim.AdamW(model.parameters(), lr=learning_rate, weight_decay=0.01)
    
    # Loss functions
    mlm_criterion = nn.CrossEntropyLoss(ignore_index=-100)
    nsp_criterion = nn.CrossEntropyLoss()
    
    train_losses = []
    mlm_losses = []
    nsp_losses = []
    
    print(f"Training BERT on {device}")
    print(f"Model parameters: {sum(p.numel() for p in model.parameters() if p.requires_grad):,}")
    
    for epoch in range(num_epochs):
        model.train()
        epoch_loss = 0
        epoch_mlm_loss = 0
        epoch_nsp_loss = 0
        
        for batch_idx, batch in enumerate(dataloader):
            # Move to device
            input_ids = batch['input_ids'].to(device)
            segment_ids = batch['segment_ids'].to(device)
            attention_mask = batch['attention_mask'].to(device)
            mlm_labels = batch['mlm_labels'].to(device)
            nsp_labels = batch['nsp_label'].to(device)
            
            optimizer.zero_grad()
            
            # Forward pass
            mlm_logits, nsp_logits = model(input_ids, segment_ids, attention_mask)
            
            # Calculate losses
            mlm_loss = mlm_criterion(mlm_logits.view(-1, mlm_logits.size(-1)), mlm_labels.view(-1))
            nsp_loss = nsp_criterion(nsp_logits, nsp_labels)
            
            # Combined loss
            total_loss = mlm_loss + nsp_loss
            
            # Backward pass
            total_loss.backward()
            
            # Gradient clipping
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
            
            optimizer.step()
            
            epoch_loss += total_loss.item()
            epoch_mlm_loss += mlm_loss.item()
            epoch_nsp_loss += nsp_loss.item()
            
            if batch_idx % 10 == 0:
                print(f'Epoch [{epoch+1}/{num_epochs}], Batch [{batch_idx}/{len(dataloader)}], '
                      f'Total Loss: {total_loss.item():.4f}, MLM: {mlm_loss.item():.4f}, '
                      f'NSP: {nsp_loss.item():.4f}')
        
        avg_loss = epoch_loss / len(dataloader)
        avg_mlm_loss = epoch_mlm_loss / len(dataloader)
        avg_nsp_loss = epoch_nsp_loss / len(dataloader)
        
        train_losses.append(avg_loss)
        mlm_losses.append(avg_mlm_loss)
        nsp_losses.append(avg_nsp_loss)
        
        print(f'Epoch [{epoch+1}/{num_epochs}] - Total Loss: {avg_loss:.4f}, '
              f'MLM Loss: {avg_mlm_loss:.4f}, NSP Loss: {avg_nsp_loss:.4f}')
    
    return train_losses, mlm_losses, nsp_losses

def visualize_attention_patterns(model, vocab, sentence="the cat sat on the mat"):
    """Visualize attention patterns for a given sentence"""
    device = next(model.parameters()).device
    model.eval()
    
    # Prepare input
    tokens = ['[CLS]'] + sentence.lower().split() + ['[SEP]']
    input_ids = [vocab.get(token, vocab['[UNK]']) for token in tokens]
    
    # Pad to reasonable length
    while len(input_ids) < 16:
        input_ids.append(vocab['[PAD]'])
    
    input_tensor = torch.tensor([input_ids[:16]]).to(device)
    
    with torch.no_grad():
        # Get encoder outputs with attention weights
        _, attention_weights = model.encoder(input_tensor)
    
    # Visualize attention from last layer, first head
    last_layer_attention = attention_weights[-1][0, 0].cpu().numpy()  # [seq_len, seq_len]
    
    # Get actual tokens (non-padded)
    actual_len = len(tokens)
    attention_matrix = last_layer_attention[:actual_len, :actual_len]
    tokens_display = tokens[:actual_len]
    
    # Plot attention heatmap
    plt.figure(figsize=(10, 8))
    plt.imshow(attention_matrix, cmap='Blues', aspect='auto')
    plt.colorbar()
    
    # Add labels
    plt.xticks(range(len(tokens_display)), tokens_display, rotation=45)
    plt.yticks(range(len(tokens_display)), tokens_display)
    plt.xlabel('Key Position')
    plt.ylabel('Query Position')
    plt.title('BERT Attention Pattern (Last Layer, Head 1)')
    
    # Add attention values as text
    for i in range(len(tokens_display)):
        for j in range(len(tokens_display)):
            plt.text(j, i, f'{attention_matrix[i, j]:.2f}', 
                    ha='center', va='center', fontsize=8)
    
    plt.tight_layout()
    plt.show()

def demonstrate_masked_language_modeling(model, vocab, sentence="the cat sat on the [MASK]"):
    """Demonstrate MLM prediction"""
    device = next(model.parameters()).device
    model.eval()
    
    # Create reverse vocab for decoding
    id_to_token = {v: k for k, v in vocab.items()}
    
    # Prepare input
    tokens = ['[CLS]'] + sentence.lower().split() + ['[SEP]']
    input_ids = [vocab.get(token, vocab['[UNK]']) for token in tokens]
    
    # Pad
    while len(input_ids) < 16:
        input_ids.append(vocab['[PAD]'])
    
    input_tensor = torch.tensor([input_ids[:16]]).to(device)
    
    with torch.no_grad():
        mlm_logits, _ = model(input_tensor)
    
    # Find mask position
    mask_pos = tokens.index('[MASK]') if '[MASK]' in tokens else 1
    
    # Get predictions for masked position
    mask_predictions = mlm_logits[0, mask_pos]
    top_predictions = torch.topk(mask_predictions, k=5)
    
    print(f"Input: {' '.join(tokens[:len([t for t in tokens if t != '[PAD]'])])}")
    print(f"Top 5 predictions for [MASK]:")
    
    for i, (score, token_id) in enumerate(zip(top_predictions.values, top_predictions.indices)):
        token = id_to_token.get(token_id.item(), '[UNK]')
        prob = F.softmax(mask_predictions, dim=-1)[token_id].item()
        print(f"  {i+1}. {token} (score: {score.item():.2f}, prob: {prob:.3f})")

def compare_bert_sizes():
    """Compare different BERT model sizes"""
    print("=== BERT Model Size Comparison ===")
    
    configs = {
        'BERT-Tiny': {'d_model': 128, 'num_heads': 2, 'd_ff': 512, 'num_layers': 2},
        'BERT-Mini': {'d_model': 256, 'num_heads': 4, 'd_ff': 1024, 'num_layers': 4},
        'BERT-Small': {'d_model': 512, 'num_heads': 8, 'd_ff': 2048, 'num_layers': 4},
        'BERT-Base': {'d_model': 768, 'num_heads': 12, 'd_ff': 3072, 'num_layers': 12},
    }
    
    vocab_size = 1000
    results = []
    
    for name, config in configs.items():
        model = BERTForPreTraining(vocab_size=vocab_size, **config)
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        memory_mb = total_params * 4 / (1024 * 1024)  # Rough memory estimate
        
        results.append({
            'name': name,
            'params': total_params,
            'memory_mb': memory_mb,
            **config
        })
        
        print(f"{name}:")
        print(f"  Parameters: {total_params:,}")
        print(f"  Memory: ~{memory_mb:.1f} MB")
        print(f"  d_model: {config['d_model']}, layers: {config['num_layers']}")
        print()
    
    # Plot comparison
    names = [r['name'] for r in results]
    params = [r['params'] for r in results]
    
    plt.figure(figsize=(10, 6))
    bars = plt.bar(names, params, color=['lightblue', 'lightgreen', 'lightcoral', 'lightyellow'])
    plt.title('BERT Model Sizes')
    plt.xlabel('Model Configuration')
    plt.ylabel('Number of Parameters')
    plt.yscale('log')
    
    # Add value labels
    for bar, param_count in zip(bars, params):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{param_count/1e6:.1f}M', ha='center', va='bottom')
    
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== BERT Implementation and Analysis ===")
    
    # Create simple dataset
    sentences, vocab = create_simple_vocab_and_data()
    print(f"Vocabulary size: {len(vocab)}")
    print(f"Number of sentences: {len(sentences)}")
    print(f"Sample vocab: {list(vocab.items())[:10]}")
    
    # Create dataset and dataloader
    dataset = SimpleBERTDataset(sentences, vocab, max_len=64, mask_prob=0.15)
    dataloader = DataLoader(dataset, batch_size=8, shuffle=True)
    
    print(f"Dataset size: {len(dataset)}")
    
    # Create BERT model (small version for demo)
    model = BERTForPreTraining(
        vocab_size=len(vocab),
        d_model=256,
        num_heads=4,
        d_ff=1024,
        num_layers=4,
        max_len=64
    )
    
    # Train the model
    print("\n=== Training BERT ===")
    train_losses, mlm_losses, nsp_losses = train_bert(model, dataloader, num_epochs=3)
    
    # Plot training curves
    plt.figure(figsize=(15, 5))
    
    plt.subplot(1, 3, 1)
    plt.plot(train_losses, 'b-', linewidth=2)
    plt.title('Total Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.subplot(1, 3, 2)
    plt.plot(mlm_losses, 'g-', linewidth=2)
    plt.title('Masked Language Model Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.subplot(1, 3, 3)
    plt.plot(nsp_losses, 'r-', linewidth=2)
    plt.title('Next Sentence Prediction Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Demonstrate MLM
    print("\n=== Masked Language Modeling Demo ===")
    demonstrate_masked_language_modeling(model, vocab, "the cat sat on the [MASK]")
    demonstrate_masked_language_modeling(model, vocab, "birds can [MASK] in the sky")
    
    # Visualize attention patterns
    print("\n=== Attention Visualization ===")
    visualize_attention_patterns(model, vocab, "the cat sat on the mat")
    
    # Compare model sizes
    compare_bert_sizes()
    
    print("\n=== BERT Key Innovations ===")
    print("1. BIDIRECTIONAL CONTEXT:")
    print("   - Unlike GPT (left-to-right) or ELMo (concatenated)")
    print("   - Self-attention allows looking at entire sequence")
    print("   - Better understanding of word meanings in context")
    
    print("\n2. MASKED LANGUAGE MODELING (MLM):")
    print("   - Randomly mask 15% of tokens")
    print("   - Predict masked tokens using bidirectional context")
    print("   - 80% [MASK], 10% random token, 10% original")
    print("   - Forces model to learn representations of all positions")
    
    print("\n3. NEXT SENTENCE PREDICTION (NSP):")
    print("   - Binary classification: sentence B follows sentence A?")
    print("   - Helps with downstream tasks requiring sentence relationships")
    print("   - Uses [CLS] token representation for classification")
    
    print("\n4. THREE TYPES OF EMBEDDINGS:")
    print("   - Token embeddings: vocabulary representation")
    print("   - Position embeddings: learned positional encoding")
    print("   - Segment embeddings: sentence A vs sentence B")
    
    print("\n=== BERT Architecture Details ===")
    print("• BERT-Base: 12 layers, 768 hidden, 12 heads, 110M parameters")
    print("• BERT-Large: 24 layers, 1024 hidden, 16 heads, 340M parameters")
    print("• Input: [CLS] sentence_A [SEP] sentence_B [SEP]")
    print("• Special tokens: [CLS] classification, [SEP] separator, [MASK] masking")
    print("• Layer normalization applied before sub-layers (Pre-LN)")
    
    print("\n=== BERT vs Other Models ===")
    print("BERT vs ELMo:")
    print("• BERT: Deep bidirectional, Transformer-based")
    print("• ELMo: Shallow bidirectional, LSTM-based")
    
    print("\nBERT vs GPT:")
    print("• BERT: Bidirectional, MLM pretraining, encoder-only")
    print("• GPT: Unidirectional, autoregressive, decoder-only")
    
    print("\n=== Applications & Fine-tuning ===")
    print("DOWNSTREAM TASKS:")
    print("• Text classification: Add classification head to [CLS]")
    print("• Named Entity Recognition: Token-level classification")
    print("• Question Answering: Predict start/end positions")
    print("• Sentiment Analysis: Sentence-level classification")
    print("• Text similarity: Compare [CLS] representations")
    
    print("\nFINE-TUNING PROCESS:")
    print("1. Load pretrained BERT weights")
    print("2. Add task-specific head(s)")
    print("3. Fine-tune entire model on target task")
    print("4. Use lower learning rates for BERT layers")
    
    print("\n=== BERT Impact ===")
    print("✓ Achieved state-of-the-art on 11 NLP tasks")
    print("✓ Showed importance of bidirectional pretraining")
    print("✓ Popularized transfer learning in NLP")
    print("✓ Inspired many variants (RoBERTa, ALBERT, DeBERTa)")
    print("✓ Foundation for modern NLP systems")
    
    print("\n=== BERT Limitations ===")
    print("❌ Computationally expensive")
    print("❌ Fixed maximum sequence length")
    print("❌ NSP task may not be necessary")
    print("❌ Discrepancy between pretraining and fine-tuning")
    print("❌ Limited to understanding, not generation")