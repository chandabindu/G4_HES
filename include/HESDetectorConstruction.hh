/*
  HESDetectorConstruction.hh

  D.Kawama
*/

#ifndef HESDetectorConstruction_h

#define HESDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class MaterialList;
class HESField;

class HESDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  HESDetectorConstruction();
  ~HESDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

private:
  MaterialList *mList_;
  MaterialList *DefineMaterials( void );

  HESField *MakeDetectorField( void );
  HESField *EMField_;

  G4VPhysicalVolume* ConstructPayload();

};

#endif

