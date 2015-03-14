;;; From Paradigms of AI

(defun mappend (fn the-list)
   "Apply function to elements of a list  and return as a single list"
   (apply #'append (mapcar fn the-list)))

(defun random-elt (choices)
  (elt choices (random (length choices))))

(defun rule-lhs (rule)
   (first rule))

(defun rule-rhs (rule)
   (rest (rest rule)))


(defun rewrites (category)
   (rule-rhs (assoc category *grammar*)))

(defparameter *simple-grammar* 
  '((sentence -> (noun-phrase verb-phrase))
    (noun-phrase -> (Article Noun))
    (verb-phrase -> (Verb noun-phrase))
    (Article -> the a)
    (Noun -> man ball woman table)
    (Verb -> hit took saw liked))
    "A test of a simple grammar")

(defvar *grammar* *simple-grammar*
  "Define the grammar used by generate")

(defun generate (phrase)
  "generate random phrase based on grammar"
  (if (listp phrase)
     (mappend #'generate phrase)
     (let ((choices (rewrites phrase)))
       (if (null choices)
          (list phrase)
          (generate (random-elt choices))))))
