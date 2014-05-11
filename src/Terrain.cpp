#include <cmath>
#include <cstring>
#include "NiceLog.h"
#define _NiceVlev Terrain_VLev

#include "Terrain.h"


vect3 operator +(vect3 v,vect3 u){vect3 r;  for(int i=0;i<3;++i)r[i]=v[i]+u[i]; return r;}
vect3 operator -(vect3 v,vect3 u){vect3 r;  for(int i=0;i<3;++i)r[i]=v[i]-u[i]; return r;}
double operator *(vect3 v,vect3 u){double r=0;for(int i=0;i<3;++i)r+=v[i]*u[i]; return r;}
vect3 operator *(vect3 v,double w){vect3 r;  for(int i=0;i<3;++i)r[i]=v[i]*w; return r;}
vect3 operator /(vect3 v,double w){vect3 r;  for(int i=0;i<3;++i)r[i]=v[i]/w; return r;}
vect3 operator *(double w,vect3 v){return v*w;}
vect3 operator ^(vect3 v,vect3 u){
  vect3 r;
  r[0]=v[1]*u[2]-u[1]*v[2];
  r[1]=v[2]*u[0]-u[2]*v[0];
  r[2]=v[0]*u[1]-u[0]*v[1];
  return r;
}
///-----------------------------------------------------------------------------
double vect3::normalize(){
  double nrm=sqrt(m2());
  if(nrm)for(int i=0;i<3;++i)c[i]/=nrm;
  return nrm;
}
///-----------------------------------------------------------------------------
bool TerrFmt::Read(char* s){
    _Log(1,"Read format from string '\%s\'\n",s);
    char* px=strchr(s,'X');
    char* py=strchr(s,'Y');
    _Log(1,"px=%s, py=%s\n",px,py);
    Order=(px>py)?eYX:eXY;
    Inv[0]=(px>s && *(px-1)=='n');
    Inv[1]=(py>s && *(py-1)=='n');
    _LogCmd(1,Print());
    return true;
}
                                   
///-----------------------------------------------------------------------------

