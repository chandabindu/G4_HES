#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TApplication.h>

using namespace std;

void TreeBranch( TTree*, const char*, int*);
void TreeBranch( TTree*, const char*, double*);
void TreeBranch( TTree*, const char*, char*);
void TreeBranch( TTree*, const char*, bool*);
void TreeBranch( TTree*, const char*, vector<int>**, TBranch**);
void TreeBranch( TTree*, const char*, vector<double>**, TBranch**);

  // ======== Offsets and scales for matrices =======
  //------- Original ---------
  //const double  XFPm=0., XpFPm=0.; 
  //const double  YFPm=0., YpFPm=0.; 
  //const double  Xptm=0., Yptm=0., Momm=0.;
  //const double  XFPr=60., XpFPr=0.15; 
  //const double  YFPr=6., YpFPr=0.01; 
  //const double  Xptr=1., Yptr=1., Momr=1.;

  //------- SPL+HES -------//
  //------- Okuyama, 20210903, ktest_large.root ----------------
#if 0
  const double  XFPm = -3.689, XpFPm = -0.01568; 
  const double  YFPm = 0.004998, YpFPm = 0.02835; 
  const double  xtm = 0., xtr=1., Xptm = -0.247;
  const double  ytm = 0., ytr=1., Yptm = -0.002119;
  const double  Momm = 0.844;
  const double  XFPr = 21.94, XpFPr = 0.1007; 
  const double  YFPr = 2.577, YpFPr = 0.007165; 
  const double  Xtr = 0.1, Xptr = 0.0369;
  const double  Ytr = 0.1, Yptr = 0.019;
  const double  Momr = 0.15;
  const double  ztm=0.0 , ztr=1.;
#endif
  //------- PCS+HES -------//
  //------- Okuyama, 20220304, data20220304_2020/case308.root ----------------
  //PCS+HES(vertical)
//at Focal Plane
  const double  XFPm  = 4.085   , XFPr  = 26.17   ; 
  const double  YFPm  = 8.814   , YFPr  =  4.73   ; 
  const double  XpFPm = 0.01694 , XpFPr =  0.118  ; 
  const double  YpFPm = 0.02048 , YpFPr =  0.01531; 
//at Target
  const double  Momm  = 0.844   , Momr  = 0.15    ;
  const double  xtm   = 0.      , xtr   = 1.      ;
  const double  ytm   = 0.      , ytr   = 1.      ;
  const double  ztm   = 0.      , ztr   = 1.      ;
  const double  Xptm  = 0.1691  , Xptr  = 0.02562 ;
  const double  Yptm  =-0.001235, Yptr  = 0.01546 ;



char name_Mmom0[100];
char name_Mxpt0[100];
char name_Mypt0[100];
char name_Mlen0[100];
char name_Mxt0[100];
char name_Myt0[100];
char name_Mzt0[100];
char name_Mzt0_oku[100];
char name_ifile[100];
TFile* file;


