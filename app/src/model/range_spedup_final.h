/*************************************************
 Coevolution of resistance/infectivity range
 *************************************************/

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>

#include "types.h"

namespace red_king {

/* Model parameters */
#define Q 0.5
#define B 0.5
#define ALPHA 1.5
#define GAM 0.5
#define WHO 0.5	/* Controls relative mutation rates */
#define BETA_P -0.434
#define A_P 2.615
#define DEM 0

#define TINY 1e-6 /* Constant value for solver */


/* These parameters affect accuracy (hence speed)*/
#define N 50 /* Number of host and parasite phenotypes */
#define HSTART 20 /* Initial host phenotype */
#define PSTART 25 /* Initial parasite phenotype */
#define NEVOL 100 /* Number of iterations (evolutionary timesteps) - ideally this needs to be higher */
#define MAXTIME 1000 /* Duration for ecological dynamics */
#define MAXSTEPS 100 //1e6 /* Maximum number of steps for ODE solver */
#define INTERVAL 100 /* Check if the system is close to equilibrium */
#define EPSILON 0 /* Extinction tolerance */
#define EQTOL 1e-2 /* Equilibrium tolerance */
#define EPS 1e-6 /* ODE solver tolerance */


/*************************************
 * Function prototypes
 *************************************/
class range {

 public:
  range();

  void my_rungkut(rk_real *x, rk_real **y, rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind);


 private:
  void ad(rk_real **xout, rk_real *u, rk_real *v, rk_real **E, rk_real *a, rk_real *beta);
  void rkqs(rk_real *x,rk_real **y,rk_real *dxdt,rk_real **dydt,rk_real *h,rk_real *hnext,rk_real *xscale,rk_real **yscale,rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind);
  void rkck(rk_real *x,rk_real **y, rk_real *dxdt,rk_real **dydt,rk_real *xout,rk_real **yout,rk_real *xerr,rk_real **yerr,rk_real h, rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind);
  void dynamic(rk_real *x, rk_real **y, rk_real *u, rk_real *v, rk_real **E, rk_real *a, rk_real *k, rk_real **k1, int nh, int np, int *host_ind, int *par_ind);
  rk_real FMAX(rk_real,rk_real);
  rk_real FMIN(rk_real,rk_real);

  rk_real **alloc_2dim_array();

  rk_real **y0;
  rk_real **dydt;
  rk_real **yscale;
  rk_real **ymax;
  rk_real **ymin;

  rk_real **ytemp;
  rk_real **yerr;

  rk_real **yk1, **yk2, **yk3, **yk4, **yk5, **yk6;

  rk_real **parsum;


};

}
