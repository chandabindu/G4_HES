#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include <string>

#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TCut.h"
#include "TLine.h"
#include "TRandom.h"
using namespace std;
void CanvasSet( TCanvas*, int, double, double, double, double);
void SetTitle( TH2D*, const char*, const char*, const char*);
void SetTitle( TH1D*, const char*, const char*, const char*);
void TreeBranch( TTree*, const char*, int*);
void TreeBranch( TTree*, const char*, double*);
void TreeBranch( TTree*, const char*, char*);
void TreeBranch( TTree*, const char*, vector<int>**, TBranch**);
void TreeBranch( TTree*, const char*, vector<double>**, TBranch**);

static const double PI = 4.*atan(1.0);

int main(int argc, char** argv){

    TApplication theApp("App", &argc, argv);

	bool BatchFlag = false;
	// bool PDFFlag = true;
	bool PDFFlag = false;

	char filename[100];
    ifstream input("parFile.dat");
    input >> filename;
    input.close();

	ostringstream RootFile;
	ostringstream LogFile;
	ostringstream PDFFile;
	RootFile << filename;	
	LogFile  << filename << "_Log";	
	PDFFile << "solid.pdf";	

	///////////////////////
	// General condition //
	///////////////////////
    gROOT->SetStyle("Plain");
    gStyle->SetTitleFontSize(0.05);
    gStyle->SetTitleSize(0.05, "X");
    gStyle->SetTitleSize(0.05, "Y");
    gStyle->SetTitleOffset(0.9, "X");
    gStyle->SetTitleOffset(0.9, "Y");
	gStyle->SetOptStat(0);
	gStyle->SetPadGridX(1);
	gStyle->SetPadGridY(1);
	gStyle->SetPalette(1);

//Reading Log File
	FILE *fp;
	const int SizeOfBuffer = 32;
    char str[SizeOfBuffer];
	double centralmom, centraltheta, thetawidth;
	double centralphi, phiwidth;

	if((fp=fopen(LogFile.str().c_str(), "r"))==NULL){
		std::cerr << "LogFile does NOT exist" << std::endl;
		exit(1);
	}

	while(fgets( str, SizeOfBuffer, fp)){
		if(sscanf( str, "Central Momentum(GeV) = %lf", &centralmom )==1){
		}
		else if(sscanf( str, "Theta central(rad) = %lf", &centraltheta )==1){
		}
		else if(sscanf( str, "Theta Gen. Range(rad) = %lf", &thetawidth )==1){
		}
		else if(sscanf( str, "Phi central(rad) = %lf", &centralphi )==1){
		}
		else if(sscanf( str, "Phi Gen. Range(rad) = %lf", &phiwidth )==1){
		}
	}
	fclose(fp);
	cout<< "central theta = " << centraltheta*180/PI << " [deg]" << endl;
	cout<< "theta width= " << thetawidth*180/PI << " [deg]" << endl;

	TFile *f = new TFile(RootFile.str().c_str());
	TTree *t = (TTree*)f->Get("tree");
	TTree *tree = (TTree*)f->Get("tree");

	double thetamin = 1.*(1.*centraltheta - thetawidth);
	double thetamax = 1.*(1.*centraltheta + thetawidth);
	double phimin = 1.*(centralphi - phiwidth);
	double phimax = 1.*(centralphi + phiwidth);
	//double omega = 2.*PI* (1-cos(thetawidth))*1000.; // [msr]
	double omega = sin(centraltheta)*(thetamax-thetamin)*(phimax-phimin)*1000.; // [msr]
	cout<< "thetamin = " << thetamin*180/PI << " [deg]" <<endl;
	cout<< "thetamax = " << thetamax*180/PI << " [deg]" <<endl;
	cout<< "phimin = " << phimin*180/PI << " [deg]" <<endl;
	cout<< "phimax = " << phimax*180/PI << " [deg]" <<endl;
	cout<< "omega = " << omega << " [msr]" <<endl;// \Delta\Omega_{gen}


//====E05-115 (HES+SPL)====//
	TCut Q1i = "sqrt(EXQ1i*EXQ1i+EYQ1i*EYQ1i)<15.24*0.5";
	TCut Q1e = "sqrt(EXQ1e*EXQ1e+EYQ1e*EYQ1e)<30.";
	TCut Q2i = "sqrt(EXQ2i*EXQ2i+EYQ2i*EYQ2i)<50.00*0.5";
	TCut Q2e = "sqrt(EXQ2e*EXQ2e+EYQ2e*EYQ2e)<38.";
	TCut Di  = "abs(EXDi)<89.2/2.&&abs(EYDi)<20./2.";
	TCut De  = "abs(EXDe)<165./2.&&abs(EYDe)<20./2.";
	TCut FP  = "abs(EXFP5)<132./2.&&abs(EYFP5)<50./2.";
	TCut RP  = "ESPLTrig&&EDCTrig";
	//TCut RP = "EVDTrig && EDCTrig";
	TCut Zpos = "-2.5<EZt && EZt<2.5";


	// ======== Histogram ======== //
	int bin_pos = 100;//=bin_mom
	double min_pos = -10.;
	double max_pos = 10.;
	int bin_2D_pos = 100;
	int bin_xp = 100;//=bin_mom
	double min_xp = 0.05;
	double max_xp = 0.25;
	int bin_2D_xp = 50;
	int bin_yp = 100;//=bin_mom
	double min_yp = -0.1;
	double max_yp = 0.1;
	int bin_2D_yp = 50;
	int bin_mom = 100;
	double min_mom = 0.55;
	double max_mom = 1.1;
	//double min_mom = 2.6;//E12-15-008
	//double max_mom = 3.4;//E12-15-008
	int bin_th = 100;
	double min_th = 0.;
	double max_th = 0.25;
	double min_costh = 0.96;//cos
	double max_costh = 1.0;//cos
	double min_ph = -PI;//-0.4292;//default=0.57
	double max_ph = PI;//0.4292;
	int bin_2D_mom = 50;
	int bin_2D_th = 50;
	int bin_2D_costh = 50;
	int bin_2D_ph = 50;

	TH1D *h_mom_gen = new TH1D( "h_mom_gen", "", bin_mom, min_mom, max_mom);
	TH1D *h_mom_q1  = new TH1D( "h_mom_q1",  "", bin_mom, min_mom, max_mom);
	TH1D *h_mom_q2  = new TH1D( "h_mom_q2",  "", bin_mom, min_mom, max_mom);
	TH1D *h_mom_result = new TH1D( "h_mom_result", "", bin_mom, min_mom, max_mom);
	t->Project( "h_mom_gen", "EMom");
	t->Project( "h_mom_q1", "EMom", Q1i&&Q1e);
	t->Project( "h_mom_q2", "EMom", Q2i&&Q2e);
	//t->Project( "h_mom_result", "EMom",Q1i&&Q1e&&Q2i&&Q2e&&Di&&De&&FP&&RP&&Zpos);
	TH1D *h_th_gen = new TH1D( "h_th_gen", "", bin_th, min_th, max_th);
	TH1D *h_th_q1 = new TH1D( "h_th_q1", "", bin_th, min_th, max_th);
	TH1D *h_th_q2 = new TH1D( "h_th_q2", "", bin_th, min_th, max_th);
	TH1D *h_th_result = new TH1D( "h_th_result", "", bin_th, min_th, max_th);
	t->Project( "h_th_gen", "ETheta*4.*atan(1.0)/180.");
	t->Project( "h_th_q1" , "ETheta*4.*atan(1.0)/180.", Q1i&&Q1e);
	t->Project( "h_th_q2" , "ETheta*4.*atan(1.0)/180.", Q2i&&Q2e);
	//t->Project( "h_th_result", "ETheta*4.*atan(1.0)/180.", Q1i&&Q1e&&Q2i&&Q2e&&Di&&De&&FP&&RP&&Zpos);
	
	TH1D *h_vdhits = new TH1D( "h_vdhits", "", 9, 1., 10.);
	SetTitle(h_vdhits, "VD Hits", "Virtual Detector ID", "Counts");
	int vdhits[10];
	for(int i=0;i<10;i++){vdhits[i]=0;}
	

//dist. at Q1, Q2, Dipole, FP
	int bdist = 100;
	double min_dist = -50.;
	double max_dist =  50.;
	TH2D *h2_Q1i = new TH2D( "h2_Q1i", "", bdist, min_dist, max_dist, bdist, min_dist, max_dist);
	TH2D *h2_Q2i = new TH2D( "h2_Q2i", "", bdist, min_dist, max_dist, bdist, min_dist, max_dist);
	TH2D *h2_Di  = new TH2D( "h2_Di" , "", bdist, min_dist, max_dist, bdist, min_dist, max_dist);
	TH2D *h2_De  = new TH2D( "h2_De" , "", bdist, min_dist, max_dist, bdist, min_dist, max_dist);
	TH2D *h2_FP  = new TH2D( "h2_FP" , "", bdist, min_dist, max_dist, bdist, min_dist, max_dist);
	t->Project( "h2_Q1i", "EYQ1i:EXQ1i");
	t->Project( "h2_Q2i", "EYQ2i:EXQ2i");
	t->Project( "h2_Di" , "EYDi:EXDi");
	t->Project( "h2_De" , "EYDe:EXDe");
	t->Project( "h2_FP" , "EYFP2:EXFP2");
	//t->Project( "h2_FP" , "YEDCFP:XEDCFP");

	TH2D *h_mom_th_gen = new TH2D( "h_mom_th_gen", "", bin_2D_mom, min_mom, max_mom, bin_2D_th, min_th, max_th);
	TH2D *h_mom_th = new TH2D( "h_mom_th", "", bin_2D_mom, min_mom, max_mom, bin_2D_th, min_th, max_th);
	t->Project( "h_mom_th_gen", "ETheta*4.*atan(1.0)/180.:EMom");
//	t->Project( "h_mom_th", "ETheta*4.*atan(1.0)/180.:EMom", Q1i&&Q1e&&Q2i&&Q2e&&Di&&De&&FP&&RP&&Zpos);

	TH2D *h_th_ph_gen = new TH2D( "h_th_ph_gen", "", bin_2D_th, min_th, max_th, bin_2D_ph, min_ph, max_ph);
	TH2D *h_costh_ph_gen = new TH2D( "h_costh_ph_gen", "", bin_2D_costh, min_costh, max_costh, bin_2D_ph, min_ph, max_ph);
	TH2D *h_th_ph = new TH2D( "h_th_ph", "", bin_2D_th, min_th/PI*180., max_th/PI*180., bin_2D_ph, min_ph/PI*180., max_ph/PI*180.);
	TH2D *h_costh_ph = new TH2D( "h_costh_ph", "", bin_2D_costh, min_costh, max_costh, bin_2D_ph, min_ph, max_ph);
	//t->Project( "h_th_ph_gen", "EPhi:cos(ETheta*4.*atan(1.0)/180.)");
	//t->Project( "h_th_ph", "EPhi:cos(ETheta*4.*atan(1.0)/180.)",Q1i&&Q1e&&Q2i&&Q2e&&Di&&De&&FP&&RP&&Zpos );

	// ========= Solid angle vs. momentum ===========
	TH1D *h_sa_mom_q1 = new TH1D("h_sa_mom_q1", "", bin_mom, min_mom, max_mom);
	TH1D *h_sa_mom_q2 = new TH1D("h_sa_mom_q2", "", bin_mom, min_mom, max_mom);
	TH1D *h_sa_mom_result = new TH1D("h_sa_mom_result", "", bin_mom, min_mom, max_mom);

	// ========= Solid angle vs. Position ===========
	TH1D *h_x_gen = new TH1D( "h_x_gen", "", bin_pos, min_pos, max_pos);
	t->Project( "h_x_gen", "EXt" , "");
	//t->Project( "h_x_gen", "EXt" , "abs(EYt)<2.&&abs(EZt)<2.");
	TH1D *h_y_gen = new TH1D( "h_y_gen", "", bin_pos, min_pos, max_pos);
	t->Project( "h_y_gen", "EYt" , "");
	//t->Project( "h_y_gen", "EYt" , "abs(EXt)<2.&&abs(EZt)<2.");
	TH1D *h_z_gen = new TH1D( "h_z_gen", "", bin_pos, min_pos, max_pos);
	t->Project( "h_z_gen", "EZt" , "");
	//t->Project( "h_z_gen", "EZt" , "abs(EXt)<2.&&abs(EYt)<2.");
	TH2D *h2_z_mom_gen = new TH2D( "h2_z_mom_gen", "", bin_2D_mom, min_mom, max_mom, bin_2D_pos, min_pos, max_pos);
	t->Project( "h2_z_mom_gen", "EZt:EMom" , "");
	TH1D *h_x_result = new TH1D( "h_x_result", "", bin_pos, min_pos, max_pos);
	TH1D *h_y_result = new TH1D( "h_y_result", "", bin_pos, min_pos, max_pos);
	TH1D *h_z_result = new TH1D( "h_z_result", "", bin_pos, min_pos, max_pos);
	TH2D *h2_z_mom_result = new TH2D( "h2_z_mom_result", "", bin_2D_mom, min_mom, max_mom, bin_2D_pos, min_pos, max_pos);
	TH1D *h_sa_x_result = new TH1D("h_sa_x_result", "", bin_pos, min_pos, max_pos);
	TH1D *h_sa_y_result = new TH1D("h_sa_y_result", "", bin_pos, min_pos, max_pos);
	TH1D *h_sa_z_result = new TH1D("h_sa_z_result", "", bin_pos, min_pos, max_pos);
	TH2D *h2_sa_z_mom_result = new TH2D( "h2_sa_z_mom_result", "", bin_2D_mom, min_mom, max_mom, bin_2D_pos, min_pos, max_pos);

	// ========= Solid angle vs. Angle =========== //2022/5/27 added
	TH1D *h_xp_gen = new TH1D( "h_xp_gen", "", bin_xp, min_xp, max_xp);
	t->Project( "h_xp_gen", "EXpt" , "");
	TH1D *h_yp_gen = new TH1D( "h_yp_gen", "", bin_yp, min_yp, max_yp);
	t->Project( "h_yp_gen", "EYpt" , "");
	TH2D *h2_yp_xp_gen = new TH2D( "h2_yp_xp_gen", "", bin_2D_xp, min_xp, max_xp, bin_2D_yp, min_yp, max_yp);
	t->Project( "h2_yp_xp_gen", "EYpt:EXpt" , "");
	TH1D *h_xp_result = new TH1D( "h_xp_result", "", bin_xp, min_xp, max_xp);
	TH1D *h_yp_result = new TH1D( "h_yp_result", "", bin_yp, min_yp, max_yp);
	TH2D *h2_yp_xp_result = new TH2D( "h2_yp_xp_result", "", bin_2D_xp, min_xp, max_xp, bin_2D_yp, min_yp, max_yp);
	TH1D *h_sa_xp_result = new TH1D("h_sa_xp_result", "", bin_xp, min_xp, max_xp);
	TH1D *h_sa_yp_result = new TH1D("h_sa_yp_result", "", bin_yp, min_yp, max_yp);
	TH2D *h2_sa_yp_xp_result = new TH2D( "h2_sa_yp_xp_result", "", bin_2D_xp, min_xp, max_xp, bin_2D_yp, min_yp, max_yp);

	int n1, n2, n1x, n1y, n1z, n1xp, n1yp;
	double val;
	double err;
	const double min_mom_gen = h_mom_gen->GetBinCenter(h_mom_gen->FindFirstBinAbove()) - 0.05;
	const double max_mom_gen = h_mom_gen->GetBinCenter(h_mom_gen->FindLastBinAbove()) + 0.05;
	const double min_mom_bin = h_mom_gen->FindBin(min_mom_gen);
	const double max_mom_bin = h_mom_gen->FindBin(max_mom_gen);
	cout <<"mom range: "<< min_mom_gen << " " << max_mom_gen << endl;
	cout <<"mom_bin: "<< min_mom_bin << " " << max_mom_bin << endl;



//----------------------------//
//------------Fill------------//
//----------------------------//
  double mom = 0, th = 0, ph = 0;
  double XFP = 0, YFP = 0, XpFP = 0, YpFP = 0;
  double xt = 0, yt = 0, zt = 0, xpt = 0, ypt = 0;
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
	TreeBranch(tree, "EXt", &xt);
	TreeBranch(tree, "EYt", &yt);
	TreeBranch(tree, "EZt", &zt);
	TreeBranch(tree, "EXpt", &xpt);
	TreeBranch(tree, "EYpt", &ypt);
	TreeBranch(tree, "EMom"   , &mom);
	TreeBranch(tree, "ETheta" , &th );
	TreeBranch(tree, "EPhi"   , &ph );
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
    int ENum = tree->GetEntries();

//event loop to find "n_acpt"
  for(int i=0;i<ENum;i++){
    tree->GetEntry(i);
    if(i%100000==0)cout<<i<<" / "<<ENum<<endl;
	th *= PI/180.;
	ph *= PI/180.;
	double cosine=cos(th*PI/180.);
	h_th_ph_gen->Fill(th,ph);
	h_costh_ph_gen->Fill(cosine,ph);

	
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
  if (    XQ1i>-1000.
       && XQ1e>-1000.
       && XQ2i>-1000.
       && XQ2e>-1000.
       && XDi>-1000.
       //&& XDe==-1000.
       && XDe>-1000.
       ){QQDTrig=true;}else{QQDTrig=false;}

    if(1){vdhits[1]++;
    if(XQ1i>-1000. && YQ1i>-1000.){
    if(XQ1e>-1000. && YQ1e>-1000.){vdhits[2]++;
    if(XQ2i>-1000. && YQ2i>-1000.){vdhits[3]++;
    if(XQ2e>-1000. && YQ2e>-1000.){vdhits[4]++;
    if(XDi >-1000. && YDi >-1000.){vdhits[5]++;
    if(XDe >-1000. && YDe >-1000.){vdhits[6]++;
    if(EDC1Trig==true)			  {vdhits[7]++;
    if(EDC2Trig==true)			  {vdhits[8]++;
	}}}}}}}}}

	if(
	//ETrig_b==1
	QQDTrig==1
       && XFP>-1000.
       && XpFP>-1000.
       && YFP>-1000.
       && YpFP>-1000.
	){

		h_mom_result->Fill(mom);
		h_th_result->Fill(th);
		h_mom_th->Fill(mom,th);
		h_th_ph->Fill(th/PI*180.,ph/PI*180.);
		h_costh_ph->Fill(cos(th),ph);
		h_x_result->Fill(xt);
		h_y_result->Fill(yt);
		h_z_result->Fill(zt);
		h_xp_result->Fill(xpt);
		h_yp_result->Fill(ypt);
		h2_z_mom_result->Fill(mom,zt);
		h2_yp_xp_result->Fill(xpt,ypt);
	}//cut conditon

  
}//ENum


  for(int i=1;i<10;i++){
	h_vdhits->SetBinContent(i,vdhits[i]);
  }

//-----                      -----//
//-- Analysis Part (after Fill) --//
//-----                      -----//
double n1_tot=0.;
double n2_tot=0.;
double n1n2_tot=0.;

	for(int i=0; i<bin_mom; i++){
		n1 = 0;
		n1 = (int)h_mom_gen->GetBinContent(i+1);

		// === Q1 entrance ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_mom_q1->GetBinContent(i+1);
		if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		h_sa_mom_q1->SetBinContent(i+1, val);
		if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
		h_sa_mom_q1->SetBinError(i+1, err);

		// === Q2 entrance ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_mom_q2->GetBinContent(i+1);
		if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		h_sa_mom_q2->SetBinContent(i+1, val);
		if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
		h_sa_mom_q2->SetBinError(i+1, err);

		// === all cuts ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_mom_result->GetBinContent(i+1);
		if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		h_sa_mom_result->SetBinContent(i+1, val);
		if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
		h_sa_mom_result->SetBinError(i+1, err);
		n1_tot+=n1;
		n2_tot+=n2;
		n1n2_tot+=n1*n2;

//Position dependence
		n1x = 0, n1y = 0, n1z = 0;
		n1x = (int)h_x_gen->GetBinContent(i+1);
		n1y = (int)h_y_gen->GetBinContent(i+1);
		n1z = (int)h_z_gen->GetBinContent(i+1);

		// === x Position ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_x_result->GetBinContent(i+1);
		if(n1x!=0 && n2!=0)val = omega*(1.0*n2/n1x);
		h_sa_x_result->SetBinContent(i+1, val);
		if(n1x!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1x - 2./sqrt(1.*n1x*n2));
		h_sa_x_result->SetBinError(i+1, err);

		// === y Position ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_y_result->GetBinContent(i+1);
		if(n1y!=0 && n2!=0)val = omega*(1.0*n2/n1y);
		h_sa_y_result->SetBinContent(i+1, val);
		if(n1y!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1y - 2./sqrt(1.*n1y*n2));
		h_sa_y_result->SetBinError(i+1, err);

		// === z Position ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_z_result->GetBinContent(i+1);
		if(n1z!=0 && n2!=0)val = omega*(1.0*n2/n1z);
		h_sa_z_result->SetBinContent(i+1, val);
		if(n1z!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1z - 2./sqrt(1.*n1z*n2));
		h_sa_z_result->SetBinError(i+1, err);

//Angular dependence
		n1xp = 0, n1yp = 0;
		n1xp = (int)h_xp_gen->GetBinContent(i+1);
		n1yp = (int)h_yp_gen->GetBinContent(i+1);

		// === xp ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_xp_result->GetBinContent(i+1);
		if(n1xp!=0 && n2!=0)val = omega*(1.0*n2/n1xp);
		h_sa_xp_result->SetBinContent(i+1, val);
		if(n1xp!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1xp - 2./sqrt(1.*n1xp*n2));
		h_sa_xp_result->SetBinError(i+1, err);

		// === yp ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_yp_result->GetBinContent(i+1);
		if(n1yp!=0 && n2!=0)val = omega*(1.0*n2/n1yp);
		h_sa_yp_result->SetBinContent(i+1, val);
		if(n1yp!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1yp - 2./sqrt(1.*n1yp*n2));
		h_sa_yp_result->SetBinError(i+1, err);

}//bin_mom = bin_pos = bin_xp = bin_yp

	// === Z vs. Mom ===
	for(int i=0; i<bin_2D_mom; i++){
		for(int j=0; j<bin_2D_pos; j++){
			n1 = 0;
			n1 = (int)h2_z_mom_gen->GetBinContent(i+1, j+1);
			n2 = 0;
			val = 0.;
			n2 = (int)h2_z_mom_result->GetBinContent(i+1, j+1);
			if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		    //if(val!=0)cout<< n1 << " " << n2 << " " << val << endl;
			h2_sa_z_mom_result->SetBinContent(i+1, j+1, val);
		}//j loop
	}//i loop

	// === xp vs. yp ===
	for(int i=0; i<bin_2D_xp; i++){
		for(int j=0; j<bin_2D_yp; j++){
			n1 = 0;
			n1 = (int)h2_yp_xp_gen->GetBinContent(i+1, j+1);
			n2 = 0;
			val = 0.;
			n2 = (int)h2_yp_xp_result->GetBinContent(i+1, j+1);
			if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
//		if(val!=0)cout<< n1 << " " << n2 << " " << val << endl;
			h2_sa_yp_xp_result->SetBinContent(i+1, j+1, val);
		}//j loop
	}//i loop

	// ========= Solid angle vs. theta ===========
	TH1D *h_sa_th_q1 = new TH1D("h_sa_th_q1", "", bin_th, min_th, max_th);
	TH1D *h_sa_th_q2 = new TH1D("h_sa_th_q2", "", bin_th, min_th, max_th);
	TH1D *h_sa_th_result = new TH1D("h_sa_th_result", "", bin_th, min_th, max_th);

	const double min_th_gen = h_th_gen->GetBinCenter(h_th_gen->FindFirstBinAbove()) - 0.02;
	const double max_th_gen = h_th_gen->GetBinCenter(h_th_gen->FindLastBinAbove()) + 0.02;
	const double max_th_gen_y = h_th_gen->GetBinContent(h_mom_gen->FindBin(centralmom))*1.1;
	const double min_th_bin = h_th_gen->FindBin(min_th_gen);
	const double max_th_bin = h_th_gen->FindBin(max_th_gen);
	cout <<"th range: "<< min_th_gen << " " << max_th_gen << endl;
	cout <<"th bin: "<< min_th_bin << " " << max_th_bin << endl;

	for(int i=0; i<bin_th; i++){
		n1 = 0;
		n1 = (int)h_th_gen->GetBinContent(i+1);

		// === Q1 entrance ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_th_q1->GetBinContent(i+1);
		if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		h_sa_th_q1->SetBinContent(i+1, val);
		if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
		h_sa_th_q1->SetBinError(i+1, err);

		// === Q2 entrance ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_th_q2->GetBinContent(i+1);
		if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		h_sa_th_q2->SetBinContent(i+1, val);
		if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
		h_sa_th_q2->SetBinError(i+1, err);

		// === all cuts ===
		n2 = 0;
		val = 0.;
		err = 0.;
		n2 = (int)h_th_result->GetBinContent(i+1);
		if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
		h_sa_th_result->SetBinContent(i+1, val);
		if(n1!=0 && n2!=0)err = val * sqrt(1./n2 + 1./n1 - 2./sqrt(1.*n1*n2));
		h_sa_th_result->SetBinError(i+1, err);
	}//bin_th loop


	// === Momentum vs. Theta ===
	TH2D *h_sa_mom_th = new TH2D("h_sa_mom_th", "", bin_2D_mom, min_mom, max_mom, bin_2D_th, min_th, max_th);
	for(int i=0; i<bin_2D_mom; i++){
		for(int j=0; j<bin_2D_th; j++){
			n1 = 0;
			n1 = (int)h_mom_th_gen->GetBinContent(i+1, j+1);
			n2 = 0;
			val = 0.;
			n2 = (int)h_mom_th->GetBinContent(i+1, j+1);
			if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
//		if(val!=0)cout<< n1 << " " << n2 << " " << val << endl;
			h_sa_mom_th->SetBinContent(i+1, j+1, val);
		}//j loop
	}//i loop

	// === Theta vs. Phi ===
	TH2D *h_sa_costh_ph = new TH2D("h_sa_costh_ph", "", bin_2D_costh, min_costh, max_costh, bin_2D_ph, min_ph, max_ph);
	for(int i=0; i<bin_2D_th; i++){
		for(int j=0; j<bin_2D_ph; j++){
			n1 = 0;
			n1 = (int)h_costh_ph_gen->GetBinContent(i+1, j+1);
			n2 = 0;
			val = 0.;
			n2 = (int)h_costh_ph->GetBinContent(i+1, j+1);
			if(n1!=0 && n2!=0)val = omega*(1.0*n2/n1);
            //if(val!=0)cout<< n1 << " " << n2 << " " << val << endl;
			h_sa_costh_ph->SetBinContent(i+1, j+1, val);
		}//j loop
	}//i loop



////////////////////
// Draw histgrams //
////////////////////
	TH1D *hframe;
	TLine *lmom = new TLine( centralmom, 0., centralmom, omega);
	TLine *lth = new TLine( centraltheta, 0., centraltheta, omega);
	lmom->SetLineColor(4);
	lth->SetLineColor(4);

	TCanvas *c0 = new TCanvas("c0", "Distributions");
//	TCanvas *c1 = new TCanvas("c1", "Momentum Acceptance (Exit)");
//	TCanvas *c2 = new TCanvas("c2", "Angular Acceptance (Exit)");
	TCanvas *c3 = new TCanvas("c3", "Momentum Acceptance Expansion");
	TCanvas *c4 = new TCanvas("c4", "Angular Acceptance Expansion");
	TCanvas *c5 = new TCanvas("c5", "Momentum vs. Theta");
	TCanvas *c6 = new TCanvas("c6", "Theta vs. Phi");
	TCanvas *c7 = new TCanvas("c7", "Position dependence");
	TCanvas *c8 = new TCanvas("c8", "Position vs. Momentum Acceptance");
	TCanvas *c9 = new TCanvas("c9", "VD Hits");
	TCanvas *c10 = new TCanvas("c10", "VD Hits2");
	TCanvas *c11 = new TCanvas("c11", "VD Hits3");
	TCanvas *c12 = new TCanvas("c12", "Angular Acceptance Events (X', Y')");
	TCanvas *c13 = new TCanvas("c13", "Angular Acceptance Solid Angle (X', Y')");
	TCanvas *c14 = new TCanvas("c14", "Theta vs. Phi");


	//======= Distributions at Q1, Q2, Dipole, FP ======
	c0->Divide(3,2);
//	gPad->SetGrid();
	c0->cd(1);
	SetTitle(h2_Q1i, "Q1 entrance",  "X", "Y");
	h2_Q1i->Draw("colz");

	c0->cd(2);
	SetTitle(h2_Q2i, "Q2 entrance",  "X", "Y");
	h2_Q2i->Draw("colz");

	c0->cd(3);
	SetTitle(h2_Di, "Dipole entrance",  "X", "Y");
	h2_Di->Draw("colz");

	c0->cd(4);
	SetTitle(h2_De, "Dipole exit",  "X", "Y");
	h2_De->Draw("colz");

	c0->cd(5);
	SetTitle(h2_FP, "Focal Plane",  "X", "Y");
	h2_FP->Draw("colz");

//	//======= Momentum Acceptance (Entrance) ======
//	c1->Divide(2,2);
//	c1->cd(1);
//	hframe = (TH1D*)gPad->DrawFrame( min_mom_gen, 0., max_mom_gen, omega + 0.5);
//	SetTitle(hframe, "Solid Angle vs. Momentum at Q1 Entrance", "Momentum [GeV/c]", "Solid Angle [msr]");
//	h_sa_mom_q1->Draw("same");
//	//lmom->Draw("same");
////	c1->RedrawAxis();
//
//	c1->cd(2);
//	SetTitle(hframe, "Solid Angle vs. Momentum at Q2 Entrance", "Momentum [GeV/c]", "Solid Angle [msr]");
//	h_sa_mom_q2->Draw("same");
//	//lmom->Draw("same");
////	c1->RedrawAxis();
//
//	//======= Angular Acceptance (Entrance) ======
//	c2->Divide(2,2);
////	gPad->SetGrid();
//	c2->cd(1);
//	SetTitle(hframe, "Solid Angle vs. Theta at Q1 Entrance", "Theta [rad]", "Solid Angle [msr]");
//	h_sa_th_q1->Draw("same");
//	//lth->Draw("same");
////	c2->RedrawAxis();
//
//	c2->cd(2);
//	SetTitle(hframe, "Solid Angle vs. Theta at Q2 Entrance", "Theta [rad]", "Solid Angle [msr]");
//	h_sa_th_q2->Draw("same");
//	//lth->Draw("same");
////	c2->RedrawAxis();

	//======= Momentum Acceptance Result ======
	c3->Divide(2,2);
	c3->cd(1);
//	hframe = (TH1D*)gPad->DrawFrame( min_mom_gen, 0., max_mom_gen, omega + 0.5);
	//SetTitle(h_sa_mom_result, "Solid Angle vs. Momentum at Reference Plane", "Momentum [GeV/c]", "Solid Angle [msr]");
	SetTitle(h_sa_mom_result, "Solid Angle vs. Momentum (w/ all Cuts)", "Momentum [GeV/c]", "Solid Angle [msr]");
//	h_sa_mom_result->GetXaxis()->SetNdivisions(506, kFALSE);
	h_sa_mom_result->GetXaxis()->SetNdivisions(505, kFALSE);
//	h_sa_mom_result->Draw("same");
	h_sa_mom_result->Draw();
//	lmom->Draw("same");
	c3->cd(2);
	SetTitle(h_mom_gen, "Solid Angle vs. Momentum (generated)", "Momentum [GeV/c]", "Solid Angle [msr]");
	h_mom_gen->GetXaxis()->SetNdivisions(505, kFALSE);
	h_mom_gen->Draw();
	c3->cd(3);
	SetTitle(h_mom_result, "Solid Angle vs. Momentum (cut)", "Momentum [GeV/c]", "Solid Angle [msr]");
	h_mom_result->GetXaxis()->SetNdivisions(505, kFALSE);
	h_mom_result->Draw();
	c3->cd(4);
	h_mom_gen->Draw();
	h_mom_result->SetLineColor(kAzure);
	h_mom_result->Draw("same");

	//======= Angular Acceptance Result======
	c4->Divide(2,2);
	c4->cd(1);
	SetTitle(h_sa_th_result, "Solid Angle vs. Theta (w/ all Cuts)", "Theta [rad]", "Solid Angle [msr]");
//	h_sa_th_result->GetXaxis()->SetNdivisions(506, kFALSE);
	h_sa_th_result->GetXaxis()->SetNdivisions(505, kFALSE);
//	h_sa_th_result->Draw("same");
	h_sa_th_result->Draw();
//	lth->Draw("same");
	c4->cd(2);
	SetTitle(h_th_gen, "Solid Angle vs. Theta (generated)", "Theta [rad]", "Solid Angle [msr]");
	hframe = (TH1D*)gPad->DrawFrame( min_th_gen, 0., max_th_gen, max_th_gen_y);
	hframe->Draw();
	h_th_gen->GetXaxis()->SetNdivisions(505, kFALSE);
	h_th_gen->Draw("same");
	c4->cd(3);
	SetTitle(h_th_result, "Solid Angle vs. Theta (cut)", "Theta [rad]", "Solid Angle [msr]");
	h_th_result->GetXaxis()->SetNdivisions(505, kFALSE);
	h_th_result->Draw();
	c4->cd(4);
	hframe->Draw();
	h_th_gen->Draw("same");
	h_th_result->SetLineColor(kAzure);
	h_th_result->Draw("same");



	//======= Theta vs. Momentum  ======
	c5->cd();
//	hframe = (TH1D*)gPad->DrawFrame( min_mom_gen, max_th_gen, max_mom_gen, max_th_gen);
	SetTitle(h_sa_mom_th, "#theta_{e} vs. Momentum (w/ all cuts)", "Momentum [GeV/c]", "#theta_{e} [rad]");
	SetTitle(h_mom_th, "#theta_{e} vs. Momentum", "Momentum [GeV/c]", "#theta_{e} [rad]");
//	h_sa_mom_th->GetXaxis()->SetNdivisions(506, kFALSE);
	h_sa_mom_th->GetXaxis()->SetNdivisions(505, kFALSE);
	h_sa_mom_th->GetYaxis()->SetNdivisions(304, kFALSE);
//	h_sa_mom_th->Draw("samecolz");
	//h_sa_mom_th->Draw("colz");

	h_mom_th->Draw("colz");




	//======= cos(Theta) vs. Phi  ======
	c6->cd();
//	hframe = (TH1D*)gPad->DrawFrame( min_mom_gen, max_th_gen, max_mom_gen, max_th_gen);
	SetTitle(h_sa_costh_ph, "cos(#theta_{e}) vs. #phi_{e} (w/ all cuts)",  "cos(#theta_{e})", "#phi_{e} [rad]");
//	h_sa_costh_ph->GetXaxis()->SetNdivisions(506, kFALSE);
	h_sa_costh_ph->GetXaxis()->SetNdivisions(505, kFALSE);
	h_sa_costh_ph->GetYaxis()->SetNdivisions(304, kFALSE);
//	h_sa_costh_ph->Draw("samecolz");
	h_sa_costh_ph->Draw("colz");

	c14->cd();
	SetTitle(h_th_ph, "#theta_{e} vs. #phi_{e} (w/ all cuts)",  "#theta_{e} [deg]", "#phi_{e} [deg]");
//	h_sa_costh_ph->GetXaxis()->SetNdivisions(506, kFALSE);
	h_th_ph->GetXaxis()->SetNdivisions(505, kFALSE);
	h_th_ph->GetYaxis()->SetNdivisions(304, kFALSE);
//	h_sa_costh_ph->Draw("samecolz");
	h_th_ph->Draw("colz");


	//======= Position dependence  ======
	c7->Divide(2,2);
	c7->cd(1);
	SetTitle(h_sa_x_result, "Solid Angle vs. X", "X [cm]", "Solid Angle [msr]");
	h_sa_x_result->GetXaxis()->SetNdivisions(505, kFALSE);
	h_sa_x_result->SetLineColor(kAzure);
	h_sa_x_result->Draw("");
	c7->cd(2);
	SetTitle(h_sa_y_result, "Solid Angle vs. Y", "Y [cm]", "Solid Angle [msr]");
	h_sa_y_result->GetXaxis()->SetNdivisions(505, kFALSE);
	h_sa_y_result->SetLineColor(kAzure);
	h_sa_y_result->Draw("");
	c7->cd(3);
	SetTitle(h_sa_z_result, "Solid Angle vs. Z", "Z [cm]", "Solid Angle [msr]");
	h_sa_z_result->GetXaxis()->SetNdivisions(505, kFALSE);
	h_sa_z_result->SetLineColor(kAzure);
	h_sa_z_result->Draw("");
	c7->cd(4);
	SetTitle(h_x_gen, "Z position", "Z [cm]", "Counts");
	h_z_result->SetLineColor(kAzure);
	h_z_result->Draw("");

	//======= Position dependence  ======
	c8->cd();
	SetTitle(h2_sa_z_mom_result, "Solid Angle (Z, Mom)", "Momentum [GeV/c]", "Z [cm]");
	h2_sa_z_mom_result->Draw("colz");

	c9->cd();
	h_vdhits->Draw("");

	c10->cd();
	c10->SetLogy(1);
	h_vdhits->Draw("");

	c11->cd();
	h_vdhits->Draw("");
	h_vdhits->Draw("text 0 same");

	//======= Angular Acceptance (X', Y') ======= //2022/5/27 added
	c12->Divide(2,2);
	c12->cd(1);
	SetTitle(h2_yp_xp_result, "Accepted Events: (X',Y')@target", "X' [rad]", "Y' [rad]");
	h2_yp_xp_result->Draw("colz");

	c12->cd(2);
	h2_yp_xp_result->GetXaxis()->SetNdivisions(504, kFALSE);
	h2_yp_xp_result->GetYaxis()->SetNdivisions(504, kFALSE);
	h2_yp_xp_result->Draw("surf3d");

	c12->cd(3);
	SetTitle(h_xp_result, "Accepted Events: X'@target", "X' [rad]", "Counts");
	h_xp_result->GetXaxis()->SetNdivisions(510, kFALSE);
	h_xp_result->Draw();

	c12->cd(4);
	SetTitle(h_yp_result, "Accepted Events: Y'@target", "Y' [rad]", "Counts");
	h_yp_result->GetXaxis()->SetNdivisions(510, kFALSE);
	h_yp_result->Draw();

	//======= Angular Acceptance (X', Y') ======= //2022/5/27 added
	c13->Divide(2,2);
	c13->cd(1);
	SetTitle(h2_sa_yp_xp_result, "Solid Angle vs. (X',Y')@target", "X' [rad]", "Y' [rad]");
	h2_sa_yp_xp_result->Draw("colz");

	c13->cd(2);
	h2_sa_yp_xp_result->GetXaxis()->SetNdivisions(504, kFALSE);
	h2_sa_yp_xp_result->GetYaxis()->SetNdivisions(504, kFALSE);
	h2_sa_yp_xp_result->Draw("surf3d");

	c13->cd(3);
	SetTitle(h_sa_xp_result, "Solid Angle vs. X'@target", "X' [rad]", "Solid Angle [msr]");
	h_sa_xp_result->GetXaxis()->SetNdivisions(510, kFALSE);
	h_sa_xp_result->Draw();

	c13->cd(4);
	SetTitle(h_sa_yp_result, "Solid Angle vs. Y'@target", "Y' [rad]", "Solid Angle [msr]");
	h_sa_yp_result->GetXaxis()->SetNdivisions(510, kFALSE);
	h_sa_yp_result->Draw();

	if(PDFFlag){
		cout << "Creating a PDF file ... " << endl;
		c0 ->Print(Form("%s[",PDFFile.str().c_str()) );
		c0 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		//c1 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		//c2 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c3 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c4 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c5 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c6 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c7 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c8 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c9 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c10 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c11 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c12 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c13 ->Print(Form("%s" ,PDFFile.str().c_str()) );
		c13 ->Print(Form("%s]" ,PDFFile.str().c_str()) );
	}

	cout<< "Finish!" << endl;
	if(!BatchFlag){
		theApp.Run();
	}

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
void TreeBranch( TTree *tree, const char *name, vector<int> **vec, TBranch **bvp){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, vec, bvp);
}
void TreeBranch( TTree *tree, const char *name, vector<double> **vec, TBranch **bvp){
	tree->SetBranchStatus(name);
	tree->SetBranchAddress(name, vec, bvp);
}

void SetTitle( TH2D *h, const char *title, const char *xaxis, const char *yaxis){
	h->GetXaxis()->SetTitle(xaxis);
	h->GetYaxis()->SetTitle(yaxis);
	h->SetTitle(title);
}

void SetTitle( TH1D *h, const char *title, const char *xaxis, const char *yaxis){
	h->GetXaxis()->SetTitle(xaxis);
	h->GetYaxis()->SetTitle(yaxis);
	h->SetTitle(title);
} 
void CanvasSet( TCanvas *c, int pad, double x1, double y1, double x2, double y2){
	c->cd(pad);
	gPad->SetGrid();
	c->DrawFrame(x1, y1, x2, y2);
}
