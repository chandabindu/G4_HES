/*
  TargetChamber.hh
  header file for Water Cherenkov Counter

  H.Nomura 2003/Dec/31
*/

#ifndef TargetChamber_h
#define TargetChamber_h 1

#include "BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4UserLimits;

class TargetChamber : public BeamLineElement
{
public:
  TargetChamber( const G4String &name, const G4ThreeVector &gPos,
		 const G4RotationMatrix *rotMtx );

  ~TargetChamber();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mBox, G4Material *mGap)
  { mBox_=mBox; mGap_=mGap; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  void SetUserLimits( G4UserLimits *limitYoke );

private:
  G4Material *mBox_, *mGap_;
  G4LogicalVolume *yokeLV_;
  G4VSolid *vSolid_;

};

#endif
