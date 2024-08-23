/* animal.pro
  animal identification game.  

    start with ?- go.     */

go :- checkmedium(Medium),
      write('I guess that the Medium is: '),
      write(Medium).

/* hypotheses to be tested */
checkmedium(lecture)   :- lecture, !.
checkmedium(unknown).             /* no diagnosis */

/* Medium identification rules */
lecture :- stimulus_situation_symbolic, 
           stimulus_response_analytical, 
           verify(feedback_required).

/* classification rules */
stimulus_situation_symbolic    :- verify(environment_numbers), !.
stimulus_situation_symbolic    :- verify(environment_formulas), !.
stimulus_situation_symbolic    :- verify(environment_computer_programs).

stimulus_response_analytical	:- verify(job_evaluating), !.
stimulus_response_analytical	:- verify(job_reasoning), !.
stimulus_response_analytical	:- verify(job_investigating).

/* how to ask questions */
ask(Question) :-
    write('Does the Medium have the following attribute: '),
    write(Question),
    write('? '),
    read(Response),
    nl,
    ( (Response == yes ; Response == y)
      ->
       assert(yes(Question)) ;
       assert(no(Question)), fail).

:- dynamic yes/1,no/1.

/* How to verify something */
verify(S) :-
   (yes(S) 
    ->
    true ;
    (no(S)
     ->
     fail ;
     ask(S))).

/* undo all yes/no assertions */
undo :- retract(yes(_)),fail. 
undo :- retract(no(_)),fail.
undo.