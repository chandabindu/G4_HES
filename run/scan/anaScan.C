#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>

#include "TApplication.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TF1.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TTree.h"
#include "TCut.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TDecompSVD.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TLatex.h"
#include "TText.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TColor.h"
#include "TPaveText.h"
#include "TMath.h"
#include "TGaxis.h"
#include "TSpline.h"
#include "TLegend.h"
#include "TEfficiency.h"

using namespace std;


const double PI = atan(1.)*4.;
void TreeBranch( TTree*, const char*, int*);
void TreeBranch( TTree*, const char*, double*);
void TreeBranch( TTree*, const char*, char*);
void TreeBranch( TTree*, const char*, vector<int>**, TBranch**);
void TreeBranch( TTree*, const char*, vector<double>**, TBranch**);

//const int NofScan = 10; //Num of total ROOT file
const int NofScan = 441; //Num of total ROOT file
const int NofLoop = NofScan; //Num of analysis ROOT file

//fg = field gradient
const int bin_fg = 21; const double min_fg = 0.5; const double max_fg = 1.5;

const int bin_mom = 100; const double min_mom =   0.544; const double max_mom =  1.144;
const int bin_dp  =1000; const double min_dp  =  -0.003; const double max_dp  =  0.003;
const int bin_dxp =1000; const double min_dxp =  -200.0; const double max_dxp =  200.0;
const int bin_dyp =1000; const double min_dyp =  -50.00; const double max_dyp =  50.00;

//const int MaxEvent = 3000; // Num of events used for Matrix Generation
//const int nParam   =  35; // 3-order
const int nParam   =  210; // 6-order

const double Reso_xt   = 0.0097;  //[cm], HES, RMS default 2022
const double Reso_yt   = 0.0120;  //[cm], HES, RMS default 2022
const double Reso_xpt  = 0.8E-3; //[rad], HES, RMS default 2022
const double Reso_ypt  = 1.5E-3; //[rad], HES, RMS default 2022

TF1* fgaus[NofScan];//Fitting function

  //------- Okuyama, 20220304, data20220304_2020/case308.root ----------------
  //PCS+HES(vertical)
//at Focal Plane
  const double  XFPm_HES  = 4.085   , XFPr_HES  = 26.17   ; 
  const double  YFPm_HES  = 8.814   , YFPr_HES  =  4.73   ; 
  const double  XpFPm_HES = 0.01694 , XpFPr_HES =  0.118  ; 
  const double  YpFPm_HES = 0.02048 , YpFPr_HES =  0.01531; 
//at Target
  const double  Momm_HES  = 0.844   , Momr_HES  = 0.15    ;
  const double  xtm_HES   = 0.      , xtr_HES   = 1.      ;
  const double  ytm_HES   = 0.      , ytr_HES   = 1.      ;
  const double  ztm_HES   = 0.      , ztr_HES   = 1.      ;
  const double  Xptm_HES  = 0.1691  , Xptr_HES  = 0.02562 ;
  const double  Yptm_HES  =-0.001235, Yptr_HES  = 0.01546 ;
//Okuyama, 20210903, ktest_large.root
  //const double  XFPm_HES = -3.689, XFPr_HES = 21.94; 
  //const double  YFPm_HES = 0.004998, YFPr_HES = 2.577;
  //const double  XpFPm_HES = -0.01568, XpFPr_HES = 0.1007;
  //const double  YpFPm_HES = 0.02835, YpFPr_HES = 0.007165;
  //const double  xtm_HES = 0., Xptm_HES = -0.247;
  //const double  ytm_HES = 0., Yptm_HES = -0.002119;
  //const double  Momm_HES = 0.844, Momr_HES = 0.15;
  //const double  xtr_HES = 0.1, Xptr_HES = 0.0369;
  //const double  ytr_HES = 0.1, Yptr_HES = 0.019;
  //const double  ztm_HES=0.0 , ztr_HES=1.;
//Okuyama, 20211203, ver_phi90_large.root
  //const double  XFPm_HES  = 3.975, XFPr_HES  = 22.58; 
  //const double  YFPm_HES  = -0.007529, YFPr_HES  = 2.947; 
  //const double  XpFPm_HES = 0.0119,    XpFPr_HES = 0.1036; 
  //const double  YpFPm_HES = -0.000012, YpFPr_HES = 0.00409; 
  //const double  xtm_HES = 0., Xptm_HES = -0.247;
  //const double  ytm_HES = 0., Yptm_HES = -0.002119;
  //const double  Momm_HES = 0.844,      Momr_HES  = 0.15;
  //const double  xtr_HES = 0.1, Xptr_HES = 0.0369;
  //const double  ytr_HES = 0.1, Yptr_HES = 0.019;
  //const double  ztm_HES=0.0 , ztr_HES=1.;

  //FillHist(omega, tree_hes, h_hes_mom_gen, h_hes_mom_hit, h_hes_mom_sa, h_hes_dp, h_hes_dxp, h_hes_dyp,
  //		 set, sa, dp, dxp, dyp, fom,
  //		 h2_hes_sa_map, h2_hes_dp_map, h2_hes_dxp_map, h2_hes_dyp_map, h2_hes_fom_map);