int main( int argc, char *argv[])
{

  TApplication app("app", &argc, argv);


  ifstream input("parFile.dat");

  input >> name_ifile;
  input >> name_Mmom0;
  input >> name_Mxpt0;
  input >> name_Mypt0;
  //input >> name_Mlen0;
  input >> name_Mxt0;
  input >> name_Myt0;
  input >> name_Mzt0;
  input >> name_Mzt0_oku;
  input.close();

  ifstream fMom( name_Mmom0 );
  ifstream fXp( name_Mxpt0);
  ifstream fYp( name_Mypt0);
  ifstream fX( name_Mxt0);
  ifstream fY( name_Myt0);
  ifstream fZ( name_Mzt0);
  ifstream fZ_oku( name_Mzt0_oku);

//	const int nParam = 15; // 2 dimension
//	const int nParam = 35; // 3 dimension
//	const int nParam = 105; // 3 dimension + 3
//	const int nParam = 70; // 4 dimension
//	const int nParam = 126; // 5 dimension
	const int nParam = 210; // 6 dimension
//	const int nParam = 330; // 7 dimension
  
  double ParMom[5][nParam];  //{par, pow_X, pow_Xp, pow_Y, pow_Yp}
  double ParXp[5][nParam];  
  double ParYp[5][nParam];  
  double ParX[5][nParam];  
  double ParY[5][nParam];  
  double ParZ[5][nParam];  
  double ParZ_oku[6][nParam];  
  
  for(int i = 0; i < nParam; i++){
    fMom >> ParMom[0][i] >> ParMom[1][i] >> ParMom[2][i] >> ParMom[3][i] >> ParMom[4][i];
    fXp >> ParXp[0][i] >> ParXp[1][i] >> ParXp[2][i] >> ParXp[3][i] >> ParXp[4][i];
    fYp >> ParYp[0][i] >> ParYp[1][i] >> ParYp[2][i] >> ParYp[3][i] >> ParYp[4][i];
    fX >> ParX[0][i] >> ParX[1][i] >> ParX[2][i] >> ParX[3][i] >> ParX[4][i];
    fY >> ParY[0][i] >> ParY[1][i] >> ParY[2][i] >> ParY[3][i] >> ParY[4][i];
    fZ >> ParZ[0][i] >> ParZ[1][i] >> ParZ[2][i] >> ParZ[3][i] >> ParZ[4][i];
    fZ_oku >> ParZ_oku[0][i] >> ParZ_oku[1][i] >> ParZ_oku[2][i] >> ParZ_oku[3][i] >> ParZ_oku[4][i] >> ParZ_oku[5][i];
    //    cout << par[i] << " " <<  pow_X[i] << " " << pow_Xp[i] << " "  << pow_Y[i] << " " << pow_Yp[i] << endl;
  }

  TChain *tree = new TChain("tree");
  tree->Add(name_ifile);

  cout << " Reading : " << name_ifile << endl;
  const int nEntry = tree->GetEntries();
  cout << "nEntry = " << nEntry << endl;

  double EMom = 0;
  double XFP = 0, YFP = 0, XpFP = 0, YpFP = 0;
  double Xt = 0, Yt = 0, Zt = 0, Xpt = 0, Ypt = 0;
  double EHODO1XFP=0, EHODO1YFP=0;
  double EHODO1XpFP=0, EHODO1YpFP=0;
  double EHODO2XFP=0, EHODO2YFP=0;
  double EHODO2XpFP=0, EHODO2YpFP=0;
  double XQ1i = 0, YQ1i = 0;
  double XQ1e=0, YQ1e=0;
  double XQ2i=0, YQ2i=0;
  double XQ2e=0, YQ2e=0;
  double XDi=0, YDi=0;
  double XDe=0, YDe=0;

//-- Cut Conditions --//
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
  char QQDTrig   = false;
  char EDC1Trig  = false;
  char EDC2Trig  = false;
  char EH1Trig   = false;
  char EH2Trig   = false;
  char ETrig     = false;
//Trig. from branch
  char QQDTrig_b = false;
  char FPTrig_b  = false;
  char EDCTrig_b = false;
  char EHTrig_b  = false;
  char ETrig_b   = false;
  char ETrig2_b  = false;
  char ETrig3_b  = false;
cout<<"Trig (offline)="<<(int)QQDTrig<<(int)EDC1Trig<<(int)EDC2Trig<<(int)EH1Trig<<(int)EH2Trig<<(int)ETrig<<endl;
cout<<"Trig (online) ="<<(int)QQDTrig_b<<(int)FPTrig_b<<(int)EDCTrig_b<<(int)ETrig_b<<(int)ETrig2_b<<(int)ETrig3_b<<endl;

	tree->SetBranchStatus("*",0);
	TreeBranch(tree, "EXQ1i", &XQ1i);
	TreeBranch(tree, "EYQ1i", &YQ1i);
	TreeBranch(tree, "EXQ1e", &XQ1e);
	TreeBranch(tree, "EYQ1e", &YQ1e);
	TreeBranch(tree, "EXQ2i", &XQ2i);
	TreeBranch(tree, "EYQ2i", &YQ2i);
	TreeBranch(tree, "EXQ2e", &XQ2e);
	TreeBranch(tree, "EYQ2e", &YQ2e);
	TreeBranch(tree, "EXDi", &XDi);
	TreeBranch(tree, "EYDi", &YDi);
	TreeBranch(tree, "EXDe", &XDe);
	TreeBranch(tree, "EYDe", &YDe);

//if EDC is ON, you can use these instead of FP2
	//TreeBranch(tree, "XEDCFP", &XFP);
	//TreeBranch(tree, "YEDCFP", &YFP);
	//TreeBranch(tree, "XpEDCFP", &XpFP);
	//TreeBranch(tree, "YpEDCFP", &YpFP);
//
	TreeBranch(tree, "EXFP2", &XFP);
	TreeBranch(tree, "EYFP2", &YFP);
	TreeBranch(tree, "EXpFP2", &XpFP);
	TreeBranch(tree, "EYpFP2", &YpFP);
////EHODO1X
	TreeBranch(tree, "EXFP4", &EHODO1XFP);
	TreeBranch(tree, "EYFP4", &EHODO1YFP);
	TreeBranch(tree, "EXpFP4", &EHODO1XpFP);
	TreeBranch(tree, "EYpFP4", &EHODO1YpFP);
////EHODO2X
	TreeBranch(tree, "EXFP5", &EHODO2XFP);
	TreeBranch(tree, "EYFP5", &EHODO2YFP);
	TreeBranch(tree, "EXpFP5", &EHODO2XpFP);
	TreeBranch(tree, "EYpFP5", &EHODO2YpFP);
	TreeBranch(tree, "EXt", &Xt);
	TreeBranch(tree, "EYt", &Yt);
	TreeBranch(tree, "EZt", &Zt);
	TreeBranch(tree, "EXpt", &Xpt);
	TreeBranch(tree, "EYpt", &Ypt);
	TreeBranch(tree, "EMom", &EMom);
	TreeBranch(tree, "EQQDTrig", &QQDTrig_b);
	TreeBranch(tree, "EFPTrig" , &FPTrig_b );
	TreeBranch(tree, "EDCTrig" , &EDCTrig_b);
	TreeBranch(tree, "EHTrig"  , &EHTrig_b );
	TreeBranch(tree, "ETrig"   , &ETrig_b  );
	TreeBranch(tree, "ETrig2"  , &ETrig2_b );
	TreeBranch(tree, "ETrig3"  , &ETrig3_b );

  for (int i=0;i<10;i++){//EDC1
    sprintf(sXEDC,"XEDC%d",i+1); 
    sprintf(sYEDC,"YEDC%d",i+1); 
    TreeBranch(tree, sXEDC, &EDC1X[i]);
    TreeBranch(tree, sYEDC, &EDC1Y[i]);
  }
  for (int i=0;i<6;i++){//EDC2
    sprintf(sXEDC,"XEDC%d",i+11); 
    sprintf(sYEDC,"YEDC%d",i+11); 
    TreeBranch(tree, sXEDC, &EDC2X[i]);
    TreeBranch(tree, sYEDC, &EDC2Y[i]);
  }
  for (int i=0;i<29;i++){//EHODO1,2
    sprintf(sXEH1,"XEH%d",i+101); 
    sprintf(sYEH1,"YEH%d",i+101); 
    sprintf(sXEH2,"XEH%d",i+201); 
    sprintf(sYEH2,"YEH%d",i+201); 
    TreeBranch(tree, sXEH1, &EH1X[i]);
    TreeBranch(tree, sYEH1, &EH1Y[i]);
    TreeBranch(tree, sXEH2, &EH2X[i]);
    TreeBranch(tree, sYEH2, &EH2Y[i]);
  }
  
  //  ----------Momentum hist----- 
  TH1D *histMom_ori = new TH1D("histMom_ori", "histMom_ori", 200, -0.002, 0.002);
  histMom_ori->SetLineColor(kAzure);
  histMom_ori->SetStats(0);

  //  ----------Xp hist------------
  TH1D *histXp_ori = new TH1D("histXp_ori", "histXp_ori", 500, -0.005, 0.005);
  histXp_ori->SetLineColor(kAzure);
  histXp_ori->SetStats(0);

  //  ---------Yp hist------------
  TH1D *histYp_ori = new TH1D("histYp_ori", "histYp_ori", 500, -0.005, 0.005);
  histYp_ori->SetLineColor(kAzure);
  histYp_ori->SetStats(0);

  //  ---------X hist------------
  TH1D *histX_ori = new TH1D("histX_ori", "histX_ori", 500, -20., 20.);
  histX_ori->SetLineColor(kAzure);
  histX_ori->SetStats(0);

  //  ---------Y hist------------
  TH1D *histY_ori = new TH1D("histY_ori", "histY_ori", 500, -20., 20.);
  histY_ori->SetLineColor(kAzure);
  histY_ori->SetStats(0);

  //  ---------Z hist------------
  TH1D *histZ_ori = new TH1D("histZ_ori", "histZ_ori", 500, -20., 20.);
  histZ_ori->SetLineColor(kAzure);
  histZ_ori->SetStats(0);

  TH2D *hist_XXp = new TH2D("hist_XXp", "hist_XXp", 50, -50., 50., 50, -0.2, 0.2);
  TH2D *hist_YYp = new TH2D("hist_YYp", "hist_YYp", 50, -10., 10., 50, -0.1, 0.1);
  TH2D *hist_XZt = new TH2D("hist_XZt", "hist_XZt", 50, -50., 50., 50, -10., 10.);
  TH2D *hist_XpZt = new TH2D("hist_XpZt", "hist_XpZt", 50, -0.2, 0.2, 50, -10., 10.);

  int ntrig = 0;
  int aqqdtrig_b = 0;
  int aqqdtrig = 0;
  int afptrig_b = 0;
  int aetrig_b = 0;
  int aetrig = 0;
  int aedctrig_b = 0;
  int aehtrig_b = 0;

  TRandom3* rnd = new TRandom3();
  for(int i = 1; i < nEntry; i++){
    tree->GetEntry(i);
    double Mom_matrix = 0;
    double Mom_true = EMom;
    double Xp_matrix = 0;
    double Xp_true = Xpt;
    double Yp_matrix = 0;
    double Yp_true = Ypt;
    double X_matrix = 0;
    double X_true = Xt;
    double Y_matrix = 0;
    double Y_true = Yt;
    double Z_matrix = 0;
    double Z_true = Zt;

    if(i%10000 == 0){
      cout << "[ " << i << " ] Filled" << endl;
    }
    
//Cut Condition
  if (    EDC1X[0]>-1000.
       && EDC1X[1]>-1000.
       && EDC1X[2]>-1000.
       && EDC1X[3]>-1000.
       && EDC1X[4]>-1000.
       && EDC1X[5]>-1000.
       && EDC1X[6]>-1000.
       && EDC1X[7]>-1000.
       && EDC1X[8]>-1000.
       && EDC1X[9]>-1000.){EDC1Trig=true;}else{EDC1Trig=false;}
  if (    EDC2X[0]>-1000.
       && EDC2X[1]>-1000.
       && EDC2X[2]>-1000.
       && EDC2X[3]>-1000.
       && EDC2X[4]>-1000.
	   && EDC2X[5]>-1000.){EDC2Trig=true;}else{EDC2Trig=false;}
  if (    EH1X[0]>-1000.
       || EH1X[1]>-1000.
       || EH1X[2]>-1000.
       || EH1X[3]>-1000.
       || EH1X[4]>-1000.
       || EH1X[5]>-1000.
       || EH1X[6]>-1000.
       || EH1X[7]>-1000.
       || EH1X[8]>-1000.
       || EH1X[9]>-1000.
       || EH1X[10]>-1000.
       || EH1X[11]>-1000.
       || EH1X[12]>-1000.
       || EH1X[13]>-1000.
       || EH1X[14]>-1000.
       || EH1X[15]>-1000.
       || EH1X[16]>-1000.
       || EH1X[17]>-1000.
       || EH1X[18]>-1000.
       || EH1X[19]>-1000.
       || EH1X[20]>-1000.
       || EH1X[21]>-1000.
       || EH1X[22]>-1000.
       || EH1X[23]>-1000.
       || EH1X[24]>-1000.
       || EH1X[25]>-1000.
       || EH1X[26]>-1000.
       || EH1X[27]>-1000.
       || EH1X[28]>-1000.){EH1Trig=true;}else{EH1Trig=false;}
  if (    EH2X[0]>-1000.
       || EH2X[1]>-1000.
       || EH2X[2]>-1000.
       || EH2X[3]>-1000.
       || EH2X[4]>-1000.
       || EH2X[5]>-1000.
       || EH2X[6]>-1000.
       || EH2X[7]>-1000.
       || EH2X[8]>-1000.
       || EH2X[9]>-1000.
       || EH2X[10]>-1000.
       || EH2X[11]>-1000.
       || EH2X[12]>-1000.
       || EH2X[13]>-1000.
       || EH2X[14]>-1000.
       || EH2X[15]>-1000.
       || EH2X[16]>-1000.
       || EH2X[17]>-1000.
       || EH2X[18]>-1000.
       || EH2X[19]>-1000.
       || EH2X[20]>-1000.
       || EH2X[21]>-1000.
       || EH2X[22]>-1000.
       || EH2X[23]>-1000.
       || EH2X[24]>-1000.
       || EH2X[25]>-1000.
       || EH2X[26]>-1000.
       || EH2X[27]>-1000.
       || EH2X[28]>-1000.){EH2Trig=true;}else{EH2Trig=false;}
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
	//if(QQDTrig && EDC1Trig && EDC2Trig && EH1Trig && EH2Trig){ETrig=true;}else{ETrig=false;}
	if(QQDTrig && EDC1Trig && EDC2Trig){ETrig=true;}else{ETrig=false;}
	if(QQDTrig_b != QQDTrig){
		cout<<"QQDTrig  ="<<QQDTrig  <<endl;
		cout<<"QQDTrig_b="<<QQDTrig_b<<endl;
		cout<<"FPTrig_b ="<<FPTrig_b <<endl;
	}
	if(QQDTrig_b)aqqdtrig_b++;
	if(QQDTrig)aqqdtrig++;
	if(FPTrig_b)afptrig_b++;
	if(ETrig_b)aetrig_b++;
	if(ETrig)aetrig++;
	if(EDCTrig_b)aedctrig_b++;
	if(EHTrig_b)aehtrig_b++;
    
    if(
        // ETrig_b == true 
        QQDTrig == true 
		&& abs(EHODO1XFP)<58.5
		&& abs(EHODO1YFP)<15.
		&& abs(EHODO2XFP)<58.5
		&& abs(EHODO2YFP)<15.
       )
      {
//resoflag
	XFP =rnd->Gaus(XFP,0.0097);   //[cm]
	YFP =rnd->Gaus(YFP,0.0120);   //[cm]
	XpFP=rnd->Gaus(XpFP,0.8e-3); //[rad]
	YpFP=rnd->Gaus(YpFP,1.5e-3); //[rad]
	double XFP_sca = (XFP - XFPm)/XFPr;
	double XpFP_sca = (XpFP - XpFPm)/XpFPr;
	double YFP_sca = (YFP - YFPm)/YFPr;
	double YpFP_sca = (YpFP - YpFPm)/YpFPr;
	//double Mom_sca = (EMom - Momm)/Momr;

	for(int j = 0; j < nParam; j++){
	  Mom_matrix  = Mom_matrix 
	    + ParMom[0][j] 
	    * pow(XFP_sca, ParMom[1][j])
	    * pow(XpFP_sca, ParMom[2][j])
	    * pow(YFP_sca, ParMom[3][j])
	    * pow(YpFP_sca, ParMom[4][j]);  
    // if(ntrig ==1 ){
    //   cout << "Mom_matrix" << Mom_matrix << endl;
    // }

	  Xp_matrix  = Xp_matrix 
	    + ParXp[0][j] 
	    * pow(XFP_sca, ParXp[1][j])
	    * pow(XpFP_sca, ParXp[2][j])
	    * pow(YFP_sca, ParXp[3][j])
	    * pow(YpFP_sca, ParXp[4][j]);  

	  Yp_matrix  = Yp_matrix 
	    + ParYp[0][j] 
	    * pow(XFP_sca, ParYp[1][j])
	    * pow(XpFP_sca, ParYp[2][j])
	    * pow(YFP_sca, ParYp[3][j])
	    * pow(YpFP_sca, ParYp[4][j]);  

	  X_matrix  = X_matrix 
	    + ParX[0][j] 
	    * pow(XFP_sca, ParX[1][j])
	    * pow(XpFP_sca, ParX[2][j])
	    * pow(YFP_sca, ParX[3][j])
	    * pow(YpFP_sca, ParX[4][j]);  
	    
	  Y_matrix  = Y_matrix 
	    + ParY[0][j] 
	    * pow(XFP_sca, ParY[1][j])
	    * pow(XpFP_sca, ParY[2][j])
	    * pow(YFP_sca, ParY[3][j])
	    * pow(YpFP_sca, ParY[4][j]);  
	    
	  Z_matrix  = Z_matrix 
	    + ParZ[0][j] 
	    * pow(XFP_sca, ParZ[1][j])
	    * pow(XpFP_sca, ParZ[2][j])
	    * pow(YFP_sca, ParZ[3][j])
	    * pow(YpFP_sca, ParZ[4][j]);  
	    
	  //Z_matrix  = Z_matrix 
	  //  + ParZ_oku[0][j] 
	  //  * pow(XFP_sca, ParZ_oku[1][j])
	  //  * pow(XpFP_sca, ParZ_oku[2][j])
	  //  * pow(YFP_sca, ParZ_oku[3][j])
	  //  * pow(YpFP_sca, ParZ_oku[4][j])  
	  //  * pow(Mom_sca, ParZ_oku[5][j]);  
	}

	Mom_matrix = Mom_matrix * Momr + Momm;
	Xp_matrix = Xp_matrix * Xptr + Xptm;
	Yp_matrix = Yp_matrix * Yptr + Yptm;
	X_matrix  = X_matrix * xtr + xtm;
	Y_matrix  = Y_matrix * ytr + ytm;
	Z_matrix  = Z_matrix * ztr + ztm;
		//cout << "Mom_matrix = " << Mom_matrix << " Mom_true = " << Mom_true << endl;

	//if( -2.5< EZ && EZ < 2.5){
	  histMom_ori->Fill( (Mom_true - Mom_matrix)/Mom_true );
	  histXp_ori->Fill( (Xp_true - Xp_matrix) );
	  histYp_ori->Fill( (Yp_true - Yp_matrix) );
	  histX_ori->Fill( (X_true - X_matrix) );
	  histY_ori->Fill( (Y_true - Y_matrix) );
	  histZ_ori->Fill( (Z_true - Z_matrix) );
	  hist_XXp->Fill(XFP,XpFP);
	  hist_YYp->Fill(YFP,YpFP);
	  hist_XZt->Fill(XFP,Z_matrix);
	  hist_XpZt->Fill(XpFP,Z_matrix);
	//}//Z selection
	
	ntrig++;
	
	}//cut condition
}//Fill

  cout << "#Triggered Events = " << ntrig <<endl;

  TCanvas *c_ori = new TCanvas("c_ori", "", 1000, 1000);
  c_ori->Divide(3,1);
  c_ori->cd();  
  c_ori->cd(1);
  histMom_ori->GetXaxis()->SetMaxDigits(2);
  histMom_ori->GetXaxis()->SetNdivisions(515);
  histMom_ori->GetXaxis()->SetLabelSize(0.045);
  histMom_ori->GetYaxis()->SetLabelSize(0.04);
  histMom_ori->Draw("");
cout<<"Momentum"<<endl;
  histMom_ori->Fit("gaus","","" , -0.0004, 0.0004);

  c_ori->cd(2);
  histXp_ori->GetXaxis()->SetMaxDigits(2);
  histXp_ori->GetXaxis()->SetNdivisions(520);
  histXp_ori->Draw("");
cout<<"X'"<<endl;
  histXp_ori->Fit("gaus","","" , -0.002, 0.002);

  c_ori->cd(3);
  histYp_ori->GetXaxis()->SetMaxDigits(2);
  histYp_ori->GetXaxis()->SetNdivisions(520);
  histYp_ori->Draw("");
cout<<"Y'"<<endl;
  histYp_ori->Fit("gaus","","" , -0.002, 0.002);

  TCanvas *cz = new TCanvas("cz", "", 1000, 1000);
  cz->Divide(3,1);
  cz->cd(1);
  histX_ori->Draw("");
  histX_ori->Fit("gaus","","" , -5., 5.);
  cz->cd(2);
  histY_ori->Draw("");
  histY_ori->Fit("gaus","","" , -0.5, 0.5);
  cz->cd(3);
  histZ_ori->Draw("");
  //histZ_ori->Fit("gaus","","" , -5., 5.);
  histZ_ori->Fit("gaus","","" , -2., 2.);
//
//  TCanvas *c_new = new TCanvas("c_new", "", 1000, 1000);
//  c_new->Divide(2,2);
//  c_new->cd(1);
//  hist_XXp->Draw("colz");
//  c_new->cd(2);
//  hist_YYp->Draw("colz");
//  c_new->cd(3);
//  hist_XZt->Draw("colz");
//  c_new->cd(4);
//  hist_XpZt->Draw("colz");
//  c_new->cd(4);
//  //TCanvas *c4 = new TCanvas("c4", "", 1000, 1000);
//  //booltest->SetStats(0);
//  //booltest->Draw("text0 colz");

//Added by ishige 2022Oct14
TCanvas *c = new TCanvas("c", "", 600, 800);
  c->Divide(1,1);
  c->cd(1);
  // histMom_ori->GetXaxis()->SetMaxDigits(2);
  // histMom_ori->GetXaxis()->SetNdivisions(515);
  // histMom_ori->GetXaxis()->SetLabelSize(0.045);
  // histMom_ori->GetYaxis()->SetLabelSize(0.04);
  histMom_ori->Draw("");
cout<<"Momentum"<<endl;
 TF1 *f = new TF1("func","gaus", -0.0004, 0.0004);
  f->SetLineColor(kPink+10);
  f->SetParameter(0, 300);
  f->SetParameter(1, 0.01);
  f->SetParameter(2, 1E-03);
  f->SetFillColor(kPink+10);
  f->SetFillStyle(3015);
  histMom_ori->Fit("func","","" , -0.0004, 0.0004);

//
cout<< "qqdtrig_b = " << aqqdtrig_b <<endl;
cout<< "qqdtrig = " << aqqdtrig <<endl;
cout<< "fptrig_b = " << afptrig_b <<endl;
cout<< "etrig_b = " << aetrig_b <<endl;
cout<< "etrig = " << aetrig <<endl;
cout<< "edctrig_b = " << aedctrig_b <<endl;
cout<< "ehtrig_b = " << aehtrig_b <<endl;

  app.Run();
  return 0;
}

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
void TreeBranch( TTree *tree, const char *name, bool *branch){
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
