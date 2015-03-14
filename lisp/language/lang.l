(defparameter *grammar3* 
    '((Sentence -> (NP VP))
      (NP -> (Art Noun))
      (VP -> (Verb NP))
      (Art -> the) 
      (Art -> a)

      (Noun -> man)
      (Noun -> ball)
      (Noun -> woman)
      (Noun -> table)
      (Verb -> hit)
      (Verb -> took)
      (Verb -> saw)
      (Verb -> liked)))

(setf *grammar* *grammar3*)

(defstruct (rule (:type list)) lhs -> rhs)
(defstruct (parse) tree rem)

(defun new-tree (cat rhs) (cons cat rhs))
(defun tree-lhs (tree) (first tree))
(defun tree-rhs (tree) (rest tree))
(defun parse-lhs (parse) (tree-lhs (parse-tree parse)))

(defun lexical-rules (word)
   (find-all word *grammar* :key #'rule-rhs :test #'equal))

(defun rules-starting-with (cat)
   (find-all word *grammar* 
      :key #'(lambda (rule) (first-or-nil (rule-rhs rule))))) 


;;;
(defun mload ()
   (load "lang.l"))
