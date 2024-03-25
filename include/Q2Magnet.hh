/*
  Q2Magnet.hh

  H.Nomura
  2003/Nov/28
*/

#ifndef Q2Magnet_h
#define Q2Magnet_h 1

#include "BeamLineElement.hh"
#include "Q2FieldMan.hh"
#include <vector>

class G4Material;
class G4VPhysicalVolume;
class G4VSolid;
class G4LogicalVolume;
class G4UserLimits;


class Q2Magnet : public BeamLineElement
{
public:
  Q2Magnet( const G4String &name, 
	    const G4ThreeVector &gPos,
	    const G4RotationMatrix *rotMtx,
	    const std::string deffilename);
  ~Q2Magnet();

  // Overloaded Functions
  bool ExistMagneticField( void ) const;
  G4ThreeVector GetlPos( const G4ThreeVector &gPos ) const;
  bool ExistElectricField( void ) const
  { return false; }
  bool IsInside( const G4ThreeVector &gPos ) const;

  G4ThreeVector GetMagneticField( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mYoke, G4Material *mGap )
  { mYoke_ = mYoke; mGap_ = mGap; }
  G4VPhysicalVolume* Place( G4VPhysicalVolume* pMother );

  void SetUserLimits( G4UserLimits *limitYoke,
		      G4UserLimits *limitGap = 0 );

private:
  G4Material *mYoke_, *mGap_;
  G4VSolid *solidField;
  G4LogicalVolume *yokeLog_, *gapLog_;
  G4double Scale;

  G4ThreeVector Field( const G4ThreeVector &pos ) const;
  G4ThreeVector KatoEq( const G4ThreeVector &pos ) const;
  Q2FieldMan fMan;

  G4VSolid *MakeGap( const G4String &name );
  int MapID;
};

#endif
