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

#include <iostream>
#include <string.h> // memcpy
#include "model.h"
#include "range_spedup_final.h"
#include "types.h"

using namespace red_king;
using namespace std;

#define WHO 0.5	/* Controls relative mutation rates */
#define EPSILON 0.01 /* Extinction tolerance */

void clear(rk_real *ptr) {
  for (int i=0; i<N; i++) {
    ptr[i]=0;
  }
}

void mclear(rk_real **ptr) {
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      ptr[i][j]=0;
    }
  }
}

void cleari(int *ptr) {
  for (int i=0; i<N; i++) {
    ptr[i]=0;
  }
}

rk_real diff(rk_real *a, rk_real *b, size_t len) {
  rk_real t=0;
  for (size_t i=0; i<len; i++) {
    t+=a[i]-b[i];
  }
  return t/(float)len;
}

model::model() {
  m_range = new range();
  m_model = 0;

  u = new rk_real[N];
  v = new rk_real[N];
  a = new rk_real[N];
  beta = new rk_real[N];
  E = new rk_real*[N];
  for (u32 i=0; i<N; i++) E[i]=new rk_real[N];

  x0 = new rk_real[N];
  y = new rk_real[N];
  last_x0 = new rk_real[N];
  last_y = new rk_real[N];
  y0 = new rk_real*[N];
  for (u32 i=0; i<N; i++) y0[i]=new rk_real[N];
  host_ind=new int[N];
  par_ind=new int[N];

  m_cost_params.amin = 0;
  m_cost_params.amax = 9.4;
  m_cost_params.a_p = 2;
  m_cost_params.betmin = 0;
  m_cost_params.bemaxtime = 11.8;
  m_cost_params.beta_p = 0.8;
  m_cost_params.g = 13.5;
  m_cost_params.h = 1.2;

  m_cost_params.umin = 0;
  m_cost_params.umax = 1;
  m_cost_params.vmin = 0;
  m_cost_params.vmax = 1;

  m_hstart = 10;
  m_pstart = 12;

  init();
}

void model::init() {
  srand(0);

  clear(u);
  clear(v);
  clear(a);
  clear(beta);
  mclear(E);
  clear(x0);
  clear(y);
  mclear(y0);
  cleari(host_ind);
  cleari(par_ind);

  /* Initialise population numbers */
  for (u32 i=0; i<N; i++)	{
    x0[i]=0.0;
    y[i]=0.0;
    for (int j=0; j<N; j++) {
      y0[i][j]=0.0;
    }
  }

  x0[m_hstart]=1.0;
  y0[m_hstart][m_pstart]=1.0;
  y[m_pstart]=y0[m_hstart][m_pstart];

  update_cost_functions();
}

void model::update_cost_functions() {
  init_trait_values(m_cost_params);
  init_cost_functions(m_cost_params);
  init_matrix(m_cost_params);

  //m_cost_params.print();
}

void model::init_trait_values(model_cost_params &cp) {
  for (int i=0; i<N; i++) {
    u[i]=cp.umin+(cp.umax-cp.umin)*i/(N-1); /* Host */
    v[i]=cp.vmin+(cp.vmax-cp.vmin)*i/(N-1); /* Parasite */
    //u[i]=0+(10-0)*i/(N-1); /* Host */
    //v[i]=0+(10-0)*i/(N-1); /* Parasite */
  }
}

void model::init_cost_functions(model_cost_params &cp) {
  /**********************************************************************
   * This section is where the trade-offs and host-parasite interactions
   * are defined (i.e. where the user would make changes via a GUI)
   *********************************************************************/
  /* Define cost functions (trade-offs) */
  for (int i=0; i<N; i++) {
    //a[i]=cp.amax-(cp.amax-cp.amin)*(1-(u[i]-cp.umax)/(cp.umin-cp.umax))/(1+cp.a_p*(u[i]-cp.umax)/(cp.umin-cp.umax)); /* Host trade-off */
    //beta[i]=cp.bemaxtime-(cp.bemaxtime-cp.betmin)*(1-(v[i]-cp.vmax)/(cp.vmin-cp.vmax))/(1+cp.beta_p*(v[i]-cp.vmax)/(cp.vmin-cp.vmax)); /* Parasite trade-off */

    a[i]=cp.amax-(cp.amax-cp.amin)*(powf(u[i],cp.a_p)); /* Host trade-off */
    beta[i]=cp.bemaxtime-(cp.bemaxtime-cp.betmin)*powf(v[i],cp.beta_p); /* Parasite trade-off */


  }
}

