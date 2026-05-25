(define (fact (n Int))
  (cond
    ((zero? n) 1)
    (else (* n (fact (- n 1))))))

(fact 5)
