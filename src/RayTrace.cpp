#include <cmath>
#include "RayTrace.h"
#include "NiceLog.h"
int Vlev=0;
#define _NiceVlev Vlev

///-----------------------------------------------------------------------------
double RayTrace::GetThickness(){
  ///get thickness (sum of distance below ground) in given direction
  double R=1;
  double Thick=0;
  int n=0;
  while(R!=0){
    R=GetDistance();
    _Log(2,"returned R=%2.4f\n",R);
    if(R>0)Thick+=R;
    _Log(2,"%d) R=%2.2f, Thick=%2.2f\n",n++,R,Thick);
    _LogCmd(1,fPos.print("Dist pos"));
  }
  _Log(2,"Thick=%2.4f\n",Thick);
  return Thick;
}
///-----------------------------------------------------------------------------
double RayTrace::find_crossing(vect3 p0, vect3 p1, double dz0, double dz1){
  const double precision=0.01;
  double xi=dz1/dz0;
  vect3 p=(-xi*p0+p1)/(1-xi);
  double dz=(p[2]-fLand->Zf(p[0],p[1]));
  if(dz<precision)return 0;
  return dz;  
}
///-----------------------------------------------------------------------------

double RayTrace::GetDistance(){
  /// propagate to next intersection point
  vect3 ray=fPos;
  _LogCmd(2,fPos.print("Initial position"));
  _LogCmd(2,fDir.print("Direction"));
//   fDir.print("Direction");
  double dz0=(ray[2]-fLand->Zf(ray[0],ray[1]));
  ///define, where are we - inside or outside.
  int side0=0, side1=0; 
  if(dz0>0)side0=1; else if(dz0<0)side0=-1;
  
  _Log(3,"dz0=%2.4f\n",dz0);
  double dz1=0;
  while(fLand->isInLims(ray[0],ray[1])){
    ray=ray+fDir;
    dz1=(ray[2]-fLand->Zf(ray[0],ray[1]));
    
    //define side
    side1=0; if(dz1>0)side1=1; else if(dz1<1)side1=-1;
    if(side0==0)side0=side1;
    
    if(side0!=side1)break;
  }
  _Log(2,"Stopped: %2.2f->%2.2f (side#%d->#%d)\n",dz0,dz1,side0,side1);
  if(side0==side1)return 0; ///outside of DEM
  
  vect3 dst=ray-fPos;
  fPos=ray;
  
  double R=sqrt(dst.m2());
  _Log(2,"Dist [%2.2f->%2.2f] = %2.4f\n",dz0,dz1,R);
  return -R*side0;
}
