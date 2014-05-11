clc; close all; clear;
source('prob_Hist.m');
[Mm,Mp,E,cz]=convFlux(ls('cz*'));

%prepare probability matrix
Mtot=Mm+Mp;

%For efficiency of calculation, I use 
% dN(E,cz) = [M(E,cz)*sz] dE dsz = [M(E,cz)*E*sz] d(log(E)) dsz
% So, Mtot=[M(E,cz)*E*sz] is more flat (so rejection algo is more efficient)

sz=(1-cz.^2).^0.5; %sin(zenith)
%sz=repmat(sz,1,length(E));
Elog=log10(E);

Mtot=Mtot.*(E*sz');
Mtot=Mtot/max(Mtot(:));
figure;
surf(cz,Elog,(Mtot));

%return;
%%% begin MC 
Npts=100000; %sample size
Cnt0=[];
Nit=200;
Ntot=0;

%cuts: TX<=1 and TY<=1, so 
%nx/nz=tf<1 => sf<CCUT
%ny/nz=tt/cf<1 => tt<cf
%tt2<cf2 => st2/ct2<cf2 => (1-ct2)/ct2<cf2 => 1/ct2<cf2+1
% => ct2>1/(1+cf2)
 
CCUT=0.70712
for it=1:Nit 
  % ------- random [-CCUT<sin(phi)<CCUT] -------
  sf=(rand(Npts,1)-0.5).*(2*CCUT); %sin(phi)
  cf2=1-sf.^2; %cos2(phi)
  THLIM=(1+cf2).^-0.5;
  % ------- random cos(theta) --------
  ct=rand(Npts,1).*(1.-THLIM)+THLIM;
  clear THLIM;
  cf=cf2.^0.5; clear cf2;
  
  #ct=(1.-(1-CCUT)*rand(Npts,1)); %cos(theta)=sin(zenith);
  logEr=(rand(Npts,1)-min(Elog))*(max(Elog)-min(Elog)); %log10(energy);
  prob=interp2(sz,Elog,Mtot,ct,logEr);
  surv=find((rand(Npts,1)<prob)); %survived rejection cut
  #printf("Before: %f of %d\n",min(ct(:)), length(ct));
  ct=ct(surv);
  cf=cf(surv);
  sf=sf(surv);
  #printf("After:  %f of %d\n",min(ct(:)), length(ct));
  Ntot=Ntot+length(surv);
  E=logEr(surv);
  
  clear logEr surv
  
  st=(1-ct.^2).^0.5; %sin(theta)
  
  nz=ct.*cf;
  nx=ct.*sf;
  ny=st;
#  tt2=(nx.^2+ny.^2)./(nz.^2);
#  ind=find(tt2<1.);
  %disp([nx./nz, ny./nz]); pause;
  [cnt,bx,by]=hist2d([nx./nz,ny./nz],[-2.01:0.01:2.01],[0:0.01:2.01]);
  data=[10.^E,nx(:),ny(:),nz(:)];
  save -ascii -append MCflux.txt data
  if(it==1)
    Cnt0=zeros(size(cnt));
  endif
  Cnt0=Cnt0+cnt;
  clear nx ny nz B cf sf ct st
  if(mod(it,50)==0)
    figure(30);
    nnx=2:(size(cnt,1)-1);
    nny=2:(size(cnt,2)-1);
    imagesc(bx(nny),by(nnx),Cnt0(nnx,nny));
    axis("xy");
    printf("===== Iteration %d. Ntot=%d =====\n",it,Ntot);
    disp('press any key');
    pause;
  endif
endfor


