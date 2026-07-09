# AI/ML Learning Repository
**Author: csgod cmps studies**

This repository contains a comprehensive collection of AI/ML concepts implemented from scratch with clear explanations and practical examples. Each file demonstrates a fundamental concept or architecture that every advanced AI engineer should understand.

## 📁 Current Implementations

### Neural Network Fundamentals (6 files)
- **01_perceptron.py** - Single layer linear classifier with visualization
- **02_multilayer_perceptron.py** - Basic feedforward neural network with backpropagation
- **04_backpropagation.py** - Detailed gradient computation and chain rule demonstration
- **05_activation_functions.py** - Comparison of ReLU, Sigmoid, Tanh, Softmax, etc.
- **06_loss_functions.py** - MSE, Cross-entropy, Huber loss analysis
- **07_optimizers.py** - SGD, Adam, RMSprop, AdaGrad comparison

### Convolutional Neural Networks (6 files)
- **03_basic_cnn.py** - Image classification with convolutional layers
- **08_lenet.py** - Classic LeNet-5 architecture implementation
- **14_resnet.py** - Revolutionary skip connections for very deep networks
- **21_alexnet.py** - Deep learning revolution with ReLU, dropout, and GPU training
- **22_vgg.py** - Very deep networks with 3×3 filters and systematic architecture
- **23_googlenet.py** - Inception modules with multi-scale feature extraction

### Recurrent Neural Networks (3 files)
- **09_vanilla_rnn.py** - Basic sequential processing with vanishing gradient demo
- **10_lstm.py** - Long Short-Term Memory with gating mechanisms
- **13_gru.py** - Gated Recurrent Units with simplified architecture

### Attention and Transformers (3 files)
- **11_attention_mechanism.py** - Bahdanau/Luong attention for seq2seq models
- **12_transformer.py** - Full transformer architecture with multi-head attention
- **20_bert.py** - Bidirectional encoder representations from transformers

### Generative Models (3 files)
- **15_autoencoder.py** - Unsupervised representation learning
- **16_vae.py** - Variational autoencoders with probabilistic latents
- **17_gan.py** - Generative adversarial networks with discriminator competition

### Specialized Architectures (2 files)
- **18_unet.py** - Encoder-decoder for semantic segmentation
- **19_q_learning.py** - Tabular reinforcement learning with exploration/exploitation

## 🎯 Key Features

Each implementation includes:
- ✅ **Clear documentation** explaining the concept
- ✅ **From-scratch implementations** for educational understanding
- ✅ **Practical examples** with real datasets
- ✅ **Visualizations** to understand the concepts
- ✅ **Comparative analysis** between different approaches
- ✅ **End-to-end training loops** with evaluation

## 📊 What You'll Learn

### Mathematical Foundations
- Gradient computation and backpropagation
- Optimization algorithms and their trade-offs
- Loss functions and their applications
- Activation functions and their properties

### Architecture Understanding
- CNN feature extraction and spatial processing
- RNN temporal modeling and sequence processing  
- Attention mechanisms and information bottlenecks
- Transformer parallelization and self-attention

### Practical Skills
- Model training and evaluation loops
- Hyperparameter tuning strategies
- Gradient clipping and stability techniques
- Visualization and interpretability methods

## 🆕 Latest Additions

### Recently Added CNN Architectures (Files 21-23)
- **21_alexnet.py** - The deep learning revolution! AlexNet introduced ReLU activation, dropout regularization, and showed the power of deep CNNs
- **22_vgg.py** - Systematic architecture design with very small (3×3) filters, proving that depth matters more than filter size
- **23_googlenet.py** - Inception modules for efficient multi-scale feature extraction with dramatically fewer parameters

## 🚀 Usage

Each file is self-contained and can be run independently:

```python
# Example: Run the transformer implementation
python ai-learnings/12_transformer.py

# Try the new CNN architectures
python ai-learnings/21_alexnet.py  # Deep learning revolution
python ai-learnings/22_vgg.py      # Systematic deep networks
python ai-learnings/23_googlenet.py # Efficient inception modules
```