int FillHist(double omega, TChain** tree_e, TH1D** h_e_momgen, TH1D** h_e_momhit, TH1D** h_e_momacc, TH1D** h_e_momreso, TH1D** h_e_xpreso, TH1D** h_e_ypreso
                , double* setting_num, double* ave_sa, double* mom_reso, double* xp_reso, double* yp_reso, double* fom 
                , TH2D* h2_e_samap, TH2D* h2_e_dpmap, TH2D* h2_e_dxpmap, TH2D* h2_e_dypmap, TH2D* h2_e_fommap){ //FillHist

  int best_setup = 0;
  double suzuki_tmp = -9999.;

  //HES
 for(int j_scan=0;j_scan<NofLoop;j_scan++){

    cout<<"OpenMatrixFile"<<endl;
    char filename_hes_mom[300];
    char filename_hes_xpt[300];
    char filename_hes_ypt[300];
    char filename_hes_zt[300];
    sprintf(filename_hes_mom, "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix20220310_0515h/mom_%d.dat",j_scan);
    sprintf(filename_hes_xpt, "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix20220310_0515h/xpt_%d.dat",j_scan);
    sprintf(filename_hes_ypt, "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix20220310_0515h/ypt_%d.dat",j_scan);
    sprintf(filename_hes_zt , "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix20220310_0515h/zt_%d.dat" ,j_scan);
    //sprintf(filename_hes_mom, "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/ana/okuyamacro/matrix/m_mom.dat");
    //sprintf(filename_hes_xpt, "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/ana/okuyamacro/matrix/m_xpt.dat");
    //sprintf(filename_hes_ypt, "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/ana/okuyamacro/matrix/m_ypt.dat");
    //sprintf(filename_hes_zt , "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/ana/okuyamacro/matrix/m_zt.dat" );
    ifstream file_hes_Mom(filename_hes_mom);
    ifstream file_hes_Xp( filename_hes_xpt);
    ifstream file_hes_Yp( filename_hes_ypt);
    ifstream file_hes_Zt(  filename_hes_zt);
  
    double ParHESMom[5][nParam];  //{par, pow_X, pow_Xp, pow_Y, pow_Yp}
    double ParHESXp[5][nParam];  
    double ParHESYp[5][nParam];  
    double ParHESZt[5][nParam];  

    cout<<"FillDataArrayForMatrix"<<endl;
    for(int i = 0; i < nParam; i++){
      file_hes_Mom >> ParHESMom[0][i] >> ParHESMom[1][i] >> ParHESMom[2][i] >> ParHESMom[3][i] >> ParHESMom[4][i];
      file_hes_Xp  >> ParHESXp[0][i]  >> ParHESXp[1][i]  >> ParHESXp[2][i]  >> ParHESXp[3][i]  >> ParHESXp[4][i];
      file_hes_Yp  >> ParHESYp[0][i]  >> ParHESYp[1][i]  >> ParHESYp[2][i]  >> ParHESYp[3][i]  >> ParHESYp[4][i];
      file_hes_Zt  >> ParHESZt[0][i]  >> ParHESZt[1][i]  >> ParHESZt[2][i]  >> ParHESZt[3][i]  >> ParHESZt[4][i];
    } //for

    //HES
    double data_hes_p_t    = 0.; //at target
    double data_hes_x_t    = 0.;
    double data_hes_y_t    = 0.;
    double data_hes_z_t    = 0.;
    double data_hes_xp_t   = 0.;
    double data_hes_yp_t   = 0.;
    double data_hes_e_p_t  = 0.; //error
    double data_hes_e_z_t  = 0.;
    double data_hes_e_xp_t = 0.;
    double data_hes_e_yp_t = 0.;
    double data_hes_x_rp   = 0.; //at ref. plane
    double data_hes_y_rp   = 0.;
    double data_hes_xp_rp  = 0.;
    double data_hes_yp_rp  = 0.;
    double true_hes_p_t    = 0.; // true value
    double true_hes_z_t    = 0.;
    double true_hes_xp_t  = 0.;
    double true_hes_yp_t  = 0.;

    cout << endl;
    cout << "******************** No. " << j_scan+1 << " / " << NofLoop << " ********************"  << endl;
    cout << endl;
//added
    double xt=0.0 , yt=0.0 , zt = 0.0;
    double xpt=0.0 , ypt=0.0;
    double mom=0.0 , th=0.0 , ph = 0.0;

    double Xt=0.0 , Yt=0.0 , Zt = 0.0;
    double Xpt=0.,Ypt=0.,LFP=0.;
	double XQ1i=0, YQ1i=0;
	double XQ1e=0, YQ1e=0;
	double XQ2i=0, YQ2i=0;
	double XQ2e=0, YQ2e=0;
	double XDi=0, YDi=0;
	double XDe=0, YDe=0;
	double PFP=0;
	double XFP=0, YFP=0;
	double XpFP=0, YpFP=0;
    double EHODO1XFP=0, EHODO1YFP=0;
    double EHODO1XpFP=0, EHODO1YpFP=0;
    double EHODO2XFP=0, EHODO2YFP=0;
    double EHODO2XpFP=0, EHODO2YpFP=0;
	double XFP1=0;
	double EHODOX = 0;
	double EHODOY = 0;
	double ZvFP1=0, YVC=0;
    char SPLTrig=0,DCTrig=0;
	char VDTrig_oku=0;
//-- Cut Conditions --//
char QQDTrig = 0;
char ETrig = 0;
double EDC1X[10];
double EDC1Y[10];
double EDC2X[6];
double EDC2Y[6];
double EH1X[29];
double EH1Y[29];
double EH2X[29];
double EH2Y[29];
char sXEDC[100];
char sYEDC[100];
char sXEH1[100];
char sYEH1[100];
char sXEH2[100];
char sYEH2[100];
char EDC1Trig = 0;
char EDC2Trig = 0;
char EH1Trig = 0;
char EH2Trig = 0;
//Trig. from branch
  char QQDTrig_b = 0;
  char FPTrig_b  = 0;
  char EDCTrig_b = 0;
  char EHTrig_b  = 0;
  char ETrig_b   = 0;
  char ETrig2_b  = 0;
  char ETrig3_b  = 0;
	tree_e[j_scan]->SetBranchStatus("*",0);
	TreeBranch(tree_e[j_scan], "EXQ1i", &XQ1i);
	TreeBranch(tree_e[j_scan], "EYQ1i", &YQ1i);
	TreeBranch(tree_e[j_scan], "EXQ1e", &XQ1e);
	TreeBranch(tree_e[j_scan], "EYQ1e", &YQ1e);
	TreeBranch(tree_e[j_scan], "EXQ2i", &XQ2i);
	TreeBranch(tree_e[j_scan], "EYQ2i", &YQ2i);
	TreeBranch(tree_e[j_scan], "EXQ2e", &XQ2e);
	TreeBranch(tree_e[j_scan], "EYQ2e", &YQ2e);
	TreeBranch(tree_e[j_scan], "EXDi", &XDi);
	TreeBranch(tree_e[j_scan], "EYDi", &YDi);
	TreeBranch(tree_e[j_scan], "EXDe", &XDe);
	TreeBranch(tree_e[j_scan], "EYDe", &YDe);
	//TreeBranch(tree_e[j_scan], "PEDCFP", &PFP);
	//TreeBranch(tree_e[j_scan], "XEDCFP", &XFP);
	//TreeBranch(tree_e[j_scan], "YEDCFP", &YFP);
	//TreeBranch(tree_e[j_scan], "XpEDCFP", &XpFP);
	//TreeBranch(tree_e[j_scan], "YpEDCFP", &YpFP);
	TreeBranch(tree_e[j_scan], "EXFP2", &XFP);
	TreeBranch(tree_e[j_scan], "EYFP2", &YFP);
	TreeBranch(tree_e[j_scan], "EXpFP2", &XpFP);
	TreeBranch(tree_e[j_scan], "EYpFP2", &YpFP);
////EHODO1X
	TreeBranch(tree_e[j_scan], "EXFP4", &EHODO1XFP);
	TreeBranch(tree_e[j_scan], "EYFP4", &EHODO1YFP);
	TreeBranch(tree_e[j_scan], "EXpFP4", &EHODO1XpFP);
	TreeBranch(tree_e[j_scan], "EYpFP4", &EHODO1YpFP);
////EHODO2X
	TreeBranch(tree_e[j_scan], "EXFP5", &EHODO2XFP);
	TreeBranch(tree_e[j_scan], "EYFP5", &EHODO2YFP);
	TreeBranch(tree_e[j_scan], "EXpFP5", &EHODO2XpFP);
	TreeBranch(tree_e[j_scan], "EYpFP5", &EHODO2YpFP);
	TreeBranch(tree_e[j_scan], "EXt", &xt);
	TreeBranch(tree_e[j_scan], "EYt", &yt);
	TreeBranch(tree_e[j_scan], "EZt", &zt);
	TreeBranch(tree_e[j_scan], "EXpt", &xpt);
	TreeBranch(tree_e[j_scan], "EYpt", &ypt);
	TreeBranch(tree_e[j_scan], "EMom", &mom);
	TreeBranch(tree_e[j_scan], "ETheta", &th);
	TreeBranch(tree_e[j_scan], "EPhi"  , &ph);
	TreeBranch(tree_e[j_scan], "EQQDTrig", &QQDTrig_b);
	TreeBranch(tree_e[j_scan], "EFPTrig" , &FPTrig_b );
	TreeBranch(tree_e[j_scan], "EDCTrig" , &EDCTrig_b);
	TreeBranch(tree_e[j_scan], "EHTrig"  , &EHTrig_b );
	TreeBranch(tree_e[j_scan], "ETrig"   , &ETrig_b  );
	TreeBranch(tree_e[j_scan], "ETrig2"  , &ETrig2_b );
	TreeBranch(tree_e[j_scan], "ETrig3"  , &ETrig3_b );

  for (int i=0;i<10;i++){//EDC1
    sprintf(sXEDC,"XEDC%d",i+1); 
    sprintf(sYEDC,"YEDC%d",i+1); 
    TreeBranch(tree_e[j_scan], sXEDC, &EDC1X[i]);
    TreeBranch(tree_e[j_scan], sYEDC, &EDC1Y[i]);
  }
  for (int i=0;i<6;i++){//EDC2
    sprintf(sXEDC,"XEDC%d",i+11); 
    sprintf(sYEDC,"YEDC%d",i+11); 
    TreeBranch(tree_e[j_scan], sXEDC, &EDC2X[i]);
    TreeBranch(tree_e[j_scan], sYEDC, &EDC2Y[i]);
  }
  for (int i=0;i<29;i++){//EHODO1,2
    sprintf(sXEH1,"XEH%d",i+101); 
    sprintf(sYEH1,"YEH%d",i+101); 
    sprintf(sXEH2,"XEH%d",i+201); 
    sprintf(sYEH2,"YEH%d",i+201); 
    TreeBranch(tree_e[j_scan], sXEH1, &EH1X[i]);
    TreeBranch(tree_e[j_scan], sYEH1, &EH1Y[i]);
    TreeBranch(tree_e[j_scan], sXEH2, &EH2X[i]);
    TreeBranch(tree_e[j_scan], sYEH2, &EH2Y[i]);
  }

//	int ENum=0;
//	double mom=0.;
//	double th=0.;
//	double ph=0.;
//	double xt=0.;
//	double yt=0.;
//	double zt=0.;
//	double xpt=0.;
//	double ypt=0.;
//	double XQ1i=0.;
//	double XQ2i=0.;
//	double XQ1e=0.;
//	double XQ2e=0.;
//	double XDi=0.;
//	double XDe=0.;
//	double XFP=0.;
//	double XpFP=0.;
//	double YQ1i=0.;
//	double YQ2i=0.;
//	double YQ1e=0.;
//	double YQ2e=0.;
//	double YDi=0.;
//	double YDe=0.;
//	double YFP=0.;
//	double YpFP=0.;
//	char trig=0;
//	char evdtrig=0;
//	char ehtrig=0;
//	char QQDTrig=0;
//	tree_e[j_scan]->SetBranchStatus("*",0);
//	tree_e[j_scan]->SetBranchStatus("EMom",1);tree_e[j_scan]->SetBranchAddress("EMom",&mom);
//	tree_e[j_scan]->SetBranchStatus("ETheta",1);tree_e[j_scan]->SetBranchAddress("ETheta",&th);
//    tree_e[j_scan]->SetBranchStatus("EPhi"    ,1);tree_e[j_scan]->SetBranchAddress("EPhi"    ,&ph     );
//    tree_e[j_scan]->SetBranchStatus("EXt"     ,1);tree_e[j_scan]->SetBranchAddress("EXt"     ,&xt     );
//    tree_e[j_scan]->SetBranchStatus("EYt"     ,1);tree_e[j_scan]->SetBranchAddress("EYt"     ,&yt     );
//    tree_e[j_scan]->SetBranchStatus("EXpt"    ,1);tree_e[j_scan]->SetBranchAddress("EXpt"    ,&xpt    );
//    tree_e[j_scan]->SetBranchStatus("EYpt"    ,1);tree_e[j_scan]->SetBranchAddress("EYpt"    ,&ypt    );
//    tree_e[j_scan]->SetBranchStatus("EZt"     ,1);tree_e[j_scan]->SetBranchAddress("EZt"     ,&zt     );
//    tree_e[j_scan]->SetBranchStatus("EXQ1i"   ,1);tree_e[j_scan]->SetBranchAddress("EXQ1i"   ,&XQ1i  );
//    tree_e[j_scan]->SetBranchStatus("EXQ2i"   ,1);tree_e[j_scan]->SetBranchAddress("EXQ2i"   ,&XQ2i  );
//    tree_e[j_scan]->SetBranchStatus("EXQ1e"   ,1);tree_e[j_scan]->SetBranchAddress("EXQ1e"   ,&XQ1e  );
//    tree_e[j_scan]->SetBranchStatus("EXQ2e"   ,1);tree_e[j_scan]->SetBranchAddress("EXQ2e"   ,&XQ2e  );
//    tree_e[j_scan]->SetBranchStatus("EXDi"    ,1);tree_e[j_scan]->SetBranchAddress("EXDi"    ,&XDi  );
//    tree_e[j_scan]->SetBranchStatus("EXDe"    ,1);tree_e[j_scan]->SetBranchAddress("EXDe"    ,&XDe  );
//    tree_e[j_scan]->SetBranchStatus("EXFP2"   ,1);tree_e[j_scan]->SetBranchAddress("EXFP2"   ,&XFP  );
//    tree_e[j_scan]->SetBranchStatus("EXpFP2"  ,1);tree_e[j_scan]->SetBranchAddress("EXpFP2"  ,&XpFP  );
//    tree_e[j_scan]->SetBranchStatus("EYQ1i"   ,1);tree_e[j_scan]->SetBranchAddress("EYQ1i"   ,&YQ1i  );
//    tree_e[j_scan]->SetBranchStatus("EYQ2i"   ,1);tree_e[j_scan]->SetBranchAddress("EYQ2i"   ,&YQ2i  );
//    tree_e[j_scan]->SetBranchStatus("EYQ1e"   ,1);tree_e[j_scan]->SetBranchAddress("EYQ1i"   ,&YQ1e  );
//    tree_e[j_scan]->SetBranchStatus("EYQ2e"   ,1);tree_e[j_scan]->SetBranchAddress("EYQ2i"   ,&YQ2e  );
//    tree_e[j_scan]->SetBranchStatus("EYDi"    ,1);tree_e[j_scan]->SetBranchAddress("EYDi"    ,&YDi  );
//    tree_e[j_scan]->SetBranchStatus("EYDe"    ,1);tree_e[j_scan]->SetBranchAddress("EYDe"    ,&YDe  );
//    tree_e[j_scan]->SetBranchStatus("EYFP2"   ,1);tree_e[j_scan]->SetBranchAddress("EYFP2"   ,&YFP  );
//    tree_e[j_scan]->SetBranchStatus("EYpFP2"  ,1);tree_e[j_scan]->SetBranchAddress("EYpFP2"  ,&YpFP  );
//    tree_e[j_scan]->SetBranchStatus("EVDTrig_oku"    ,1);tree_e[j_scan]->SetBranchAddress("EVDTrig_oku"    ,&evdtrig);
//    tree_e[j_scan]->SetBranchStatus("EHTrig" ,1);tree_e[j_scan]->SetBranchAddress("EHTrig" ,&ehtrig);

    double ENum_e = tree_e[j_scan]->GetEntries();
    int NofFill_e = 0;
    Long64_t tentryt;

    cout << "Event Fill of HES" << endl;
    //HES
    for(int jj = 1; jj <= ENum_e; jj++){ //event fill

        if(jj%10000==0) cout << jj << " / " << ENum_e << endl;
        tree_e[j_scan]->GetEntry(jj);

	        h_e_momgen[j_scan]->Fill(mom);

  if (    XQ1i>-1000.
       && XQ1e>-1000.
       && XQ2i>-1000.
       && XQ2e>-1000.
       && XDi>-1000.
       && XDe>-1000.
       && XFP>-1000.
       && XpFP>-1000.
       && YFP>-1000.
       && YpFP>-1000.
       ){QQDTrig=true;}else{QQDTrig=false;}
			//if(evdtrig == 1 && ehtrig == 1) //oku
			//if(QQDTrig == true)
			if(ETrig_b == true){ //oku

              if(th*PI/180.>0.033)h_e_momhit[j_scan]->Fill(mom);
  
              //Fill for Momentum Reconstruction
              data_hes_p_t    = (mom- Momm_HES)/Momr_HES;
              data_hes_x_t    = (  xt -  xtm_HES)/ xtr_HES;
              data_hes_y_t    = (  yt -  ytm_HES)/ ytr_HES;
              data_hes_z_t    = (  zt -  ztm_HES)/ ztr_HES;
              data_hes_xp_t   = ( xpt - Xptm_HES)/Xptr_HES;
              data_hes_yp_t   = ( ypt - Yptm_HES)/Yptr_HES;
              data_hes_e_p_t  =         1e-5;
              data_hes_e_z_t  =         1e-5;
              data_hes_e_xp_t =         1e-5;
              data_hes_e_yp_t =         1e-5;
              data_hes_x_rp   = ( XFP -  XFPm_HES)/ XFPr_HES;
              data_hes_y_rp   = ( YFP -  YFPm_HES)/ YFPr_HES;
              data_hes_xp_rp  = ( XpFP - XpFPm_HES)/XpFPr_HES;
              data_hes_yp_rp  = ( YpFP - YpFPm_HES)/YpFPr_HES;
              true_hes_p_t    = mom;
              true_hes_z_t    =   zt;
              true_hes_xp_t   =  xpt;
              true_hes_yp_t   =  ypt;
  
              //HES Detector Resolution
              if(Reso_xt>0.){
                data_hes_x_rp  = ( gRandom->Gaus(XFP, Reso_xt) -  XFPm_HES)/ XFPr_HES;
              } //if Reso_xt>0.
              if(Reso_yt>0.){
                data_hes_y_rp  = ( gRandom->Gaus(YFP, Reso_yt) -  YFPm_HES)/ YFPr_HES;
              } //if Reso_yt>0.
              if(Reso_xpt>0.){
                data_hes_xp_rp  = ( gRandom->Gaus(XpFP, Reso_xpt) -  XpFPm_HES)/ XpFPr_HES;
              } //if Reso_xpt>0.
              if(Reso_ypt>0.){
                data_hes_yp_rp  = ( gRandom->Gaus(YpFP, Reso_ypt) -  YpFPm_HES)/ YpFPr_HES;
              } //if Reso_ypt>0.
  
              // reconstructed value by backward matrix
              double calc_hes_p_t  = 0.; //at target
              double calc_hes_xp_t = 0.;
              double calc_hes_yp_t = 0.;
              double calc_hes_zt_t = 0.;
  
              //Reconstruction
              for(int j_matrix = 0; j_matrix < nParam; j_matrix++){
                //HES
                calc_hes_p_t = calc_hes_p_t
                             + ParHESMom[0][j_matrix] 
                             * pow(data_hes_x_rp , ParHESMom[1][j_matrix])
                             * pow(data_hes_xp_rp, ParHESMom[2][j_matrix])
                             * pow(data_hes_y_rp , ParHESMom[3][j_matrix])
                             * pow(data_hes_yp_rp, ParHESMom[4][j_matrix]);  
    
                calc_hes_xp_t = calc_hes_xp_t
                             + ParHESXp[0][j_matrix] 
                             * pow(data_hes_x_rp , ParHESXp[1][j_matrix])
                             * pow(data_hes_xp_rp, ParHESXp[2][j_matrix])
                             * pow(data_hes_y_rp , ParHESXp[3][j_matrix])
                             * pow(data_hes_yp_rp, ParHESXp[4][j_matrix]);  
    
                calc_hes_yp_t = calc_hes_yp_t
                             + ParHESYp[0][j_matrix] 
                             * pow(data_hes_x_rp , ParHESYp[1][j_matrix])
                             * pow(data_hes_xp_rp, ParHESYp[2][j_matrix])
                             * pow(data_hes_y_rp , ParHESYp[3][j_matrix])
                             * pow(data_hes_yp_rp, ParHESYp[4][j_matrix]);  
    
                calc_hes_zt_t = calc_hes_zt_t
                             + ParHESZt[0][j_matrix] 
                             * pow(data_hes_x_rp , ParHESZt[1][j_matrix])
                             * pow(data_hes_xp_rp, ParHESZt[2][j_matrix])
                             * pow(data_hes_y_rp , ParHESZt[3][j_matrix])
                             * pow(data_hes_yp_rp, ParHESZt[4][j_matrix]);  
              } //for j_matrix

  	          calc_hes_p_t  = calc_hes_p_t  * Momr_HES + Momm_HES;
  	          calc_hes_xp_t = calc_hes_xp_t * Xptr_HES + Xptm_HES;
  	          calc_hes_yp_t = calc_hes_yp_t * Yptr_HES + Yptm_HES;
  	          calc_hes_zt_t = calc_hes_zt_t * ztr_HES + ztm_HES;
  
              //Fill Reconstructed mom/angle resolution
              h_e_momreso[j_scan]->Fill((true_hes_p_t - calc_hes_p_t)/true_hes_p_t);
              h_e_xpreso[j_scan]->Fill((true_hes_xp_t - calc_hes_xp_t)*1000.);
              h_e_ypreso[j_scan]->Fill((true_hes_yp_t - calc_hes_yp_t)*1000.);
    
              NofFill_e ++;
    
		}//trig
    } //for event fill

    cout<<"FillSolidAngle"<<endl;
    int n1, n2;
    double val;
    double err;

    //
    // Solid Angle v.s Momentum
    //
    for(int i=0; i<bin_mom; i++){
      n1 = 0;
      n1 = h_e_momgen[j_scan]->GetBinContent(i+1);
      
      // Ref. Plane
      n2 = 0;
      val = 0.;
      err = 0.;
      n2 = h_e_momhit[j_scan]->GetBinContent(i+1);
      if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
//        if(val!=0)cout<< n1 << " " << n2 << " " << val << endl;
      h_e_momacc[j_scan]->SetBinContent(i+1, val);
      if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
      h_e_momacc[j_scan]->SetBinError(i+1, err);
      
    } //for bin(mom)


    cout<<"Fill Avarage Solid Angle"<<endl;
    //double integral_min = min_mom; //[GeV/c]
    //double integral_max = max_mom; //[GeV/c]
    double integral_min = 0.844-0.844*0.175; //[GeV/c]
    double integral_max = 0.844+0.844*0.175; //[GeV/c]

    double e_sa = h_e_momacc[j_scan]->Integral(bin_mom*(integral_min-min_mom)/(max_mom-min_mom), bin_mom*(integral_max-min_mom)/(max_mom-min_mom)) / (bin_mom*(integral_max-min_mom)/(max_mom-min_mom) - bin_mom*(integral_min-min_mom)/(max_mom-min_mom));
    //ave_sa[j_scan] = h_e_momacc[j_scan]->Integral(bin_mom*(integral_min-min_mom)/(max_mom-min_mom), bin_mom*(integral_max-min_mom)/(max_mom-min_mom)) / (bin_mom*(integral_max-min_mom)/(max_mom-min_mom) - bin_mom*(integral_min-min_mom)/(max_mom-min_mom));
    
    ave_sa[j_scan] = e_sa;
    cout << "Ave. Solid Angle: " << ave_sa[j_scan] << " [msr]" << endl;
    setting_num[j_scan] = (double) j_scan+1;

    //Momentum or Angle Resolution
    //double e_momreso = h_e_momreso[j_scan]->GetRMS();
    //double e_xpreso  = h_e_xpreso[j_scan] ->GetRMS();
    //double e_ypreso  = h_e_ypreso[j_scan] ->GetRMS();
    
	fgaus[j_scan] = new TF1(Form("fgaus[%d]",j_scan),"gaus",-0.004,0.004);
	fgaus[j_scan]->SetNpx(2000);
    h_e_momreso[j_scan]->Fit(Form("fgaus[%d]",j_scan),"","",-0.008,0.008);
    double e_momreso = 2.35*fgaus[j_scan]->GetParameter(2);
	if(h_e_momreso[j_scan]->Integral()<2000.)e_momreso=2e-2;
	cout<<"Integral="<<h_e_momreso[j_scan]->Integral()<<endl;
    double e_xpreso  = h_e_xpreso[j_scan] ->GetRMS();
    double e_ypreso  = h_e_ypreso[j_scan] ->GetRMS();

    //double FoM  = e_sa/e_momreso;
    double FoM  = sqrt(e_sa)/e_momreso;

    mom_reso[j_scan] = e_momreso;
    xp_reso[j_scan]  = e_xpreso;
    yp_reso[j_scan]  = e_ypreso;
    fom[j_scan]      = FoM;

    //Fill 2D Maps
    int binnum_q1 =  1 + (int (j_scan/21));
    int binnum_q2 =  1 + (int (j_scan%21));
    h2_e_samap  -> SetBinContent(binnum_q1, binnum_q2,         e_sa);
    h2_e_dpmap  -> SetBinContent(binnum_q1, binnum_q2,    e_momreso);
    h2_e_dxpmap -> SetBinContent(binnum_q1, binnum_q2,     e_xpreso);
    h2_e_dypmap -> SetBinContent(binnum_q1, binnum_q2,     e_ypreso);
    h2_e_fommap -> SetBinContent(binnum_q1, binnum_q2,  FoM);

    //evaluate suzuki parameter (in order to tell you the best setup)
    if(FoM > suzuki_tmp){
      suzuki_tmp = FoM;
      best_setup = j_scan;
    } //if

  } //for j_scan

  return best_setup;

} //FillHist()

