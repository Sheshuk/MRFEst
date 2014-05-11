clc; close all; clear;
source('prob_Hist.m');
[Mm,Mp,E,cz]=convFlux(ls('cz*'));

Nsp=10000;
Nit=100;
mxZ=max(Mm(:))
mxE=1000000;
szV=repmat([max(E)-min(E);max(cz)-min(cz)],1,Nsp);
shV=repmat([min(E);min(cz)],1,Nsp);
X=[];
Y=[];
Z=[];
for i=1:Nit
  printf("=== it#%d/%d ===\n",i,Nit);
  v=shV+szV.*rand(2,Nsp);
  z=interp2(cz,E,Mm,v(2,:),v(1,:),'cubic');
%  a=find(rand(1,Nsp)<=Z/mxZ);
%  X=[X,v(2,a)];
%  Y=[Y,v(1,a)];
X=[X,v(2,:)];
Y=[Y,v(1,:)];
Z=[Z,z];
endfor
printf("Now we have %d points\n",length(X));
figure;
imagesc(cz,E,log(Mm));
axis('xy');
hold on;
a=find(Z <1e-12);
b=find(Z>=1e-12);

plot(X(a),Y(a),"ko");
plot(X(b),Y(b),"kx");
axis('xy');
hold off;
Sim=[X;Y;Z]';
save flux.MC Sim;
