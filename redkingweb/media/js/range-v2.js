/*************************************************
 Fast range coevolution
 *************************************************/

/***********************************
 * Model parameters
 ***********************************/
var A=5.5 /* Maximum per-capita host birth rate*/
var B=0.5 /* Natural mortality */
var G=20.0 /* Range function parameter */
var F=0.0 /* Impact of disease on fecundity */
var Q=0.5 /* Density-dependence on birth rate */
var ALPHA=1.5 /* Disease-associated mortality */
var BETA=17.0 /* Transmission rate */
var GAMMA=0.5 /* Recovery rate */
var CH1=0.85 /* Host cost parameter 1 */
var CP1=0.97 /* Parasite cost parameter 1 */
var CH2=3.0 /* Host cost parameter 2 (>0:accelerating, =0:linear, <0:decelerating) */
var CP2=-1.0 /* Parasite cost parameter 2 (>0:accelerating, =0:linear, <0:decelerating) */

var K0 = 0.1
var K1 = 0.1
var K2 = 0.1
var K3 = 0.1

var parasite_colour = "#ff8400";
var host_colour = "#6fb3c8";

var transmission_function = "universal";
var transmission_type = "transmission";

/******************************************
 * Parameters for solver
 ******************************************/
var MAXTIME=10 /* Length of simulation */
var N=101 /* Number of host and parasite phenotypes */
var HSTART=0.05 /* Initial host phenotype */
var PSTART=0.05 /* Initial parasite phenotype */
var NEVOL=500 /* Number of iterations (evolutionary timesteps) - ideally this needs to be higher */
var EPSILON=1e-3 /* Extinction tolerance */
var WHO=0.5 /* Controls relative mutation rates */

/***********************************
 * Runge-Kutta parameters
 ***********************************/
var MAXSTEPS=1e4 //1e4 /* Maximum number of steps for ODE solver */
var EPS=1e-2 /* ODE solver tolerance */
var TINY=1e-6 /* Constant value for solver */
var b21=0.2
var b31=3.0/40.0
var b32=9.0/40.0
var b41=0.3
var b42=-0.9
var b43=1.2
var b51=-11.0/54.0
var b52=2.5
var b53=-70.0/27.0
var b54=35.0/27.0
var b61=1631.0/55296
var b62=175.0/512.0
var b63=575.0/13824.0
var b64=44275.0/110592
var b65=253.0/4096.0
var c1=37.0/378.0
var c3=250.0/621.0
var c4=125.0/594.0
var c6=512.0/1771.0
var dc5=-277.00/14336

/***************************************
 * Main program
 ***************************************/

var _u = OneDArray(N)
var _v = OneDArray(N)
var _a = OneDArray(N)
var _alpha = OneDArray(N)
var _E = TwoDArray(N,N) 

function range_init() {            
    /* Initialise discretised trait values */
    for (var i=0; i<N; i++) {
	_u[i]=i/(N-1); /* Host */
	_v[i]=_u[i]; /* Parasite */
    }
}

