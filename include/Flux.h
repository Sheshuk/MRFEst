#include <iostream>
#include "stdio.h"

using namespace std;

class Flux{
 public:
  Flux();
  void  SetHondaFlux();
  void  SetHondaFlux1();
  float GetFlux(float theta);
  void  SetEmin(float e){Emin = e;};
  void  Print();

 private:
  float *flux;
  float Emin;//minimum E in flux intergration 

};
