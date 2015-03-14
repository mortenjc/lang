(defvar *db* nil)

(defun make-cd (title artist rating ripped)
  (list :title title :artist artist :rating rating :ripped ripped))

(defun add-record (cd)
  (push cd *db*))

(defun dump-db ()
   ( dolist (entry *db*)
     (format t "~{~a:~10t~a~%~}~%" entry))
)

(defun prompt-read (prompt)
  (format *query-io* "~a: " prompt)
  (force-output *query-io*)
  (read-line *query-io*))

(defun prompt-for-cd ()
  (make-cd 
    (prompt-read "Title")
    (prompt-read "Artist")
    (or (parse-integer  (prompt-read "Rating") :junk-allowed t) 0)
    (y-or-n-p "Ripped? [y/n]:")))

(defun add-cds ()
  (loop (add-record(prompt-for-cd))
    (if (not (y-or-n-p "Another?")) (return))))

(defun save-db (filename)
  (with-open-file (out filename
        :direction :output
        :if-exists :supersede)
  (with-standard-io-syntax 
    (print *db* out))))
  
(defun load-db (filename)
   (with-open-file (in filename)
   (with-standard-io-syntax)
   (setf *db* (read in))))

(defun select (selector)
  (remove-if-not selector *db*))
