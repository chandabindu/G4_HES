#ifndef EDC2_h
#define EDC2_h 1

#include "BeamLineElement.hh"

//#include <vector>

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;

class EDC2 : public BeamLineElement
{
public:
  EDC2( const G4String &name, const G4ThreeVector &gPos,
		const G4RotationMatrix *rotMtx, int id );

  ~EDC2();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mMat1, G4Material *mMat2,
		     G4Material *mMat3, G4Material *mMat4, 
		     G4Material *mMat5, G4Material *mMat6 )
  { 
    mBox_ = mMat1; mGas_ = mMat2; 
    mMylar_ = mMat3; mLayer_ = mMat4; 
    mAnode_ = mMat5; mShield_ = mMat6; 
  }

  G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  G4VPhysicalVolume *DCGas;
  G4VPhysicalVolume *DCBox;

  G4LogicalVolume *GetDetectorLV( void ) { return DetectorLV_; }
  

private:
  G4Material *mBox_, *mGas_, *mMylar_, *mLayer_, *mAnode_, *mShield_;
  G4LogicalVolume *DetectorLV_;
  G4VSolid *vSolid_;
  int Id_;
};

#endif
