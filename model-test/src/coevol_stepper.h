#include "range_spedup_final.h"

typedef unsigned int u32;

class coevol_stepper {
public:
    coevol_stepper(double **xout, double *u, double *v, double **E, double *a, double *beta);
    void step();




    double *u, *v, **E, *a;
    double *x0, *y, **y0;
    double  rtype, r1, r2, xtotal, xcum, temp;
    int *host_ind, *par_ind;
    int i, j, evol_count, nh, np, mutator, pop_choice;

    double **xout;
};
