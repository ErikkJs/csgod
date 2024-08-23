# Lab 0: A simple reflex agent and performance metrics

CMPS 3560 - Artificial Intelligence

Alberto C. Cruz, Ph.D.

Department of Computer and Electrical Engineering and Computer Science

# Prelab

The following questions should be answered before the start of the lab. Read the entire lab first, and then answer the questions. Definitions should be answered in your own words. Cutting and pasting from any other source is considered plagiarism.

1. What is the difference between ground truth and a predicted label?
1. The rule given with this code is as follows: `IF 'sepal width' > 3.0 THEN 'prediction' is 'I. Versicolor'` Using this rule, what true positive rate do you expect? What accuracy?

# Introduction

## Goal

* Develop a program that implements a simple reflex agent
* Understand sensitivity, specificity, precision and accuracy

## Required Files

* This repository contains the lab manual and C++ code, but you are not required to use the code

## Background


In today’s lab, we will make a simple reflex agent that predicts the species of flower. The simple reflect agent uses an IF-THEN rule like an expert system. We will distinguish between flower species of Iris Virginica and Iris Virginica. The procedure follows:

1. Read in data measurements of 100 flowers from a comma separated value file (CSV)
1. Improve a simple reflex agent
1. Calculate the specificity, precision and accuracy

You must complete the lab with a partner. Collaboration with your lab partner is okay. Collaboration with other groups is also okay. Yet, sharing of code between groups is not okay and is cheating. I will assign a single grade to each group. If disputes arise you may switch partners. But, you must notify the instructor of any changes before any the start of the next lab. This repository contains base C++ code but you are not required to use it. I am familiar with C, C++, Java, MATLAB, Prolog, and Javascript. Refer to the syllabus for grading policies.

## Reflex agents

A reflex agent is an AI agent that acts only on the current stimulus. Reflex agents follow IF condition THEN action rules. This is like rule-based expert systems, except those systems have inference. Rule-based expert systems are discussed in Chapter 2. The reflex agent for today carries out a prediction using an IF-THEN rule. It does not use inference, consider the history of the data, or learn. An example: `IF 'fruit' is 'red' THEN 'fruit' is 'apple'`.  

As it turns out, there are red fruits out there that are not apples. Our agent can be wrong. This is an important concept. So, to measure the performance of our system, we will test it with many samples.

A sample consists of some data of the object. In the case of our apple, some relevant features are the weight, and color. It also has an associated correct prediction. For example:

1. 6 oz, yellow ... Bannana
1. 3 oz, red ... Strawberry

## Performance metrics

We will test the agent sample by sample, noting if it was correct. Noting the number of failures is absolute, and will vary based on how many samples you test on. A better metric is true positive rate, also known as sensitivity. Let's say we're trying to correctly predict the apples. Then the sensitivity is:

> ( True positives ) / ( Number of positives ) = ( Number of samples that were correctly predicted as an apple ) / ( Total number of apples )

So, for example, if the dataset contained a yellow apple, the reflex agent would fail, and the number of samples that were predicted as an apple are less than the total number of apples in the dataset. Other metrics are the specificity:

> ( True negatives ) / ( Number of negatives ) = ( Number of samples that were correctly NOT predicted as an apple ) / ( Total number of non-apples )

The true positive rate only measures how well we do for apples. You could have a system that just predicted apples 100%, but this would not be useful. It would cause a lot of false alarms (instances where you labelled something an apple but it was not). Hence, we are interested in the true negative rate, also known as specificity. Another important absolute metric is false alarms. This is the number of non-apples that were labelled an apple. It is used in the metric precision:

( True positives ) / ( Number of true positives + Number of false positives )

Bringing it all together, perhaps you want to know the overall performance of the system (how it performances for predicting both apples and non-apples. You would use the following equation, describing the accuracy of the system:

( True positives + True negatives ) / ( Number of samples )

## Iris dataset

Today we will predict the species of a flower. The full data set can be found at the UCI Machine Learning Repository: http://archive.ics.uci.edu/ml/datasets/Iris. Do not download the full data set. Use only the subset that is given with this lab. The data file is a comma-separated data file. Each column is described as follows:

1. Sepal length
1. Sepal width
1. Petal length
1. Petal width
1. Species (-1 for Iris Setosa, 1 for Iris Virginica)

Each row is a different measurement taken from a flower. Each measurement is called a sample. The first column is the petal length in cm and the second row is the species of the sample. The membership of each sample is called a label. Consider the following set of values:

> { 5.2, 3.5, 1.4, 0.2, -1 }

This sample has a sepal length of 5.2 cm, a sepal width of 3.5 cm, a petal length of 1.4 cm and a petal width of 0.2 cm. It has the label -1 corresponding to Iris Setosa. The correct label is referred to as the ground truth. `lab0.cpp` contains the following rule:

> IF 'sepal width' >= 3.0 THEN 'prediction' is '1'

You can see for yourself looking at the data that this is a very poor rule.

## Technical Approach

### Part 1: Improve the agent

The agent should have a very poor performance. A make file has been provided, and if you execute `make test`, it will run the program on the dataset using the given rule. It should report that you only had 4 true positives. Of the 50 positive samples, the agent was correct only 4 time. It is your job to improve the performance of the system. Come up with a rule on your own. It is possible to achieve 100%.

### Part 2: Performance metrics

As it stands, the program only reports the number of true positives and the sensitivity. Write code to implement the specificity, precision and accuracy.

## Requirements for check off

* This lab must be checked off in person
* Achieve at least 70% true positive rate
* Show the instructor your code, and the values for specificity, precision and accuracy
