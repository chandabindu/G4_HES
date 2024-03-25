#include "getMatrix.h"
#include <fstream>
#include <iostream>
#include <TMatrixD.h>
#include <TFile.h>
#include <TTree.h>
using namespace std;


int main()
{
    double tVal0[Maxevent][11];
    double fVal0[Maxevent][4];
    double mom0[Maxevent];
    double emom0[Maxevent];
    double xpt0[Maxevent];
    double expt0[Maxevent];
    double ypt0[Maxevent];
    double eypt0[Maxevent];
    double xf0[Maxevent];
    double xpf0[Maxevent];
    double yf0[Maxevent];
    double ypf0[Maxevent];
    double xt0[Maxevent];
    double yt0[Maxevent];
    double zt0[Maxevent];
    int nTrig0=0;
    
    char name_Mmom0[300];
    char name_Mxpt0[300];
    char name_Mypt0[300];
    char name_Mxt0[300];
    char name_Myt0[300];
    char name_Mzt0[300];
    char name_Mzt0_oku[300];
    char name_ifile[300];
    TFile* file;

    ifstream input("parFile.dat");
    input >> name_ifile;
    input >> name_Mmom0;
    input >> name_Mxpt0;
    input >> name_Mypt0;
    input >> name_Mxt0;
    input >> name_Myt0;
    input >> name_Mzt0;
    input >> name_Mzt0_oku;
    input.close();

    file = new TFile(name_ifile);

    cout << "input file name = " << name_ifile << endl << endl;

    cout << " Reading file..."<<endl;
    //  ofstream ofs("branch_value.txt");
    nTrig0=readvalue(file,Maxevent,fVal0,tVal0,0);
    cout << " Read "<< nTrig0 << " events"<<endl;
    for (int i=0;i<Maxevent;i++){
      //  ====== for Trig0 =========
      //  ---- at the target ----
      xpt0[i]  = tVal0[i][0]; 
      ypt0[i]  = tVal0[i][1]; 
      mom0[i]  = tVal0[i][2]; 
      //len0[i]  = tVal0[i][3]; 
      expt0[i] = tVal0[i][4]; 
      eypt0[i] = tVal0[i][5]; 
      emom0[i] = tVal0[i][6]; 
      //elen0[i] = tVal0[i][6];  // 7 ?
      //elen0[i] = tVal0[i][7];
      zt0[i]   = tVal0[i][8];
      xt0[i]   = tVal0[i][9];
      yt0[i]   = tVal0[i][10];
      //  ---- at the focal plane ----
      xf0[i]  = fVal0[i][0]; 
      xpf0[i] = fVal0[i][1]; 
      yf0[i]  = fVal0[i][2]; 
      ypf0[i] = fVal0[i][3]; 
 		// cout<< xpt0[i] << " " << ypt0[i] << " " << mom0[i] << " " << xf0[i] << " " << xpf0[i] << " " << yf0[i] << " " << ypf0[i] <<endl;
    //   ofs << i << " event" << endl;
    //   //  ---- at the target ----
    //   ofs << tVal0[i][0] << endl; 
    //   ofs << tVal0[i][1] << endl; 
    //   ofs << tVal0[i][2] << endl; 
    //   //len0[i]  = tVal0[i][3]; 
    //   ofs << tVal0[i][4] << endl; 
    //   ofs << tVal0[i][5] << endl; 
    //   ofs << tVal0[i][6] << endl; 
    //   //elen0[i] = tVal0[i][6];  // 7 ?
    //   //elen0[i] = tVal0[i][7];
    //   ofs << tVal0[i][8] << endl;
    //   ofs << tVal0[i][9] << endl;
    //   ofs << tVal0[i][10] << endl;
    //   //  ---- at the focal plane ----
    //   ofs << fVal0[i][0] << endl; 
    //   ofs << fVal0[i][1] << endl; 
    //   ofs << fVal0[i][2] << endl; 
    //   ofs << fVal0[i][3] << endl; 
    }

    cout << "Momentum reconstruction Matrix is being calculated ..." <<endl;
    calcMatrix(nTrig0, mom0, emom0, xf0, xpf0, yf0, ypf0, name_Mmom0, 6);
    cout<< name_Mmom0 << " has been generated." <<endl;
    
    cout << "X' reconstruction Matrix is being calculated ..." <<endl;
    calcMatrix(nTrig0, xpt0, expt0, xf0, xpf0, yf0, ypf0, name_Mxpt0, 6);
    cout<< name_Mxpt0 << " has been generated." <<endl;

    cout << "Y' reconstruction Matrix is being calculated ..." <<endl;
    calcMatrix(nTrig0, ypt0, eypt0, xf0, xpf0, yf0, ypf0, name_Mypt0, 6);
    cout<< name_Mypt0 << " has been generated." <<endl;
    
    cout << "Z at target reconstruction Matrix is being calculated ..." <<endl;
    calcMatrix(nTrig0, zt0, zt0, xf0, xpf0, yf0, ypf0, name_Mzt0, 3);
    cout<< name_Mzt0 << " has been generated." <<endl;

//    cout << "Z at target reconstruction Matrix (with p term) is being calculated ..." <<endl;
//    calcMatrix_oku(nTrig0, zt0, zt0, xf0, xpf0, yf0, ypf0, mom0, name_Mzt0_oku);
//    cout<< name_Mzt0_oku << " has been generated." <<endl;
 
    cout << "X at target reconstruction Matrix is being calculated ..." <<endl;
    calcMatrix(nTrig0, xt0, xt0, xf0, xpf0, yf0, ypf0, name_Mxt0, 3);
    cout<< name_Mxt0 << " has been generated." <<endl;

    cout << "Y at target reconstruction Matrix is being calculated ..." <<endl;
    calcMatrix(nTrig0, yt0, yt0, xf0, xpf0, yf0, ypf0, name_Myt0, 3);
    cout<< name_Myt0 << " has been generated." <<endl;
  return 0;
}

