#lang scheme/gui
(require "gfx/sizeable.ss")
(require "gfx/mainframe.ss")
; Make some pens and brushes

; Make a 800 x 600 frame
(define frame (new frame% [label "Скиф на схеме"]
                   [width 800]
                   [height 600]))

(define (rect-painter dc w h)
  (send dc draw-rectangle 0 0 w h))

(define stest (make-area 10 30 100 200 (new sizeable%
                                            [paint-callback rect-painter])))
(define stest2 (make-area 109 35 400 220 (new sizeable%
                                            [paint-callback rect-painter])))
(define stest3 (make-area 210 20 290 100 (new sizeable%
                                            [paint-callback rect-painter])))
(define stest4 (make-area 309 45 480 120 (new sizeable%
                                            [paint-callback rect-painter])))
(define main (new mainframe% (area-list (list stest stest2 stest3 stest4)) (parent frame)))
  ; Show the frame
;  (send frame maximize #t)
  (send frame show #t)