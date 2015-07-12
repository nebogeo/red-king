#lang racket
(require plot)

;  /* Initialise discretised trait values */
;	for (i=0; i<N; i++) {
;		u[i]=UMIN+(UMAX-UMIN)*i/(N-1); /* Host */
;		v[i]=VMIN+(VMAX-VMIN)*i/(N-1); /* Parasite */
;	}
;
; /* Define cost functions (trade-offs) */
;	for (i=0; i<N; i++) {
;		a[i]=AMAX-(AMAX-AMIN)*(1-(u[i]-UMAX)/(UMIN-UMAX))/(1+A_P*(u[i]-UMAX)/(UMIN-UMAX)); /* Host trade-off */
;		beta[i]=BEMAXTIME-(BEMAXTIME-BETMIN)*(1-(v[i]-VMAX)/(VMIN-VMAX))/(1+BETA_P*(v[i]-VMAX)/(VMIN-VMAX)); /* Parasite trade-off */
;	}
;
;    /* Define host-parasite interaction matrix */
;    for (i=0; i<N; i++){
;        for (j=0; j<N; j++){
;            E[i][j] = beta[j]*(1-1/(1+exp(-2*(u[i]-v[j]))));
;        }
;    }

;; host
 
(define Q 0.5)
(define B 0.5)
(define ALPHA 1.5)
(define GAM 0.5)
(define WHO 0.5)
(define BETA_P -0.434)
(define A_P 2.615)
(define DEM 0)

;;/* Function parameters */
(define BETMIN 0.491)
(define BEMAXTIME 17.117)
(define AMIN 0.782)
(define AMAX 5.454)
(define UMIN 0.0)
(define UMAX 10.0)
(define VMIN 0.0)
(define VMAX 10.0)
(define TINY 1e-6)

(define N 50) 

(define (umin->umax i)
  (+ UMIN (* (- UMAX UMIN) (/ i (- N 1)))))

(define (vmin->vmax i)
  (+ VMIN (* (- VMAX VMIN) (/ i (- N 1)))))
 
(define (host-tradeoff i)
  (- AMAX (* (- AMAX AMIN) 
             (/
              (- 1 (/ (- (umin->umax i) UMAX) (- UMIN UMAX))) 
              (+ 1 (* A_P (/ (- (umin->umax i) UMAX)) (- UMIN UMAX)))))))

(define (parasite-tradeoff i)
  (- BEMAXTIME 
     (/ 
      (* (- BEMAXTIME BETMIN) (- 1 (/ (- (vmin->vmax i) VMAX) (- VMIN VMAX))))         
      (+ 1 (* BETA_P (/ (- (vmin->vmax i) VMAX) (- VMIN VMAX)))))))

;;/* Define host-parasite interaction matrix */
(define (hp-matrix i j)
  (* (parasite-tradeoff j)
     (- 1 (/ 1 (+ 1 (exp (- (* 2 (- (umin->umax i)
                                    (vmin->vmax j))))))))))


(plot (function host-tradeoff 0 N) #:title "host tradeoff")
(plot (function parasite-tradeoff 0 N) #:title "parasite tradeoff")
(plot3d (surface3d hp-matrix 0 50 0 50) #:title "host-parasite interaction")

(plot-file (function host-tradeoff 0 N) "host.png" #:title "host tradeoff")
(plot-file (function parasite-tradeoff 0 N) "parasite.png" #:title "parasite tradeoff")
(plot3d-file (surface3d hp-matrix 50 0 0 50) "matrix.png" #:title "host-parasite interaction")
