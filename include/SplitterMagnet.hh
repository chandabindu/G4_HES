/*
  SplitterMagnet.hh

  D.Kawama
*/

#ifndef SplitterMagnet_h
#define SplitterMagnet_h 1

#include "BeamLineElement.hh"
#include "SplitterFieldMan.hh"

#include <vector>

class G4Material;
class G4VPhysicalVolume;
class G4VSolid;
class G4LogicalVolume;
class G4UserLimits;

class SplitterMagnet : public BeamLineElement
{
public:
  SplitterMagnet( const G4String &name, 
		  const G4ThreeVector &gPos,
		  const G4RotationMatrix *rotMtx,
		  const std::string deffilename );
  ~SplitterMagnet();

  // Overloaded Functions

  bool ExistMagneticField( void ) const;
  bool ExistElectricField( void ) const
  { return false; }
  bool IsInside( const G4ThreeVector &gPos ) const;

  G4ThreeVector GetMagneticField( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mYoke, G4Material *mGap )
  { mYoke_ = mYoke; mGap_ = mGap; }
  G4VPhysicalVolume * Place( G4VPhysicalVolume *pMother );

  void SetUserLimits( G4UserLimits *limitYoke,
		      G4UserLimits *limitGap = 0 );

private:
  G4Material *mYoke_, *mGap_;
  G4VSolid *solidField;
  G4LogicalVolume *gapLog_, *yokeLog_;
  G4double Scale;

  G4ThreeVector Field( const G4ThreeVector &pos ) const;
  SplitterFieldMan fMan;
};

#endif
