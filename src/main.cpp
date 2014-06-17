
#include <iostream>
#include "stdio.h"
#include "time.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TRotation.h"
#include "math.h"

#include "Flux.h"
#include "Attenuation.h"
#include "Terrain.h"
#include "RayTrace.h"
#include "MyConfig.h"
#include "NiceLog.h"
using namespace std;

#define _NiceVlev gVLev

///global variables
int gVLev=0;
Terrain Land;
vect3 det_pos;
vect3 det_ang;

Config gCfg;
Attenuation  att("./data/Transmission_Tree.root");
Flux        flux;
TRandom3 *rn = new TRandom3(time(0));
float density=2.2;

///detector normal direction
TRotation DetToGlob;
///---------------------------------------------------------------------------

double FluxMC_int(double tx, double ty, double dtx, double dty, double & thick, TH2* h_Hole=0){
  ///new MC method by ASh. Make uniformly distributed TX and TY.
  // double d[2]={(tx>=0)?1:-1,(ty>=0)?1:-1};
//   if(ty<0)return 0;
  double integral = 0;
  double TX,TY;
  double w,cx;
  
  thick=0;
  vect3 Dir(0,0,1); 
  RayTrace RT(det_pos, Dir,&Land);
  for(int i=0; i<gCfg.HIST.N_iter; i++){
    if(gCfg.HIST.N_iter==1){TX=tx+0.5*dty; TY=ty+0.5*dty;}
    else{
      TX= rn->Uniform(tx,tx+dtx);
      TY= rn->Uniform(ty,ty+dty);
    }
    cx= sqrt(1./(1+TX*TX)); ///cos Phi
    double TTh=TY*cx; /// tg(theta)=TY/sqrt(1+TX*TX)
    //_DumpF(TX);
    //_DumpF(TY);
    ///correct way to calculate direction vector: multiply by rotation matrix
    TVector3 v0(TX,-TY,1);
    v0=(DetToGlob*v0);
    v0.SetMag(1.);
    RT.fDir.Set(v0.X(),v0.Y(),v0.Z());
    w=1./((1+TX*TX)*(1+TTh*TTh));
    RT.fPos=det_pos;
    
    double THETA=90-v0.Theta()*TMath::RadToDeg();//TMath::RadToDeg()*TMath::ATan(tyn);

    if(THETA<0){
      THETA=-THETA;
      RT.fDir*=-1;
    }
//     RT.fDir.print("direction");
    double th=RT.GetThickness();
    if(h_Hole){ ///substract thickness, obtained from h_Hole histogram: make hole!
      th-=h_Hole->Interpolate(TX,TY); 
      if(th<0)th=0;
    }
      
    double flx= flux.GetFlux(THETA);
    double at= att.GetAttenuation(THETA,th*density);
    integral += at*flx*w*w;
    thick+=th;
  }
  thick=thick/(gCfg.HIST.N_iter);
  return integral*dtx*dty/(gCfg.HIST.N_iter);
}
///---------------------------------------------------------------------------

