#include "HESParamMan.hh"

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

ofstream LogFile;
namespace{
  const int SizeOfBuffer = 144;
}

HESParamMan *HESParamMan::thisPointer=0;
//////////////////////////////////////////////////////
HESParamMan::HESParamMan( const char *inputFileName )
  : fileName(inputFileName)
//////////////////////////////////////////////////////
{
  ReadParam();
  LogFile.open(LogFileName);
  LogFile << "Hbook file name : "         << HbookFileName   << G4endl;
  LogFile << "Input Data Name : "         << InputDataName   << G4endl;
  LogFile << "PCS field map : "           << PCSFieldMap     << G4endl;
  LogFile << "PCS scale factor : "        << Pscale          << G4endl;
  LogFile << "PTheta(degree): "           << PTheta          << G4endl; //Added by ishige 2022Sep28
  // LogFile << "TargetToPCS(cm): "          << T2P             << G4endl; //Added by ishige 2022Oct13
  LogFile << "Q1 field map : "            << Q1FieldMap      << G4endl;
  LogFile << "Q1scale factor : "          << Q1scale         << G4endl;
  LogFile << "Q2 field map : "            << Q2FieldMap      << G4endl;
  LogFile << "Q2scale factor : "          << Q2scale         << G4endl;
  LogFile << "D field map : "             << DFieldMap       << G4endl;
  LogFile << "Dscale factor : "           << Dscale          << G4endl;
  LogFile << "Tilt Angle(degree) = "      << TiltAngle       << G4endl;
  LogFile << "Theta Angle(degree) = "     << HesThetaAngle   << G4endl;
  LogFile << "Phi Angle(degree) = "       << HesPhiAngle     << G4endl;
  LogFile << "FP Angle(degree) = "        << FPAngle         << G4endl;
  LogFile << "Central Momentum(GeV) = "   << CentMom         << G4endl;
  LogFile << "Momentum Emittance(GeV) = " << AcptMom         << G4endl;
  LogFile << "Theta central(rad) = "      << CentTheta       << G4endl;
  LogFile << "Theta Gen. Range(rad) = "   << AcptTheta       << G4endl;
  LogFile << "Phi central(rad) = "        << CentPhi         << G4endl;
  LogFile << "Phi Gen. Range(rad) = "     << AcptPhi         << G4endl;
  LogFile << "Generate X (cm) = "         << GeneX           << G4endl;
  LogFile << "Generate Y (cm) = "         << GeneY           << G4endl;
  LogFile << "Generate Z (cm) = "         << GeneZ           << G4endl;
  LogFile << "Raster X (cm) = "           << RasterX         << G4endl;
  LogFile << "Raster Y (cm) = "           << RasterY         << G4endl;
  LogFile << "Raster Z (cm) = "           << RasterZ         << G4endl;
  LogFile << "Beam Size X (mm) = "        << BSizeX         << G4endl;
  LogFile << "Beam Size Y (mm) = "        << BSizeY         << G4endl;
  LogFile << "Target Material: "          << TargetMaterial  << G4endl;
  LogFile << "Target Thickness: "         << TargetThickness << G4endl;
  LogFile << "EventFlag: "                << EventFlag       << G4endl;
  LogFile << "PhysicsFlag: "              << PhysicsFlag     << G4endl;
  LogFile << "EDCFlag: "                  << EDCFlag         << G4endl;
  LogFile << "Particle ID:  "             << ParticleID      << G4endl;

  thisPointer = this;
}

