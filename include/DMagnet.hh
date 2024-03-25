/*
  DMagnet.hh

  D.Kawama
*/

#ifndef DMagnet_h
#define DMagnet_h 1

#include "BeamLineElement.hh"
#include "DFieldMan.hh"

#include <vector>


class G4Material;
class G4VPhysicalVolume;
class G4VSolid;
class G4LogicalVolume;
class G4UserLimits;

class DMagnet : public BeamLineElement
{
public:
  DMagnet( const G4String &name, 
	   const G4ThreeVector &gPos,
	   const G4RotationMatrix *rotMtx,
	   const std::string deffilename);
  ~DMagnet();

  // Overloaded Functions
  bool ExistMagneticField( void ) const;
  G4ThreeVector GetlPos( const G4ThreeVector &gPos ) const;
  bool ExistElectricField( void ) const
  { return false; }
  bool IsInside( const G4ThreeVector &gPos ) const;

  G4ThreeVector GetMagneticField( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mYoke ,G4Material *mGap )
  { mYoke_ = mYoke; mGap_ = mGap; }
  G4VPhysicalVolume* Place( G4VPhysicalVolume* pMother );

  void SetUserLimits( G4UserLimits *limitYoke,
		      G4UserLimits *limitGap = 0 );

  G4LogicalVolume *GetDetectorLV( void ) { return DetectorLV_; }

private:
  G4Material *mYoke_, *mGap_;
  G4VSolid *solidField;
  G4LogicalVolume *gapLV_, *yokeLV_, *DetectorLV_;
  G4double Scale;

  G4ThreeVector Field( const G4ThreeVector &pos ) const;
  DFieldMan fMan;
};

#endif
