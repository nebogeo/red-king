#include "coevol_stepper.h"
#include "range_spedup_final.h"

coevol_stepper::coevol_stepper(double **xout, double *_u, double *_v, double **_E, double *_a, double *beta) {
    u = _u;
    v = _v;
    E = _E;
    a = _a;

    x0 = new double[N];
    y = new double[N];
    y0 = new double*[N];
    for (u32 i=0; i<N; i++) y0[i]=new double[N];
    host_ind=new int[N];
    par_ind=new int[N];

    /* Initialise population numbers */
    for (u32 i=0; i<N; i++)	{
        x0[i]=0.0;
        y[i]=0.0;
        for (j=0; j<N; j++) {
            y0[i][j]=0.0;
        }
    }
    x0[HSTART-1]=1.0;
    y0[HSTART-1][PSTART-1]=1.0;
    y[PSTART-1]=y0[HSTART-1][PSTART-1];
    evol_count = 0;
}


void coevol_stepper::step() {
    /* Find which phenotypes are present */
    nh = 0;
    np = 0;
    for (i=0; i<N; i++)	{
        if(x0[i]>0){
            host_ind[nh]=i;
                nh++;
        }
        if(y[i]>0){
            par_ind[np]=i;
            np++;
        };
    }

    /* Check if all hosts or parasites have been driven extinct */
    if(nh==0){
        for (i=0; i<N; i++) {
            x0[i]=0;
            y[i]=0;
        }
        printf("Breaking - hosts driven extinct\n");
        return;
    }
    if(np==0){
        for (i=0; i<N; i++) {
            y[i]=0;
        }
        printf("Breaking - parasites driven extinct\n");
        return;
    }

    /* Call ODE solver */
    my_rungkut(x0,(double (*)[N])y0,u,v,(double (*)[N])E,a,nh,np,host_ind,par_ind);

    /* Check for extinct phenotypes */
    for (i=0; i<N; i++) {
        y[i]=0;
        if (x0[i]<EPSILON) x0[i]=0;
        for (j=0; j<N; j++) {
            if (y0[j][i]<EPSILON) {
                y0[j][i]=0;
            }
            else{
                /* Work out parasite phenotype densities */
                y[i]+=y0[j][i];
            }
        }
    }

    /* Store densities */
    //for (i=0; i<N; i++) {
    //    xout[i][evol_count]=x0[i];
    //    xout[i+N][evol_count]=y[i];
    //}

    /* Mutation routine */
    rtype=double(rand())/RAND_MAX;
    if (rtype<WHO) pop_choice=0;
    else pop_choice=1;

    r1=double(rand())/RAND_MAX;
    xtotal = 0.0;
    xcum = 0.0;
    for (i=0; i<N; i++) {
        if (pop_choice==0) xtotal+=x0[i];
        if (pop_choice==1) xtotal+=y[i];
    }

    for (i=0; i<N; i++) {
        if (pop_choice==0) xcum+=x0[i];
        if (pop_choice==1) xcum+=y[i];
        if (r1 < xcum/xtotal) {	/* Find which strain will mutate */
            mutator=i;
            break;
        }
    }

    r2=double(rand())/RAND_MAX;
    if (r2<0.5 && mutator>0){	/* Mutate up or down? */
        if (pop_choice==0) {
            x0[mutator-1]+=x0[mutator]/10.0;
            for (j=0; j<N; j++) {
                y0[mutator-1][j]+=y0[mutator][j]/10.0;
            }
        }
        else for (j=0; j<N; j++) y0[j][mutator-1]+=y0[j][mutator]/10.0;
    }
    else if (r2>0.5 && mutator<N-1){
        if (pop_choice==0) {
            x0[mutator+1]+=x0[mutator]/10.0;
            for (j=0; j<N; j++) {
                y0[mutator+1][j]+=y0[mutator][j]/10.0;
            }
        }
        else for (j=0; j<N; j++) y0[j][mutator+1]+=y0[j][mutator]/10.0;
    }


    evol_count++;
}
