/*
  VDetectorQ2.cc

  D.Kawama
*/

#include "VDetectorQ2.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4IntersectionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//////////////////////////////////////////////////////////
VDetectorQ2::VDetectorQ2( const G4String &name,
			  const G4ThreeVector &gPos,
			  const G4RotationMatrix *rotMtx,
			  int id )
  : BeamLineElement(name,gPos,rotMtx),
    Id_(id), mVol_(0), logDetector_(0), Vsolid_(0)
/////////////////////////////////////////////////////////
{
}

////////////////////////////
VDetectorQ2::~VDetectorQ2()
///////////////////////////
{
}

//////////////////////////////////////////////////////////////
bool VDetectorQ2::IsInside( const G4ThreeVector &gPos ) const
/////////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( Vsolid_->Inside(lPos)!=kOutside ) return true;
  else return false;
}
/////////////////////////////////////////////////////////////////
G4VPhysicalVolume *VDetectorQ2::Place(G4VPhysicalVolume *pMother)
/////////////////////////////////////////////////////////////////
{
  G4double Width = 1.*cm;


  G4Trd* TrdA = new G4Trd( "Q2 Trd 1", Width/2, Width/2,
			   28.9*cm, 2.5*cm, 31.1*cm );
  G4Trd* TrdB = new G4Trd( "Q2 Trd 2", Width/2, Width/2,
			   2.5*cm, 28.9*cm, 31.1*cm );

  G4IntersectionSolid* Q2Trd =
    new G4IntersectionSolid( "Q2 Gap-3", TrdA, TrdB );

  G4RotationMatrix* rotTrd = new G4RotationMatrix;
  rotTrd-> rotateY(90.*degree);
  G4DisplacedSolid* VDsolid = 
    new G4DisplacedSolid( elemName_, Q2Trd, rotTrd,
			  G4ThreeVector(0., 0., 0.) );



  G4LogicalVolume *VDlogical =
    new G4LogicalVolume( VDsolid, mVol_, elemName_, 0, 0, 0 );

  G4VPhysicalVolume *VirtualD =
    new G4PVPlacement( &rotMtx_, gPos_, elemName_, VDlogical,
		       pMother, false, Id_ );
  
  G4double maxStep = 0.1*Width;
  VDlogical-> SetUserLimits(new G4UserLimits(maxStep));

  VDlogical-> SetVisAttributes(G4VisAttributes::Invisible);
  //G4VisAttributes *VisAtt =
  //  new G4VisAttributes( G4Colour::G4Colour(0.,1.,1.) );
  //VDlogical-> SetVisAttributes( VisAtt );
  logDetector_ = VDlogical;
  Vsolid_ = VDsolid;

  return VirtualD;
}
