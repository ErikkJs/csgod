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

/* Medium identification rules */
lecture         :-  stimulus_situation_symbolic,
                    stimulus_response_analytical,
                    verify(feedback(required)), !.

lecture         :-  stimulus_situation_visual,
                    stimulus_response_oral,
                    verify(feedback(required)), !.

lecture         :-  stimulus_situation_verbal,
                    stimulus_response_analytical,
                    verify(feedback(required)).

workshop        :-  stimulus_situation_physical_object,
                    stimulus_response_hands_on,
                    verify(feedback(required)).

videocassette   :-  stimulus_situation_visual,
                    stimulus_response_documented,
                    verify(feedback(not_required)).

role_play       :-  stimulus_situation_verbal,
                    stimulus_response_oral,
                    verify(feedback(required)).

/* classification rules */
stimulus_situation_verbal :-  verify(environment_papers), !;
                              verify(environment_manuals), !;
                              verify(environment_documents), !;
                              verify(environment_textbooks).

stimulus_situation_visual :-  verify(environment_pictures), !;
                              verify(environment_illustrations), !;
                              verify(environment_photographs), !;
                              verify(environment_diagrams).

stimulus_situation_physical_object :-  verify(environment_machines), !;
                                       verify(environment_buildings), !;
                                       verify(environment_tools).

stimulus_situation_symbolic :-  verify(environment_numbers), !;
                                verify(environment_formulas), !;
                                verify(environment_computer_programs).

stimulus_response_oral :-  verify(job_lecturing), !;
                           verify(job_advising), !;
                           verify(job_counselling).

stimulus_response_hands_on :-  verify(job_building), !;
                               verify(job_repairing), !;
                               verify(job_troubleshooting).

stimulus_response_documented :-  verify(job_writing), !;
                                 verify(job_typing), !;
                                 verify(job_drawing).

stimulus_response_analytical :-  verify(job_evaluating), !;
                                 verify(job_reasoning), !;
                                 verify(job_investigating).

/* Questions */
ask(Question) :-
    write('Does the Medium have a(n) '),
    write(Question),
    write('? '),
    read(Response),
    nl,
    ( (Response == yes ; Response == y)
       ->
       assert(yes(Question)) ;
       assert(no(Question)), fail).

:- dynamic yes/1,no/1.

/* verification */
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
