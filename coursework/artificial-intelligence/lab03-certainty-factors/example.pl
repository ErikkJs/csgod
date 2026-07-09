/* There are two ways to write comments in prolog. This is a multi-line comment. */
% Single line comments start with a percent symbol and terminate at EOL.

% By convention facts are placed at the top of the source file.
% The car is gray.
car(gray).
% Some item, we'll call 'a' is 'orange'
color(a,orange).
% Some other item we'll call 'b' is 'tan'
color(b,tan).
% ... both objects are 'round'
shape(a,round).
shape(b,round).
% ... whatever 'b' was, I ate it.
ate(albert,b).
ate(jane,salad).

% Rules follow the facts.
% An object is a hamburger if it is tan and round.
hamburger(X) :- color(X,tan), shape(X,round).
% An object is an orange if it is orange and round.
orange(X) :- color(X,orange), shape(X,round).
% I don't know about you, but a hamburger is pretty filling...
full(X) :- ate(X, Y), hamburger(Y).
