(define map
  (vector
    #\# #\# #\# #\# #\#
    #\# #\. #\. #\. #\#
    #\# #\. #\. #\. #\#
    #\# #\. #\. #\. #\#
    #\# #\# #\# #\# #\#))
(define mapw 5)
(define maph 5)

(define (draw-map)
  (let yloop ((y 0))
    (if (< y maph)
      (begin
        (let xloop ((x 0))
          (if (< x mapw)
            (begin (display (vector-ref map (+ (* y mapw) x)))
              (xloop (+ x 1)))))
        (newline)
        (yloop (+ y 1))))))

(draw-map)
