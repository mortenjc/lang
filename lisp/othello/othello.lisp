(defconstant all-directions '(-11 -10 -9 -1 1 9 10 11))

(defconstant empty 0 "An empty square")
(defconstant black 1 "A black piece")
(defconstant white 2 "A white piece")
(defconstant outer 3 "Squares outside 8x8 board")

(deftype piece () `(integer ,empty ,outer))

(defun name-of (piece) (char ".*O?" piece))

(defun opponent (player) (if (eql player black) white black))

(deftype board () '(simple-array piece (100)))

(defun bref (board square) (aref board square))

(defsetf bref (board square) (val) 
   `(setf (aref ,board ,square) ,val))

(defun copy-board (board)
   (copy-seq board))

(defconstant all-squares
   (loop for i from 11 to 88 when (<= 1 (mod i 10) 8) collect i))

(defun initial-board ()
  (let ((board (make-array 100 :element-type 'piece
                    :initial-element outer)))
     (dolist (square all-squares)
        (setf (bref board square) empty))
     (setf (bref board 44) white (bref board 45) black
           (bref board 54) black (bref board 55) white)
     board))

(defun print-board (board)
   (format t "~2&      1 2 3 4 5 6 7 8    [~c=~2a ~c=~2a  (~@d)]"
         (name-of black) (count black board)
         (name-of white) (count white board)
         (count-difference black board))
   (loop for row from 1 to 8 do
      (format t "~&   ~d " (* 10 row))
      (loop for col from 1 to 8
          for piece = (bref board (+ col (* 10 row)))
          do (format t "~c " (name-of piece))))
   (format t "~2&"))


(defun count-difference (player board)
   (- (count player board)
      (count (opponent player) board)))

(defun valid-p (move)
   (and (integerp move) (<= 11 move 88) (<= 1 (mod move 10) 8)))

(defun legal-p (move player board)
   (and (eql (bref board move) empty)
        (some #'(lambda (dir) (would-flip? move player board dir))
          all-directions)))

(defun make-move (move player board)
  (setf (bref board move) player)
  (dolist (dir all-directions)
     (make-flips move player board dir))
     board)

(defun make-flips (move player board dir)
   (let ((bracketer (would-flip? move player board dir)))
      (when bracketer
         (loop for c from (+ move dir) by dir until (eql c bracketer)
              do (setf (bref board c) player)))))

(defun would-flip? (move player board dir)
   (let ((c (+ move dir)))
      (and (eql (bref board c) (opponent player))
         (find-bracketing-piece (+ c dir) player board dir))))

(defun find-bracketing-piece (square player board dir)
   (cond ((eql (bref board square) player) square)
         ((eql (bref board square) (opponent player))
            (find-bracketing-piece (+ square dir) player board dir))
         (t nil)))

(defun othello (bl-strategy wh-strategy &optional (print t))
   (let ((board (initial-board)))
      (loop for player = black
                then (next-to-play board player print)
            for strategy = (if (eql player black)
                               bl-strategy
                               wh-strategy)
            until (null player)
            do (get-move strategy player board print))
   (when print
      (format t "~&The game is over. final result:")
      (print-board board)
      (count-difference black board))))

(defun next-to-play (board previous-player print)
    (let ((opp (opponent previous-player)))
        (cond ((any-legal-move? opp board) opp)
              ((any-legal-move? previous-player board)
                  (when print 
                     (format t "~&~c has no moves and must pass." (name-of opp)))
                  previous-player)
              (t nil))))

(defun any-legal-move? (player board)
   (some #'(lambda (move) (legal-p move player board)) all-squares))

(defun get-move (strategy player board print)
   (when print (print-board board))
   (let ((move (funcall strategy player (copy-board board))))
     (cond
       ((and (valid-p move) (legal-p move player board))
        (when print
           (format t "~&~c moves to ~d." (name-of player) move))
        (make-move move player board))
       (t (warn "Illegal move ~d" move)
           (get-move strategy player board print)))))

(defun human (player board)
   (declare (ignore board))
   (format t "~&~c to move: " (name-of player))
   (read))

(defun random-elt (list)
   (elt list (random (length list))))

(defun random-strategy (player board)
   (random-elt (legal-moves player board))) 

(defun legal-moves (player board)
   (loop for move in all-squares
      when (legal-p move player board) collect move))

(defun max-diff (player board)
   (funcall (maximizer #'count-difference) player board))

(defun maximizer (eval-fn)
  #'(lambda (player board)
      (let* ((moves (legal-moves player board))
             (scores (mapcar #'(lambda (move)
                        (funcall eval-fn player 
                           (make-move move player
                               (copy-board board))))
                             moves))
             (best (apply #'max scores)))
       (elt moves (position best scores)))))