void model::init_matrix(model_cost_params &cp) {
  /* Define host-parasite interaction matrix */
  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      if (m_model==0) {
        E[i][j] = beta[j]*(1-1/(1+exp(-cp.g*(u[i]-v[j]))));
      } else {
        rk_real t = (v[j]-u[i])/(cp.g*0.1*v[i]+cp.h);
        E[i][j] = beta[j]*exp(-(t*t));
      }
    }
  }
}


void model::step() {
  int nh = 0;
  int np = 0;
  check_phenotypes(nh,np);

  /* Call ODE solver */
  m_range->my_rungkut(x0,y0,u,v,E,a,nh,np,host_ind,par_ind);

  // Check for extinct phenotypes
  for (int i=0; i<N; i++) {
    y[i]=0;
    if (x0[i]<EPSILON) x0[i]=0;
    for (int j=0; j<N; j++) {
      if (y0[j][i]<EPSILON) {
        y0[j][i]=0;
      } else {
        // Work out parasite phenotype densities
        y[i]+=y0[j][i];
      }
    }
  }

  mutate();

  //cerr<<diff(x0,last_x0,N)+diff(y,last_y,N)<<endl;
  //memcpy(last_x0,x0,sizeof(rk_real)*N);
  //memcpy(last_y,y,sizeof(rk_real)*N);

}



void model::check_phenotypes(int &nh, int& np) {
  /* Find which phenotypes are present */
  for (int i=0; i<N; i++)	{
    //cerr<<x0[i]<<" "<<y[i]<<endl;
    if(x0[i]>0){
      host_ind[nh]=i;
      nh++;
    }
    if(y[i]>0){
      par_ind[np]=i;
      np++;
    };
  }

  //std::cout <<  "Hosts:" << nh << ". Parasites:" << np << "\n";

  /* Check if all hosts or parasites have been driven extinct */
  if(nh==0) {
    for (int i=0; i<N; i++) {
      x0[i]=0;
      y[i]=0;
    }
    printf("Breaking - hosts driven extinct\n");
    return;
  }
  if(np==0){
    for (int i=0; i<N; i++) {
      y[i]=0;
    }
    printf("Breaking - parasites driven extinct\n");
    return;
  }
}

void model::mutate() {

    /* Mutation routine */
    rk_real rtype=rk_real(rand())/RAND_MAX;
    int pop_choice=1;
    if (rtype<WHO) pop_choice=0;

    rk_real r1=rk_real(rand())/RAND_MAX;
    rk_real xtotal = 0.0;
    rk_real xcum = 0.0;
    for (int i=0; i<N; i++) {
        if (pop_choice==0) xtotal+=x0[i];
        if (pop_choice==1) xtotal+=y[i];
    }

    int mutator = 0;

    for (int i=0; i<N; i++) {
        if (pop_choice==0) xcum+=x0[i];
        if (pop_choice==1) xcum+=y[i];
        if (r1 < xcum/xtotal) {	/* Find which strain will mutate */
            mutator=i;
            break;
        }
    }

    rk_real r2=rk_real(rand())/RAND_MAX;
    if (r2<0.5 && mutator>0){	/* Mutate up or down? */
        if (pop_choice==0) {
            x0[mutator-1]+=x0[mutator]/10.0;
            for (int j=0; j<N; j++) {
                y0[mutator-1][j]+=y0[mutator][j]/10.0;
            }
        }
        else for (int j=0; j<N; j++) y0[j][mutator-1]+=y0[j][mutator]/10.0;
    }
    else if (r2>0.5 && mutator<N-1){
        if (pop_choice==0) {
            x0[mutator+1]+=x0[mutator]/10.0;
            for (int j=0; j<N; j++) {
                y0[mutator+1][j]+=y0[mutator][j]/10.0;
            }
        }
        else for (int j=0; j<N; j++) y0[j][mutator+1]+=y0[j][mutator]/10.0;
    }
}
