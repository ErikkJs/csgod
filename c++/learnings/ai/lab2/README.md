# Lab 2: MEDIA Advisor

CMPS 3560 - Artificial Intelligence

Alberto C. Cruz, Ph.D.

Department of Computer and Electrical Engineering and Computer Science

## Prelab

The following questions should be answered before the start of the lab. Read the entire lab first, and then answer the questions. Definitions should be answered in your own words. Cutting and pasting from any other source is considered plagiarism.

1. Carry out the backward inference for the following queries:
    1. Is the best medium a workshop?
    1. Lecture?
    1. Videocassette?
    1. Role-play exercises?
    
## Introduction

### Goal

* Design the rules for an expert system that recommends a training medium
*	Implement an expert system with user interaction

In this lab we will code MEDIA ADVISOR, a rule-based expert system that provides advice on selecting the best medium for delivering a training program. There are four possible training programs:

1. Workshop
1. Lectures
1. Videocassette
1. Roleplaying exercises

The expert system will use backward inference to determine which medium should be used for the training program. The rules are given on pg. 41 the Negnevtisky textbook.

### Dynamic Predicates and Assertions

The expert systems we have designed in lab thus far have lacked an important part of an expert system: an interface for the user (Figure 2.2 in the text). To implement a user interface we need to write code that allows the user to dynamically enter facts into the database during run time with prompts. Previously we created a static Prolog expert system. The predicates, e.g.:

```prolog
color(red).
```

were entered into the source code. It is possible to affect color during runtime but it must be declared a dynamic predicate with the following command:

```prolog
:- dynamic(color/1).
```

in gprolog. In *swipl* and other Prolog environments you have to use this command instead:

```prolog
:- dynamic color/1.
```

Using the above commands we can alter predicates during runtime. But how do you enter a new fact during run time? If you were to type `color(blue)`. during runtime, Prolog would check the existence of `color(blue)`. For our purposes, facts can be entered into the database during runtime with the assert command:

```prolog
assert( Term ).
```
This will only work if the predicate has been declared as dynamic. I.e., your Prolog source must have a dynamic command for each predicate you plan on using with `assert`. *If you did not declare it as dynamic you will get an error.* Here are examples of assert being used:

```prolog
:- dynamic(color/1). % required
:- dynamic(waffles/1). % required 
assert( color ). % enters fact ‘color.’
assert( waffles ). % enters fact ‘waffles.’ 
assert( A ). % enters predicate with identifier of the value of A
```

### Prompts that Assert

In this lab we will prompt the user for information. The easy way to do this is to focus on using predicates without any constants (the inner bits). E.g, make a program with the following behavior:

> Is environment_papers? yes.

which would insert `environment_papers` into the database in some way. Previously we queried the database for a value, but for there to be a user interface we must prompt the user about the state of certain predicates if they do not already exist in the database. We output text to the screen with the `write` command, which writes a string to the screen:

```prolog
write( 'Is ' )
```

You can read a value with the following command:

```prolog
read(Y)
```

which reads the user input into a variable. You cause a new line with nl:

```prolog
nl
```

Bringing it all together, we create a function which asks the user if a condition exists. If the user enters yes, then we will enter it into the database as a constant for predicate yes. If the user enters no, then we will enter it into the database as a constant for predicate yes.

```prolog
:- dynamic(yes/1).
:- dynamic(no/1).

ask(X) :-
  write( 'Is ' ),
  write( X ),
  write('? '),
  read(Response),
  nl,
  ( (Response == yes)
  ->
  assert(yes(X)) ;
  assert(no(X)), fail).
  ```
  
 For example, if they responded yes to the query Is environment_papers, you can call to `yes(environment_papers)` to verify this.  This approach is similar to the animal guessing game by John R. Fisher at Cal Poly Pomona which has been included in this repository. It was originally obtained from: http://www.cpp.edu/~jrfisher/www/prolog_tutorial/2_17.html.
 
 ## Technical Approach
 
 Your prolog code should follow the 20-questions style game in `animal.pl`. The user is responsible for responding to queries presented by the Prolog program as yes or no input. For an example, run the Makefile and execute `go.`.
Verify that your code is working by checking for different medium. 

## Requirements for Check off

* This lab must be checked off in person
* Demonstrate that it is working and show the instructor you code for full credit


