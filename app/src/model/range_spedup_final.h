/*************************************************
 Coevolution of resistance/infectivity range
 *************************************************/

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>

#include "types.h"

namespace red_king {


/* These parameters affect accuracy (hence speed)*/
#define N 25 /* Number of host and parasite phenotypes */


/*************************************
 * Function prototypes
 *************************************/
class range {

 public:
  range();

  void my_rungkut(rk_real *x, rk_real **y, rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind);


 private:
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