void helpme(){
  printf("--- Muon radiography simulation --- \n");
  printf("\tusage: MuRay [options]\n");
  printf("\t** options description: ** \n");
  printf("\t   -h\t display this help \n");
  printf("\t   -v=Number\t set verbose level\t(def=0) \n");
  printf("\t   -E=Value \t set minimum energy (GeV)\t(def=1. GeV)\n");
  printf("\t   -H=filename\t set file with \"holes\" histogram \t(def: none)\n");
  printf("\t   -cfg=filename\t read config file\n");
  printf("\t   -dem=filename\t override DEM file\n");
}
///====================================================================================================================
TH2D* LoadHole(char* fname){
  TH2D* h_Hole=0;
  _Log(1,"read hole from \"%s\"\n",fname);
  TFile f(fname,"READ");
  if(!f.IsOpen()){_Err(0,"Could not open file \"%s\" with hole!\n",fname); return 0;}
  gROOT->cd();
  h_Hole=(TH2D*)f.Get("hHole")->Clone("h_Hole");
  if(!h_Hole){_Err(1,"Could get histogram \"hHole\" from file \"%s\"\n",fname); return 0;}
  f.Close();
  TCanvas c;
  _LogCmd(2,_DumpX(h_Hole));
  h_Hole->Draw("colz");
  c.Print("hole.png");
  return h_Hole;
}
///====================================================================================================================
int main(int argc, char *argv[]){
  double Emin=10;  
  TH2D* h_Hole=0;
  if(argc<2){helpme(); return 1;}
  for(int n=1;n<argc;++n){
    _DumpS(argv[n]);
    if(strcmp(argv[n],"-h")==0){helpme(); return 1;}
    if(strncmp(argv[n],"-v=",3)==0){gVLev=atoi(argv[n]+3); continue;}
    if(strncmp(argv[n],"-E=",3)==0){Emin=atof(argv[n]+3); continue;}
    if(strncmp(argv[n],"-H=",3)==0){h_Hole=LoadHole(argv[n]+3); continue;}
    if(strncmp(argv[n],"-cfg=",5)==0){ gCfg.Read(argv[n]+5); continue; }
    if(strncmp(argv[n],"-dem=",5)==0){gCfg.DEM.File=argv[n]+5; continue;}
  }
  Terrain_VLev=gVLev;
  
  Land.Load(gCfg.DEM.File.data(),gCfg.DEM.Format);
  det_pos[0]=gCfg.DET.Pos[0];
  det_pos[1]=gCfg.DET.Pos[1];
  det_pos[2]=gCfg.DET.Pos[2];
  det_ang[0]=gCfg.DET.Ang[0];
  det_ang[1]=gCfg.DET.Ang[1];
  det_ang[2]=gCfg.DET.Ang[2];
  det_pos[0]-=Land.X_gps();
  det_pos[1]-=Land.Y_gps();
  
  flux.SetEmin(Emin);
  flux.SetHondaFlux1();
  //flux.Print();
  
  //histograms
  TH2D *h_all2  = new TH2D("h_Flu","Flux; TX; TY",
  gCfg.HIST.TX_Nb,gCfg.HIST.TX_Lim[0],gCfg.HIST.TX_Lim[1],gCfg.HIST.TY_Nb,gCfg.HIST.TY_Lim[0],gCfg.HIST.TY_Lim[1]);
  TH2D *h_all3 =  new TH2D("h_FluLog","LogFlux; TX; TY",
  gCfg.HIST.TX_Nb,gCfg.HIST.TX_Lim[0],gCfg.HIST.TX_Lim[1],gCfg.HIST.TY_Nb,gCfg.HIST.TY_Lim[0],gCfg.HIST.TY_Lim[1]);
  TH2D *h_thick = new TH2D("h_Thick","Thickness, m; TX; TY",
  gCfg.HIST.TX_Nb,gCfg.HIST.TX_Lim[0],gCfg.HIST.TX_Lim[1],gCfg.HIST.TY_Nb,gCfg.HIST.TY_Lim[0],gCfg.HIST.TY_Lim[1]);
  //double TXBinSize = gCfg.HIST.TX_BinSize();
  //double TYBinSize = gCfg.HIST.TY_BinSize();
  double done=0,d0=0;

//   double x=GetThickness(0,0);
//   return 0;
  
  TFile l(Form("%s.root",gCfg.SAVE.FluxFile.data()),"RECREATE");
  float tx,ty;
  float dtx,dty;
  double thick;
  //set rotation
  DetToGlob.RotateZ(TMath::DegToRad()*(det_ang[2]));  
  DetToGlob.RotateX(TMath::DegToRad()*(det_ang[0]));  
  DetToGlob.Invert();
  TVector3 ntst(0.1,0.1,1);
  ntst=DetToGlob*TVector3(-0.1,0.,1);  ntst.Print();
  ntst=DetToGlob*TVector3(0.,0.,1);  ntst.Print();
  ntst=DetToGlob*TVector3(0.1,0.,1);  ntst.Print();
  for(int ix=0;ix<=h_all2->GetNbinsX();++ix){
    done=ix*100./(h_all2->GetNbinsX()-1);
    if(done-d0>1.){
      printf("Processing: %3.3f%% completed\r",done);
      fflush(stdout);
      d0=done;
    }
    tx =h_all2->GetXaxis()->GetBinLowEdge(ix);
    dtx=h_all2->GetXaxis()->GetBinWidth(ix);
    
    for(int iy=0;iy<=h_all2->GetNbinsY();++iy){
      ty =h_all2->GetYaxis()->GetBinLowEdge(iy);
      dty=h_all2->GetYaxis()->GetBinWidth(iy);
      double flux_MC = gCfg.DET.Exposure()*1000*FluxMC_int(tx,ty,dtx,dty, thick, h_Hole);
      int nb=h_all2->FindBin(tx+0.5*dtx,ty+0.5*dty);
      h_all2->SetBinContent(nb,flux_MC);
      h_all3->SetBinContent(nb,flux_MC!=0?log(flux_MC):0);
      h_thick->SetBinContent(nb,thick);
    }
  }
  
      printf("\nDone!\n");
      gStyle->SetPalette(1,0);
      gStyle->SetOptStat(0);
      TCanvas c("c","c",800,800);
      c.SetGrid();

      h_all2->Draw("colz");
      c.Print(Form("%s.png",gCfg.SAVE.FluxPlot.data()));
      h_all3->Draw("colz");
      c.Print(Form("%sL.png",gCfg.SAVE.FluxPlot.data()));
      h_thick->Write();
      
      h_all2->Write();
      h_all3->Write();
      l.Close();
  
  return 1;
}
