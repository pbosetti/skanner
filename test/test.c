/***************************************************
 * Automatically generated by Maple.
 * Created On: Wed Mar 07 14:32:12 2012.
***************************************************/
#ifdef WMI_WINNT
#define EXP __declspec(dllexport)
#else
#define EXP
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mplshlib.h>
static MKernelVector kv;
EXP ALGEB M_DECL SetKernelVector(MKernelVector kv_in, ALGEB args) { kv=kv_in; return(kv->toMapleNULL()); }

/***************************************************
* Variable Definition for System:

* State variable(s):
*    x[ 0] = `Main.'SlidingMass1::TF1'.v_rel`(t)
*    x[ 1] = `Main.Position.value`(t)
*
* Output variable(s):
*    y[ 0] = `Main.Position.value`(t)
*    y[ 1] = `Main.'SlidingMass1::Velocity'`(t)
*    y[ 2] = `Main.'SlidingMass1::M1'.a`(t)
*
* Input variable(s):
*    u[ 0] = `Main.'SlidingMass1::Force'`(t)
*
* Parameter(s):
*    p[ 0] = `Main.'SlidingMass1::M'` (default = 1.)
*
************************************************/

/* Fixed parameters */
#define NDIFF 2
#define NDFA 2
#define NEQ 8
#define NPAR 1
#define NINP 1
#define NDISC 0
#define NIX1 6
#define NOUT 3
#define NCON 0
#define NEVT 0
#ifdef EVTHYST
#define NZC 2*NEVT
#else
#define NZC NEVT
#endif

typedef struct {
  double h;    /* Integration step size */
  double *w;   /* Float workspace */
  long *iw;    /* Integer workspace */
  long err;    /* Error flag */
  char *buf;   /* Error message */
} SolverStruct;

static void SolverError(SolverStruct *S, char *errmsg)
{
  sprintf(S->buf,"Error at t=%20.16e: %s\n",S->w[0],errmsg);
  if(S->err==-1) kv->error(S->buf);
  S->err=1;
}

static double dsn_zero=0.0;
static unsigned char dsn_undefC[8] = { 0, 0, 0, 0, 0, 0, 0xF8, 0x7F };
static double *dsn_undef = (double *)&dsn_undefC;
static unsigned char dsn_posinfC[8] = { 0, 0, 0, 0, 0, 0, 0xF0, 0x7F };
static double *dsn_posinf = (double *)&dsn_posinfC;
static unsigned char dsn_neginfC[8] = { 0, 0, 0, 0, 0, 0, 0xF0, 0xFF };
static double *dsn_neginf = (double *)&dsn_neginfC;
#define trunc(v) ( (v>0.0) ? floor(v) : ceil(v) )


static void fp(long N, double T, double *Y, double *YP)
{
  double m, v;

  if( Y[0]>=0. ) {
    m = 1.;
    v = 1.;
  }
  else {
    m = 1.;
    v = -1.;
  }
  Y[4] = v/m;
  m = 1.;
  v = fabs(Y[0]);
  Y[5] = v/m;
  m = 1.;
  v = -Y[0];
  YP[1] = v/m;
  if( Y[5]<0.01 ) {
    m = 1.;
    v = 2452.41970901797979*Y[0];
  }
  else {
    m = 1.;
    v = 5.*Y[4]*(exp(-10.*Y[5])+4.)+0.001*Y[0];
  }
  Y[3] = v/m;
  m = Y[8];
  v = Y[7]+Y[3];
  Y[2] = v/m;
  m = 1.;
  v = -Y[2];
  YP[0] = v/m;
}

static void otp(double T, double *Y, double *YP)
{
  Y[6] = -Y[0];
}

static void inpfn(double T, double *U)
{
  U[0] = 0.;
}

