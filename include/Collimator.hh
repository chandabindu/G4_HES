/*
  Collimator.hh
  header file for Water Cherenkov Counter

  H.Nomura 2003/Dec/31
*/

#ifndef Collimator_h
#define Collimator_h 1

#include "BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class Collimator : public BeamLineElement
{
public:
  Collimator( const G4String &name, const G4ThreeVector &gPos,
	      const G4RotationMatrix *rotMtx, int id );

  ~Collimator();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mBox, G4Material *mGap)
  { mBox_=mBox; mGap_=mGap; }

  G4LogicalVolume *GetDetectorLV( void ) { return DetectorLV_; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );

private:
  G4Material *mBox_, *mGap_;
  G4LogicalVolume *DetectorLV_;
  G4VSolid *vSolid_;
  int Id_;

  G4VSolid *MakeGap( const G4String &name);
};

#endif
