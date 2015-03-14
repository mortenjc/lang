
(defparameter *pata* '((?* ?x) need a vacation))
(defparameter *patb* '((?* ?x)))
(defparameter *patc* '(i ?x need a vacation))
(defparameter *patd* '((?* ?x) need a (?* ?y)))

(defparameter *input* '(i really need a long vacation))


(defun tst (pattern)
   (pat-match pattern *input*))

(defun trc ()
   (trace pat-match)
   (trace match-variable)
   (trace segment-match))

(defun utrc ()
   (untrace pat-match)
   (untrace match-variable)
   (untrace segment-match))

;;; From auxfunctions

(defun random-elt (lst)
   (elt lst (random (length lst))))

(defun starts-with (list x)
  (and (consp list) (eql (first list) x)))
;;;

(defconstant fail nil)
(defconstant no-bindings '((t . t)))

;;;
(defun variable-p (x)
   (and (symbolp x) (equal (char (symbol-name x) 0) #\?)))

(defun segment-pattern-p (pattern)
  (and (consp pattern) 
       (starts-with (first pattern) '?*)))

;;;
(defun get-binding (var bindings)
  (assoc var bindings))

(defun binding-val (binding)
   (cdr binding))

(defun lookup (var bindings)
  (binding-val (get-binding var bindings)))

(defun extend-bindings (var val bindings)
  (cons (cons var val) 
      (if (eq bindings no-bindings)
          nil
          bindings)))


;;; Core of the pattern matching function
(defun pat-match (pattern input &optional (bindings no-bindings))
   (cond ((eq bindings fail) fail)
         ((variable-p pattern)
          (match-variable pattern input bindings))
         ((eql pattern input) bindings)
         ((segment-pattern-p pattern)
          (segment-match pattern input bindings))
         ((and (consp pattern) (consp input))
          (pat-match (rest pattern) (rest input)
                     (pat-match (first pattern) (first input) bindings)))
         (t fail)))

(defun match-variable (var input bindings)
   (let ((binding (get-binding var bindings)))
     (cond ((not binding) (extend-bindings var input bindings))
           ((equal input (binding-val binding)) bindings)
           (t fail))))

(defun segment-match (pattern input bindings &optional (start 0))
   (let ((var (second (first pattern)))
         (pat (rest pattern)))
      (if (null pat)
        (match-variable var input bindings)
        (let ((pos (position (first pat) input
                                   :start start :test #'equal)))
           (if (null pos)
               fail
               (let ((b2 (pat-match pat (subseq input pos) bindings)))
                  (if (eq b2 fail)
                      (segment-match pattern input bindings (+ pos 1))
                      (match-variable var (subseq input 0 pos) b2))))))))




