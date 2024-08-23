

%fax or fiction
item(a).
item(b).
item(c).
item(d).
item(e).

% y+ d -> z 
%z(X) :- y(X,true) , d(X,true).
item(z) :- item(d), item(y).

% x+b+e -> y
%y(X) :- x(X,true) , b(X,true) , e(X,true).
item(y) :- item(x), item(b).

% A -> x
%x(X) :- a(X,true).  
item(x) :- item(a).

% C -> L 
%l(X) :- c(X,true) .
item(l) :- item(c).
% L + M -> n
%n(x) :- l(X,true), m(X,true).
item(n) :- item(l), item(m).


