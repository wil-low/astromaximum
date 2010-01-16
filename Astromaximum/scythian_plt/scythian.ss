#lang scheme/gui
; Make some pens and brushes
(define black-pen (make-object pen% "BLACK" 1 'solid))
(define white-brush (make-object brush% "WHITE" 'solid))
(define wheel-radius 250)

(define (draw-circle dc x y radius)
  (send dc draw-ellipse (- x radius) (- y radius) (* radius 2) (* radius 2)))

; Define a procedure to draw a wheel
(define (draw-wheel dc)
  (send dc clear)
  (send dc set-pen black-pen)
  (send dc set-brush white-brush)
  (send dc set-origin (+ wheel-radius 40) (+ wheel-radius 40))
  (draw-circle dc 0 0 wheel-radius)
  )

; Make a 800 x 600 frame
(define frame (new frame% [label "Скиф на схеме"]
                   [width 800]
                   [height 600]))
; Create a 800 x 600 bitmap
(define wheel-bitmap (make-object bitmap% 800 600))
; Create a drawing context for the bitmap
(define bm-dc (make-object bitmap-dc% wheel-bitmap))
; A bitmap's initial content is undefined; clear it before drawing
(send bm-dc clear)
(send bm-dc set-smoothing 'smoothed)

; Make a drawing area whose paint callback copies the bitmap
(define canvas
  (new canvas% [parent frame]
       [paint-callback
        (lambda (canvas dc)
          (draw-wheel bm-dc)
          (send dc draw-bitmap wheel-bitmap 0 0))]))

; Show the frame
(send frame show #t)