/////////////////////////////
bool HESParamMan::ReadParam()
////////////////////////////
{
  // define initial value
  strcpy( HbookFileName, "hbook/geant.hbook" );
  strcpy( ErikaFileName, "data/erika.dat" );
  strcpy( TulFileName,   "data/tul.dat" );
  strcpy( dumFileName,   "dummy/dum.dat" );

  strcpy( InputDataName, "" );
  GenerationID = 0;
  ParticleID = 1;

  PCSFieldMap = "param/SM1_empty.map";
  Q1FieldMap = "param/HES_Q1_test.map";
  Q2FieldMap = "param/HES_Q2_test.map";
  DFieldMap = "param/HES_D_test.map";
  Dscale = Q1scale = Q2scale = 1.;
  Q1mapID = Q2mapID = 1;
  
  PTheta = 0.0; //Added by ishige 2022Sep28
  // T2P = 80*cm;  //Added by ishige 202Oct13

  TargetThickness = 100.0;
  TargetMaterial = "Vanadium";

  EventFlag = EDCFlag = PhysicsFlag = SSFlag = 0;
  CentMom = AcptMom = 0.;
  TiltAngle = CentTheta = AcptTheta = CentPhi = AcptPhi = 0.;
  RasterX = RasterY = RasterZ = GeneZ = 0.;
  
  
  FILE *fp = fopen( fileName, "r" );
  if( !fp ){
    G4cerr << "HESParamMan: file open fail \"" << fileName << "\"" 
	   << G4endl;
    exit( -1 );
  }

  char str[SizeOfBuffer];
  char str1[SizeOfBuffer];

  while( fgets( str, SizeOfBuffer, fp ) ){
    if( str[0] == '#' ) continue;

    if( sscanf( str, "HBOOK: %s", str1 )==1 ){
      strcpy( HbookFileName, str1 );
      strcpy( LogFileName, str1 );
      strcat( LogFileName, "_Log" );
      G4cout <<  LogFileName << G4endl;
    }else if( sscanf( str, "INPUT: %s", str1 )==1 ){
      GenerationID = 1;
      strcpy( InputDataName, str1 );
    }else if( sscanf( str, "PCSMAP: %s %lf", str1, &Pscale )==2 ){
      PCSFieldMap = str1;
    }else if( sscanf(str, "PTheta(degree): %lf", &PTheta ) == 1){ // Added by ishige 2022Sep28
    // }else if( sscanf(str, "TargetToPCS(cm): %lf", &T2P  ) == 1){  //Added by ishige 2022Oct13
	}else if( sscanf( str, "DMAP: %s %lf", str1, &Dscale )==2 ){
      DFieldMap = str1;
    }else if( sscanf( str, "Q1MAP: %s %lf", str1, &Q1scale )==2 ){
      Q1FieldMap = str1;
    }else if( sscanf( str, "Q2MAP: %s %lf", str1, &Q2scale )==2 ){
      Q2FieldMap = str1;
    }else if( sscanf( str, "TiltAngle(degree): %lf",      &TiltAngle )    ==1 ){
    }else if( sscanf( str, "HES_ThetaAngle(degree): %lf", &HesThetaAngle )==1 ){
    }else if( sscanf( str, "HES_PhiAngle(degree): %lf",   &HesPhiAngle )  ==1 ){
    }else if( sscanf( str, "FP Angle(degree): %lf",       &FPAngle )      ==1 ){
    }else if( sscanf( str, "CentralMomentum(GeV): %lf",   &CentMom )      ==1 ){
    }else if( sscanf( str, "MomentumWidth(GeV): %lf",     &AcptMom )      ==1 ){
    }else if( sscanf( str, "Theta Central(rad): %lf",     &CentTheta )    ==1 ){
    }else if( sscanf( str, "Theta Width(rad): %lf",       &AcptTheta )    ==1 ){
    }else if( sscanf( str, "Phi Central(rad): %lf",       &CentPhi )      ==1 ){
    }else if( sscanf( str, "Phi Width(rad): %lf",         &AcptPhi )      ==1 ){
    }else if( sscanf( str, "Raster X(cm): %lf",           &RasterX )      ==1 ){
    }else if( sscanf( str, "Raster Y(cm): %lf",           &RasterY )      ==1 ){
    }else if( sscanf( str, "Raster Z(cm): %lf",           &RasterZ )      ==1 ){
    }else if( sscanf( str, "Generate X(cm): %lf",         &GeneX )        ==1 ){
    }else if( sscanf( str, "Generate Y(cm): %lf",         &GeneY )        ==1 ){
    }else if( sscanf( str, "Generate Z(cm): %lf",         &GeneZ )        ==1 ){
    }else if( sscanf( str, "BeamSize X(mm): %lf",         &BSizeX )        ==1 ){
      if(BSizeX > 0) GenerationID = 2;
    }else if( sscanf( str, "BeamSize Y(mm): %lf",         &BSizeY )        ==1 ){
      if(BSizeY > 0) GenerationID = 2;
    }else if( sscanf( str, "TargetMaterial & Thickness(mg/cm^2): %s %lf", str1, &TargetThickness )==2 ){
      TargetMaterial = str1;
    }else if( sscanf( str, "EventFlag: %d",               &EventFlag )    ==1 ){
    }else if( sscanf( str, "PhysicsFlag: %d",             &PhysicsFlag )  ==1 ){
    }else if( sscanf( str, "EDCFlag: %d",                 &EDCFlag )      ==1 ){
    }else if( sscanf( str, "SSFlag: %d",                  &SSFlag )       ==1 ){
    }else if( sscanf( str, "Particle ID: %d",             &ParticleID )   ==1 ){
    }else if( sscanf( str, "AngFlag: %d",             &AngFlag )   ==1 ){ // added by ishige 2023Feb21
    }else if( sscanf( str, "MomFlag: %d",             &MomFlag )   ==1 ){ // added by ishige 2023Feb21
  
    }


  }

  return true;
}
