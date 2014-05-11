#ifdef unzen
/// unzen
//#define   PHI_detector 31.740975
//#define THETA_detector 9.4936
double   PHI_detector 90+37.17
double THETA_detector 10.5
#endif

#ifdef strom
/// stromboli
double   PHI_detector 218.39 //-218.39+90 
double THETA_detector 0
#endif
// #define   PHI_detector -123.0+90
// #define THETA_detector 90 //+10.5

// #define   PHI_detector -108.2+90
// #define THETA_detector 90+10.5
const double surface =    1e4;//10*12*10*8;//cm^2
// const double surface =    100.8*10;//cm^2
const double sec     =     86400;
const double days    =       1;//154;
//10x12cm2 x 10 x 8 
// 90 days

//histogram parameters

/*#define  THETAMIN  0
#define  THETAMAX  1.6
#define    PHIMIN -0.8
#define    PHIMAX  0.8
#define THETABINS  160
#define   PHIBINS  160
#define    N_ITER  10
*/


#define THETABINS  120
#define   PHIBINS  120
#define  THETAMIN -0.6
#define  THETAMAX  0.6
#define    PHIMIN -0.6
#define    PHIMAX  0.6
#define    N_ITER  10


