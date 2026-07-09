# Lab 1: Backward Chaining and Tracing in Prolog

CMPS 3560 - Artificial Intelligence

Alberto C. Cruz, Ph.D.

Department of Computer and Electrical Engineering and Computer Science

## Prelab

Prelabs are short homework assignments that must be completed before the start of lab. Refer to the syllabus for procedures about how the prelabs are checked off. Consider the following rules:

* IF A AND B THEN L
* IF L AND B THEN M
* IF L AND M THEN P
* IF A AND P THEN L
* IF P THEN Q

1. Give a graph of the inference chain.
1. If A and B are fact, carry out backward chaining for the query: Q.

## Introduction

### Goals

* Know how to use the Prolog interpreter gprolog or swiprolog
*	Encode facts and rules in Prolog source code
*	Invoke queries to a Prolog inference engine
*	Trace a Prolog program to understand backward chaining

### Required Files

* example.pl in this repository

### Recommended Reading

* Prolog slides from Moodle
* http://www.cse.unsw.edu.au/~billw/dictionaries/prolog/tracing.html 

### Background

Prolog is a declarative language first created in 1972 by Alain Colmerauer. A Prolog program consists of a set of statements called procedures. Statements are either a fact or a rule, similar to the description of rule-based expert systems in the text. Prolog it literally first order logic with a few exceptions. For this reason, Prolog lends itself well to implementing rule-based expert systems and will be our focus for the next three weeks. A user interacts with the Prolog interface, and the inference engine operates under the hood determining if the statement posited by the user is true or false. The only statements that are consulted, or executed, are statements that are goals or sub goals of the user’s statement. Thus, ordering of different rules in your source code does not effect when they are executed. However, multiple declarations of a rule with the same name must be grouped together, and rules with the same name are visited from top to bottom (more on that later). Each statement contains one or more clauses within the procedure which are ordered. The Prolog IDE we will use in class is gprolog , which is already on sleipnir. You can bring up gprolog by executing `gprolog` while in the server.

When you want to quit prolog execute the following command: `halt.`. Note the period. The period terminates a gprolog procedure. Your code must be entered in a text file separately and then loaded while running gprolog using the following command: `?- consult( 'example.pl' ).`, which will load factorial.pl into gprolog.

### Facts

Open up vi, emacs, nano etc. and take a look at `example.pl`. As stated previously, you must write your code in a source file and then execute it later while running gprolog. Prolog code contains facts and rules. The following syntax is used to enter facts, also known as axioms and assertions:

> car(gray).

This statement enters the fact ‘car’ is ‘gray.’ The following fact has multiple clauses: 

> color(a, orange).

This statement would be read as some object ‘a’ has the ‘color’ ‘orange.’ By convention, we will place the operator after the first clause. There are a few more facts. Try to understand them before moving on.

### Rules

Rules are encoded with the consequent first, followed by the operator `:-`, then the clauses of the antecedent separated by commas. For example:

> hamburger(X) :- color(X,tan), shape(X,round).

> full(X) :- ate(X, hamburger).
  
The first rule is interpreted as: If the 'color' of some object 'X' is 'tan' and it's 'shape' is 'round', then 'X' is a hamburger. The second rule is read as, if 'X' ate a 'hamburger' specifically, then 'X' is full. Invoking `full(X).` during runtime will check for all objects in the database that ate a hamburger. If you invoke full(jill) it will check if jill ate a hamburger. This is how you will implement queries in Prolog.

### Queries

When you are done looking at example.pl, quit your text editor and then run gprolog and consult your source code (see above for the command to do this). To check if a rule has been proven (e.g. it has fired/it is true/it is in the database) invoke the following command:

> | ?- color(a,orange).

> true.

This is an example of a ground query. We happen to know that the 'color' of 'a' is 'orange' because we explicitly gave it as a fact. Now try:

> | ?- orange(a).

> true.

This attempts to prove that ‘a’ is a 'orange’ according to the rule established in example.pl. Since `color(a,orange).` is fact, and `shape(a,round).` is fact, the rule `orange(X) :- color(X,orange), shape(X,round).` will be satisfied for `X=a`. Note that we did not enter that 'a' is an 'orange' explicitly, the Prolog inference engine is using inference to discover facts that were not explicitly given in your Prolog source file. Consider if it us a hamburger: 

> | ?- hamburger(a).

> false.

We know for a fact that ‘a’ is an ‘orange’. The first term for checking is an item is a hamburger checks if the 'color' is 'tan', which is false for 'a'. Prolog visits each clause in a left-to-right fashion, carrying out a DFS search on each clause. If a term comes up false, it halts the inference and returns false. 

A non-ground query is a query invocation with a variable in it. Recall that variables start with an uppercase letter. 

> ?- full(Y).

gprolog should respond with the following result: 

> Y = albert 

It may seem like gprolog has hung at this point, but it is actually stepping through all objects which cause `full(Y)` to evaluate to true. To step through to the next object you must enter a semicolon, your screen output will look like this: 

> Y = albert ;

> no

There were no other ground terms that satisfied `full(Y)`. Jane ate a salad, not a hamburger. This brief introduction should be all that is needed to complete the lab. For a glimpse into a more advanced Prolog program, here is an implementation of recursive factorial:

```prolog
factorial(0,1). 
factorial(N,F) :-  
   N>0,
   N1 is N-1,
   factorial(N1,F1),
   F is N * F1.
```

Note that since clauses are ordered the antecedent can be written as if we were programming in an imperative language.

### Tracing a Sentence

Read recommended reading item #2 on how to trace a Prolog program.

## Technical Approach

### Inference Example from Textbook

To write a program in Prolog you will write your code in vi or emacs and then use gprolog to execute your code. Prolog code usually has the extension .pro or .pl. The following rules were discussed in Section 2.6.1 of the text:

* IF Y AND D THEN Z
* IF X AND B AND E THEN Y
* IF A THEN X
* IF C THEN L
* IF L AND M THEN N

Enter these rules into your Prolog source code. Following the book’s example enter A, B, C, D and E as fact. Verify that all objects are fact except for N *using trace*. Be sure that you understand how backward chaining is being executed.

For check off, demonstrate to the instructor that you have implemented the example from the textbook, and execute a trace to determine if N is fact.

#### Tips

* The book example is actually in propositional logic, not first order logic. You will need to create a dummy function for each term. For example: `item(y)`.
* Do not forget to upload your source code files to Moodle.

## Requirements for Check Off

* This lab must be checked off in person
* For full credit, show the instructor your Prolog source code and demonstrate inference in Prolog
