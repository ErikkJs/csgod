
item(a,1.0).
item(b,0.7).
item(c,0.75).
item(d,0.8).
item(e,0.5).
item(m,-1.0).

item(z,CF) :- item(y,Y), item(d,D), min(Y,D,R), CF is R * 0.7.
item(y,CF) :- item(x,X), item(b,B), item(e,E), min(X,B,E,R), CF is R* 0.95.
item(x,CF) :- item(a,CF).
item(l,CF) :- item(c,R), CF is R * 0.85.
item(n,CF) :- item(l,L), item(m,M), min(L,M,R), CF is R *1.0 .


min(X,Y,X):- X<Y, !.  
min(_,Y,Y).

min(X,Y,Z,Y):- X<Y,X<Z ,!.
min(X,Y,Z,Y):- Y<X,Y<Z ,!.
min(X,Y,Z,Y):- Z<Y,Z<X.

