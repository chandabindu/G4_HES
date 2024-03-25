/*
  VDetectorPB.cc

  D.Kawama
*/

#include "VDetectorPB.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4IntersectionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

VDetectorPB::VDetectorPB( const G4String &name,
			  const G4ThreeVector &gPos,
			  const G4RotationMatrix *rotMtx,
			  int id )
  : BeamLineElement(name,gPos,rotMtx),
    Id_(id), mVol_(0), logDetector_(0), Vsolid_(0)
{
}

VDetectorPB::~VDetectorPB()
{
}

bool VDetectorPB::IsInside( const G4ThreeVector &gPos ) const
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( Vsolid_->Inside(lPos)!=kOutside ) return true;
  else return false;
}

G4VPhysicalVolume *VDetectorPB::Place(G4VPhysicalVolume *pMother)
{

  G4RotationMatrix* rotTrd = new G4RotationMatrix;
  rotTrd-> rotateY(0.*degree);
  G4Box* VDsolid = new G4Box( elemName_, 100.*cm, 95.*cm, 0.5*cm);



  G4LogicalVolume *VDlogical =
    new G4LogicalVolume( VDsolid, mVol_, elemName_, 0, 0, 0 );

  G4VPhysicalVolume *VirtualD =
    new G4PVPlacement( &rotMtx_, gPos_, elemName_, VDlogical,
		       pMother, false, Id_ );

//  VDlogical-> SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes *VisAtt = new G4VisAttributes();
    //new G4VisAttributes( G4Colour::G4Colour(0.,1.,1.) );
  VisAtt->SetColour(0.0 , 1.0 , 1.0);
  VDlogical-> SetVisAttributes( VisAtt );
    
  logDetector_ = VDlogical;
  Vsolid_ = VDsolid;

  return VirtualD;
}