Most implementations work with both synthetic data (for quick demonstration) and real datasets (for practical learning).

## 📈 Implementation Quality

- **Educational Focus**: Code is written for clarity and understanding
- **Complete Examples**: Each concept includes full training/evaluation pipelines
- **Comparative Analysis**: Multiple approaches are compared side-by-side
- **Visualization**: Extensive plots and visualizations for intuition
- **Best Practices**: Modern techniques like gradient clipping, batch normalization

## 🔬 Advanced Concepts Covered

### Optimization Techniques
- Momentum and adaptive learning rates
- Gradient clipping for stability
- Learning rate scheduling
- Weight initialization strategies

### Regularization Methods
- Dropout for preventing overfitting
- Batch normalization for stable training
- Early stopping strategies
- Data augmentation techniques

### Architecture Insights
- Information bottlenecks in seq2seq models
- Vanishing gradient problems and solutions
- Attention as a solution to fixed-size representations
- Multi-head attention for diverse relationship modeling

## 📚 Remaining Concepts to Implement

The full planned curriculum includes 66 concepts total. Here are the major categories still to be implemented:

### Advanced CNN Architectures
- ~~AlexNet~~, ResNet variants, U-Net variants, YOLO, Siamese Networks, DenseNet, EfficientNet

### Advanced RNN Variants  
- GRU, Bidirectional RNN, Seq2Seq, Encoder-Decoder

### Transformer Variants  
- Self-Attention, Multi-Head Attention, ~~BERT~~, GPT, Vision Transformer

### Generative Models
- ~~Autoencoder~~, ~~VAE~~, ~~GAN~~, DCGAN, CycleGAN, Diffusion Models

### Reinforcement Learning
- ~~Q-Learning~~, DQN, Policy Gradient, Actor-Critic, PPO

### Specialized Architectures
- Graph Neural Networks, Capsule Networks, Memory Networks

### Meta-Learning & Few-Shot
- MAML, Prototypical Networks, Knowledge Distillation

### Classical ML (for comparison)
- Linear/Logistic Regression, SVM, Random Forest, K-Means, PCA

## 🎓 Learning Path Recommendations

### Beginner Path
1. Start with **01_perceptron.py** for basic concepts
2. Understand **04_backpropagation.py** for training fundamentals
3. Explore **05_activation_functions.py** and **06_loss_functions.py**
4. Move to **02_multilayer_perceptron.py** for deeper networks

### Intermediate Path
1. Learn CNNs with **03_basic_cnn.py** and **08_lenet.py**
2. Understand sequences with **09_vanilla_rnn.py** and **10_lstm.py**
3. Master optimization with **07_optimizers.py**

### Advanced Path
1. Study attention with **11_attention_mechanism.py**
2. Master modern architectures with **12_transformer.py**
3. Implement remaining advanced concepts

## 💡 Tips for Maximum Learning

1. **Read the Code**: Each implementation is heavily commented
2. **Run the Examples**: Execute each file to see results
3. **Modify Parameters**: Experiment with different hyperparameters
4. **Visualize Results**: Pay attention to the plots and visualizations
5. **Compare Approaches**: Notice differences between similar concepts

## 🔧 Dependencies

```bash
pip install torch torchvision numpy matplotlib scikit-learn seaborn
```

## 📊 Repository Statistics

- **Current Files**: 24 implementations (23 concepts + README)  
- **Lines of Code**: ~20,000+ lines of educational Python
- **Concepts Covered**: Fundamentals → Advanced Architectures → Modern AI
- **Visualizations**: 200+ plots and analyses for deep understanding
- **Progress**: 23 out of 66 planned concepts (35% complete!)

---

This repository represents a comprehensive, hands-on approach to understanding AI/ML from first principles. Each implementation is designed to build intuition and practical skills that transfer to real-world applications.