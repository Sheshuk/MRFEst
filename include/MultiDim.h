#include <cassert>
///-------------------------------------------------
typedef unsigned char DimSize
///-------------------------------------------------
struct{
  int Nbins;
  double Min, Max;
}MD_Axis;
///-------------------------------------------------
///-------------------------------------------------

template <DimSize _NDIM, class T> class MD_v{
public:
  MD_v(T val){SetAll(val);}
  void SetAll(T val)    {for(DimSize i=0; DimSize<_NDIM; ++i)eC[i]=val;}
  T& operator [](DimSize n){return eC[n];}
  T& operator +(MD_v<_NDIM,T> vec){for(DimSize i=0; DimSize<_NDIM; ++i)eC[i]=eC[i]+vec[i];}
  void Set(T v0, T v1, T v2, T v3){ assert(_NDIM>=4); eC[0]=v0; eC[1]=v1; eC[2]=v2; eC[3]=v3;}
  const DimSize Size(){return _NDIM;}
public:
  T eC[_NDIM];
}
///-------------------------------------------------
template <DimSize _NDIM> class MD_hist{
public:
  MD_hist(int Ndim);
  bool Ind_1dMd(int i1,int n0,int 
  
private:
  MD_Axis* eAxes;
  double* eData;
}
