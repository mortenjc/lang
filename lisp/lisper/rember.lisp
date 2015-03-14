;;;; Remove first occurance of a from l
(defun rember (a l)
   (cond ((null l)  '())
         ((eq (car l) a) (cdr l))
         ( t  (cons (car l) 
                    (rember a (cdr l))))))

;;;; Remove all occurances of a from l
(defun remall (a l)
   (cond ((null l)  '())
         ((eq (car l) a) (remall a (cdr l)))
         ( t  (cons (car l) 
                    (remall a (cdr l))))))

;;;; Removes all occurences of any element of l1 from l2
(defun remall* (l1 l2)
   (cond ((null l1) l2)
       ((member (car l1) l2) 
            (remall* (remall (car l1) l1) (remall (car l1) l2)))))

;;;; Is set l1 a member of set l2?
(defun member? (l1 l2)
   (cond ((null l1) 't)
   ((member (car l1) l2) (member? (cdr l1) l2))
   ( t nil)))

;;;; Is the set l1 a subset of set l2?
(defun subset? ( l1 l2 )
   (cond ((null l1) 't)
         ((and (member? (car l1) l2) (subset? (cdr l1) l2)))))

;;;; Make a set out of l1 by removing duplicates
(defun makeset (l1)
   (cond ((null l1) '())
         ((member (car l1) (cdr l1)) (makeset (cdr l1)))
         ( t (cons (car l1) (makeset (cdr l1))))))

(defun mylist (l1 l2 n1)
   (cond ((null l1) l2) 
         ( t (mylist (cdr l1) (append l2 (list (append '(place-fret) (car l1) (list (1+ (mod (list-length l2)n1)))))) n1)
         )))

(defun myfun ()
(lambda (l1 l2 n1)
   (cond ((null l1) l2) 
         ( t (mylist (cdr l1) (append l2 (list (append '(place-fret) (car l1) (list (1+ (mod (list-length l2)n1)))))) n1)
         )) a b 2))


(defparameter l1 '(here))
(defparameter l2 '(here and list))
(defparameter l3 '(here is a pie and a longer list))
(defparameter l4 '((5 3) (5 2) (5 1) (4 1) (4 3) (4 5) (3 1) (3 3) (2 1) (2 5)))
