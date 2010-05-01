#lang scheme
(require "swisseph.ss")

(provide astrodata)

(define (calc
(define (astrodata date)
  (map swe:planets 
         