//____________________________________________________________________________________________
void SetTH1(TH1 *h, TString hname, TString xname, TString yname, int LColor, int LStyle, int FStyle, int FColor){
  h->SetTitle(hname);
  h->SetLineColor(LColor);
  h->SetLineStyle(LStyle);
  //h->SetLineWidth(1);
  h->SetLineWidth(2);
  h->SetFillStyle(FStyle);
  h->SetFillColor(FColor);
  //h->SetMinimum(0.8);
  h->SetMinimum(0.);
  h->SetMarkerColor(FColor);

  h->GetXaxis()->SetTitle(xname);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleFont(42);
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetLabelOffset(0.01);

  h->GetYaxis()->SetTitle(yname);
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelOffset(0.01);
  ((TGaxis*)h->GetYaxis())->SetMaxDigits(3);
}
//____________________________________________________________________________________________
void SetTH2(TH2 *h, TString name, TString xname, TString yname, double min, int MColor, double MStyle, double MSize){
  h->SetTitle(name);
//  h->SetMinimum(min);
  h->SetLineWidth(1);
  h->SetTitleSize(0.06,"");
  h->SetMarkerColor(MColor);
  h->SetMarkerStyle(1);
  h->SetMarkerSize(0.1);
//  h->SetMarkerColor(1);

  h->GetXaxis()->SetTitle(xname);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleFont(42);
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetTitleSize(0.06);
  //h->GetXaxis()->SetLabelFont(42);
  //h->GetXaxis()->SetLabelOffset(0.01);
  h->GetXaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetLabelOffset(0.01);

  h->GetYaxis()->SetTitle(yname);
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleFont(42);
  //h->GetYaxis()->SetTitleOffset(0.85);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetYaxis()->SetTitleSize(0.06);
  //h->GetYaxis()->SetLabelFont(42);
  //h->GetYaxis()->SetLabelOffset(0.01);
  h->GetYaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelOffset(0.01);
  ((TGaxis*)h->GetYaxis())->SetMaxDigits(3);

  h->GetXaxis()->SetDecimals(2);
  h->GetYaxis()->SetDecimals(2);

  h->SetStats(0);
}
void SetGrErr(TGraphErrors *gr, TString hname, TString xname, TString yname, int LColor, int MColor, int MStyle, double Yoffset, double min, double max){
  gr->SetTitle(hname);
  gr->SetName(hname);
  gr->GetXaxis()->SetTitle(xname);
  gr->GetXaxis()->CenterTitle();
  gr->GetXaxis()->SetTitleFont(42);
  gr->GetXaxis()->SetTitleSize(0.06);
  gr->GetXaxis()->SetTitleOffset(1.0);
  gr->GetXaxis()->SetLabelFont(42);
  gr->GetXaxis()->SetLabelSize(0.05);
  gr->GetXaxis()->SetLabelOffset(0.01);
  gr->GetYaxis()->SetTitle(yname);
  gr->GetYaxis()->SetTitleOffset(1.15);
  gr->GetYaxis()->CenterTitle();
  gr->GetYaxis()->SetTitleFont(42);
  gr->GetYaxis()->SetTitleSize(0.06);
  gr->GetYaxis()->SetLabelFont(42);
  gr->GetYaxis()->SetLabelSize(0.05);
  gr->GetYaxis()->SetLabelOffset(0.01);
  gr->SetLineColor(LColor);
  gr->SetLineWidth(2);
  gr->SetMarkerStyle(MStyle);
  gr->SetMarkerColor(MColor);
  gr->SetMarkerSize(1.0);
//  gr->GetYaxis()->SetTitleOffset(Yoffset);
  gr->GetYaxis()->SetRangeUser(min,max);
}