Terrain::Terrain(){
  #ifdef _USE_OPENGL
  _Log(0,"Using opengl!\n");
  #endif
  Set0();
}
///-----------------------------------------------------------------------------
Terrain::~Terrain(){ 
  Clear();
}
///-----------------------------------------------------------------------------
void Terrain::Clear(){
  _Log(2,"Clear terrain\n");
  if(gVts)delete[] gVts;
  if(gNrm)delete[] gNrm; 
  if(gCol)delete[] gCol;
  if(gInd){
    for(int ns=0;ns<gNstripes;++ns)
      if(gInd[ns])delete[]gInd[ns];
    delete[] gInd;
  }
  Set0();
}
///-----------------------------------------------------------------------------
void Terrain::Set0(){
  _Log(2,"Reset terrain\n");
  gVts=gNrm=gCol=0;
  gInd=0;
  gNvts=gNvtX=gNvtY=0;
  gGps[0]=gGps[1]=0;
  gNstripes=gStripeSize=0;
}
///-----------------------------------------------------------------------------
double Terrain::Zi(double nx,double ny){
  if(nx<0 || nx>gNvtX)return 0;
  if(ny<0 || ny>gNvtY)return 0;
  int nnx=floor(nx+.5);
  int nny=floor(ny+.5);
  double z00=Z(nnx,nny);
  double dx=nx-nnx;
  double dy=ny-nny;
  double dnx=dx>0?1:-1;
  double dny=dy>0?1:-1;
  double tx=(Z(nnx+dnx,nny)-z00)/dnx;// /(X(nnx+1,nny)-X(nnx,nny));
  double ty=(Z(nnx,nny+dny)-z00)/dny;// /(Y(nnx,nny+1)-Y(nnx,nny));
  //   printf("z00[%d %d]=%2.4f, dx dy = %g, %g\n",nnx,nny,z00,dx,dy);
  return z00+tx*dx+ty*dy;
}
///-----------------------------------------------------------------------------
double Terrain::Zf(double x,double y){
  double ix=indX(x);
  double iy=indY(y);
  
  //printf("x=%2.1f vs %2.1f\n",x,X(int(ix),int(iy)));
  //printf("y=%2.1f vs %2.1f\n",y,Y(int(ix),int(iy)));
  //printf("dx=%2.1f (%2.1f) dy=%2.1f (%2.1f)\n",x-Xmin(),ix,y-Ymin(),iy);
  return Zi(ix,iy);
}
///-----------------------------------------------------------------------------
void Terrain::Add(Terrain *t, double c){
  int ind=0;
  double dx=t->X_gps()-X_gps();
  double dy=t->Y_gps()-Y_gps();
  printf("dx=%2.1f, dy=%2.1f\n",dx,dy);
  for(int n=0; n<gNvts; ++n){
    ind=3*n;
    gVts[ind+2]+=t->Zf(gVts[ind]-dx,gVts[ind+1]-dy)*c;
    }
}
///-----------------------------------------------------------------------------
void Terrain::Shift(double dx, double dy, double dz){
//   int ind=0;
  gGps[0]+=dx; gGps[1]+=dy;
  gLims[3]+=dz; gLims[4]+=dz;
  
//   for(int n=0; n<gNvts; ++n){
//     ind=3*n;
//     gVts[ind  ]+=dx; 
//     gVts[ind+1]+=dy; 
//     gVts[ind+2]+=dz; 
//   }
}
///-----------------------------------------------------------------------------
void Terrain::MakeHole(double x, double y, double R){
  double r[2]; double dr;
  printf("Make hole at [%2.2f %2.2f] of size %2.2f\n",x,y,R);
  for(int n=0;n<gNvts;++n){
    r[0]=X(n)-x;
    r[1]=Y(n)-y;
    dr=(r[0]*r[0]+r[1]*r[1])/(R*R);
    if(dr<1)gVts[3*n+2]*=sin(dr*1.5708);
  }
}
///-----------------------------------------------------------------------------
void Terrain::Write(char* fname){
  _Log(1,"Saving terrain to file \"%s\"\n",fname);
  FILE* outf=fopen(fname,"w");
  fprintf(outf,"%%Size= %d %d\n",gNvtX,gNvtY);
  gNvts=gNvtX*gNvtY;
  int ind=0;
  for(int nx=0; nx<gNvtX; ++nx)
    for(int ny=0; ny<gNvtY; ++ny){
  //for(int n=0; n<gNvts; ++n){
    ind=3*Ind_2d1d(nx,ny);
    fprintf(outf,"%2.3f %2.3f %2.3f\n",gVts[ind]+gGps[0],gVts[ind+1]+gGps[1],gVts[ind+2]);
  }
  fclose(outf);
}
///-----------------------------------------------------------------------------
void TerrFmt::Ind_1d2d(int nn, int& nx,int& ny){
  switch(Order){
    case eXY:  nx=nn/NY; ny=nn%NY;  break;
    case eYX:  ny=nn/NX; nx=nn%NX;  break;
    default: nx=0; ny=0;
  }
  if(DoInvertNx())nx=NX-1-nx;
  if(DoInvertNy())ny=NY-1-ny;
}
///-----------------------------------------------------------------------------
int TerrFmt::Ind_2d1d(int nx,int ny){
  if(DoInvertNx())nx=NX-1-nx;
  if(DoInvertNy())ny=NY-1-ny;
  switch(Order){
    case eXY: return nx*NY+ny;
    case eYX: return ny*NX+nx;
    default: return 0;
  }
}
///-----------------------------------------------------------------------------

