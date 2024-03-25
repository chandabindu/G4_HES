#ifndef param_h
#define param_h

#include "TTree.h"
#include "TFile.h"

const int Maxevent=2000;

void calcMatrix(int nTrig, double* yVal, double* eVal, 
		double* xf, double* xpf,
		double* yf, double* ypf,
		char* Mname, int order);
void calcMatrix_oku(int nTrig, double* yVal, double* eVal, 
		double* xf, double* xpf,
		double* yf, double* ypf, double* mom,
		char* Mname, int order);
int readvalue(TFile *f, int nMax, double fVal[][4], double tVal[][11],int flag);
int readvalue(int scan, TFile *f, int nMax, double fVal[][4], double tVal[][11],int flag);
bool DCana(int N, TTree* tree);

#endif

