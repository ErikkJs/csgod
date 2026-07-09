go :- hypothesize(Animal),
      write('I guess that the medium is: '),
      write(Animal),
      nl,
      undo.

/* hypotheses to be tested */
hypothesize(lecture)   :- lecture, !.
hypothesize(videocassette) :- videocassette, !.
hypothesize(workshop)   :- workshop, !.
hypothesize(roleplaying_exercises)  :- roleplaying_exercises, !.
hypothesize(unknown).             /* no diagnosis */

/* animal identification rules */

workshop :- physical_object,
            handson,
            verify(feedback).

lecture :-  symbolic,
            analytic,
            verify(feedback),!.

lecture :- visual,
            oral,
            verify(feedback),!.

lecture :- verbal,
            analytic,
            verify(feedback).

videocassette :- visual,
                 documented,
                 verify(no_feedback).

roleplaying_exercises :- verbal,
                         oral,
                        verify(feedback).


/* classification rules */

verbal :- verify(papers),!.
verbal :- verify(manuals),!.
verbal :- verify(documents),!.
verbal :- verify(textbooks).

visual :- verify(diagrams),!.
visual :- verify(photographs),!.
visual :- verify(illustrations),!.
visual :- verify(pictures).

physical_object :- verify(machines),!.
physical_object :- verify(tools),!.
physical_object :- verify(buildings).

symbolic :- verify(numbers),!.
symbolic :- verify(forumlas),!.
symbolic:- verify(computer programs).

oral :- verify(lecturing),!.
oral :- verify(advising),!.
oral:- verify(counselling).

handson :- verify(building),!.
handson :- verify(repairing),!.
handson :- verify(troubleshooting).

documented :- verify(writing),!.
documented :- verify(typing),!.
documented :- verify(drawing).

analytic :- verify(evaluating),!.
analytic :- verify(reasoning),!.
analytic :- verify(investigating).


/* how to ask questions */
ask(Question) :-
    write('Does the medium have the following attribute: '),
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
