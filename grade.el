; ===========================================================================
; FILE        : grade.el
; LAST REVISED: October 19, 1999
; AUTHOR      : Peter Chapin
;
; This file contains a few useful(?) Emacs-Lisp functions that facili-
; tate grading student submissions. This file is highly experimental.
; Basically I don't know what I'm doing and I'm trying to learn.
; ===========================================================================

(defun insert-icomment-mark ()
  "Inserts an instructors comment into the document"
  (interactive)
  (insert "\n+++ "))

(global-set-key "\C-ci" insert-icomment-mark)


;
; This is where I'm putting my saved keyboard macros. (For what that is
; worth).
;
(fset 'grade-icomment-mark
   [?\C-a return return up up return ?+ ?+ ?+ ? ])