function recalc_cost_functions() { 
    var CP = OneDArray(N)
    var CH = OneDArray(N)

    /* Cost functions */
    for (i=0; i<N; i++) {
        if(CH2==0){
            CH2 = 0.001;
        }
        if(CP2==0){
            CP2 = 0.001;
        }
	CH[i] = FMAX(0,1-CH1*(1-Math.exp(CH2*_u[i]))/(1-Math.exp(CH2)));
	CP[i] = FMAX(0,1-CP1*(1-Math.exp(CP2*_v[i]))/(1-Math.exp(CP2)));

        //CH[i] = FMAX(0,1-(CH1-1)*(1-Math.exp(CH2*_u[i]))/(1-Math.exp(CH2)));
        //CP[i] = FMAX(0,1-(CP1-1)*(1-Math.exp(CP2*_v[i]))/(1-Math.exp(CP2)));
        _a[i] = A*CH[i];
    }    

    plot_tradeoff(CH,"host_tradeoff_canvas","host",host_colour,"host trait","host cost");
    plot_tradeoff(CP,"parasite_tradeoff_canvas","parasite",parasite_colour,"parasite trait","parasite cost");
    
    /* Define host-parasite interaction matrix */
    /* User picks one of the following through transmission function and parasite cost options */  

    if (transmission_function == "range") {
	if (transmission_type == "transmission") {  
	    /* If Range && transmission cost */
	    for (i=0; i<N; i++){
		_alpha[i] = ALPHA;
		for (j=0; j<N; j++){
		    _E[i][j] = CP[j]*BETA/(1+Math.exp(K0*(_u[i]-_v[j])));
		}
	    }
	} else {
	    /* If Range && virulence cost*/
	    for (i=0; i<N; i++){
		_alpha[i] = ALPHA*(1-CP[i]);
		for (j=0; j<N; j++){
		    _E[i][j] = BETA/(1+Math.exp(K0*(_u[i]-_v[j])));
		}
	    }
	}
    }
    
    if (transmission_function == "matching") {
	if (transmission_type == "transmission") {      
	    /* If Matching && transmission cost */
	    for (i=0; i<N; i++){
		_alpha[i] = ALPHA;
		for (j=0; j<N; j++){
		    temp = (_u[i]-_v[j])/(K1*(_v[j]+K2));
		    _E[i][j] = CP[j]*BETA*(Math.exp(-temp*temp));
		}
	    }
	} else {
	    /* If Matching && virulence cost */
	    for (i=0; i<N; i++){
		_alpha[i] = ALPHA*(1-CP[i]);
		for (j=0; j<N; j++){
		    temp = (_u[i]-_v[j])/(K1*_v[j]+K2);
		    _E[i][j] = BETA*(Math.exp(-temp*temp));
		}
	    }
	}
    }

    if (transmission_function == "universal") {
	/* If Universal - virulence cost only */
	for (i=0; i<N; i++){
            _alpha[i] = ALPHA*(1-CP[i]);
            for (j=0; j<N; j++){
		_E[i][j] = BETA*FMAX(0,(1-K3*_u[i])*_v[j]);
            }
	}
    }
/*
    for (var i=0; i<N; i++){
	_alpha[i] = ALPHA;
	for (var j=0; j<N; j++){
            _E[i][j] = CP[j]*BETA/(1+Math.exp(G*(_u[i]-_v[j])));
	}
    }   
*/
    plot_matrix(_E,"matrix_canvas",parasite_colour);
    plot_heatmap(_E,"heatmap_canvas");

}

function safelog10(n) {
    if (n==0) return 0
    else return FMIN(1,Math.abs(Math.log10(0)))
}

/***************************************
 * Adaptive dynamics function
 ***************************************/

