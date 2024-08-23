% comment the next two lines out when testing - put back in to submit
:- initialization(q1).
:- initialization(q2).
:- initialization(q3).
:- initialization(end).

% FACTS. 
mammal(kitty).
mammal(ratatat).
mammal(fido).
claws(kitty).
tail(ratatat).
bestfriend(fido).
feathers(tweety).

% 9 RULES. 
% If X is a mammal then X has fur. 
fur(X) :- mammal(X).

% If X has fur and X has a tail then X is a rat. 
rat(X) :- fur(X), tail(X).

% If X has claws and X has fur then X is a cat.
cat(X) :- claws(X), fur(X).

% If X is a cat then X meows.
meows(X) :- cat(X).

% If X has feathers then X is a bird.
bird(X) :- feathers(X).

% If X is a bestfriend and X has fur then X is a dog.
dog(X) :- bestfriend(X), fur(X).

% If X is a dog and Y meows then X likes Y.
likes(X, Y) :- dog(X), meows(Y).

% If X is a cat and Y is a bird then X likes Y.
likes(X, Y) :- cat(X), bird(Y).

% If X is a cat and Y is a rat then X likes Y.
likes(X, Y) :- cat(X), rat(Y).

% Part B-----
max2(X, Y, R) :-
    X < Y -> R = Y;
    R = X.

max3(X, Y, Z, R) :-
    max2(X,Y,Rx),
    max2(Rx, Z, R).

moves(N, R) :-
    R is round(2**N - 1).
    




writeln(T) :- write(T), nl.
% QUERY. Uncomment after you have written all predicates.

q1 :- writeln('Who likes whom? '),
      findall(X, (likes(X,Y), format('~w ~w ~n',[X,Y])),_).
q2 :- write('Move cnt for 5: '),
      findall(X, (moves(5,X), format('~w', [X])),_).
q3 :- nl, write('Max(7,3,9)? '),
      findall(X, (max3(7,3,9,X), format('~w ~n', [X])),_).

end :- writeln('done.'), halt.
