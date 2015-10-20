\
(sin 1.0)\
(sin (sin 1.57))\

(define `x sin)\

(x (x 1.57))\
 
 (empty? `())\
 
 (define `map (lambda `(f l) `(cond (empty? l) l `(cons (f (car l)) (map f (cdr l)))) ))\
 
map\

(define `l `(1 2 3))\

l
\
(car l)
\
(cdr l)
\
(cons (car l) (cdr l))
\
(cond (empty? l) l (cons (car l) (cdr l)))
\
(define `sum (lambda `(l) `(cond (empty? l) 0.0 `(+ (car l) (sum (cdr l))))))
\
(+ 1.0 2.0)
\
(sin (+ 1.0 0.57))
\
(sum `())  \
(sum `(1.0 2.0)) \
(sum `(1.0 2.0 3.0 4.0)) \



(map sin `())   \
(map sin `(1.57 3.1415926 0)) \


; (empty? ())

; (empty? `(1 2 3))

; (cond (empty? `(1 2 3)) map "FALSE")

 "end"\
 