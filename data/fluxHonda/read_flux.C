
void read_flux(const char* fname){
  ifstream inf;
  inf.open(fname);
  char line[255];
  double t,E, T, F;
  while(!inf.eof()){
    inf.getline(line,255);
    sscanf(line,"%lf %lf %lf",&E,&T,&F);
   }
  inf.close();
}
