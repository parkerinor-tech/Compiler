
.function foo
foo:
adjsp 4
;;; return
;;; varref
pushcvar -12
returnv
push 0
returnv
main:
;;; print
push 1
call @print
swap
pop
pop
halt
