#include <iostream>
#include "stdio.h"

using namespace std;

class Attenuation{

 public:
  Attenuation(const char* AttTreeFile);
  double GetAttenuation(double theta, double thickness);
  
 private:
  //attenuation information
  double     ll[402];
  double mu[41][402]; //mu[0][yy] refers to horizontal muons 
  //mu[1][yy] refers to quasi-horizontal muons (89° azimuth angle) 
  //......................

};
