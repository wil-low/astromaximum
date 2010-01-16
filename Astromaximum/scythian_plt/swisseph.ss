#lang scheme
(require scheme/foreign)
(unsafe!)
(provide swe:get-planet-name swe:julday swe:planets swe:sun swe:moon)

(define swe:zodiac-text '(Овен Телец Близнецы Рак Лев Дева Весы Скорпион Стрелец Козерог Водолей Рыбы))
(define swe:planets '(swe:sun swe:moon))
(define libswe (ffi-lib "./libswe.so"))
(define-values (swe:sun swe:moon) (apply values (build-list 2 values)))
;  (apply values (list 0 1)))
(define swe:get-planet-name 
  (get-ffi-obj "swe_get_planet_name" libswe (_fun _int32 (_string/latin-1 = "") -> _string/latin-1)))

(define swe:julday
  (get-ffi-obj "swe_julday" libswe (_fun _int32 _int32 _int32 _double _int32 -> _double)))

(define swe:set-ephe-path
  (get-ffi-obj "swe_set_ephe_path" libswe (_fun _string/latin-1 -> _void)))

(define swe:calc
  (get-ffi-obj "swe_calc" libswe 
               (_fun _double _int32 _int32 (data : (_cvector o _double 6)) (error : (_bytes o 255)) -> 
                     (res : _int32) ->
                     (values res data error))))

(define (zodiac-degree angle)
  (string-append
   (number->string (inexact->exact(+ 1 (truncate (remainder (truncate angle) 30)))))
   "* "
   (symbol->string (list-ref swe:zodiac-text (inexact->exact(truncate (/ angle 30)))))))

(swe:set-ephe-path "/home/willow/amax/sweph")
(for ((i (in-range 1 365)))
  (let-values (((res data error) (swe:calc (swe:julday 2010 01 i 1. 1) swe:sun 1)))
    (printf "~a: ~a (~a)~n" i (cvector-ref data 0) (zodiac-degree (cvector-ref data 0)))))
