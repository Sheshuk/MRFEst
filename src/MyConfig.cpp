#include <iostream>
#include <cstring>
#include "MyConfig.h"

void Config::Init(){
    TEX.gMap="...................";
    TEX.gCompas="...................";
    TEX.gDet="...................";
    TEX.gSky="base/Sky2.jpg";
    TEX.MapC[0]=0; TEX.MapC[1]=0;
    TEX.MapS[0]=1; TEX.MapS[1]=1;
    TEX.gPhoto="";
    SAVE.ShotName= "shot_N";
    SAVE.FluxFile= "flux_N";
    SAVE.FluxPlot= "plot_N";
    HIST.TX_Nb=120;
    HIST.TY_Nb=120;
    HIST.TX_Lim[0]=-0.6; HIST.TX_Lim[1]=0.6;
    HIST.TY_Lim[0]=-0.6; HIST.TY_Lim[1]=0.6;
    HIST.N_iter=10;

    DET.Surf=1e4;
    DET.ExpDays=1;
}

void Config::Read(char* fname){
  FILE* f=fopen(fname,"r");
  printf("read config file %s\n",fname);
  char line[255];
  char s[100];
  double v[4];
  while(!feof(f)){
    fgets(line,255,f);
    if(strlen(line)==0)continue;
    printf("line=%s",line);
    if(line[0]=='#') continue;
    if(sscanf(line,"DET.Pos:    %lf %lf %lf",v+0,v+1,v+2)==3){memcpy(DET.Pos,v,3*sizeof(double)); continue;}
    if(sscanf(line,"DET.Ang:    %lf %lf %lf",v+0,v+1,v+2)==3){memcpy(DET.Ang,v,3*sizeof(double)); continue;}
    if(sscanf(line,"DET.Size:   %lf %lf",v+0,v+1)==2)        {memcpy(DET.Size,v,2*sizeof(double)); continue;}
    if(sscanf(line,"DET.Tmax:  %lf",v+0)==1)                  {DET.TXmax=v[0]; continue;}
    if(sscanf(line,"DET.Surf:  %lf",v+0)==1)                  {DET.Surf=v[0]; continue;}
    if(sscanf(line,"DET.ExpDays:  %lf",v+0)==1)               {DET.ExpDays=v[0]; continue;}
    
    if(sscanf(line,"HIST.TX_nbin:   %lf",v+0)==1){HIST.TX_Nb=int(v[0]); continue;}
    if(sscanf(line,"HIST.TY_nbin:   %lf",v+0)==1){HIST.TY_Nb=int(v[0]); continue;}
    if(sscanf(line,"HIST.TX_lim:    %lf %lf",v+0,v+1)==2){memcpy(HIST.TX_Lim,v,2*sizeof(double)); continue;}
    if(sscanf(line,"HIST.TY_lim:    %lf %lf",v+0,v+1)==2){memcpy(HIST.TY_Lim,v,2*sizeof(double)); continue;}
    if(sscanf(line,"HIST.N_iter:     %lf",v+0)==1){HIST.N_iter=int(v[0]); continue;}
    
    if(sscanf(line,"DET.Pos:    %lf %lf %lf",v+0,v+1,v+2)==3){memcpy(DET.Pos,v,3*sizeof(double)); continue;}
    if(sscanf(line,"Window.Size: %lfx%lf",v+0,v+1)==2){memcpy(WND.Size,v,2*sizeof(double)); continue;}
    if(sscanf(line,"Window.Name: %s",s)==1)     {WND.Name=line+13; continue;}
    if(sscanf(line,"FoV: %lf",v+0)==1)          {WND.FoV=v[0]; continue;}
    if(sscanf(line,"TEX.Sky: %s",s)==1) {TEX.gSky=s; continue;}
    if(sscanf(line,"TEX.Map: %s",s)==1) {TEX.gMap=s; continue;}
    if(sscanf(line,"TEX.MapCoord:  %lf %lf %lf %lf",v+0,v+1,v+2,v+3)==4){
      printf("v4= %f %f %f %f\n",v[0],v[1],v[2],v[3]);
      TEX.MapC[0]=v[0];
      TEX.MapC[1]=v[1];
      TEX.MapS[0]=v[2];
      TEX.MapS[1]=v[3];
      printf("MapCrd=[%2.4f %2.4f] MapSize=[%2.4f %2.4f]\n",TEX.MapC[0],TEX.MapC[1],TEX.MapS[0],TEX.MapS[1]);
      continue;}
    if(sscanf(line,"TEX.Det: %s",s)==1)         
    {
       TEX.gDet=s; 
       printf("TEX.Det -> %s\n=%s",s,TEX.gDet.data()); 
       continue;}
    if(sscanf(line,"TEX.Compass: %s",s)==1)     {TEX.gCompas=s; continue;}
    if(sscanf(line,"DEM.File: %s",s)==1)        {DEM.File=s; continue;}
    if(sscanf(line,"DEM.Format: %s",s)==1)  DEM.Format.Read(s);
    if(sscanf(line,"SAVE.ScrShot: %s",s)==1){SAVE.ShotName=s; continue;}    
    if(sscanf(line,"SAVE.FluxFile: %s",s)==1){SAVE.FluxFile=s; continue;}    
    if(sscanf(line,"SAVE.FluxPlot: %s",s)==1){SAVE.FluxPlot=s; continue;}    
  }
  fclose(f);
  printf("Dem file=%s\n",DEM.File.data());
  printf("TXmax=%2.5f\n",DET.TXmax);
  Print();
}

void Config::Print(){
  printf("**** MuRay Configuration: ****\n");
  printf("WND.Size = [%4.1f %4.1f]\n",WND.Size[0], WND.Size[1]);
  printf("WND.Name = \"%s\"\n",WND.Name.data());
  printf("WND.FoV = %4.2f\n",  WND.FoV);
  printf("SAVE.ShotName = \"%s\"\n",SAVE.ShotName.data());
  printf("DET.Pos = [%4.2f %4.2f %4.2f]\n",DET.Pos[0],DET.Pos[1],DET.Pos[2]);
  printf("DET.Ang = [%4.2f %4.2f %4.2f]\n",DET.Ang[0],DET.Ang[1],DET.Ang[2]);
  printf("DET.TXmax = %4.2f\n",DET.TXmax);
  printf("DET.Size = [%4.2f %4.2f]\n",DET.Size[0],DET.Size[1]);
  printf("DET.Surf =  %4.2f cm2\n",DET.Surf);
  printf("DET.ExpDays =  %4.2f\n",DET.ExpDays);
  printf("TEX.gPhoto = \"%s\"\n",(TEX.gPhoto.empty()==false)?TEX.gPhoto.data():"NULL");
  printf("TEX.gMap = \"%s\"\n",(TEX.gMap.empty()==false)?TEX.gMap.data():"NULL");
  printf("TEX.gCompas = \"%s\"\n",(TEX.gCompas.empty()==false)?TEX.gCompas.data():"NULL");
  printf("TEX.gDet = \"%s\"\n",(TEX.gDet.empty()==false)?TEX.gDet.data():"NULL");
  printf("TEX.gSky = \"%s\"\n",(TEX.gSky.empty()==false)?TEX.gSky.data():"NULL");
  printf("TEX.MapC = [%4.2f %4.2f]\n",TEX.MapC[0],TEX.MapC[1]);
  printf("TEX.MapS = [%4.2f %4.2f]\n",TEX.MapS[0],TEX.MapS[1]);
  printf("*******************************\n");
}