static void SolverUpdate(double *u, double *p, long first, long internal, SolverStruct *S)
{
  long i;

  inpfn(S->w[0],u);
  for(i=0; i<NINP; i++) S->w[i+NDIFF+NIX1-NINP+1]=u[i];
  if(p) for(i=0; i<NPAR; i++) S->w[i+NEQ+1]=p[i];
  fp(NEQ,S->w[0],&S->w[1],&S->w[NEQ+NPAR+1]);
  if(S->w[NEQ+NPAR+1]-S->w[NEQ+NPAR+1]!=0.0) {
    SolverError(S,"index-1 and derivative evaluation failure");
    return;
  }
  if(internal) return;
}

static void SolverOutputs(double *y, SolverStruct *S)
{
  otp(S->w[0],&S->w[1],&S->w[NEQ+NPAR+1]);
  y[ 0]=S->w[ 2];
  y[ 1]=S->w[ 7];
  y[ 2]=S->w[ 3];
}

static void EulerStep(double *u, SolverStruct *S)
{
  long i;

  S->w[0]+=S->h;
  for(i=1;i<=NDIFF;i++) S->w[i]+=S->h*S->w[NEQ+NPAR+i];
  SolverUpdate(u,NULL,0,0,S);
}

static void SolverSetup(double t0, double *ic, double *u, double *p, double *y, double h, SolverStruct *S)
{
  long i;

  S->h = h;
  S->iw=NULL;
  S->w[0] = t0;
  S->w[1] =  0.00000000000000000e+00;
  S->w[2] =  0.00000000000000000e+00;
  S->w[3] =  1.00000000000000000e+02;
  S->w[4] =  0.00000000000000000e+00;
  S->w[5] =  1.00000000000000000e+00;
  S->w[6] =  0.00000000000000000e+00;
  S->w[7] = -0.00000000000000000e+00;
  S->w[8] =  1.00000000000000000e+02;
  S->w[9] =  1.00000000000000000e+00;
  if(ic) for(i=0;i<NDIFF;i++) {
    S->w[i+1]=ic[i];
    S->w[i+NEQ+NPAR+1]=0.0;
  }
  SolverUpdate(u,p,1,0,S);
  SolverOutputs(y,S);
}

/*
  Parametrized simulation driver
*/
EXP long M_DECL ParamDriverC(double t0, double dt, long npts, double *ic, double *p, double *out, char *errbuf, long internal)
{
  double u[NINP],y[NOUT],w[1+2*NEQ+NPAR+NDFA+NEVT];
  long i,j;
  SolverStruct S;

  /* Setup */
  for(i=0;i<npts*(NOUT+1);i++) out[i]=*dsn_undef;
  S.w=w;
  if(internal==0) S.err=0; else S.err=-1;
  S.buf=errbuf;
  SolverSetup(t0,ic,u,p,y,dt,&S);
  /* Output */
  out[0]=t0; for(j=0;j<NOUT;j++) out[j+1]=y[j];
  /* Integration loop */
  for(i=1;i<npts;i++) {
    /* Take a step with states */
    EulerStep(u,&S);
    if( S.err>0 ) break;
    /* Output */
    SolverOutputs(y,&S);
    out[i*(NOUT+1)]=S.w[0]; for(j=0;j<NOUT;j++) out[i*(NOUT+1)+j+1]=y[j];
  }

  return(i);
}

