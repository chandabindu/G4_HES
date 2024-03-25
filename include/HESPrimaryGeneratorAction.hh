#ifndef HESPrimaryGeneratorAction_h
#define HESPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"


class G4ParticleGun;
class G4Event;

class HESDetectorConstruction;
class HESAnalysis;

class HESPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  HESPrimaryGeneratorAction( HESDetectorConstruction* detector,
			     HESAnalysis* analysisManager = 0 );
  ~HESPrimaryGeneratorAction();
  
  virtual void GeneratePrimaries( G4Event* anEvent );
  
private:
  HESAnalysis* anaMan;
  G4ParticleGun *gunParticle;
  void GenerateBeam( G4Event* anEvent );
  void GenParUni( G4Event* anEvent );
  void GenParCalib( G4Event* anEvent );
  void GenFromFile( G4Event* anEvent );
  void SetMom(G4ThreeVector gMom, G4ThreeVector gPos, G4Event* anEvent, G4int evID);

  G4double CentMom, AcptMom;
  G4double CentTheta, AcptTheta;
  G4double CentPhi, AcptPhi;
  G4double Xi, Yi, Zi;
  G4double xR, yR, zR;
  G4double bsizex, bsizey;

  G4int angFlag, momFlag; // added by ishig 2023Feb21

  FILE *dataFile;
  int skipLine = 7;

};

#endif
