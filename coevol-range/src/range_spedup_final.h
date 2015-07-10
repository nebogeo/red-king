/*************************************************
 Coevolution of resistance/infectivity range
 *************************************************/

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>

/******************************************
 * Parameters that could be changed via GUI
 ******************************************/

/* Model parameters */
#define Q 0.5
#define B 0.5
#define ALPHA 1.5
#define GAM 0.5
#define WHO 0.5	/* Controls relative mutation rates */
#define BETA_P -0.434
#define A_P 2.615
#define DEM 0

/* Function parameters */
#define BETMIN 0.491
#define BEMAXTIME 17.117
#define AMIN 0.782
#define AMAX 5.454

/******************************************
 * Fixed parameters for solver
 ******************************************/
#define UMIN 0.0 /* Minimum host trait */
#define UMAX 10.0 /* Maximum host trait */
#define VMIN 0.0 /* Minimum parasite trait */
#define VMAX 10.0 /* Maximum parasite trait */
#define TINY 1e-6 /* Constant value for solver */

/* These parameters affect accuracy (hence speed)*/
#define N 50 /* Number of host and parasite phenotypes */
#define HSTART 20 /* Initial host phenotype */
#define PSTART 25 /* Initial parasite phenotype */
#define NEVOL 100 /* Number of iterations (evolutionary timesteps) - ideally this needs to be higher */
#define MAXTIME 1000 /* Duration for ecological dynamics */
#define MAXSTEPS 20 //1e6 /* Maximum number of steps for ODE solver */
#define INTERVAL 100 /* Check if the system is close to equilibrium */
#define EPSILON 0 /* Extinction tolerance */
#define EQTOL 1e-2 /* Equilibrium tolerance */
#define EPS 1e-6 /* ODE solver tolerance */

/*************************************
 * Function prototypes
 *************************************/
void ad(double **xout, double *u, double *v, double E[][N], double *a, double *beta);
void orig_rungkut(double *x, double y[][N], double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind);
void my_rungkut(double *x, double y[][N], double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind);
void rkqs(double *x,double y[][N],double *dxdt,double dydt[][N],double *h,double *hnext,double *xscale,double yscale[][N],double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind);
void rkck(double *x,double y[][N], double *dxdt,double dydt[][N],double *xout,double yout[][N],double *xerr,double yerr[][N],double h, double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind);
void dynamic(double *x, double y[][N], double *u, double *v, double E[][N], double *a, double *k, double k1[][N], int nh, int np, int *host_ind, int *par_ind);
double FMAX(double,double);
double FMIN(double,double);
double** array_maker(int rows, int cols);
void free_array(double **array, int rows);