bool Terrain::Load(char* fname, TerrFmt fmt){
 _Log(1,"Loading terrain from file \"%s\"\n",fname);
  FILE* inf=fopen(fname,"r");
  if(inf==NULL){_ErrF(0,"Error opening file \"%s\"\n",fname); return false;}
  if(fscanf(inf,"%%Size= %d %d",&gNvtX,&gNvtY)<2){
    _ErrF(1,"Error reading DEM size\n"); 
    fclose(inf);
    return false;
  }
  
  //define reading format
  TerrFmt rFormat=fmt;
  rFormat.NX=gNvtX;
  rFormat.NY=gNvtY;

  //define writing format
  TerrFmt wFormat=rFormat;
  wFormat.Order=TerrFmt::eXY;
//  wFormat.Inv[0]=false;
//  wFormat.Inv[1]=false;
//  wFormat.NX=gNvtX;
//  wFormat.NY=gNvtY;
  gFormat=wFormat;
  
  _Log(1,"reading DEM of size %dx%d\n",gNvtX,gNvtY);
  gNvts=gNvtX*gNvtY;
  gVts=new GLdouble[gNvts*3];
  gCol=new GLdouble[gNvts*3];
  gNrm=new GLdouble[gNvts*3];
  double x,y,z;
  int ind=0;
  int n=0;
   
  //for(int nx=0; nx<gNvtX; ++nx){
  //for(int ny=0; ny<gNvtY; ++ny){
  int nx,ny;
  for(int nn=0; nn<gNvtX*gNvtY; ++nn){
      rFormat.Ind_1d2d(nn,nx,ny);
      ind=wFormat.Ind_2d1d(nx,ny);
      ind*=3;
      if(fscanf(inf,"%lf %lf %lf",&x,&y,&z)<3){_ErrF(0,"Error reading point #%d in file \"%s\"\n",nn,fname); return false;};
      if(ferror(inf)){_Err(0,"Error while reading file \"%s\"\n",fname); return false;}
      if(n==0){gGps[0]=x; gGps[1]=y;}
      x-=gGps[0]; y-=gGps[1];
      
      gVts[ind]=x;gVts[ind+1]=y; gVts[ind+2]=z;
      
      if(z<=0.3){gCol[ind]=.5;gCol[ind+1]=.7; gCol[ind+2]=1.;}
      else{   gCol[ind]=.9*z/900;gCol[ind+1]=.8*z/900; gCol[ind+2]=.7*z/900.;}
      if(n==0){
	gLims[0]=gLims[1]=x;
	gLims[2]=gLims[3]=y;
	gLims[4]=gLims[5]=z;
      } else{
	if(x<gLims[0])gLims[0]=x;
	if(x>gLims[1])gLims[1]=x;
	if(y<gLims[2])gLims[2]=y;
	if(y>gLims[3])gLims[3]=y;
	if(z<gLims[4])gLims[4]=z;
	if(z>gLims[5])gLims[5]=z;
      }
      n++;
//       ind+=3;
//       printf("[%d %d]= %2.1f %2.1f %2.1f\n",nx,ny,gVts[ind],gVts[ind+1],gVts[ind+2]);
//    }
  }
  SetCrd();
//   for(int n=0; n<3; ++n){
//     printf("%d) %2.1f %2.1f %2.1f\n",n,gVts[3*n],gVts[3*n+1],gVts[3*n+2]);
//   }
  //Shift(Xmin(),Ymin(),0);
  _Log(1,"read sucessfully %d points\n",gNvts);
  _LogCmd(1,PrintLims());
  #ifdef _USE_OPENGL
  glVertexPointer(3,GL_DOUBLE,0,gVts);
  glColorPointer(3,GL_DOUBLE,0,gCol);
  #endif
/*  ///test
  for(int nx=0; nx<5;nx++)
  for(int ny=0; ny<5;ny++){
  double xx=X(nx,ny);
  double yy=Y(nx,ny);
  printf("%d:%d>>>\t %4.1f %4.1f\t<<<%2.1f:%2.1f\n",nx,ny,xx,yy,indX(xx),indY(yy));
  }
  */
  return true;
}
///-----------------------------------------------------------------------------
void Terrain::SetCrd(){
  if(gCrd) delete[]gCrd;
  gCrd=new GLdouble[gNvts*2];
  double xdc=gGps[0]-gGpsTex[0];
  double xlc=gGpsTex[2]-gGpsTex[0];
  double ydc=gGps[1]-gGpsTex[1];
  double ylc=gGpsTex[3]-gGpsTex[1];
  for(int n=0; n<gNvts; ++n){
    gCrd[2*n]  =(X(n)+xdc)/xlc;
    gCrd[2*n+1]=1.-(Y(n)+ydc)/ylc;
    //     printf("crd= %f %f\n",gCrd[2*n],gCrd[2*n+1]);
  }
  #ifdef _USE_OPENGL
  glTexCoordPointer(2,GL_DOUBLE,0,gCrd);
  #endif
}
///-----------------------------------------------------------------------------
void Terrain::CalcIndices(int lod){
  
  ///preapre stripes for drawing
  double Nlod=lod;
  gNstripes=int(gNvtX/Nlod)-1;
  
  gStripeSize=2*int(gNvtY/Nlod);
  
  gInd=new GLuint*[gNstripes];
  for(int ns=0;ns<gNstripes;++ns){
    gInd[ns]=new GLuint[gStripeSize];
    for(int i=0;i<gNvtY/Nlod;++i){
      gInd[ns][2*i]  =   Ind_2d1d(ns*Nlod,i*Nlod);
      gInd[ns][2*i+1]=   Ind_2d1d((ns+1)*Nlod,i*Nlod);
//         gInd[ns][2*i]  =   int(ns*Nlod+ gNvtX*i*Nlod);
//         gInd[ns][2*i+1]=   int(gInd[ns][2*i]+Nlod);
    }
  }
}
///-----------------------------------------------------------------------------
void Terrain::CalcNormals(){
  vect3 o,a,b,n(0,0,0);
  for(int nx=1; nx<gNvtX-1; ++nx)
    for(int ny=1; ny<gNvtY-1; ++ny){
      o=Pnt(nx,ny);
      
      a=Pnt(nx  ,ny+1)-o;
      b=Pnt(nx+1,ny+1)-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx+1 ,ny)-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx+1 ,ny-1)-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx ,ny-1)-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx-1,ny-1)-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx-1,ny  )-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx-1,ny+1 )-o;
      n= n+(b^a);
      
      a=b;
      b=Pnt(nx  ,ny+1)-o;
      n= n+(b^a);
      
      n.normalize();