void anaScan(                                    //MAIN FUNCTION
){
  gStyle->SetOptStat(0);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);

  cout<<"OpenHESRootFile"<<endl;
  TChain *tree_hes[NofScan];

  for(int j=0;j<NofScan;j++){
    //tree_hes[j] = new TChain("tree"); tree_hes[j] -> Add(Form("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/data20220111_21x21/case%d.root",j));
    tree_hes[j] = new TChain("tree"); tree_hes[j] -> Add(Form("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/data20220310_0515h/case%d.root",j));
  } //for j

  cout<<"OpenLogFile"<<endl;
  std::string filename = "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/data20220310_0515h/case0";//as a representitive
  //std::string filename = "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/data/case0";//as a representitive
  std::ostringstream LogFile;
  LogFile << filename << ".root_Log";	
  
  FILE *fp;
  const int SizeOfBuffer = 64;
  char str[SizeOfBuffer];

  double centralmom   = 0.;
  double centraltheta = 0.;
  double centralphi   = 0.;
  double thetamin     = 0.;
  double phimin       = 0.;
  double momwidth     = 0.;
  double thetawidth   = 0.;
  double phiwidth     = 0.;
  double thetamax     = 0.;
  double phimax       = 0.;
  double omega        = 0.; // [msr]
  
  if((fp=fopen(LogFile.str().c_str(), "r"))==NULL){
    std::cerr << "file open fail" << std::endl;
    exit(1);
  }

  while(fgets( str, SizeOfBuffer, fp)){
    if(sscanf( str, "Theta central(rad) = %lf", &centraltheta )==1){
    }
    else if(sscanf( str, "Theta Gen. Range(rad) = %lf", &thetawidth )==1){
    }
    else if(sscanf( str, "Central Momentum(GeV) = %lf", &centralmom )==1){
    }
    else if(sscanf( str, "Momentum Emittance(GeV) = %lf", &momwidth )==1){
    }
    else if(sscanf( str, "Phi central(rad) = %lf", &centralphi )==1){
    }
    else if(sscanf( str, "Phi Gen. Range(rad) = %lf", &phiwidth )==1){
    }
  }
  fclose(fp);
  cout<< "momntum = " << centralmom << " +- " << momwidth << " [GeV/c]" << endl;
  cout<< "central theta = " << centraltheta*180/PI << " [deg]" << endl;
  cout<< "theta width= " << thetawidth*180/PI << " [deg]" << endl;
  if(centralphi == 3.1415) centralphi = 1.*PI;
  
  thetamin = 1.*(1.*centraltheta - thetawidth);
  //thetamin = 0.;
  thetamax = 1.*(1.*centraltheta + thetawidth);
  //thetamax = 1.*thetawidth;
  double mommin = 1.*(centralmom - momwidth);
  double mommax = 1.*(centralmom + momwidth);
  phimin = 1.*(centralphi - phiwidth);
  //phimin = -1.*(centralphi - phiwidth);
  phimax = 1.*(centralphi + phiwidth);
  //omega = 1.0*(phimax - phimin) * (cos(thetamin) - cos(thetamax))*1000.; // [msr]
