class MCpart{
  public: 
    ///getters
    double E() {return E;}
    double Px() {return p[0];}
    double Py() {return p[1];}
    double Pz() {return p[2];}
    double X()  {return r[0];}
    double Y()  {return r[1];}
    double Z()  {return r[2];}
    double Pt2() {return p[0]*p[0]+p[1]*p[1];}
    double Pt() {return sqrt(p[0]*p[0]+p[1]*p[1]);}
    double Smear()
  private:
    double E;    ///energy
    double p[3]; ///momentum
    double r[3]; ///position
}

template <int M, class T> 
class MD_Data{
  public:
    MD_Data()
    T& operator [](int n){return eVect[n];}
    
  protected:
   T eVect[M];
}
template <int M, class T> 
class MD_Table{
  ///Multi-dimensional table (histogram) with data.
  ///Features: 
  /// * fill data like histogram 
  /// * read and save table to file
  public:
     MD_Table();
     void InitArray();
     int Fill
  protected:
     T *eDATA;    ///main array with data
     unsigned int eN[M]; ///number of bins for each dim
     T eMax  [M]; ///upper limits for dims
     T eMin  [M]; ///lower limits for dims
}


