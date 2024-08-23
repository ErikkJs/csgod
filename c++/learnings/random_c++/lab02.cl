

;;Printing the dots based on variable X
(defun printDots(x)
  
    (loop for i from 1 to x do 
        (format t ".")
        )

)

;; Prints the dots recursivly
(defun printDotsRe(x)
    (if (> x 0) (format t ".") )
    ;; calls recursive
    (if (> x 0) (printDotsRe (- x 1)))
)

;; counts the number of "a" that occur 
(defun amount(x)
    (if (null x)
        0
        (if (eql (car x) 'a)
        (+ 1 (amount (cdr x)))
        (amount (cdr x))))
)
    
    
;; fibonacci 
(defun fib(n)
    (cond
        ((= n 0) 0)
        ((= n 1) 1)
        ((> n 1)(+ (fib (- n 1)) (fib (- n 2))))
    )
)

;; returns the greater of two
(defun greater (x y)
    (if (> x y) x y)
)

;; adds recursivly (summation)
(defun summation (n)
    (if (= n 1)
    1
    (+ n (summation(- n 1))))

)



(defun test ()
    (format t "CMPS 3500 Lab2 Name: Erik Soto or santa claus himself~%")
    (format t "01. 3 iterative dots: ") (printDots 3) (format t "~%")
    (format t "02. 3 recursive dots: ") (printDotsRe 3) (format t "~%")
    (format t "03. a's in (b c (a) a): ~D~%" (amount '(b c (a) a)))
    (format t "04. fib 8 = ~D~%" (fib 8)) 
    (format t "05. greater 9 or 2? ~D~%" (greater 9 2)) 
    (format t "06. 1+2+3+4+5+6 = ~D~%" (summation 6) ) 

)
     
(test)

