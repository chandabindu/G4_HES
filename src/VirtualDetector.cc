/*
  VirtualDetector.cc

  D.Kawama
*/

#include "VirtualDetector.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

///////////////////////////////////////////////////////////////////////////
VirtualDetector::VirtualDetector( const G4String &name,
				  const G4ThreeVector &gPos,
				  const G4RotationMatrix *rotMtx,
				  double SizeX, double SizeY, double SizeZ,
				  int id )
  : BeamLineElement(name,gPos,rotMtx),
    Length_(0.5*SizeX), Height_(0.5*SizeY), Width_(0.5*SizeZ),
    Id_(id), mVol_(0), logDetector_(0)
//////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////
VirtualDetector::~VirtualDetector()
////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////
bool VirtualDetector::IsInside( const G4ThreeVector &gPos ) const
//////////////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );

  return ( fabs(lPos.x()) <= Length_ &&
	   fabs(lPos.y()) <= Height_ &&
	   fabs(lPos.z()) <= Width_ );
}

/////////////////////////////////////////////////////////////////////
G4VPhysicalVolume *VirtualDetector::Place(G4VPhysicalVolume *pMother)
/////////////////////////////////////////////////////////////////////
{
  G4Box *VDsolid = new G4Box( elemName_, Length_, Height_, Width_ );

  G4LogicalVolume *VDlogical =
    new G4LogicalVolume( VDsolid, mVol_, elemName_, 0, 0, 0 );

  G4VPhysicalVolume *VirtualD =
    new G4PVPlacement( &rotMtx_, gPos_, elemName_, VDlogical,
		       pMother, false, Id_ );
  
  //G4double maxStep = 0.1*Width_;
  //VDlogical-> SetUserLimits(new G4UserLimits(maxStep));
  
  //VDlogical-> SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes *VisAtt = new G4VisAttributes( G4Color(0,0,0));
  VDlogical->SetVisAttributes( VisAtt );
  logDetector_ = VDlogical;

  return VirtualD;
}
