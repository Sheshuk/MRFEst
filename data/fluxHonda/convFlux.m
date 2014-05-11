function [Mm,Mp,E,cz]=convFlux(files)
  theta=str2num(files(:,3:4));
  theta=1-(theta-0.5)/20;
  theta=[1;theta;0];
  files=[files(1,:);files;files(size(files,1),:)];
  [cz,I]=sort(theta);
  for i_f=1:size(files,1)
    printf("Read file %s for cos(theta)=%2.4f\n",files(I(i_f),:),cz(i_f));
    S=load(files(I(i_f),:));
    %printf("%d) read size:"),i_f,disp(size(S));
    if(i_f==1)
      E=S(:,1);
      Mm=S(:,2);
      Mp=S(:,3);
    else
      Mm=[Mm,S(:,2)];
      Mp=[Mp,S(:,3)];
    endif
  endfor
  surf=0.1*0.12;
  nrm=3600*24*surf;
  Mp=Mp*nrm;
  Mm=Mm*nrm;
  printf("%d) Now we have matrix of size:"),i_f,disp(size(Mp));
  figure(1);
  subplot(1,2,1);
  imagesc(log10(E),cz,log10(Mm)');
  axis("xy");
  xlabel ("log(E)")
  ylabel ("cos(theta)")
  title("mu- flux");
  subplot(1,2,2);
  imagesc(log10(E),cz,log10(Mp)');
  axis("xy");
  xlabel ("log(E)")
  ylabel ("cos(theta)")
  colorbar("EastOutside");
  title("mu+ flux");
  
  figure;
  sz=(1-cz.^2).^0.5;
  szz=repmat(sz,1,length(E));
  Mint_m=szz'.*Mm;
  Mint_p=szz'.*Mp;
  am=2*sum(Mint_m,2);
  ap=2*sum(Mint_p,2);
  
  hold on;
  loglog(E,am,'-b');
  loglog(E,ap,'.r');
  hold off;
endfunction