//  omega = 1.0*(phimax - phimin) * (1.0-cos(thetawidth))*1000.; // [msr]
  omega = sin(centraltheta)*(thetamax-thetamin)*(phimax-phimin)*1000.; // [msr]
  cout<< "mommin = "   << mommin << " [GeV/c]" <<endl;
  cout<< "mommax = "   << mommax << " [GeV/c]" <<endl;
  cout<< "thetamin = " << thetamin*180/PI << " [deg]" <<endl;
  cout<< "thetamax = " << thetamax*180/PI << " [deg]" <<endl;
  cout<< "phimin = " << phimin*180/PI << " [deg]" <<endl;
  cout<< "phimax = " << phimax*180/PI << " [deg]" <<endl;
  cout<< "omega = " << omega << " [msr]" <<endl;


  cout << "Make Hist" << endl;
  TH1D* h_hes_mom_gen[NofScan];
  TH1D* h_hes_mom_hit[NofScan];
  TH1D* h_hes_mom_sa[NofScan];
  TH1D* h_hes_dp[NofScan];
  TH1D* h_hes_dxp[NofScan];
  TH1D* h_hes_dyp[NofScan];

  for(int j=0;j<NofScan;j++){
    h_hes_mom_gen[j] = new TH1D(Form("h_hes_mom_gen_%d",j), Form("h_hes_mom_gen_%d",j), bin_mom, min_mom,max_mom);
    h_hes_mom_hit[j] = new TH1D(Form("h_hes_mom_hit_%d",j), Form("h_hes_mom_hit_%d",j), bin_mom, min_mom,max_mom);
    h_hes_mom_sa[j]  = new TH1D(Form("h_hes_mom_sa_%d",j) , Form("h_hes_mom_sa_%d",j) , bin_mom, min_mom,max_mom);
    h_hes_dp[j]      = new TH1D(Form("h_hes_dp_%d" ,j)    , Form("h_hes_dp_%d" ,j)    , bin_dp , min_dp ,max_dp );
    h_hes_dxp[j]     = new TH1D(Form("h_hes_dxp_%d",j)    , Form("h_hes_dxp_%d",j)    , bin_dxp, min_dxp,max_dxp);
    h_hes_dyp[j]     = new TH1D(Form("h_hes_dyp_%d",j)    , Form("h_hes_dyp_%d",j)    , bin_dyp, min_dyp,max_dyp);
  } //for j

  TH2D* h2_hes_sa_map  = new TH2D("h2_hes_sa_map"  ,"h2_hes_sa_map"  ,bin_fg,min_fg,max_fg,bin_fg,min_fg,max_fg);
  TH2D* h2_hes_dp_map  = new TH2D("h2_hes_dp_map"  ,"h2_hes_dp_map"  ,bin_fg,min_fg,max_fg,bin_fg,min_fg,max_fg);
  TH2D* h2_hes_dxp_map = new TH2D("h2_hes_dxp_map" ,"h2_hes_dxp_map" ,bin_fg,min_fg,max_fg,bin_fg,min_fg,max_fg);
  TH2D* h2_hes_dyp_map = new TH2D("h2_hes_dyp_map" ,"h2_hes_dyp_map" ,bin_fg,min_fg,max_fg,bin_fg,min_fg,max_fg);
  TH2D* h2_hes_fom_map = new TH2D("h2_hes_fom_map" ,"h2_hes_fom_map" ,bin_fg,min_fg,max_fg,bin_fg,min_fg,max_fg); //suzuki parameter

  cout << "Setting" << endl;
  for(int j=0;j<NofScan;j++){
    SetTH1(h_hes_mom_gen[j], Form("Setting No. %d",j+1), "Momentum [GeV/c]"  , "Counts"           ,kAzure  +7,1,3005,kAzure  +7);
    SetTH1(h_hes_mom_hit[j], Form("Setting No. %d",j+1), "Momentum [GeV/c]"  , "Counts"           ,kAzure  +7,1,3005,kAzure  +7);
    SetTH1(h_hes_mom_sa[j] , Form("Setting No. %d",j+1), "Momentum [GeV/c]"  , "Solid Angle [msr]",kAzure  +7,1,3005,kAzure  +7);
    SetTH1(h_hes_dp[j]     , Form("Setting No. %d",j+1), "dp/p [true - calc]", "Counts"           ,kAzure  +7,1,3005,kAzure  +7);
    SetTH1(h_hes_dxp[j]    , Form("Setting No. %d",j+1), "dX' [mrad]"        , "Counts"           ,kAzure  +7,1,3005,kAzure  +7);
    SetTH1(h_hes_dyp[j]    , Form("Setting No. %d",j+1), "dY' [mrad]"        , "Counts"           ,kAzure  +7,1,3005,kAzure  +7);
  } //for j

  SetTH2(h2_hes_sa_map ,"Solid Angle [msr]","Q1 Scale Factor","Q2 Scale Factor",0.,kAzure  +7,1.,0.8);
  SetTH2(h2_hes_dp_map ,"dp/p"             ,"Q1 Scale Factor","Q2 Scale Factor",0.,kAzure  +7,1.,0.8);
  SetTH2(h2_hes_dxp_map,"dX' [mrad]"       ,"Q1 Scale Factor","Q2 Scale Factor",0.,kAzure  +7,1.,0.8);
  SetTH2(h2_hes_dyp_map,"dY' [mrad]"       ,"Q1 Scale Factor","Q2 Scale Factor",0.,kAzure  +7,1.,0.8);
  SetTH2(h2_hes_fom_map,"FoM"     ,"Q1 Scale Factor","Q2 Scale Factor",0.,kAzure  +7,1.,0.8);

  //Data array for TGraph
  double set[NofScan] = {};
  double e_set[NofScan] = {};
  double sa[NofScan] = {};
  double e_sa[NofScan] = {};
  double dp[NofScan] = {};
  double e_dp[NofScan] = {};
  double dxp[NofScan] = {};
  double e_dxp[NofScan] = {};
  double dyp[NofScan] = {};
  double e_dyp[NofScan] = {};
  double fom[NofScan] = {};
  double e_fom[NofScan] = {};

  cout << "Fill Hist" << endl;
  int best_setup = FillHist(omega, tree_hes, h_hes_mom_gen, h_hes_mom_hit, h_hes_mom_sa, h_hes_dp, h_hes_dxp, h_hes_dyp, set, sa, dp, dxp, dyp, fom, h2_hes_sa_map, h2_hes_dp_map, h2_hes_dxp_map, h2_hes_dyp_map, h2_hes_fom_map);

  cout << "Make Graph" << endl;
  TGraphErrors* g_sa  = new TGraphErrors(NofScan,set, sa,e_set, e_sa);
  TGraphErrors* g_dp  = new TGraphErrors(NofScan,set, dp,e_set, e_dp);
  TGraphErrors* g_dxp = new TGraphErrors(NofScan,set,dxp,e_set,e_dxp);
  TGraphErrors* g_dyp = new TGraphErrors(NofScan,set,dyp,e_set,e_dyp);
  TGraphErrors* g_fom = new TGraphErrors(NofScan,set,fom,e_set,e_fom);
  SetGrErr(g_sa , "", "Setting Number", "Ave. Solid Angle [msr]"    , kAzure  +7, kAzure  +7, 21, 0.8, 0.,  11.000);
  SetGrErr(g_dp , "", "Setting Number", "dp/p Resolution (FWHM)"     , kViolet+7, kViolet+7, 21, 0.8, 0.,  0.004);
  SetGrErr(g_dxp, "", "Setting Number", "X' Resolution (RMS) [mrad]", kOrange+7, kOrange+7, 21, 0.8, 0.,  100.0);
  SetGrErr(g_dyp, "", "Setting Number", "Y' Resolution (RMS) [mrad]", kRed     , kRed     , 21, 0.8, 0.,  100.0);
  SetGrErr(g_fom, "", "Setting Number", "FoM"          , kYellow+2, kYellow+2, 21, 0.8, 0.,  20000);

