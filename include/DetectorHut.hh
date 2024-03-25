/*
  DetectorHut.hh

  H.Nomura
  2003/Dec/12
*/

#ifndef DetectorHut_h
#define DetectorHut_h 1

#include "BeamLineElement.hh"

#include <vector>

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;

class DetectorHut : public BeamLineElement
{
public:
  DetectorHut( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx );

  ~DetectorHut();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterial( G4Material *mVolume )
  { mVol_ = mVolume; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );

  G4VPhysicalVolume *GetPhysVolume( void ) { return PhysVol_; }
 
private:
  G4Material *mVol_;
  G4VPhysicalVolume *PhysVol_;
  G4VSolid *vSolid_;

};

#endif
