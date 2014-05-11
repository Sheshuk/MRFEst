#include <iostream>
#include "stdio.h"
#include "stdlib.h"

#include "Flux.h"
#include "TMath.h"
#include "TGraph.h"

using namespace std;

Flux::Flux()
{
  flux = new float[41];
  flux [0] = 0;
  flux [1] = 4.48406e-09;
  flux [2] = 6.69936e-09;
  flux [3] = 9.7282e-09;
  flux [4] = 1.35708e-08;
  flux [5] = 1.8185e-08;
  flux [6] = 2.35119e-08;
  flux [7] = 2.94904e-08;
  flux [8] = 3.60622e-08;
  flux [9] = 4.31732e-08;
  flux [10] = 5.07731e-08;
  flux [11] = 5.88151e-08;
  flux [12] = 6.7255e-08;
  flux [13] = 7.60506e-08;
  flux [14] = 8.51617e-08;
  flux [15] = 9.45495e-08;
  flux [16] = 1.04176e-07;
  flux [17] = 1.14006e-07;
  flux [18] = 1.24002e-07;
  flux [19] = 1.34129e-07;
  flux [20] = 1.44355e-07;
  flux [21] = 1.54645e-07;
  flux [22] = 1.64966e-07;
  flux [23] = 1.75287e-07;
  flux [24] = 1.85576e-07;
  flux [25] = 1.95802e-07;
  flux [26] = 2.05936e-07;
  flux [27] = 2.15948e-07;
  flux [28] = 2.25809e-07;
  flux [29] = 2.35491e-07;
  flux [30] = 2.44968e-07;
  flux [31] = 2.54213e-07;
  flux [32] = 2.63201e-07;
  flux [33] = 2.71906e-07;
  flux [34] = 2.80306e-07;
  flux [35] = 2.88378e-07;
  flux [36] = 2.96098e-07;
  flux [37] = 3.03448e-07;
  flux [38] = 3.10406e-07;
  flux [39] = 3.16953e-07;
  flux [40] = 3.23072e-07;

  Emin = 10.;//GeV
}

void Flux::SetHondaFlux1()
{
  
  float p,mplus,mminus;
  
  TGraph FluI(101);
  TGraph FluTot(20);
  float flux_int=0;
  float theta=0;
  
  for(int i=0;i<20;i++){
    char name[100];
    sprintf(name,"./data/fluxHonda/cz%02i.mflx",i+1);
    printf("reading file %s\n",name);
    FILE *inf = fopen(name,"r");
    int np=0;
    while(!feof(inf))
    {
      fscanf(inf,"%f %f %f",&p,&mplus,&mminus);
      FluI.SetPoint(np++,p,(p>Emin)?(mplus+mminus):0);
    }
    fclose(inf);
    flux_int=FluI.Integral()*(TMath::DegToRad()*TMath::DegToRad())/10000.;
    theta=90-TMath::RadToDeg()*(acos(1.-0.05*i)+acos(1.-0.05*(i+1)))/2;
//    printf("theta[%d] = %2.4f\n",i,theta);
    FluTot.SetPoint(i,theta,flux_int);
  }
  for (int n=0;n<41;++n){
    flux [n]=FluTot.Eval(n);
  }
}

void Flux::SetHondaFlux()
{
  
  float p1,mplus1,mminus1;
  float p2,mplus2,mminus2;
  
  float ff[20];
  
  for(int i=0;i<20;i++)
    ff[i] = 0;


  //loop, starting from horizontal muons
  for(int i=20;i>0;i--)
    {
      char name[100];
      sprintf(name,"./data/fluxHonda/cz%02i.mflx",i);
  
      FILE *in = fopen(name,"r");
      //cout << " Reading " << name ;

      printf(" Minimum energy for integration = %f \n",Emin);

      fscanf(in,"%f %f %f",&p1,&mplus1,&mminus1);
      while(!feof(in))
	{
	  fscanf(in,"%f %f %f",&p2,&mplus2,&mminus2);

	  if(p1>Emin)//cut on minimum momentum
	    ff[20-i] += ((mplus1+mminus1+mplus2+mminus2)/2.)*TMath::Abs(p2-p1); //integral by trapezoid method
	  
	  //scambio posto
	  p1 = p2;
	  mplus1 = mplus2;
	  mminus1 = mminus2;
	}
      fclose(in);

      //conversion muons/sr/m^2/sec --> muons/deg^2/cm^2/sec
      ff[20-i] *= (TMath::Pi()/180.)*(TMath::Pi()/180.)/10000.; 

      cout << i <<" : Emin = " << Emin << " GeV  :  Integral Flux = " << ff[20-i] << endl;
    }
  


  for(int i=0;i<20;i++)  //loop on cos(theta) intervals
    {
      //i==0 -> horizontal muons
      
      float thetamin = 90.-180.*acos(0.05*i)/3.14;     //degrees
      float thetamax = 90.-180.*acos(0.05*(i+1.))/3.14;//degrees
      
      //cout << " thetamin = " << thetamin << " ,  thetamax = " << thetamax << endl;

      if(i==0) thetamin = 0;
      //i==0 -> horizontal muons

      for(int j=0;j<41;j++) //loop on theta (degrees)
	{
	  if(j>=thetamin
	     &&j<thetamax)
	    {
	      flux[j] = ff[i];
	    }
	}
    }
}


float Flux::GetFlux(float theta){
  if(theta <  0)theta=-theta;
  if(theta > 40)return 0;
  
  int itheta=(int)TMath::Floor(theta);
  float x=theta-itheta;
  float res=flux[itheta]+(flux[itheta+1]-flux[itheta])*x;
//   return flux[itheta];
  return res;

}



void Flux::Print(){

  for(int j=0;j<41;j++) //loop on theta (degrees)
    {
      printf("FLUX[%d degrees] = %2.20g\n",j,flux[j]);
    }
//       cout << j << " degrees , flux =  " << flux[j] << endl;
//     }

}
