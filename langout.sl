main:
adjsp 24
;;; assign
push 10
push 0
pushfp
plus
push 0
push 4
times
plus
popvarind
;;; assign
push 20
push 0
pushfp
plus
push 1
push 4
times
plus
popvarind
;;; assign
push 30
push 0
pushfp
plus
push 2
push 4
times
plus
popvarind
;;; assign
;;; varref
push 0
pushfp
plus
push 0
push 4
times
plus
pushvarind
;;; varref
push 0
pushfp
plus
push 1
push 4
times
plus
pushvarind
plus
push 20
pushfp
plus
popvarind
;;; print
;;; varref
pushvar 20
call @print
swap
pop
pop
;;; print
;;; varref
push 0
pushfp
plus
push 2
push 4
times
plus
pushvarind
call @print
swap
pop
pop
halt
