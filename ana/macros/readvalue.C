#include <fstream>
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TCut.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include "TBranch.h"
#include <TDecompSVD.h>
#include <math.h>

using namespace std;

void TreeBranch( TTree*, const char*, int*);
void TreeBranch( TTree*, const char*, double*);
void TreeBranch( TTree*, const char*, char*);
void TreeBranch( TTree*, const char*, vector<int>**, TBranch**);
void TreeBranch( TTree*, const char*, vector<double>**, TBranch**);

////////////////////////////////////////////////////////////
int readvalue(TFile *f, int nMax, double fVal[][4], double tVal[][11], int flag)
   ////////////////////////////////////////////////////////////
{
  // ===== Resolution flag ============
  Bool_t resoflag = true;
  //Bool_t resoflag = false;
  // ================================

  
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
  

//------------------
  int nentries=0;
  int nTrig=0;
  TTree *tree = (TTree*)f->Get("tree");
  nentries =(int)tree->GetEntries();
cout << nentries << " entries in tree" << endl;
  TRandom3* rnd = new TRandom3();

  // ============ Variable definitions =================
  int jj=0;
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
	//TreeBranch(tree, "PEDCFP", &PFP);
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

cout<<"nMax="<<nMax<<endl;
  if (nMax>0){ 
    for (int i=0; i<nentries; i++){

        tree->GetEntry(i);

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
        if( jj<nMax 
		// && ETrig_b == true 
		&& QQDTrig == true 
		&& abs(EHODO1XFP)<58.5
		&& abs(EHODO1YFP)<15.
		&& abs(EHODO2XFP)<58.5
		&& abs(EHODO2YFP)<15.
	  		){
	  			// ----  Okuyama , 3Sep2021 ----
	  			if(resoflag==true){
	  				XFP =rnd->Gaus(XFP,0.0097);   //[cm]
	  				YFP =rnd->Gaus(YFP,0.0120);   //[cm]
	  				XpFP=rnd->Gaus(XpFP,0.8e-3); //[rad]
	  				YpFP=rnd->Gaus(YpFP,1.5e-3); //[rad]
	  				//EMom =rnd->Gaus(EMom,1.0e-4);  //[GeV]
	  				//Xpt =rnd->Gaus(Xpt,1.0e-3);  //[rad]
	  				//Ypt =rnd->Gaus(Ypt,1.0e-3);  //[rad]
	  			}
	  			// ----- at focal plane -------
	  			fVal[jj][0]=(XFP-XFPm)/XFPr;   //[cm]
	  			fVal[jj][1]=(XpFP-XpFPm)/XpFPr;//[rad]
	  			fVal[jj][2]=(YFP-YFPm)/YFPr;   //[cm]
	  			fVal[jj][3]=(YpFP-YpFPm)/YpFPr;//[rad]
	  			// ----- at target -------
	  			tVal[jj][0]=(Xpt-Xptm)/Xptr;//[rad]
	  			tVal[jj][1]=(Ypt-Yptm)/Yptr;//[rad]
	  			tVal[jj][2]=(EMom-Momm)/Momr;//[GeV/c]

          cout << "Emom: " << tVal[jj][2] << endl;
          

	  			tVal[jj][3]=0.;  //[cm]
	  			tVal[jj][4]=1e-4; //expt
	  			tVal[jj][5]=1e-4; //eypt
	  			tVal[jj][6]=1e-5; //emom
	  			tVal[jj][7]=1e-5; //eL
	  			tVal[jj][8]=(Zt-ztm)/ztr;
	  			tVal[jj][9]=(Xt-xtm)/xtr;
	  			tVal[jj][10]=(Yt-ytm)/ytr;
	  			jj++;
	  		} // if( jj<nMax && ETrig==1 )
	} // for( int i ) 
		nTrig=jj;
  } // if( nMax>0 )

	return nTrig;
}//readvalue

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

