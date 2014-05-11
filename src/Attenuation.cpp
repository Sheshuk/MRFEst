#include <iostream>
#include <stdio.h>

#include "TFile.h"
#include "TTree.h"

#include "Attenuation.h"

using namespace std;

Attenuation::Attenuation(const char* AttTreeFile){

  TFile file(AttTreeFile,"READ");
  TTree *tree = 0;
  tree = (TTree*)file.Get("tree");
  
  double l;
  double mu_surv[41];
  mu_surv[0]=0;
  //tree->SetBranchAddress("mu_surv89v",mu_surv+1);
  tree->SetBranchAddress("lengthv",&l);
  for(int i=1;i<=40;i++) tree->SetBranchAddress(Form("mu_surv%iv",90-i),mu_surv+i);//89
 
  int entries = tree->GetEntries(); 

  for(int i=0;i<entries;i++)
    {
      tree->GetEntry(i);
      ll[i] = l;
      for(int j=1;j<=40;j++)
	{
	  mu[j][i] = mu_surv[j];
// 	  printf("mu[%d][%d]=%2.4g\n",j,i,mu[j][i]);
	}
    }
  
  file.Close();
  
}

double Attenuation::GetAttenuation(double theta, double thickness)
{
  if(thickness==0)return 1;
  if(thickness>4000)return 0;
  
  float ftheta=theta+0.5;
  float fthick=0.1*thickness;
  int itheta = int(ftheta) ;
  int ithick = int(fthick);
  
  //bilinear interpolation
  float dx=ftheta-itheta;
  float dy=fthick-ithick;
  if(itheta<0)   return 0;
  if(itheta>39)   return 0;
  float z00=mu[itheta  ][ithick  ];
  float z01=mu[itheta  ][ithick+1];
  float z10=mu[itheta+1][ithick  ];
  float z11=mu[itheta+1][ithick+1];
  //printf("z=[%4.4f %4.4f %4.4f %4.4f]\n");
  float Z=z00*(1-dx)*(1-dy)+z01*(1-dx)*dy+z10*dx*(1-dy)+z11*dx*dy;
//  printf("Att [%4.4f %4.4f] = %4.4g\n",theta,thickness,Z);
  return Z;
}
