/*************************************************
 Coevolution of resistance/infectivity range
 *************************************************/

// by Ben Ashby etc...

#include "range_spedup_final.h"

using namespace std;
using namespace red_king;

/***************************************
 * Adaptive dynamics function
 ***************************************/

/* Model parameters */

#define Q 0.5
#define B 0.5
#define ALPHA 1.5
#define GAM 0.5

#define TINY 1e-6 /* Constant value for solver */

#define MAXTIME 1000 /* Duration for ecological dynamics */
#define MAXSTEPS 5 //1e6 /* Maximum number of steps for ODE solver */
#define INTERVAL 100 /* Check if the system is close to equilibrium */
#define EQTOL 1e-2 /* Equilibrium tolerance */
#define EPS 1e-6 /* ODE solver tolerance */


range::range() {
  y0 = alloc_2dim_array();

  y0  = alloc_2dim_array();
  dydt  = alloc_2dim_array();
  yscale  = alloc_2dim_array();
  ymax  = alloc_2dim_array();
  ymin  = alloc_2dim_array();

  ytemp  = alloc_2dim_array();
  yerr  = alloc_2dim_array();

  yk1 = alloc_2dim_array();
  yk2 = alloc_2dim_array();
  yk3 = alloc_2dim_array();
  yk4 = alloc_2dim_array();
  yk5 = alloc_2dim_array();
  yk6  = alloc_2dim_array();

  parsum  = alloc_2dim_array();
}

rk_real **range::alloc_2dim_array() {
  rk_real **t = new rk_real*[N];
  for (int i=0; i<N; i++) {
    t[i] = new rk_real[N];
  }
  return t;
}


/*****************************************
 * ODE solver
 ****************************************/
void range::my_rungkut (rk_real *x, rk_real **y, rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind){

    int i,j,exitflag,count;
    rk_real maxsteps,t,nextcheck;
    rk_real hnext[1], h[1], dxdt[N], /*dydt[N][N],*/ xscale[N], /*yscale[N][N],*/ xmax[N], xmin[N]/*, ymax[N][N], ymin[N][N]*/;

    /* Other params */
    exitflag = 0;
    count=0;
    h[0] = 1e-3;
    hnext[0] = 1e-3;
    t=0;
    nextcheck = INTERVAL;

    for (i=0; i<nh; i++){
        xmax[host_ind[i]]=x[host_ind[i]];
        xmin[host_ind[i]]=x[host_ind[i]];
        for (j=0; j<np; j++){
            ymax[host_ind[i]][par_ind[j]]=y[host_ind[i]][par_ind[j]];
            ymin[host_ind[i]][par_ind[j]]=y[host_ind[i]][par_ind[j]];
        }
    }

    /* Main loop: */
    do{
        /* This ensures the final step lands us on the final time point */
        if(1.1*hnext[0]>(MAXTIME-t)){
            hnext[0] = MAXTIME-t;
            h[0] = MAXTIME-t;
            t=MAXTIME;
        }
        else{
            h[0] = hnext[0];
            t+=h[0];
        }
        if(t==MAXTIME) {
            exitflag=1;
        }

        /* This is where the equations are first solved */
        dynamic(x,y,u,v,E,a,dxdt,dydt,nh,np,host_ind,par_ind);

        /* Adjust the step size to maintain accuracy */
        for (i=0; i<N; i++){
            xscale[i]=fabs(x[i])+fabs(dxdt[i]*(*h))+TINY;
            for (j=0; j<N; j++){
                yscale[i][j]=fabs(y[i][j])+fabs(dydt[i][j]*(*h))+TINY;
            }
        }
        rkqs(x,y,dxdt,dydt,h,hnext,xscale,yscale,u,v,E,a,nh,np,host_ind,par_ind);
        count++;

        /* Check if we're close to equilibrium - if so, finish early */
        for (i=0; i<nh; i++){
            xmax[host_ind[i]]=FMAX(xmax[host_ind[i]],x[host_ind[i]]);
            xmin[host_ind[i]]=FMIN(xmin[host_ind[i]],x[host_ind[i]]);
            for (j=0; j<np; j++){
                ymax[host_ind[i]][par_ind[j]]=FMAX(ymax[host_ind[i]][par_ind[j]],y[host_ind[i]][par_ind[j]]);
                ymin[host_ind[i]][par_ind[j]]=FMIN(ymin[host_ind[i]][par_ind[j]],y[host_ind[i]][par_ind[j]]);
            }
        }
        if(t>nextcheck){
            exitflag = 1;
            for (i=0; i<nh; i++){
                if(fabs(xmax[host_ind[i]]-xmin[host_ind[i]])>EQTOL){
                    exitflag = 0;
                    break;
                }
                for (j=0; j<np; j++){
                    if(fabs(ymax[host_ind[i]][par_ind[j]]-ymin[host_ind[i]][par_ind[j]])>EQTOL){
                        exitflag = 0;
                        break;
                    }
                }
            }
            if(exitflag==1){
                t=MAXTIME;
                break;
            }
            nextcheck+=INTERVAL;
            for (i=0; i<nh; i++){
                xmax[host_ind[i]]=x[host_ind[i]];
                xmin[host_ind[i]]=x[host_ind[i]];
                for (j=0; j<np; j++){
                    ymax[host_ind[i]][par_ind[j]]=y[host_ind[i]][par_ind[j]];
                    ymin[host_ind[i]][par_ind[j]]=y[host_ind[i]][par_ind[j]];
                }
            }
        }

    }while(count<(MAXSTEPS-1) && t<=MAXTIME && exitflag==0);
}

