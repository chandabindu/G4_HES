#ifndef HESAnalysis_h
#define HESAnalysis_h 1


#include "globals.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "Define.hh"

#ifdef USE_CSTREAM
#include <cstdio>
#else
#include <fstream>
#endif

class G4Run;
class G4Event;
class G4Step;

class HESAnalysis 
{
public:
  HESAnalysis();
  virtual ~HESAnalysis();

public:
  void BeginOfRun( const G4Run *aRun );
  void EndOfRun( const G4Run *aRun );
  void BeginOfEvent( const G4Event *anEvent );
  void EndOfEvent( const G4Event *anEvent );
  void PrimaryGeneration( const G4ThreeVector &pos,
			  const G4ThreeVector &mom,
			  const G4int ID);
  void SaveFile( void ) const;
  void Terminate( void ) const;
  void SetDataFile( const char *datafile );
  void InitializeEvent( void );
  void SetActive( void ) { fActive_=true; }
  void SetInActive( void ) { fActive_=false; }

private:
  G4String filename_;
  G4int trigNum;
  G4ThreeVector gPos_, gMom_;
  G4bool fTriggered;
  G4bool fActive_;
  G4bool fOutput;
  G4int evID;
  G4double event[100];

  G4double Mom;
  G4double Theta;
  G4double Phi;
  G4double Xpt,Ypt;
  G4double Xt,Yt,Zt;
  
  G4double X[n_vd],Y[n_vd],Z[n_vd];
  G4double Xp[n_vd],Yp[n_vd];
  G4double Xv[n_vd],Yv[n_vd],Zv[n_vd];
  G4double P[n_vd];
  G4double L[n_vd];
  
  G4double EDC1X[10],EDC1Y[10],EDC1Z[10];
  G4double EDC1Xp[10],EDC1Yp[10];
  G4double EDC1P[10] ;
  G4double EDCFPX,EDCFPY,EDCFPZ;
  G4double EDCFPXp,EDCFPYp;
  G4double EDCFPP,EDCFPL ;
  G4double EDC2X[6],EDC2Y[6],EDC2Z[6];
  G4double EDC2Xp[6],EDC2Yp[6];
  G4double EDC2P[6];
  
  G4double EH1X[29],EH1Y[29],EH1Z[29];
  G4double EH1Xp[29],EH1Yp[29];
  G4double EH1P[29],EH1A[29],EH1T[29];
  
  G4double EH2X[29],EH2Y[29],EH2Z[29];
  G4double EH2Xp[29],EH2Yp[29];
  G4double EH2P[29],EH2A[29],EH2T[29];
  
  G4double XG,YG,ZG;

  G4bool QQDTrig  = false;
  G4bool FPTrig   = false;
  G4bool EDCTrig  = false;
  G4bool EDC1Trig = false;
  G4bool EDC2Trig = false;
  G4bool EHTrig   = false;
  G4bool EH1Trig  = false;
  G4bool EH2Trig  = false;
  G4bool ETrig    = false;
  G4bool ETrig2   = false;
  G4bool ETrig3   = false;
G4double pBeam,pVD;

  std::ofstream DataFile_;

  void NormalAnalysis( const G4Event *anEvent );

  FILE *fprim, *ftrig;
  FILE *fDummy;
#ifdef USE_CSTREAM
  FILE *fpout;
#else
  std::ofstream fout;
#endif

  void outputData( const G4Event *anEvent );
  void defineHistograms( void );

public:
  G4bool GetTriggerStatus( void ) { return fTriggered; }
  //void SetEventID( G4int id ) { evID_ = id; }

private:
  G4double RandGauss( G4double center, G4double dev );

};

#endif
