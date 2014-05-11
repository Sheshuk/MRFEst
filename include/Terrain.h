#ifndef __TERRAIN__H
#define __TERRAIN__H

#ifdef _USE_OPENGL //define this to use in OPENGL applications.
  #include <GL/gl.h>
  #include <GL/glu.h>
#else 
typedef double GLdouble;
typedef unsigned int GLuint;
#endif

#include <cassert>
#include <iostream>

static int Terrain_VLev=6;
class vect3{
public:
  vect3(){for(int i=0;i<3;++i)c[i]=0;}
  vect3(double x,double y, double z){Set(x,y,z);}
  ~vect3(){};
  vect3(const vect3& v){for(int i=0;i<3;++i)c[i]=v.c[i];}
  double & operator[](int n){assert(n>=0 && n<3); return c[n];}
  const double& operator[] (int n) const{assert(n>=0 && n<3); return c[n];} 
  
  vect3 & operator+=(const vect3& v){for(int i=0;i<3;++i)c[i]+=v[i]; return *this;}
  vect3 & operator-=(const vect3& v){for(int i=0;i<3;++i)c[i]-=v[i]; return *this;}
  vect3 & operator*=(const double f){for(int i=0;i<3;++i)c[i]*=f; return *this;}
  vect3 & operator/=(const double f){for(int i=0;i<3;++i)c[i]/=f; return *this;}
        
  void Set(double x,double y, double z){c[0]=x; c[1]=y; c[2]=z;}
  double normalize();
  double m2(){return c[0]*c[0]+c[1]*c[1]+c[2]*c[2];}
  void print(const char* name){printf("%s = [%2.2f %2.2f %2.2f]\n",name,c[0],c[1],c[2]);}
private:
  double c[3];
};

vect3 operator +(vect3 v,vect3 u);
vect3 operator -(vect3 v,vect3 u);
double operator *(vect3 v,vect3 u);
vect3 operator *(vect3 v,double w);
vect3 operator /(vect3 v,double w);
vect3 operator *(double w,vect3 v);
vect3 operator ^(vect3 v,vect3 u);

struct TerrFmt{
   //reading format
   enum {eXY, eYX} Order;
   bool Inv[2];
   int NX,NY;
   //methods:
   bool Read(char* s);
   bool DoInvertNx(){return Inv[0];}
   bool DoInvertNy(){return Inv[1];}
   
   int Ind_2d1d(int nx,int ny);
   void Ind_1d2d(int nn, int& nx,int& ny);
      
   void Print(){
    printf("Format=\t");
    if(eXY)printf("%s:%s",Inv[0]?"!X":" X",Inv[1]?"!Y":" Y");
    if(eYX)printf("%s:%s",Inv[1]?"!Y":" Y",Inv[0]?"!X":" X");
    printf("\n");
   }
   
  };
///--------------------------------------------------------
class Terrain{

public:
  
  Terrain();
  ~Terrain();
  bool Load(char* fname, TerrFmt fmt);
  void Write(char* fname);
  bool Render();
  void CalcIndices(int lod=1);
  void CalcNormals();
  void Set0();
  void SetCrd();
  void Clear();
  double X(int n){if(n<0||n>gNvts)return 0; return gVts[3*n+0];}
  double X(int nx,int ny){return X(Ind_2d1d(nx,ny));}
  double Xcrd(int n){return (gVts[3*n]-Xmin())/Xsize();}
  int Nx(){return gNvtX;}
  double Y(int n){if(n<0||n>gNvts)return 0; return gVts[3*n+1];}
  double Y(int nx,int ny){return Y(Ind_2d1d(nx,ny));}
  double Ycrd(int n){return (gVts[3*n+1]-Ymin())/Ysize();}
  int Ny(){return gNvtY;}
  double Z(int n){if(n<0||n>gNvts)return 0; return gVts[3*n+2];}
  double Z(int nx,int ny){return Z(Ind_2d1d(nx,ny));}
  double Zi(double nx,double ny);
  double Zf(double x,double y);
  double ZfG(double x,double y){return Zf(x-Xmin(),y-Ymin());}
  
  double Xmin(){return gLims[0];}
  double Xmax(){return gLims[1];}
  double Ymin(){return gLims[2];}
  double Ymax(){return gLims[3];}
  double Xmean(){return (gLims[1]+gLims[0])/2.;}
  double Ymean(){return (gLims[3]+gLims[2])/2.;}
  double Xsize(){return  (gLims[1]-gLims[0]);}
  double Ysize(){return  (gLims[3]-gLims[2]);}
  double Lim(int n){return gLims[n];};
  double X_gps(){return gGps[0];}
  double Y_gps(){return gGps[1];}
  int N(){return gNvts;}
  bool isInLims(double x,double y){return (x>Xmin()&&x<Xmax()&&y>Ymin()&&y<Ymax());}

  void PrintLims(){printf("Land x=[%2.1f %2.1f] y=[%2.1f %2.1f] z=[%2.1f %2.1f], GPS=[%2.1f %2.1f]\n",gLims[0],gLims[1],gLims[2],gLims[3],gLims[4],gLims[5],gGps[0],gGps[1]);}
  vect3 Pnt(int n){return vect3(X(n),Y(n),Z(n));}
  vect3 Pnt(int nx, int ny){return Pnt(Ind_2d1d(nx,ny));}
//   int Index(int nx,int ny){return nx*gNvtY+ny;}
  //int Ind_2d1d(int nx,int ny){return ny*gNvtX+nx;}
  int Ind_2d1d(int nx,int ny){return gFormat.Ind_2d1d(nx,ny);}
  double indX(double x){double r=(x-Xmin())*(Nx()-1)/Xsize(); return gFormat.Inv[0]?(Nx()-r):r;}
  double indY(double y){double r=(y-Ymin())*(Ny()-1)/Ysize(); return gFormat.Inv[1]?(Ny()-r):r;}

  void Ind_1d2d(int nn, int& nx,int& ny){gFormat.Ind_1d2d(nn,nx,ny);}
  void SetNrm(int nx,int ny, vect3 v){int c=Ind_2d1d(nx,ny); for(int i=0;i<3;++i) gNrm[3*c+i]=v[i];};
  void SetGpsTex(double x0,double y0,double x1,double y1){gGpsTex[0]=x0; gGpsTex[1]=y0;gGpsTex[2]=x1; gGpsTex[3]=y1;}
  void Add(Terrain *t,double c=1);
  void Shift(double dx, double dy, double dz);
  void MakeHole(double x, double y, double R);
private:
  GLdouble * gVts;
  GLdouble * gNrm;
  GLdouble * gCrd;
  GLdouble * gCol;
  int gNvtX,gNvtY,gNvts;
  
  int gNstripes;
  int gStripeSize;
  
  double gLims[6];
  double gGps[2];
  double gGpsTex[4];
  GLuint ** gInd;
  bool gInvertAxes[2];
  TerrFmt gFormat; 
};

#endif
