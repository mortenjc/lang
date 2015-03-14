
(defun fib (n)
   "Stupid recursive Fibb function"
   (cond 
      ((< n 3) '1 )
      ( t (+ (fibb(- n 1)) (fibb(- n 2))))))
          

(defun fib2 (n)
   "Tail recursive Fibb function "
   (labels ((calc-fib (n a b)
       (if (= n 0) 
           (calc-fib (- n 1) b (+ a b)))))
     (calc-fib n 0 1)))



(defun mctest (a @rest body)
  `(collect ,a ,body))


