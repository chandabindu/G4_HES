/*
  SieveSlit.hh
  2004/Sep/21 H.Nomura
*/

#ifndef SieveSlit_h
#define SieveSlit_h 1

#include "BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;
class G4VSensitiveDetector;

class SieveSlit : public BeamLineElement
{
public:
  SieveSlit( const G4String &name,
	     const G4ThreeVector &gPos,
	     const G4RotationMatrix *rotMtx );

  ~SieveSlit();

  bool ExistMagneticField( void ) const { return false; }
  bool ExistElectricField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mVol1, G4Material *mVol2 )
  { mBox_ = mVol1; mHole_ = mVol2; }

  void SetSD( G4VSensitiveDetector *pSDetector );

  G4LogicalVolume *GetDetector1( void )
  { return logDetector1_; }
  G4LogicalVolume *GetDetector2( void )
  { return logDetector2_; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );

private:
  G4Material *mBox_, *mHole_;
  G4LogicalVolume *logDetector1_, *logDetector2_;
  G4LogicalVolume *holeLV[70];


  G4VSolid *vSolid_;
};

#endif
