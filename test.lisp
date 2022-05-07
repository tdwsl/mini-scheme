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

;(quit)

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

(let ((name (read-name)))
  (display greeting)
  (display name)
  (display "!")
  (newline))
