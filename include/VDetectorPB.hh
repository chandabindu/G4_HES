/*
  VDetectorPB.hh

  D.Kawama
*/

#ifndef VDetectorPB_h
#define VDetectorPB_h 1

#include "BeamLineElement.hh"

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class VDetectorPB : public BeamLineElement
{
public:
  VDetectorPB( const G4String &name, const G4ThreeVector &gPos,
	       const G4RotationMatrix *rotMtx, int id );

  ~VDetectorPB();

  bool ExistMagneticField( void ) const { return false; }
  bool ExistElectricField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mVolume )
  { mVol_ = mVolume; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );

  G4LogicalVolume *GetDetectorLV( void ) { return logDetector_; }

private:
  int Id_;
  G4Material *mVol_;
  G4LogicalVolume *logDetector_;
  G4VSolid *Vsolid_;

};
#endif