EXP ALGEB M_DECL ParamDriver( MKernelVector kv_in, ALGEB *args )
{
  double t0,tf,dt,*ic,*p,*out;
  M_INT nargs,bounds[4],npts,naout,i;
  RTableSettings s;
  ALGEB outd;
  char buf[1000];

  kv=kv_in;
  nargs=kv->numArgs((ALGEB)args);
  if( nargs<5 || nargs>6 )
    kv->error("incorrect number of arguments");

  /* Process time vals */
  if( !kv->isNumeric(args[1]) )
    kv->error("argument #1, the initial time, must be numeric");
  t0=kv->mapleToFloat64(args[1]);
  if( !kv->isNumeric(args[2]) )
    kv->error("argument #2, the final time, must be numeric");
  tf=kv->mapleToFloat64(args[2]);
  if( t0>=tf )
    kv->error("the final time must be larger than the initial time");
  if( !kv->isNumeric(args[3]) )
    kv->error("argument #3, the time step, must be a positive numeric value");
  dt=kv->mapleToFloat64(args[3]);
  if(dt<=0)
    kv->error("argument #3, the time step, must be a positive numeric value");
  npts=(M_INT)ceil((tf+1e-10-t0)/dt)+1;

  /* Processing ic in */
  if( NDIFF==0 )
    ic=NULL;
  else if( kv->isInteger(args[4]) && kv->mapleToInteger32(args[4])==0 )
    ic=NULL;
  else if( !kv->isRTable(args[4]) ) {
    ic=NULL;
    kv->error("argument #4, the initial data, must be a 1..ndiff rtable");
  }
  else {
    kv->rtableGetSettings(&s,args[4]);
    if( s.storage != RTABLE_RECT || s.data_type != RTABLE_FLOAT64 ||
       s.num_dimensions != 1 || kv->rtableLowerBound(args[4],1)!=1 ||
       kv->rtableUpperBound(args[4],1) != NDIFF )
      kv->error("argument #4, the initial data, must be a 1..ndiff rtable");
    ic=(double *)kv->rtableData(args[4]);
  }

  /* Processing parameters in */
  if( NPAR==0 )
    p=NULL;
  else if( kv->isInteger(args[5]) && kv->mapleToInteger32(args[5])==0 )
    p=NULL;
  else if( !kv->isRTable(args[5]) ) {
    p=NULL;
    kv->error("argument #5, the parameter data, must be a 1..npar rtable");
  }
  else {
    kv->rtableGetSettings(&s,args[5]);
    if( s.storage != RTABLE_RECT || s.data_type != RTABLE_FLOAT64 ||
       s.num_dimensions != 1 || kv->rtableLowerBound(args[5],1)!=1 ||
       kv->rtableUpperBound(args[5],1) != NPAR )
      kv->error("argument #5, the parameter data, must be a 1..npar rtable");
    p=(double *)kv->rtableData(args[5]);
  }

  /* Output data table */
  if( nargs==6 ) {
    outd=NULL;
    if( !kv->isRTable(args[6]) ) {
      out=NULL;
      naout=0;
      kv->error("argument #6, the output data, must be a 1..npts,1..nout+1 C_order rtable");
    }
    else {
      kv->rtableGetSettings(&s,args[6]);
      if( s.storage != RTABLE_RECT || s.data_type != RTABLE_FLOAT64 ||
         s.order != RTABLE_C || s.num_dimensions != 2 ||
         kv->rtableLowerBound(args[6],1)!=1 ||
         kv->rtableLowerBound(args[6],2)!=1 ||
         kv->rtableUpperBound(args[6],2) != NOUT+1 )
        kv->error("argument #6, the output data, must be a 1..npts,1..nout+1 C_order rtable");
      naout=kv->rtableUpperBound(args[6],1);
      if( naout<1 )
        kv->error("argument #6, the output data, must have at least 1 output slot");
      out=(double *)kv->rtableData(args[6]);
      if(naout<npts) npts=naout;
    }
  }
  else {
    kv->rtableGetDefaults(&s);
    bounds[0]=1; bounds[1]=npts;
    bounds[2]=1; bounds[3]=NOUT+1;
    s.storage=RTABLE_RECT;
    s.data_type=RTABLE_FLOAT64;
    s.order=RTABLE_C;
    s.num_dimensions=2;
    s.subtype=RTABLE_ARRAY;
    outd=kv->rtableCreate(&s,NULL,bounds);
    out=(double *)kv->rtableData(outd);
    naout=npts;
  }
  for(i=0;i<naout*(NOUT+1);i++) out[i]=*dsn_undef;

  i=ParamDriverC(t0,dt,npts,ic,p,out,buf,1);

  /* All done */
  if(outd==NULL)
    return(kv->toMapleInteger(i));
  else
    return(outd);
}
