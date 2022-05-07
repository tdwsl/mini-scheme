; simple program to test my lisp interpreter :^)
(let ((name "user"))
  (set! name "world")
  (display "Hello, ")
  (display name)
  (display "!")
  (newline)
  (display "4 + 5 = ")
  (display (+ 4 5))
  (newline))
(display "out")
(newline)

(let loop ((i 0))
  (if (< i 6)
    (begin
      (display "i = ")
      (display i)
      (display "/5")
      (newline)
      (loop (+ i 1)))))

(define (hello-again name)
  (display "Hello again, ")
  (display name)
  (display "!")
  (newline))

(hello-again "world")

(define (read-name)
  (display "What is your name?")
  (newline)
  (read))

(define greeting "Hello, ")

;(let ((name (read-name)))
  ;(display greeting)
  ;(display name)
  ;(display "!")
  ;(newline))

(display (string-ref greeting 0))
(newline)

(let (
      (mylist (vector (vector
		"Monday"
		"Tuesday"
		"Wednesday"
		"Thursday"))))
  (let loop ((i 0))
    (if (< i (length (vector-ref mylist 0)))
      (begin
	(display (vector-ref (vector-ref mylist 0) i))
	(newline)
	(loop (+ i 1))))))
