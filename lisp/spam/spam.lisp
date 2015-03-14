;;;; A spam filter following Peter Seibels "practical common lisp"
;;;; Also a great time to get familiar with Emacs
;;;; Not a lot of recurison here


(defparameter *max-ham-score*  .4)
(defparameter *min-spam-score* .6)

(defvar *feature-database* (make-hash-table :test #'equal))

;;; Global Statistics Counters
(defvar *total-spams* 0)
(defvar *total-hams*  0)


(defun classify (text)
  (classification (score (extract-features text))))

(defun classification (score)
  (cond
 ((<= score *max-ham-score*  ) 'ham)
    ((>= score *min-spam-score* ) 'spam)
    (t 'unsure )))


(defclass word-feature ()
  ((word
    :initarg :word
    :accessor word
    :initform (error "Must supply :word")
    :documentation "The word this feature represents")
   (spam-count
    :initarg  :spam-count
    :accessor spam-count 
    :initform 0
    :documentation "Number of spams we have seen this feature in")
   (ham-count
    :initarg :ham-count
    :accessor ham-count
    :initform 0
    :documentation "Number of hams we have seen this feature in")))


(defun clear-database ()
 (setf *feature-database* (make-hash-table :test #'equal)))

(defun intern-feature (word)
  (or (gethash word *feature-database*)
      (setf (gethash word *feature-database*)
            (make-instance 'word-feature :word word))))

;; Well we should eventually do better
;; This function should split a string into a list but doesn't 
(defun extract-words (text)
  (delete-duplicates text))

(defun extract-features (text)
  (mapcar #'intern-feature (extract-words text)))


(defmethod print-object ((object word-feature) stream)
  (print-unreadable-object (object stream :type t)
    (with-slots (word ham-count spam-count) object
      (format stream "~s :hams ~d :spams ~d" word ham-count spam-count))))


(defun train (text type)
  (dolist (feature (extract-features text))
     (increment-count feature type))
  (increment-total-count type))

(defun increment-count (feature type)
  (ecase type
    (ham  (incf (ham-count  feature)))
    (spam (incf (spam-count feature)))))

(defun increment-total-count (type)
  (ecase type
    (ham (incf *total-hams*))
    (spam (incf *total-spams*))))

(defun spam-probability (feature)
  (with-slots (spam-count ham-count) feature
    (/ spam-count (+ spam-count ham-count))))

(defun bayesian-spam-probability (feature &optional 
                                  (assumed-probability 1/2)
                                  (weight 1))
  (let ((basic-probability (spam-probability feature))
        (data-points (+ (spam-count feature) (ham-count feature))))
     (/ (+ (* weight assumed-probability)
           (* data-points basic-probability))
        (+ weight data-points))))

(defun untrained-p (feature)
  (with-slots (ham-count spam-count) feature
    (and (zerop ham-count) (zerop spam-count))))


(defun fisher (probs number-of-probs )
  (inverse-chi-square 
    (* -2 (reduce #'+ probs :key #'log))
    (*  2 number-of-probs)))

(defun inverse-chi-square (value degrees-of-freedom)
  (assert (evenp degrees-of-freedom))
  (min
    (loop with m = (/ value 2)
      for i below (/ degrees-of-freedom 2)
      for prob = (exp (- m)) then (* prob (/ m i))
      summing prob)
    1.0))

;;; Stupid helper function to load this program
(defun l ()
  (load "spam.lisp"))

