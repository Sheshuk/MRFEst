#include <iostream>
#include "math.h"
#include "stdio.h"

#define PI 3.141592653589793

const int Nbinx=491;
const int Nbiny=453;
using namespace std;

class Dem{
 public:
  Dem(const char* fname);
  void ReadDem(FILE *in);
  double GetX(int i);
  double GetY(int j);
  double GetZ(int ix,int iy){return z[ix][iy];};
 
  double GetXindex(double x);
  double GetYindex(double y);
  double GetThickness(double phi, double theta);
  
  void ShiftXdet(double dx){ x_det += dx; };
  void ShiftYdet(double dy){ y_det += dy; };
  void ShiftZdet(double dz){ z_det += dz; };
  
  void SetDetPosition(double x, double y, double z){x_det=x-GPS_xy[0]; y_det=y-GPS_xy[1]; z_det=z;}
  double GetCrdZ(double x, double y);
  double X_gps(){return GPS_xy[0];}
  double Y_gps(){return GPS_xy[1];}
 private:
  //dem info
  double x[Nbinx][Nbiny];
  double y[Nbinx][Nbiny];
  double z[Nbinx][Nbiny];
  
  //detector position
  /*  static const double x_det = 10005.4; */
  /*  static const double y_det =  3872.8; */
  /*  static const double z_det =  1284.0; */
  double x_det;
  double y_det;
  double z_det;
  
  double GPS_xy[2];


  //edges of dem map
  double xmin,xmax;
  double ymin,ymax;
  
};