/*****************************************
 * This generates the adaptive step-size
 ****************************************/
void range::rkqs(rk_real *x,rk_real **y,rk_real *dxdt,rk_real **dydt,rk_real *h,rk_real *hnext,rk_real *xscale,rk_real **yscale,rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind)
{
    rk_real xtemp[N], /*ytemp[N][N],*/ xerr[N];//, yerr[N][N];
    rk_real htemp,errmax;
    int i,j;

    htemp= *h;

    rkck(x,y,dxdt,dydt,xtemp,ytemp,xerr,yerr,*h,u,v,E,a,nh,np,host_ind,par_ind);
    *hnext= *h;

    for(;;)
    {
        rkck(x,y,dxdt,dydt,xtemp,ytemp,xerr,yerr,*h,u,v,E,a,nh,np,host_ind,par_ind);
        errmax= 0.0;
        for(i=0;i<N;i++){
            errmax= FMAX(errmax,fabs(xerr[i]/xscale[i]));
            for(j=0;j<N;j++){
                errmax= FMAX(errmax,fabs(yerr[i][j]/yscale[i][j]));
            }
        }
        errmax/= EPS;
        if(errmax<=1.0) break;
        htemp= 0.9*(*h)*pow(errmax,-0.25);
        *h= (*h>=0.0 ? FMAX(htemp,0.1*(*h)) : FMIN(htemp,0.1*(*h)));
    }
    if(errmax > 1.89E-4) {
        *hnext= 0.9*(*h)*pow(errmax,-0.2);
    }
    else {
        *hnext= 5.0*(*h);
    }

    for(i=0;i<N;i++){
        x[i]= xtemp[i];
        for(j=0;j<N;j++){
            y[i][j]= ytemp[i][j];
        }
    }
}

/*****************************************
 * This is the standard RK solver
 ****************************************/