//        a.print("a");
//        b.print("b");
      if(n[2]<0)n=n*-1;
      SetNrm(nx,ny,n);
//       printf("%d : %d nrm = %2.1f %2.1f %2.1f\n",nx,ny,n[0],n[1],n[2]);
  }
  #ifdef _USE_OPENGL 
  glNormalPointer(GL_DOUBLE,0,gNrm);
  #endif
}
///-----------------------------------------------------------------------------

bool Terrain::Render(){
  #ifdef _USE_OPENGL
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
//   glColor4f(0.1,1,0.1,1);
  
//   GLuint ind[4];
//   printf("drw\n");
//   for(int nx=0;nx<gNvtX-1;++nx)
//     for(int ny=0;ny<gNvtY-1;++ny){
//     ind[0]=nx*gNvtY;
//     ind[1]=ind[0]+1;
//     ind[2]=ind[0]+gNvtY;
//     ind[3]=ind[0]+gNvtY+1;
//     glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,ind);
//   }
//   glColor4f(0,0,1,1);
//   glDrawArrays(GL_POINTS,0,gNvts);
  
  glEnableClientState(GL_COLOR_ARRAY);
//   glColor4f(.1,.5,.1,1);
  for(int ns=0;ns<gNstripes;++ns){
    glDrawElements(GL_QUAD_STRIP,gStripeSize,GL_UNSIGNED_INT,gInd[ns]);
//        glDrawElements(GL_TRIANGLE_STRIP,gStripeSize,GL_UNSIGNED_INT,gInd[ns]);
  }
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  #else 
  _Err(1,"OPENGL was not enabled: render not possible!\n");
  #endif
  return true;
}
