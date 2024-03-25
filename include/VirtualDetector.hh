/*
  VirtualDetector.hh

  D.Kawama
*/

#ifndef VirtualDetector_h
#define VirtualDetector_h 1

#include "BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;

class VirtualDetector : public BeamLineElement
{
public:
  VirtualDetector( const G4String &name, const G4ThreeVector &gPos,
		   const G4RotationMatrix *rotMtx,
		   double SizeX, double SizeY, double SizeZ, int id );

  ~VirtualDetector();

  bool ExistMagneticField( void ) const { return false; }
  bool ExistElectricField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterial( G4Material *mVolume )
  { mVol_ = mVolume; }
  void SetMaterials( G4Material *mVolume )
  { mVol_ = mVolume; }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );

  G4LogicalVolume *GetDetectorLV( void ) { return logDetector_; }

private:
  G4double Length_, Height_, Width_;
  int Id_;
  G4Material *mVol_;
  G4LogicalVolume *logDetector_;

};
#endif
