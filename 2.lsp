list

; `a
`a car

(car `(a b c))

(list a 1 `(1 2))

(set `defun  (macro (id args body) (set id (lambda args body))))

(defun a (x) (sin x))

(set `b (lambda `(x) `(sin (sin x))))
; (set `a (macro (x) (sin (sin x))))

(eval `(sin 1.0))



; (set `x 3.0)

(a 1.0)
; (sin (sin 2.0))
; (a 2.0)

; x

(map sin `(0.5 1 2))

; `a
; a

; (label b a)

; `a b "a"

; (a (2 3 4))
; (b (2 3 4))

