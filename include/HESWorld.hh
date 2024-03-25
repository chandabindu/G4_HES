/*
  HESWorld.hh

  D.Kawama
*/

#ifndef HESWorld_h
#define HESWorld_h 1

#include "BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;

class HESWorld : public BeamLineElement
{
public:
  HESWorld( const G4String &name, const G4ThreeVector &gPos,
		   const G4RotationMatrix *rotMtx,
		   double SizeX, double SizeY, double SizeZ );

  ~HESWorld();

  bool ExistMagneticField( void ) const { return false; }
  bool ExistElectricField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mVolume )
  { mVol_ = mVolume; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );

  G4LogicalVolume *GetDetectorLV( void ) { return logDetector_; }

private:
  G4double Length_, Height_, Width_;
  G4Material *mVol_;
  G4LogicalVolume *logDetector_;

};
#endif
