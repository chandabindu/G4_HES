#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "stdlib.h"
using namespace std;

const int MaxChar = 144;

void MakeSh( string save_dir);

int main(){

  //=== Magnet Setting ===//
  //const double Q1_min =  0.055;
  //const double Q1_max = -0.055;
  //const double Q2_min = -0.055;
  //const double Q2_max =  0.055;
  const double Q1_min =  0.5;
  const double Q1_max =  1.5;
  const double Q2_min =  0.5;
  const double Q2_max =  1.5;

  vector<double> Q1;
  vector<double> Q2;
  vector<double> Q3;

  //=== Input file ===//
  char fileName[100] = "../input/sample.in"; // template input file

  //=== Save Directory ===//
  // Don't forget "/" at the end 
  //string save_dir = "./temp/";
  string save_dir = "/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/";

  //=== Number of setup ===//
  int up_q1 = 21;  // Number of Q1 magnet change   
  int up_q2 = 21;  // Number of Q2 magnet change


 // system( ("mkdir " + save_dir).c_str() );
 // system( ("mkdir " + save_dir + "input").c_str() );
 // system( ("mkdir " + save_dir + "data").c_str() );

  double tmp = 0;
  char tmp_c[144] = "";
  char str[144];
  ofstream param_out((save_dir + "data/out.dat").c_str());

  string sys = string(fileName);
  sys = "cp " + sys + " " + save_dir;
  const char *sys_char = sys.c_str();
  system( sys_char ); //cp fileName save_dir

  int cnt_q1 = 0;  // Move from 0 to up_q1-1
  int cnt_q2 = 0;
  int cnt_all = 0; 
  int max = up_q1 * up_q2;

  for( int i = 0; i < up_q1; i++ ){
    for( int j = 0; j < up_q2; j++ ){
      Q1.push_back( Q1_min + (Q1_max-Q1_min)/(up_q1-1) * i  );
      Q2.push_back( Q2_min + (Q2_max-Q2_min)/(up_q2-1) * j  );
    }
  }

  

  for(int i = 0; i < 600; i++){
    FILE *fp = fopen( fileName, "r");
    string a = save_dir + "input/copy" + std::to_string(i) + ".in";
    ofstream output(a.c_str());

    param_out << "case: " << i << endl;
    
    while( fgets(str, MaxChar, fp) != 0 ){
      if( sscanf( str, "HBOOK: %c", tmp_c) == 1){
		//output << "HBOOK: " << save_dir << "data/case" << i << ".root" << endl;
		//param_out << "HBOOK: " << save_dir << "data/case" << i << ".root" << endl;
		output << "HBOOK: " << "../run/scan/temp/data/case" << i << ".root" << endl;
		param_out << "HBOOK: " << "../run/scan/temp/data/case" << i << ".root" << endl;
      }
      else if( sscanf( str, "DMAP: %s %lf", tmp_c, &tmp) == 2){
		output << "DMAP: " << tmp_c << " " << tmp << endl;
		param_out << "DMAP: " << tmp_c << " " << tmp << endl;
      }
      else if( sscanf( str, "Q1MAP: %s %lf", tmp_c, &tmp) == 2){
		double q1;
		q1 = Q1[cnt_all];
		output << "Q1MAP: " << tmp_c << " " << q1 << endl;
		param_out << "Q1MAP: " << tmp_c << " " << q1 << endl;
      }
      else if( sscanf( str, "Q2MAP: %s %lf", tmp_c, &tmp) == 2){
		double q2;
		q2 = Q2[cnt_all];
		output << "Q2MAP: " << tmp_c << " " << q2 << endl;
		param_out << "Q2MAP: " << tmp_c << " " << q2 << endl;
      }
      else {
		output << str;
      }
    }

    param_out << endl;
    fclose(fp);

    cnt_all++;
    cnt_q2++;
    if(cnt_q2 == up_q2){
      cnt_q2 = 0;
      cnt_q1++;
    }
    if(cnt_q1 == up_q1){
      cout << i+1 << " input files were created." << endl;
      break;
    }
  }

  MakeSh( save_dir );

  return 0;

}

void MakeSh( string save_dir ){

  ofstream shfile("kscan.sh");

  shfile << "#!/bin/bash" << endl << endl;

  shfile << "for i in `seq $1 $2`" << endl;
  shfile << "do" << endl;
  shfile << "    echo +++++++++++++++++++++++++++++++++++++++++++++" << endl;
  shfile << "    echo ++++++++++++++++++++ ${i} +++++++++++++++++++" << endl;
  shfile << "    echo +++++++++++++++++++++++++++++++++++++++++++++" << endl;

  shfile << "    ./HES ./macro/gun.mac " << save_dir << "input/copy${i}.in" << endl << endl;

  shfile << "done" << endl;

}
