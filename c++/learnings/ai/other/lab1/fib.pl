fib(X,Y) :- X==0, Y is 0.
fib(X,Y) :- X==1, Y is 1.
fib(X,Y) :- X > 1,
            C is X-1, fib(C,A),
            D is X-2, fib(D,B),
            Y is  A+B.
