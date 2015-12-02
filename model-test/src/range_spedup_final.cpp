// Ben Ashby

#include "range_spedup_final.h"

using namespace std;

/***************************************
 * Main program
 ***************************************/
int _main (int argc, char* argv[]) {

    double u[N], v[N], a[N], beta[N], E[N][N], **xout;
    int i, j;
    char filename[100];

    /* Create output file */
    sprintf(filename, "range_spedup_final.txt");
    std::cout << "filename:" << filename << "\n";
    std::ifstream infile(filename);
    if(infile.good()) {
        std::cout << "File already exists, deleting file...\n";
        remove(filename);
        std::cout << "Deleted\n";
    }
    std::ofstream out(filename, std::ios::app);
    if (!out){
        std::cout << "Cannot create file\n";
        exit(1);
    }

    /* Initialise discretised trait values */
	for (i=0; i<N; i++) {
		u[i]=UMIN+(UMAX-UMIN)*i/(N-1); /* Host */
		v[i]=VMIN+(VMAX-VMIN)*i/(N-1); /* Parasite */
	}

    /**********************************************************************
     * This section is where the trade-offs and host-parasite interactions
     * are defined (i.e. where the user would make changes via a GUI)
     *********************************************************************/

    /* Define cost functions (trade-offs) */
	for (i=0; i<N; i++) {
		a[i]=AMAX-(AMAX-AMIN)*(1-(u[i]-UMAX)/(UMIN-UMAX))/(1+A_P*(u[i]-UMAX)/(UMIN-UMAX)); /* Host trade-off */
		beta[i]=BEMAXTIME-(BEMAXTIME-BETMIN)*(1-(v[i]-VMAX)/(VMIN-VMAX))/(1+BETA_P*(v[i]-VMAX)/(VMIN-VMAX)); /* Parasite trade-off */
	}

    /* Define host-parasite interaction matrix */
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            E[i][j] = beta[j]*(1-1/(1+exp(-2*(u[i]-v[j]))));
        }
    }

    /**********************************************************************
     * End of user-defined section
     *********************************************************************/

    /* Call adaptive dynamics routine (main solver) */
    xout = array_maker(2*N+2,NEVOL);
    ad(xout,u,v,E,a,beta);

    /* Output to file */
    for (i=0; i<NEVOL; i++) {
        for (j=0; j<(N*2+2); j++) {
            out << xout[j][i] << " ";
        }
        out << "\n";
    }
    free_array(xout,2*N+2);
}

/***************************************
 * Adaptive dynamics function
 ***************************************/
void ad(double **xout, double *u, double *v, double E[][N], double *a, double *beta){

    double x0[N], y[N], y0[N][N];
    double  rtype, r1, r2, xtotal, xcum, temp;
    int host_ind[N], par_ind[N];
    int i, j, evol_count, nh, np, mutator, pop_choice;

    /* Initialise population numbers */
	for (i=0; i<N; i++)	{
		x0[i]=0.0;
		y[i]=0.0;
		for (j=0; j<N; j++) {
			y0[i][j]=0.0;
		}
	}
    x0[HSTART-1]=1.0;
	y0[HSTART-1][PSTART-1]=1.0;
    y[PSTART-1]=y0[HSTART-1][PSTART-1];

    /* Main loop */
    for (evol_count=0; evol_count<NEVOL; evol_count++) {

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

        /* Display progress */
        if(evol_count%10==0){
            std::cout << "Timer=" << evol_count << ". Hosts:" << nh << ". Parasites:" << np << "\n";
        }

        /* Check if all hosts or parasites have been driven extinct */
        if(nh==0){
            for (i=0; i<N; i++) {
                x0[i]=0;
                y[i]=0;
            }
            printf("Breaking - hosts driven extinct\n");
            break;
        }
        if(np==0){
            for (i=0; i<N; i++) {
                y[i]=0;
            }
            printf("Breaking - parasites driven extinct\n");
            break;
        }

        /* Call ODE solver */
        my_rungkut(x0,y0,u,v,E,a,nh,np,host_ind,par_ind);

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
        for (i=0; i<N; i++) {
            xout[i][evol_count]=x0[i];
            xout[i+N][evol_count]=y[i];
        }

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
    }
}


/*****************************************
 * ODE solver
 ****************************************/
void my_rungkut (double *x, double y[][N], double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind){

    int i,j,exitflag,count;
    double maxsteps,t,nextcheck;
    double hnext[1], h[1], dxdt[N], dydt[N][N], xscale[N], yscale[N][N], xmax[N], xmin[N], ymax[N][N], ymin[N][N];

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
void rkqs(double *x,double y[][N],double *dxdt,double dydt[][N],double *h,double *hnext,double *xscale,double yscale[][N],double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind)
{
    double xtemp[N], ytemp[N][N], xerr[N], yerr[N][N];
    double htemp,errmax;
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
void rkck(double *x,double y[][N],double *dxdt,double dydt[][N],double *xout,double yout[][N],double *xerr,double yerr[][N],double h, double *u, double *v, double E[][N], double *a, int nh, int np, int *host_ind, int *par_ind)
{
    int i,j;
	double xk1[N], xk2[N], xk3[N], xk4[N], xk5[N], xk6[N];
	double yk1[N][N], yk2[N][N], yk3[N][N], yk4[N][N], yk5[N][N], yk6[N][N];
	double xtemp[N], ytemp[N][N];
    static double b21=0.2,b31=3.0/40.0,b32=9.0/40.0,b41=0.3,b42=-0.9,b43=1.2,
    b51=-11.0/54.0,b52=2.5,b53=-70.0/27.0,b54=35.0/27.0,b61=1631.0/55296,
    b62=175.0/512.0,b63=575.0/13824.0,b64=44275.0/110592,b65=253.0/4096.0,
    c1=37.0/378.0,c3=250.0/621.0,c4=125.0/594.0,c6=512.0/1771.0,dc5=-277.00/14336;
    double dc1=c1-2825.0/27648.0,dc3=c3-18575.0/48384.0,dc4=c4-13525.0/55296.0,
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
void dynamic(double *x, double y[][N], double *u, double *v, double E[][N], double *a, double *dxdt, double dydt[][N], int nh, int np, int *host_ind, int *par_ind){

	int i,j;
	double xsum, ysum;
	double betsum[N], gamsum[N], ystrain[N];
	double parsum[N][N];

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
double FMAX(double l,double r)
{
    if(l>r)return l;
    else   return r;
}

/***************************************
 * Simple min function
 ***************************************/
double FMIN(double l,double r)
{
    if(l<r)return l;
    else   return r;
}

/***************************************
 * Make 2D double array
 ***************************************/
double** array_maker(int rows, int cols) {

    double** new_array;
    new_array = (double**) malloc(rows*sizeof(double*));
    for (int i = 0; i < rows; i++)
        new_array[i] = (double*) malloc(cols*sizeof(double));

    return new_array;
}

/***************************************
 * Free 2D double array
 ***************************************/
void free_array(double **array, int rows) {

    for (int i = 0; i < rows; i++) free(array[i]);
    free(array);
}
