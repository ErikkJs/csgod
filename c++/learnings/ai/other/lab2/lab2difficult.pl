/*  lab2.pl
    Media Advisor: A Demonstration Rule-Based Expert System.

    start with ?- go.   */

go :-   checkmedium(Medium),
        write('I guess that the Medium is: '),
        write(Medium).

/* hypotheses to be tested */
checkmedium(lecture)        :- lecture, !.
checkmedium(workshop)       :- workshop, !.
checkmedium(videocassette)  :- videocassette, !.
checkmedium(role_play)      :- role_play, !.
checkmedium(unknown).       %no diagnosis

/* dynamic assertions */
:- dynamic environment/1.          %required
:- dynamic job/1.                  %required
:- dynamic stimulus_response/1.    %required
:- dynamic stimulus_situation/1.   %required
:- dynamic feedback/1.             %required
:- dynamic yes/1,no/1.

/* Medium identification rules */
lecture         :-  verify(stimulus_response(symbolic)),
                    verify(stimulus_response(analytical)),
                    verify(feedback(required)).

lecture         :-  verify(stimulus_response(visual)),
                    verify(stimulus_response(oral)),
                    verify(feedback(required)).

lecture         :-  verify(stimulus_response(verbal)),
                    verify(stimulus_response(analytical)),
                    verify(feedback(required)).

workshop        :-  verify(stimulus_situation(physical_object)),
                    verify(stimulus_response(hands_on)),
                    verify(feedback(required)).

videocassette   :-  verify(stimulus_situation(visual)),
                    verify(stimulus_response(documented)),
                    verify(feedback(not_required)).

role_play       :-  verify(stimulus_situation(verbal)),
                    verify(stimulus_response(oral)),
                    verify(feedback(required)).

/* classification rules */


/* Questions */
ask(Question) :-
    write(': '),
    write(Question),
    write('? '),
    read(Response),
    nl,
    Term =.. [Question, Response], assert(Term).

/* verification */
verify(Truth) :-
    (yes(Truth)
    ->  true ;
    (no(Truth)
    ->  fail ;
    ask(Truth))).

/* undo all yes/no assertions */
undo :- retract(yes(_)),fail.
undo :- retract(no(_)),fail.
undo.
