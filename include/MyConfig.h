#ifndef __MYCONFIG__H
#define __MYCONFIG__H
#include <string>
#include "Terrain.h"
///-----------------------------------------
class Config{
public:
struct {
  double Size[2];
  std::string  Name;
  double FoV;
}WND;

struct {
  std::string ShotName;
  std::string FluxFile;
  std::string FluxPlot;
}SAVE;

struct {
  double Pos[3];
  double Ang[3];
  double TXmax;
  double Size[2];
  double Surf;
  double ExpDays;
  double ExpSec(){return ExpDays*86400;}
  double Exposure(){return ExpSec()*Surf;}
}DET;

struct {
  int TX_Nb,TY_Nb;
  double TX_Lim[2];
  double TY_Lim[2];
  int N_iter;
  
  double TX_BinSize(){return (TX_Lim[2]-TX_Lim[1])/TX_Nb;}
  double TY_BinSize(){return (TY_Lim[2]-TY_Lim[1])/TY_Nb;}
}HIST;

struct {
 std::string gPhoto;
 std::string gMap;
 std::string gCompas;
 std::string gDet;
 std::string gSky;
 double MapC[2];
 double MapS[2];
}TEX;
///-----------------------------------------    
struct {
  std::string File;
  TerrFmt Format;
}DEM;
///-----------------------------------------
Config(char* fname=0){Init(); if(fname)Read(fname);}
void Init();
void Read(char* fname);
void Print();
};


#endif