void range::rkck(rk_real *x,rk_real **y,rk_real *dxdt,rk_real **dydt,rk_real *xout,rk_real **yout,rk_real *xerr,rk_real **yerr,rk_real h, rk_real *u, rk_real *v, rk_real **E, rk_real *a, int nh, int np, int *host_ind, int *par_ind)
{
    int i,j;
	rk_real xk1[N], xk2[N], xk3[N], xk4[N], xk5[N], xk6[N];
	//rk_real yk1[N][N], yk2[N][N], yk3[N][N], yk4[N][N], yk5[N][N], yk6[N][N];
	rk_real xtemp[N]; //, ytemp[N][N];

    rk_real b21=0.2,b31=3.0/40.0,b32=9.0/40.0,b41=0.3,b42=-0.9,b43=1.2,
    b51=-11.0/54.0,b52=2.5,b53=-70.0/27.0,b54=35.0/27.0,b61=1631.0/55296,
    b62=175.0/512.0,b63=575.0/13824.0,b64=44275.0/110592,b65=253.0/4096.0,
    c1=37.0/378.0,c3=250.0/621.0,c4=125.0/594.0,c6=512.0/1771.0,dc5=-277.00/14336;
    rk_real dc1=c1-2825.0/27648.0,dc3=c3-18575.0/48384.0,dc4=c4-13525.0/55296.0,
    dc6=c6-0.25;

    for(i=0;i<N;i++){
        xtemp[i] = x[i] + b21*h*dxdt[i];
        for (j=0; j<N; j++){
            ytemp[i][j]= y[i][j]+b21*h*dydt[i][j];
        }
    }
    dynamic(xtemp,ytemp,u,v,E,a,xk2,yk2,nh,np,host_ind,par_ind);

    for(i=0;i<N;i++){
        xtemp[i] = x[i]+h*(b31*dxdt[i]+b32*xk2[i]);
        for (j=0; j<N; j++){
            ytemp[i][j] = y[i][j]+h*(b31*dydt[i][j]+b32*yk2[i][j]);
        }
    }
    dynamic(xtemp,ytemp,u,v,E,a,xk3,yk3,nh,np,host_ind,par_ind);

    for(i=0;i<N;i++){
        xtemp[i]= x[i]+h*(b41*dxdt[i]+b42*xk2[i]+b43*xk3[i]);
        for (j=0; j<N; j++){
            ytemp[i][j] = y[i][j]+h*(b41*dydt[i][j]+b42*yk2[i][j]+b43*yk3[i][j]);
        }
    }
    dynamic(xtemp,ytemp,u,v,E,a,xk4,yk4,nh,np,host_ind,par_ind);

    for(i=0;i<N;i++){
        xtemp[i]= x[i]+h*(b51*dxdt[i]+b52*xk2[i]+b53*xk3[i]+b54*xk4[i]);
        for (j=0; j<N; j++){
            ytemp[i][j] = y[i][j]+h*(b51*dydt[i][j]+b52*yk2[i][j]+b53*yk3[i][j]+b54*yk4[i][j]);
        }
    }
    dynamic(xtemp,ytemp,u,v,E,a,xk5,yk5,nh,np,host_ind,par_ind);

    for(i=0;i<N;i++){
        xtemp[i]= x[i]+h*(b61*dxdt[i]+b62*xk2[i]+b63*xk3[i]+b64*xk4[i]+b65*xk5[i]);
        for (j=0; j<N; j++){
            ytemp[i][j] = y[i][j]+h*(b61*dydt[i][j]+b62*yk2[i][j]+b63*yk3[i][j]+b64*yk4[i][j]+b65*yk5[i][j]);
        }
    }
    dynamic(xtemp,ytemp,u,v,E,a,xk6,yk6,nh,np,host_ind,par_ind);

    for(i=0;i<N;i++){
        xout[i]= x[i]+h*(c1*dxdt[i]+c3*xk3[i]+c4*xk4[i]+c6*xk6[i]);
        xerr[i]= h*(dc1*dxdt[i]+dc3*xk3[i]+dc4*xk4[i]+dc5*xk5[i]+dc6*xk6[i]);
        for (j=0; j<N; j++){
            yout[i][j]= y[i][j]+h*(c1*dydt[i][j]+c3*yk3[i][j]+c4*yk4[i][j]+c6*yk6[i][j]);
            yerr[i][j]= h*(dc1*dydt[i][j]+dc3*yk3[i][j]+dc4*yk4[i][j]+dc5*yk5[i][j]+dc6*yk6[i][j]);
        }
    }
}

/**************************************************************************
 * Population dynamics function - could expand the model to allow more
 * user-defined inputs
 *************************************************************************/
void range::dynamic(rk_real *x, rk_real **y, rk_real *u, rk_real *v, rk_real **E, rk_real *a, rk_real *dxdt, rk_real **dydt, int nh, int np, int *host_ind, int *par_ind){

	int i,j;
	rk_real xsum, ysum;
	rk_real betsum[N], gamsum[N], ystrain[N];
	//rk_real parsum[N][N];

    xsum=ysum=0;

    for (i=0; i<N; i++) ystrain[i]=0;

	for (i=0; i<N; i++) {
		xsum+=x[i];
		gamsum[i]=0;
		betsum[i]=0;
        dxdt[i]=0;
		for (j=0; j<N; j++) {
            dydt[i][j]=0;
			ystrain[j]+=y[i][j];
			ysum+=y[i][j];
			parsum[i][j]=0;
		}
	}

	for (i=0; i<nh; i++) {
		for (j=0; j<np; j++) {
			betsum[host_ind[i]]+=E[host_ind[i]][par_ind[j]]*x[host_ind[i]]*ystrain[par_ind[j]];
			parsum[host_ind[i]][par_ind[j]]+=E[host_ind[i]][par_ind[j]]*x[host_ind[i]]*ystrain[par_ind[j]];
            gamsum[host_ind[i]]+=GAM*y[host_ind[i]][par_ind[j]];
		}
	}

    /* These are the ODEs to solve */
    for (i=0; i<nh; i++) {
		dxdt[host_ind[i]] = a[host_ind[i]]*x[host_ind[i]]-Q*(xsum+ysum)*x[host_ind[i]]-B*x[host_ind[i]]-betsum[host_ind[i]];
        for (j=0; j<np; j++) {
            dydt[host_ind[i]][par_ind[j]] = parsum[host_ind[i]][par_ind[j]]-(ALPHA+B+GAM)*y[host_ind[i]][par_ind[j]];
        }
    }
    for (i=0; i<N; i++) {
        dxdt[i] += gamsum[i];
    }
}

/***************************************
 * Simple max function
 ***************************************/
rk_real range::FMAX(rk_real l,rk_real r)
{
    if(l>r)return l;
    else   return r;
}

/***************************************
 * Simple min function
 ***************************************/
rk_real range::FMIN(rk_real l,rk_real r)
{
    if(l<r)return l;
    else   return r;
}
