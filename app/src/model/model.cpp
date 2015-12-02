// Copyright (C) 2015 Foam Kernow
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "model.h"
#include "range_spedup_final.h"
#include "jellyfish/core/types.h"

using namespace red_king;

/******************************************
 * Parameters that could be changed via GUI
 ******************************************/

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

model::model() {

  u = new double[N];
  v = new double[N];
  a = new double[N];
  beta = new double[N];
  E = new double*[N];
  for (u32 i=0; i<N; i++) E[i]=new double[N];

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

  init();

}

void model::init() {
  init_trait_values();
  init_cost_functions();
  init_matrix();
  xout = array_maker(2*N+2,NEVOL);
}

void model::init_trait_values() {
  for (i=0; i<N; i++) {
    u[i]=UMIN+(UMAX-UMIN)*i/(N-1); /* Host */
    v[i]=VMIN+(VMAX-VMIN)*i/(N-1); /* Parasite */
  }
}

void model::init_cost_functions() {
  /**********************************************************************
   * This section is where the trade-offs and host-parasite interactions
   * are defined (i.e. where the user would make changes via a GUI)
   *********************************************************************/
  /* Define cost functions (trade-offs) */
  for (i=0; i<N; i++) {
    a[i]=AMAX-(AMAX-AMIN)*(1-(u[i]-UMAX)/(UMIN-UMAX))/(1+A_P*(u[i]-UMAX)/(UMIN-UMAX)); /* Host trade-off */
    beta[i]=BEMAXTIME-(BEMAXTIME-BETMIN)*(1-(v[i]-VMAX)/(VMIN-VMAX))/(1+BETA_P*(v[i]-VMAX)/(VMIN-VMAX)); /* Parasite trade-off */
  }
}

void model::init_matrix() {
    /* Define host-parasite interaction matrix */
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            E[i][j] = beta[j]*(1-1/(1+exp(-2*(u[i]-v[j]))));
        }
    }
}


void model::step() {
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

    std::cout <<  "Hosts:" << nh << ". Parasites:" << np << "\n";

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
