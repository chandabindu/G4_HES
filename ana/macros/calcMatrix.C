#include <fstream>
#include <iostream>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>
#include <math.h>
#include "getMatrix.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
void calcMatrix(int Nevent, double* yVal, double* eVal, 
		double* xf, double* xpf,
		double* yf, double* ypf,
		char* Mname, int order)
//////////////////////////////////////////////////////////////////////////
{
  ofstream ofs("matrix.txt");
  ofs << "# xf" << endl;
  for(int i = 0; i<Maxevent; i++){
    ofs << i << xf[i] << endl;
  }
  ofs << "# xpf" << endl;
  for(int i = 0; i<Maxevent; i++){
    ofs << i << xpf[i] << endl;
  }
  ofs << "# yf" << endl;
  for(int i = 0; i<Maxevent; i++){
    ofs << i << yf[i] << endl;
  }
  ofs << "# ypf" << endl;
  for(int i = 0; i<Maxevent; i++){
    ofs << i << ypf[i] << endl;
  }

  ofs.close();

  int term = 210;
  switch(order){//max order of matrix
	case 2: term = 15;break;
	case 3: term = 35;break;
	case 4: term = 70;break;
	case 5: term = 126;break;
	case 6: term = 210;break;
	case 7: term = 330;break;
	default: cout<<"No match: int order, Please check calcMatrix() function in getMatrix.cc"<<endl;
  }
//	const int term = 15; // 2 dimension
//	const int term = 35; // 3 dimension
//	const int term = 70; // 4 dimension
//	const int term = 126; // 5 dimension
//	const int term = 210; // 6 dimension
//	const int term = 330; // 7 dimension
  int pxf[term],pxpf[term];
  int pyf[term],pypf[term];
  double norm[Maxevent];
  for (int i=0;i<Maxevent;i++){
    norm[i]=1;
  }
  TMatrixD A(Nevent,term);
  TVectorD x(Nevent); 
  TVectorD y,e; 
  TVectorD v0,v1,v2,v3,v4;
  v0.Use(Nevent, norm);
  v1.Use(Nevent, xf);
  v2.Use(Nevent, xpf);
  v3.Use(Nevent, yf);
  v4.Use(Nevent, ypf);
  TMatrixD v(Nevent,5);
  TMatrixDColumn(v,0) = v0;
  TMatrixDColumn(v,1) = v1;
  TMatrixDColumn(v,2) = v2;
  TMatrixDColumn(v,3) = v3;
  TMatrixDColumn(v,4) = v4;
  ofstream matrix;
  y.Use(Nevent, yVal);
  e.Use(Nevent, eVal);
  matrix.open(Mname);
  int npar=0;

  int n=0;
  int a=0,b=0,c=0,d=0;
  for (n=0;n<order+1;n++){
     for (d=0 ; d<n+1 ; d++){
       for (c=0 ; c<n+1 ; c++){
	 for (b=0 ; b<n+1 ; b++){
	   for (a=0 ; a<n+1 ; a++){
	     if ( a+b+c+d==n ){
	       pxf[npar] =a;
	       pxpf[npar]=b;
	       pyf[npar] =c;
	       pypf[npar]=d;
	       //if (a<=nXf && b<=nXpf && c<=nYf && d<=nYpf && npar<nParam)
	       if (a<=order && b<=order && c<=order && d<=order && npar<term){
					 for (int event=0 ; event<Nevent ; event++){ 
						 x(event) = pow(xf[event],double(a))*pow(xpf[event],double(b))*
						 pow(yf[event],double(c))*pow(ypf[event],double(d));
					 }
	       }
	       else{
					 for (int event=0;event<Nevent;event++){ 
						 x(event) = 0.;
					 }
	       }
	       TMatrixDColumn(A,npar) = x;
	       npar++;
	     }
	   }
	 }
       }
     }
  }
  
  TMatrixD Aw = A;
  TVectorD yw = y;
  /*for (int irow = 0; irow < A.GetNrows(); irow++) {
    TMatrixDRow(Aw,irow) *= 1/e(irow);
    yw(irow) /= e(irow);
    }*/
  //TDecompSVD svd(Aw,1e-5);
  TDecompSVD svd(Aw);
  //TDecompQRH svd(Aw);
  Bool_t ok;
  const TVectorD c_svd = svd.Solve(yw,ok);
  /*if (Flag==0){
    const TVectorD sig = svd.GetSig();
    ofstream fsig("sig_svd.dat");
    for (int irow = 0; irow < sig.GetNrows(); irow++) {
    fsig << sig(irow) <<endl;
    }
    fsig.close();
    }*/
  
  //const TVectorD c_svd = NormalEqn(A,y,e);
  
  TMatrixD Params(1,term);
  TMatrixDRow(Params,0) = c_svd;
  for(int i=0;i<term;i++){
    if (i<term){
      double par = Params(0,i);
      //if (fabs(par)<1e-5){par=0.;}
      matrix << par << " " 
	     << pxf[i] <<" "<< pxpf[i]<<" " 
	     << pyf[i] <<" "<< pypf[i] 
	     << endl;
    }
    else{
      matrix << 0. << " " 
	     << pxf[i] <<" "<< pxpf[i]<<" " 
	     << pyf[i] <<" "<< pypf[i] 
	/*<< -1 <<" "<< -1<<" " 
	  << -1 <<" "<< -1 */
	     << endl;
    }
  }
  matrix.close();
}

