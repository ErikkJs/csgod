%Computation for minimum value of two & three numbers
min(X,Y,Z) :- X < Y, Z is X.
min(X,Y,Z) :- Y < X, Z is Y.
min(X,Y,Z,Q) :- X < Y, X < Z, Q is X.
min(X,Y,Z,Q) :- Y < X, Y < Z, Q is Y.
min(X,Y,Z,Q) :- Z < X, Z < Y, Q is Z.

%Facts & Rules
fact(a).
fact(b).
fact(c).
fact(d).
fact(e).
fact(m).
rule(z) :- rule(y), fact(d).
rule(y) :- rule(x), fact(b).
rule(x) :- fact(a).
rule(l) :- fact(c).
rule(n) :- rule(l), fact(m).

%Certainty Factors
exist(a,CF) :- fact(a) -> CF is 1.00.
exist(b,CF) :- fact(b) -> CF is 0.70.
exist(c,CF) :- fact(c) -> CF is 0.75.
exist(d,CF) :- fact(d) -> CF is 0.80.
exist(e,CF) :- fact(e) -> CF is 0.50.
exist(m,CF) :- fact(m) -> CF is -1.0.
exist(z,CF) :- rule(z) -> exist(y,A), exist(d,B), min(A,B,C), CF is C*0.70.
exist(y,CF) :- rule(y) -> exist(x,A), exist(b,B), exist(e,C), min(A,B,C,D), CF is D*0.95.
exist(x,CF) :- rule(x) -> exist(a,A), CF is A*1.00.
exist(l,CF) :- rule(l) -> exist(c,A), CF is A*0.85.
exist(n,CF) :- rule(n) -> exist(l,A), exist(m,B), min(A,B,C), CF is C*1.00.