//  cout << "Draw" << endl;
//  //Momentum Acceptance
//  TCanvas* c00[NofScan];
//  for(int i_ch=430;i_ch<433;i_ch++){
//    c00[i_ch] = new TCanvas(Form("c00_%d",i_ch),Form("c00_%d",i_ch),800,600);
//    c00[i_ch]->Clear();c00[i_ch]->Divide(1,1,1E-4,1E-4);
//      //c00[i_ch]->cd(1);/*gPad->SetLogy(1);*/h_hes_mom_gen[i_ch]->Draw("EP");
//      //c00[i_ch]->cd(1);/*gPad->SetLogy(1);*/h_hes_mom_hit[i_ch]->Draw("EP");
//      c00[i_ch]->cd(1);/*gPad->SetLogy(1);*/h_hes_mom_sa[i_ch]->Draw("EP");
//  } //for i_ch

  TCanvas* c01;
  c01 = new TCanvas(Form("c01"),Form("c01"),800,600);
  c01->Clear();c01->Divide(1,1,1E-4,1E-4);
    c01->cd(1);/*gPad->SetLogy(1);*/g_sa->Draw("APL");


//  //Momentum Resolution
//  TCanvas* c02[NofScan];
//  for(int i_ch=430;i_ch<433;i_ch++){
//    c02[i_ch] = new TCanvas(Form("c02_%d",i_ch),Form("c02_%d",i_ch),800,600);
//    c02[i_ch]->Clear();c02[i_ch]->Divide(1,1,1E-4,1E-4);
//      c02[i_ch]->cd(1);/*gPad->SetLogy(1);*/h_hes_dp[i_ch]->Draw("EP");
//  } //for i_ch

  TCanvas* c03;
  c03 = new TCanvas(Form("c03"),Form("c03"),800,600);
  c03->Clear();c03->Divide(1,1,1E-4,1E-4);
    c03->cd(1);/*gPad->SetLogy(1);*/g_dp->Draw("APL");