function range(xout, u, v, E, a) {
    
    var x0 = OneDArray(N)
    var y = OneDArray(N)
    var y0 = TwoDArray(N,N)
    var rtype, r1, r2, xtotal, xcum, temp, htemp, ptemp;
    var host_ind = OneDArray(N)
    var par_ind = OneDArray(N)
    var mflag = 0
    var i, j, evol_count=0, nh, np, mutator, pop_choice, HSTART0, PSTART0, Hflag, Pflag;
    
    this.get_host = function() { return x0; }
    this.get_parasite = function() { return y; }

    Hflag=0;
    Pflag=0;
    HSTART0=0;
    PSTART0=0;
    /* Initialise population numbers */    
    for (i=0; i<N; i++)	{
	x0[i]=0.0;
	y[i]=0.0;
	for (j=0; j<N; j++) {
	    y0[i][j]=0.0;
	}
        if(Hflag==0 && u[i]>=HSTART){
            HSTART0=i;
            Hflag=1;
        }
        if(Pflag==0 && v[i]>=PSTART){
            PSTART0=i;
            Pflag=1;
        }
        if(Hflag>0 && Pflag>0) break;
    }  
    x0[HSTART0]=1.0;
    y0[HSTART0][PSTART0]=1.0;
    y[PSTART0]=y0[HSTART0][PSTART0];    
    nh = 1;
    np = 1;
    host_ind[0] = HSTART0;
    par_ind[0] = PSTART0;

    this.run = function() {        
        /* Display progress */
        //if(evol_count%100==0){
        //console.log("Timer="+evol_count+". Hosts:"+nh+". Parasites:"+np)
	//}
        
        /* Call ODE solver */
        my_rungkut(x0,y0,E,a,nh,np,host_ind,par_ind);
        
        /* Check for extinct phenotypes */
        for (j=0; j<N; j++) y[j]=0;
        for (i=0; i<nh; i++) {
            if (x0[host_ind[i]]<EPSILON) x0[host_ind[i]]=0;
            for (j=0; j<np; j++) {
                if (y0[host_ind[i]][par_ind[j]]<EPSILON){
                    y0[host_ind[i]][par_ind[j]]=0;
                }
                else{
                    /* Work out parasite phenotype densities */
                    y[par_ind[j]]+=y0[host_ind[i]][par_ind[j]];
                }
            }
        }
        
        /* Record which phenotypes are present */
        nh = 0;
        np = 0;        
        for (i=0; i<N; i++)	{
            temp=x0[i];
            for (j=0; j<N; j++) temp+=y0[i][j];                
            if(temp>0){
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
            /* Store densities */
            for (j=evol_count;j<NEVOL;j++){
                for (i=0; i<N; i++) {
                    xout[i][j]=0;
                    xout[i+N][j]=0;
                }
            }

	    show_host_extinct();	    
            return;
        }
        if(np==0){
            /* Store densities */
            for (j=evol_count;j<NEVOL;j++){
                for (i=0; i<N; i++) {
                    xout[i][j]=x0[i];
                    xout[i+N][j]=0;
                }
            }            

	    show_parasite_extinct();	    
            return;
        }
        
        /* Store densities */
        for (i=0; i<N; i++) {
            xout[i][evol_count]=x0[i];
            xout[i+N][evol_count]=y[i];
        }
        
        /* Mutation routine */
        rtype=Math.random()
        if (rtype<WHO) pop_choice=0;
        else pop_choice=1;
        
        r1=Math.random()
        xtotal = 0.0;
        xcum = 0.0;
        if (pop_choice==0){
            for (i=0; i<nh; i++) xtotal+=x0[host_ind[i]];
            for (i=0; i<nh; i++) {
                xcum+=x0[host_ind[i]];
                if (r1 < xcum/xtotal) {	/* Find which strain will mutate */
                    mutator=host_ind[i];
                    break;
                }
            }
        } else{
            for (j=0; j<np; j++) xtotal+=y[par_ind[j]];
            for (j=0; j<np; j++) {
                xcum+=y[par_ind[j]];
                if (r1 < xcum/xtotal) {	/* Find which strain will mutate */
                    mutator=par_ind[j];
                    break;
                }
            }
        }
        
        r2=Math.random()
        if (r2<0.5 && mutator>0){	/* Mutate up or down? */
            if (pop_choice==0) {
                x0[mutator-1]+=x0[mutator]/10.0;
                for (j=0; j<np; j++) {
                    y0[mutator-1][par_ind[j]]+=y0[mutator][par_ind[j]]/10.0;
                }
                mflag=1;
                for(i=0;i<nh;i++){
                    if(host_ind[i]==(mutator-1)){
                        mflag=0;
                        break;
                    }
                }
                if(mflag>0){
                    host_ind[nh]=mutator-1;
                    nh++;
                }
            }
            else {
                for (i=0; i<nh; i++) y0[host_ind[i]][mutator-1]+=y0[host_ind[i]][mutator]/10.0;
                mflag=1;
                for(j=0;j<np;j++){
                    if(par_ind[j]==(mutator-1)){
                        mflag=0;
                        break;
                    }
                }
                if(mflag>0){
                    par_ind[np]=mutator-1;
                    np++;
                }
            }
        }
        else if (r2>0.5 && mutator<N-1){
            if (pop_choice==0) {
                x0[mutator+1]+=x0[mutator]/10.0;
                for (j=0; j<np; j++) {
                    y0[mutator+1][par_ind[j]]+=y0[mutator][par_ind[j]]/10.0;
                }
                mflag=1;
                for(i=0;i<nh;i++){
                    if(host_ind[i]==(mutator+1)){
                        mflag=0;
                        break;
                    }
                }
                if(mflag>0){
                    host_ind[nh]=mutator+1;
                    nh++;
                }
            }
            else {
                for (i=0; i<nh; i++) y0[host_ind[i]][mutator+1]+=y0[host_ind[i]][mutator]/10.0;
                mflag=1;
                for(j=0;j<np;j++){
                    if(par_ind[j]==(mutator+1)){
                        mflag=0;
                        break;
                    }
                }
                if(mflag>0){
                    par_ind[np]=mutator+1;
                    np++;
                }
            }
	}

	evol_count += 1	
    }    
}


/*****************************************
 * ODE solver
 ****************************************/
function my_rungkut (x, y, E, a, nh, np, host_ind, par_ind){
    
    var i,j,exitflag,count;
    var maxsteps,t;
    var hnext=[0]
    var h=[1]
    var dxdt = OneDArray(N)
    var dydt = TwoDArray(N,N)
    var xscale = OneDArray(N)
    var yscale = TwoDArray(N,N)
    
    /* Other params */
    exitflag = 0;
    count=0;
    h[0] = 1e-3;
    hnext[0] = 1e-3;
    t=0;
    
    /* Main loop: */
    do {
	//console.log("main loop")

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
        dynamic(x,y,E,a,dxdt,dydt,nh,np,host_ind,par_ind);
        
        /* Adjust the step size to maintain accuracy */
        for (i=0; i<nh; i++){
            xscale[host_ind[i]]=Math.abs(x[host_ind[i]])+Math.abs(dxdt[host_ind[i]]*(h[0]))+TINY;
            for (j=0; j<np; j++){
                yscale[host_ind[i]][par_ind[j]]=Math.abs(y[host_ind[i]][par_ind[j]])+Math.abs(dydt[host_ind[i]][par_ind[j]]*(h[0]))+TINY;
            }
        }
        rkqs(x,y,dxdt,dydt,h,hnext,xscale,yscale,E,a,nh,np,host_ind,par_ind);
        count++;
        
        for (i=0; i<nh; i++){
            x[host_ind[i]] = FMAX(0,x[host_ind[i]]);
            for (j=0; j<np; j++){
                y[host_ind[i]][par_ind[j]] = FMAX(0,y[host_ind[i]][par_ind[j]]);
            }
        }
    } while(count<(MAXSTEPS-1) && t<=MAXTIME && exitflag==0);
}

/*****************************************
 * This generates the adaptive step-size
 ****************************************/
function rkqs(x,y,dxdt,dydt,h,hnext,xscale,yscale,E,a,nh,np,host_ind,par_ind)
{
    //console.log("rkqs")

    var xtemp = OneDArray(N)
    var ytemp = TwoDArray(N,N)
    var xerr = OneDArray(N)
    var yerr = TwoDArray(N,N)
    var htemp,errmax;
    var i,j;
    
    for(;;)
    {
        rkck(x,y,dxdt,dydt,xtemp,ytemp,xerr,yerr,h[0],E,a,nh,np,host_ind,par_ind);
        errmax= 0.0;
        for(i=0;i<nh;i++){
            errmax= FMAX(errmax, Math.abs(xerr[host_ind[i]]/xscale[host_ind[i]]));
            for(j=0;j<np;j++){
                errmax= FMAX(errmax, Math.abs(yerr[host_ind[i]][par_ind[j]]/yscale[host_ind[i]][par_ind[j]]));
            }
        }
        errmax/= EPS;
        if(errmax<=1.0) break;
        htemp= 0.9*(h[0])*Math.pow(errmax,-0.25);
        h[0]= (h[0]>=0.0 ? FMAX(htemp,0.1*(h[0])) : FMIN(htemp,0.1*(h[0])));
    }
    if(errmax > 1.89E-4) {
        hnext[0]= 0.9*(h[0])*Math.pow(errmax,-0.2);
    }
    else {
        hnext[0]= 5.0*(h[0]);
    }
    
    for(i=0;i<nh;i++){
        x[host_ind[i]]= xtemp[host_ind[i]];
        for(j=0;j<np;j++){
            y[host_ind[i]][par_ind[j]]= ytemp[host_ind[i]][par_ind[j]];
        }
    }
}

/*****************************************
 * This is the standard RK solver
 ****************************************/
function rkck(x,y,dxdt,dydt,xout,yout,xerr,yerr,h,E,a,nh,np,host_ind,par_ind)
{
    //console.log("rkck")

    var i,j;
    var xk1 = OneDArray(N)
    var xk2 = OneDArray(N)
    var xk3 = OneDArray(N)
    var xk4 = OneDArray(N)
    var xk5 = OneDArray(N)
    var xk6 = OneDArray(N)
    var yk1 = TwoDArray(N,N)
    var yk2 = TwoDArray(N,N)
    var yk3 = TwoDArray(N,N)
    var yk4 = TwoDArray(N,N)
    var yk5 = TwoDArray(N,N)
    var yk6 = TwoDArray(N,N)
    var xtemp = OneDArray(N)
    var ytemp = TwoDArray(N,N)
    var dc1=c1-2825.0/27648.0,dc3=c3-18575.0/48384.0,dc4=c4-13525.0/55296.0,dc6=c6-0.25;
    
    for(i=0;i<nh;i++){
        xtemp[host_ind[i]] = x[host_ind[i]] + b21*h*dxdt[host_ind[i]];
        for (j=0; j<np; j++){
            ytemp[host_ind[i]][par_ind[j]]= y[host_ind[i]][par_ind[j]]+b21*h*dydt[host_ind[i]][par_ind[j]];
        }
    }
    dynamic(xtemp,ytemp,E,a,xk2,yk2,nh,np,host_ind,par_ind);
    
    for(i=0;i<nh;i++){
        xtemp[host_ind[i]] = x[host_ind[i]]+h*(b31*dxdt[host_ind[i]]+b32*xk2[host_ind[i]]);
        for (j=0; j<np; j++){
            ytemp[host_ind[i]][par_ind[j]] = y[host_ind[i]][par_ind[j]]+h*(b31*dydt[host_ind[i]][par_ind[j]]+b32*yk2[host_ind[i]][par_ind[j]]);
        }
    }
    dynamic(xtemp,ytemp,E,a,xk3,yk3,nh,np,host_ind,par_ind);
    
    for(i=0;i<nh;i++){
        xtemp[host_ind[i]]= x[host_ind[i]]+h*(b41*dxdt[host_ind[i]]+b42*xk2[host_ind[i]]+b43*xk3[host_ind[i]]);
        for (j=0; j<np; j++){
            ytemp[host_ind[i]][par_ind[j]] = y[host_ind[i]][par_ind[j]]+h*(b41*dydt[host_ind[i]][par_ind[j]]+b42*yk2[host_ind[i]][par_ind[j]]+b43*yk3[host_ind[i]][par_ind[j]]);
        }
    }
    dynamic(xtemp,ytemp,E,a,xk4,yk4,nh,np,host_ind,par_ind);
    
    for(i=0;i<nh;i++){
        xtemp[host_ind[i]]= x[host_ind[i]]+h*(b51*dxdt[host_ind[i]]+b52*xk2[host_ind[i]]+b53*xk3[host_ind[i]]+b54*xk4[host_ind[i]]);
        for (j=0; j<np; j++){
            ytemp[host_ind[i]][par_ind[j]] = y[host_ind[i]][par_ind[j]]+h*(b51*dydt[host_ind[i]][par_ind[j]]+b52*yk2[host_ind[i]][par_ind[j]]+b53*yk3[host_ind[i]][par_ind[j]]+b54*yk4[host_ind[i]][par_ind[j]]);
        }
    }
    dynamic(xtemp,ytemp,E,a,xk5,yk5,nh,np,host_ind,par_ind);
    
    for(i=0;i<nh;i++){
        xtemp[host_ind[i]]= x[host_ind[i]]+h*(b61*dxdt[host_ind[i]]+b62*xk2[host_ind[i]]+b63*xk3[host_ind[i]]+b64*xk4[host_ind[i]]+b65*xk5[host_ind[i]]);
        for (j=0; j<np; j++){
            ytemp[host_ind[i]][par_ind[j]] = y[host_ind[i]][par_ind[j]]+h*(b61*dydt[host_ind[i]][par_ind[j]]+b62*yk2[host_ind[i]][par_ind[j]]+b63*yk3[host_ind[i]][par_ind[j]]+b64*yk4[host_ind[i]][par_ind[j]]+b65*yk5[host_ind[i]][par_ind[j]]);
        }
    }
    dynamic(xtemp,ytemp,E,a,xk6,yk6,nh,np,host_ind,par_ind);
    
    for(i=0;i<nh;i++){
        xout[host_ind[i]]= x[host_ind[i]]+h*(c1*dxdt[host_ind[i]]+c3*xk3[host_ind[i]]+c4*xk4[host_ind[i]]+c6*xk6[host_ind[i]]);
        xerr[host_ind[i]]= h*(dc1*dxdt[host_ind[i]]+dc3*xk3[host_ind[i]]+dc4*xk4[host_ind[i]]+dc5*xk5[host_ind[i]]+dc6*xk6[host_ind[i]]);
        for (j=0; j<np; j++){
            yout[host_ind[i]][par_ind[j]]= y[host_ind[i]][par_ind[j]]+h*(c1*dydt[host_ind[i]][par_ind[j]]+c3*yk3[host_ind[i]][par_ind[j]]+c4*yk4[host_ind[i]][par_ind[j]]+c6*yk6[host_ind[i]][par_ind[j]]);
            yerr[host_ind[i]][par_ind[j]]= h*(dc1*dydt[host_ind[i]][par_ind[j]]+dc3*yk3[host_ind[i]][par_ind[j]]+dc4*yk4[host_ind[i]][par_ind[j]]+dc5*yk5[host_ind[i]][par_ind[j]]+dc6*yk6[host_ind[i]][par_ind[j]]);
        }
    }
}

/**************************************************************************
 * Population dynamics function 
 *************************************************************************/
function dynamic(x, y, E, a, dxdt, dydt, nh, np, host_ind, par_ind){

    //console.log("dynamic")
	
    var i,j;
    var xsum, ysum;
    var hostsum = OneDArray(N)
    var gamsum = OneDArray(N)
    var ystrain = OneDArray(N)
    var parsum = TwoDArray(N,N)
	
	xsum=ysum=0;
    for (j=0; j<np; j++) ystrain[par_ind[j]]=0;	
    for (i=0; i<nh; i++) {
		xsum+=x[host_ind[i]];
		for (j=0; j<np; j++) {
			ystrain[par_ind[j]]+=y[host_ind[i]][par_ind[j]];
			ysum+=y[host_ind[i]][par_ind[j]];
		}
	}
    
	for (i=0; i<nh; i++) {
		hostsum[host_ind[i]]=0;
		gamsum[host_ind[i]]=0;
		for (j=0; j<np; j++) {
			parsum[host_ind[i]][par_ind[j]]=E[host_ind[i]][par_ind[j]]*x[host_ind[i]]*ystrain[par_ind[j]];
            hostsum[host_ind[i]]+=parsum[host_ind[i]][par_ind[j]];			
            gamsum[host_ind[i]]+=GAMMA*y[host_ind[i]][par_ind[j]];
		}
	}
    
    /* These are the ODEs to solve */
    for (i=0; i<nh; i++) {
	dxdt[host_ind[i]] = a[host_ind[i]]*x[host_ind[i]]-Q*(xsum+ysum)*x[host_ind[i]]-B*x[host_ind[i]]-hostsum[host_ind[i]]+gamsum[host_ind[i]];
        for (j=0; j<np; j++) {
            //dydt[host_ind[i]][par_ind[j]] = parsum[host_ind[i]][par_ind[j]]-(ALPHA+B+GAMMA)*y[host_ind[i]][par_ind[j]];
	    dydt[host_ind[i]][par_ind[j]] = parsum[host_ind[i]][par_ind[j]]-(_alpha[par_ind[j]]+B+GAMMA)*y[host_ind[i]][par_ind[j]];
        }
    }
}

/***************************************
 * Simple max function
 ***************************************/
function FMAX(l, r)
{
    if(l>r)return l;
    else   return r;
}

/***************************************
 * Simple min function
 ***************************************/
function FMIN(l, r)
{
    if(l<r)return l;
    else   return r;
}


