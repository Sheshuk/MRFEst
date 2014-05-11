#include <iostream>
#include <TMath.h>
#include "stdio.h"

#include "Dem.h"

using namespace std;
///-----------------------------------------------------------------------
Dem::Dem(const char* fname){

//   x_det = 10005.4;
//   y_det =  3872.8;
//   z_det =  1284.0;
  x_det = 0;
  y_det = 0;
  z_det = 0;
  
  FILE *in = fopen(fname,"r");
  ReadDem(in);
  fclose(in);

}
///-----------------------------------------------------------------------
void Dem::ReadDem(FILE *in)
{
  double a,b,c;
  for(int i=Nbinx;i>=0;i--) 
    for(int j=Nbiny;j>=0;j--)
      {
	x[j][i]=0;
	y[j][i]=0;
	z[j][i]=0;
      }
  
  for(int j=0;j<Nbinx;j++) 
    for(int i=0;i<Nbiny;i++)
      {

	fscanf(in,"%lf %lf %lf",&a,&b,&c);
	if(i==0 && j==0){
	  GPS_xy[0]=a;
	  GPS_xy[1]=b;
	}
	a-=GPS_xy[0];
	b-=GPS_xy[1];
	
	x[i][j]=a;
	y[i][j]=b;
	z[i][j]=c;
	if(j==0 && i==0){
	  xmin=a; xmax=a;
	  ymin=b; ymax=b;
	}
	else{
	  if(a>xmax)xmax=a;
	  if(a<xmin)xmin=a;
	  if(b>ymax)ymax=b;
	  if(b<ymin)ymin=b;
	}
	
      }
   printf("x in [%2.1f %2.1f] y in [%2.1f %2.1f]\n",xmin,xmax,ymin,ymax);
}
///-----------------------------------------------------------------------

double Dem::GetX(int i){
  return (xmax-xmin)*(i/(double)Nbinx)+xmin;
}
///-----------------------------------------------------------------------
double Dem::GetY(int j){
  return (ymax-ymin)*(j/(double)Nbiny)+ymin;
}
///-----------------------------------------------------------------------
double Dem::GetXindex(double x){
  return (Nbinx-1.)*(x-xmin)/(xmax-xmin);
}
///-----------------------------------------------------------------------
double Dem::GetYindex(double y){
  return (Nbiny-1.)*(y-ymin)/(ymax-ymin);
}
///-----------------------------------------------------------------------

// double Dem::GetThickness(double phi, double theta)//phi, theta are in degrees
// {
//   double X,Y,Z;
//   
//   int indx,indy;
//   indx = (int)GetXindex(x_det);
//   indy = (int)GetYindex(y_det);
// //   printf("1) x in [%2.1f %2.1f] y in [%2.1f %2.1f]\n",xmin,xmax,ymin,ymax);
//   //double s1,s2;
//   double stot = 0,step = 1; 
//   double stp[3]={cos(Rad(phi)),sin(Rad(phi)),tan(Rad(theta))};
//   printf("X,Y,Z=%2.4f %2.4f %2.4f\t zsurf=%2.4f\n",x_det,y_det,z_det,z[indx][indy]);
//   printf("step=%2.4f %2.4f %2.4f\n",stp[0],stp[1],stp[2]);
//   for(double s = 8000; s > -1; s-= step)//distance on the xy plane
//     {
//       X = x_det + s*step*stp[0];
//       Y = y_det + s*step*stp[1];
//       Z = z_det + s*step*stp[2];
// 
//       indx = (int)GetXindex(X);
//       indy = (int)GetYindex(Y);
// //       printf("dX=%2.4f\n",(X-xmin)/(xmax-xmin));
// //       printf("dY=%2.4f\n",(Y-ymin)/(ymax-ymin));
// //       printf("ind [%2.1f %2.1f]= [%d %d]\n",X,Y,indx,indy);
//       if(indx>Nbinx||indy>Nbiny) continue;
//       if(indx<0||indy<0)      continue;
// //       printf("Z vs zmap= %2.1 vs %2.1\n",Z,z[indx][indy]);
//       if(z[indx][indy] > Z)
// 	{
// 	  stot += step;
// 	}
//       
//     }
// 
//   //stot -= 10;
//   if(stot<0) stot = 0;
// 
//   return stot*sqrt(1+stp[2]*stp[2]);
// }
///-----------------------------------------------------------------------
double Dem::GetCrdZ(double x, double y){
  if(x<xmin || x>=xmax || y<ymin || y>=ymax)return -9999;
  int indx=(int)GetXindex(x);
  int indy=(int)GetYindex(y);
  return z[indx][indy];
}
///-----------------------------------------------------------------------
double Dem::GetThickness(double phi, double theta)//phi, theta are in degrees
{
//   printf("phi=%2.2f theta=%2.2f\n",phi,theta);
  while(phi> 180)phi-=360;
//   printf("phi=%2.2f\n",phi);
  while(phi<-180)phi+=360;
//   printf("phi=%2.2f\n",phi);
  while(theta> 90)theta-=180;
//   printf("theta=%2.2f\n",theta);
  while(theta<-90)theta+=180;
//   printf("theta=%2.2f\n",theta);
  
  double crd[3]={x_det,y_det,z_det};
  double stp[3]={cos(TMath::DegToRad()*(phi)),sin(TMath::DegToRad()*(phi)),tan(TMath::DegToRad()*(theta))};
//   printf("stp=[%3.3f %3.3f %3.3f]\n",stp[0],stp[1],stp[2]);
  //   printf("1) x in [%2.1f %2.1f] y in [%2.1f %2.1f]\n",xmin,xmax,ymin,ymax);
  //double s1,s2;
//   double smax = 5000,step = 1; 
  double step=0.1;
  double stot=0;
  double zcrd;
  bool bordr=true;
  while(crd[0]>xmin && crd[0]<xmax && crd[1]>ymin && crd[1]<ymax && crd[2]>0)
    {
      for(int i=0;i<3;++i)crd[i]+=step*stp[i];
      step*=1.01;
      zcrd=GetCrdZ(crd[0],crd[1]);
      printf("at [%2.1f %2.1f] z=%2.4f vs %2.4f surf (%s)\n",crd[0],crd[1],crd[2],zcrd,(crd[2]<zcrd)?"below!":"outside");
      if(zcrd<crd[2]){stot+=step;}else{ bordr=false; break;}
/*      if(zcrd>crd[2]){
	stot += step;
      }
      */
    }
    if(bordr) return 0;
    return stot*sqrt(1.+stp[2]*stp[2]);
}