//  //X' Resolution
//  TCanvas* c04[NofScan];
//  for(int i_ch=0;i_ch<25;i_ch++){
//    c04[i_ch] = new TCanvas(Form("c04_%d",i_ch),Form("c04_%d",i_ch),800,600);
//    c04[i_ch]->Clear();c04[i_ch]->Divide(1,1,1E-4,1E-4);
//      c04[i_ch]->cd(1);/*gPad->SetLogy(1);*/h_hes_dxp[i_ch]->Draw("EP");
//  } //for i_ch

  TCanvas* c05;
  c05 = new TCanvas(Form("c05"),Form("c05"),800,600);
  c05->Clear();c05->Divide(1,1,1E-4,1E-4);
    c05->cd(1);/*gPad->SetLogy(1);*/g_dxp->Draw("APL");


//  //Y' Resolution
//  TCanvas* c06[NofScan];
//  for(int i_ch=0;i_ch<25;i_ch++){
//    c06[i_ch] = new TCanvas(Form("c06_%d",i_ch),Form("c06_%d",i_ch),800,600);
//    c06[i_ch]->Clear();c06[i_ch]->Divide(1,1,1E-4,1E-4);
//      c06[i_ch]->cd(1);/*gPad->SetLogy(1);*/h_hes_dyp[i_ch]->Draw("EP");
//  } //for i_ch

  TCanvas* c07;
  c07 = new TCanvas(Form("c07"),Form("c07"),800,600);
  c07->Clear();c07->Divide(1,1,1E-4,1E-4);
    c07->cd(1);/*gPad->SetLogy(1);*/g_dyp->Draw("APL");

  //2D Map
  TCanvas* c08;
  c08 = new TCanvas(Form("c08"),Form("c08"),800,600);
  c08->Clear();c08->Divide(1,1,1E-4,1E-4);
    c08->cd(1);/*gPad->SetLogy(1);*/h2_hes_sa_map->Draw("colz");

  TCanvas* c09;
  c09 = new TCanvas(Form("c09"),Form("c09"),800,600);
  c09->Clear();c09->Divide(1,1,1E-4,1E-4);
    c09->cd(1);gPad->SetLogz(1);h2_hes_dp_map->Draw("colz");

  TCanvas* c10;
  c10 = new TCanvas(Form("c10"),Form("c10"),800,600);
  c10->Clear();c10->Divide(1,1,1E-4,1E-4);
    c10->cd(1);/*gPad->SetLogy(1);*/h2_hes_dxp_map->Draw("colz");

  TCanvas* c11;
  c11 = new TCanvas(Form("c11"),Form("c11"),800,600);
  c11->Clear();c11->Divide(1,1,1E-4,1E-4);
    c11->cd(1);/*gPad->SetLogy(1);*/h2_hes_dyp_map->Draw("colz");

  TCanvas* c12;
  c12 = new TCanvas(Form("c12"),Form("c12"),800,600);
  c12->Clear();c12->Divide(1,1,1E-4,1E-4);
    c12->cd(1);/*gPad->SetLogy(1);*/g_fom->Draw("APL");

  TCanvas* c13;
  c13 = new TCanvas(Form("c13"),Form("c13"),800,600);
  c13->Clear();c13->Divide(1,1,1E-4,1E-4);
    c13->cd(1);/*gPad->SetLogy(1);*/h2_hes_fom_map->Draw("colz");


  //best setup
  TCanvas* c96;
  c96 = new TCanvas(Form("c96"),Form("c96"),800,600);
  c96->Clear();c96->Divide(1,1,1E-4,1E-4);
    c96->cd(1);/*gPad->SetLogy(1);*/h_hes_mom_sa[best_setup]->Draw("EP");

  TCanvas* c97;
  c97 = new TCanvas(Form("c97"),Form("c97"),800,600);
  c97->Clear();c97->Divide(1,1,1E-4,1E-4);
    c97->cd(1);/*gPad->SetLogy(1);*/h_hes_dp[best_setup]->Draw("EP");

  TCanvas* c98;
  c98 = new TCanvas(Form("c98"),Form("c98"),800,600);
  c98->Clear();c98->Divide(1,1,1E-4,1E-4);
    c98->cd(1);/*gPad->SetLogy(1);*/h_hes_dxp[best_setup]->Draw("EP");

  TCanvas* c99;
  c99 = new TCanvas(Form("c99"),Form("c99"),800,600);
  c99->Clear();c99->Divide(1,1,1E-4,1E-4);
    c99->cd(1);/*gPad->SetLogy(1);*/h_hes_dyp[best_setup]->Draw("EP");

  cout << "Creating a PDF file ... " << endl;
  c01 ->Print("scan_result.pdf[");
  c01 ->Print("scan_result.pdf");
  c03 ->Print("scan_result.pdf");
  c05 ->Print("scan_result.pdf");
  c07 ->Print("scan_result.pdf");
  c08 ->Print("scan_result.pdf");
  c09 ->Print("scan_result.pdf");
  c10 ->Print("scan_result.pdf");
  c11 ->Print("scan_result.pdf");
  c12 ->Print("scan_result.pdf");
  c13 ->Print("scan_result.pdf");
  c96 ->Print("scan_result.pdf");
  c97 ->Print("scan_result.pdf");
  c98 ->Print("scan_result.pdf");
  c99 ->Print("scan_result.pdf");
  c99 ->Print("scan_result.pdf]");

    //double fg_q1 =  0.055 - 0.005*(int (best_setup/23));
    //double fg_q2 = -0.055 + 0.005*(int (best_setup%23));
    double fg_q1 =  min_fg + ((max_fg-min_fg)/(bin_fg-1))*(int (best_setup/bin_fg));
    double fg_q2 =  min_fg + ((max_fg-min_fg)/(bin_fg-1))*(int (best_setup%bin_fg));

  cout << endl << endl << endl;
  cout << "Best Q1 Scale Factor: " << fg_q1 << endl;
  cout << "Best Q2 Scale Factor: " << fg_q2 << endl;
  cout << endl << endl << endl;

} //anaScan()

void TreeBranch( TTree *tree, const char *name, int *branch){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, branch);
}
void TreeBranch( TTree *tree, const char *name, double *branch){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, branch);
}
void TreeBranch( TTree *tree, const char *name, char *branch){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, branch);
}
void TreeBranch( TTree *tree, const char *name, vector<int> **vec, TBranch **bvp){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, vec, bvp);
}
void TreeBranch( TTree *tree, const char *name, vector<double> **vec, TBranch **bvp){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, vec, bvp);
}

