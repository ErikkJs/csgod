;; Erik Soto
;; Lab 3


(setq *print-case* :downcase) 


;;Remove elements from a list Function
(defun myrem (x l &optional l0)
(cond (( null l)(reverse l0))
((eq x (car l))(myrem x (cdr l) l0))
(T (myrem x (cdr l) (cons (car l) l0))))
)

;;Remove recursively Function 
(defun myrec-rem (x l &optional l0)
  (cond ((null l) (reverse l0))
        ((eq x (car l)) (myrec-rem x (cdr l) l0))
        (T (myrec-rem x (cdr l) (cons (if (not (atom (car l))) 
                                        (myrec-rem x (car l)) 
                                        (car l))
                                     l0)))))
;;Flatten Function
(defun myflat (l)
  (cond ((null l) nil) 
	((atom l) (list l)) 
	( t (append (myflat (car l)) (myflat (cdr l)))))) 

;;Palindrome Function - Helper Function
(defun remove-it (l)
  (cond ((null l) nil) 
        ((null (cdr l)) nil)
        (t (cons (car l) (remove-it (cdr l)))))) 

;;Palindrome Function 
(defun mypalindro (l)
  (cond ((null l) t) 
        ((null (cdr l)) t) 
        (t (and (equal (car l) (car (last l)))
                (mypalindro (remove-it (cdr l)))))))


;;Prefix to Infix Function

(defun infixize (x l)
  (cond ((null l) nil) ; if the list is empty, then return nil
        ((atom l) nil) ; if the list is an atom, then return nil
        ; if the list has only one element, then just return the coresponding
        ; infix expression in a list
        ((null (cdr l)) (list (infix (car l)))) 
        ; otherwise, builds a list whose first element is the infix expression
        ; corresponding to the first element of l, the second element is
        ; the operator x, and the rest is just infixize applied to the cdr of l.
        (t (append (list (infix (car l)) x) (infixize x (cdr l))))))

; infix function
(defun infix (l)
  (cond ((null l) nil) ; if l is empty, then return nil
        ((atom l) l) ; if l is an atom, then return it
        ; otherwise just call infixize with the operator and the rest of the list
        (t (infixize (car l) (cdr l))))) 


(defun test() 
	 (format t "(myrem 'b '(a b c b d)) ~%~A" (myrem 'b '(a b c b d))) (format t "~%") 
	 (format t "(myrem 'b '(a c d e f)) ~%~A" (myrem 'b '(a c d e f))) (format t "~%") 
	 (format t "(myrem 'a '(b (a d) a d)) ~%~A" (myrem 'a '(b (a d) a d))) (format t "~%") 
	 (format t "(myrem 'a '()) ~%~A" (myrem 'a '())) (format t "~%") (format t "~%") 
	 (format t "(myrec-rem 'a '(a b c d)) ~%~A" (myrec-rem 'a '(a b c d))) (format t "~%")
	 (format t "(myrec-rem 'a '((a b) a (c d e a) (a a) b)) ~%~A" (myrec-rem 'a '((a b) a (c d e a) (a a) b))) (format t "~%") 
	 (format t "(myrec-rem 'a '((((a)))) ~%~A" (myrec-rem 'a '((((a)))))) (format t "~%") (format t "~%") 
	 (format t "(myflat '((a b) a(a b) a a)) ~%~A" (myflat '((a b) a(a b) a a))) (format t "~%") 
	 (format t "(myflat '((a () b (c d)))) ~%~A" (myflat '(a () b (c d)))) (format t "~%")
	 (format t "(myflat '((a (())))) ~%~A" (myflat '(a (())))) (format t "~%")
	 (format t "(myflat '(a (b c) (d e (f) ((((((g)))) h)) i) j (k) l))) ~%~A" (myflat '(a (b c) (d e (f) ((((((g)))) h)) i) j (k) l))) (format t "~%")  (format t "~%") 
	 (format t "(mypalindro '(a b b a)) ~%~A" (mypalindro '(a b b a))) (format t "~%")
	 (format t "(mypalindro '(a b c b a)) ~%~A" (mypalindro '(a b c b a))) (format t "~%")
	 (format t "(mypalindro '(a b c)) ~%~A" (mypalindro '(a b c))) (format t "~%")
	 (format t "(mypalindro '(a (d e) b (d e) a)) ~%~A" (mypalindro '(a (d e) b (d e) a))) (format t "~%")
	 (format t "(mypalindro '(a (d e) b (e d) a)) ~%~A" (mypalindro '(a (d e) b (e d) a))) (format t "~%") (format t "~%") 
	 (format t "(myinfix '(+ 2 3)) ~%~A" (infix '(+ 2 3))) (format t "~%")
	 (format t "(myinfix '(+ 2 x (* y 3))) ~%~A" (infix '(+ 2 x (* y 3)))) (format t "~%")
	 (format t "(myinfix '(+ 2 x (* y (- z 3) 4))) ~%~A" (infix '(+ 2 x (* y (- z 3) 4)))) (format t "~%")
	
) 

(test) 



