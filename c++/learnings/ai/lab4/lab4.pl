
%Membership functions
membership( short, X, 1 ) :-    X < 160, !.
membership( short, X, A ) :-    X < 170, A is -X/10 + 17.0, !.
membership( short, _, 0 ).

membership( average, X, 0 ) :-  X < 165;
                                X >= 185, !.
membership( average, X, B ) :-  X < 175, B is X/10 - 16.5;
                                X >=175, B is -X/10 + 18.5.

membership( tall, X, 0 ) :-     X < 180, !.
membership( tall, X, G ) :-     X >= 180, X < 190, G is X/10 - 18.0, !.
membership( tall, _, 1 ).

%Hedge functions
little( short, X, Z ) :-    membership( short, X, Y), Z is Y**1.3.
little( average, X, Z ) :-  membership( average, X, Y), Z is Y**1.3.
little( tall, X, Z ) :-     membership( tall, X, Y), Z is Y**1.3.

slightly( short, X, Z ) :-   membership( short, X, Y), Z is Y**1.7.
slightly( average, X, Z ) :- membership( average, X, Y), Z is Y**1.7.
slightly( tall, X, Z ) :-    membership( tall, X, Y), Z is Y**1.7.

very( short, X, Z ) :-      membership( short, X, Y), Z is Y**2.
very( average, X, Z ) :-    membership( average, X, Y), Z is Y**2.
very( tall, X, Z ) :-       membership( tall, X, Y), Z is Y**2.

extremely( short, X, Z ) :-     membership( short, X, Y), Z is Y**3.
extremely( average, X, Z ) :-   membership( average, X, Y), Z is Y**3.
extremely( tall, X, Z ) :-      membership( tall, X, Y), Z is Y**3.
