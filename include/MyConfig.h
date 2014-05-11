#ifndef __MYCONFIG__H
#define __MYCONFIG__H
//#include <cstring>
#include "Terrain.h"
///-----------------------------------------
class Config{
public:
struct {
  double Size[2];
  char  Name[50];
  double FoV;
}WND;

struct {
  char ShotName[30]= "shot_N";
  char FluxFile[30]= "flux_N";
  char FluxPlot[30]= "plot_N";
}SAVE;

struct {
  double Pos[3];
  double Ang[3];
  double TXmax;
  double Size[2];
  double Surf=1e4;
  double ExpDays=1;
  double ExpSec(){return ExpDays*86400;}
  double Exposure(){return ExpSec()*Surf;}
}DET;

struct {
  int TX_Nb=120;
  double TX_Lim[2]={-0.6, 0.6};
  
  int TY_Nb=120;
  double TY_Lim[2]={-0.6, 0.6};
  
  double TX_BinSize(){return (TX_Lim[2]-TX_Lim[1])/TX_Nb;}
  double TY_BinSize(){return (TY_Lim[2]-TY_Lim[1])/TY_Nb;}
  int N_iter=10;
}HIST;

static const char STRSIZE=100;
struct {
 char *gPhoto=0;
 char gMap[STRSIZE]="...................";
 char gCompas[STRSIZE]="...................";
 char gDet[STRSIZE]="...................";
 char gSky[STRSIZE]="base/Sky2.jpg";;
 double MapC[2]={0,0};
 double MapS[2]={1,1};
}TEX;
///-----------------------------------------    
struct {
  char File[STRSIZE]="DEM/Strom_2007.xyz";
  TerrFmt Format;
}DEM;
///-----------------------------------------
void Read(char* fname);
void Print();
};


#endif

