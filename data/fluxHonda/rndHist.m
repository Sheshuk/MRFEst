function [prob,indM]=probHist(M)
  %normalize prob matrix to one
  v1=M(:)/sum(M(:)); 
  indM=reshape((1:length(v1)),size(M))
  
  rng=1:length(v1);
  sumM=(rng>=rng')
  %prepare integrated probability matrix
  prob=v1'*sumM;
endfunction

function [x,y]=rndHist(r,prob,indM)
  ind=lookup(prob,r,'l')
  [x,y]=find(indM==ind)
endfunction
