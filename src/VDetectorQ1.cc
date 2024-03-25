/*
  VDetectorQ1.cc

  D.Kawama
*/

#include "VDetectorQ1.hh"

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

///////////////////////////////////////////////////////////
VDetectorQ1::VDetectorQ1( const G4String &name,
			  const G4ThreeVector &gPos,
			  const G4RotationMatrix *rotMtx,
			  int id )
  : BeamLineElement(name,gPos,rotMtx),
    Id_(id), mVol_(0), logDetector_(0), Vsolid_(0)
///////////////////////////////////////////////////////////
{
}

///////////////////////////
VDetectorQ1::~VDetectorQ1()
///////////////////////////
{
}

//////////////////////////////////////////////////////////////
bool VDetectorQ1::IsInside( const G4ThreeVector &gPos ) const
//////////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( Vsolid_->Inside(lPos)!=kOutside ) return true;
  else return false;
}
//////////////////////////////////////////////////////////////////
G4VPhysicalVolume *VDetectorQ1::Place(G4VPhysicalVolume *pMother)
//////////////////////////////////////////////////////////////////
{
  G4double Width = 1.*cm;


  G4Trd* TrdA = new G4Trd( "Q1 Trd 1", Width/2, Width/2,
			   21.6*cm, 1.5*cm, 22.0*cm );
  G4Trd* TrdB = new G4Trd( "Q1 Trd 2", Width/2, Width/2,
			   1.5*cm, 21.6*cm, 22.0*cm );

  G4IntersectionSolid* Q1Trd =
    new G4IntersectionSolid( "Q1 Gap-3", TrdA, TrdB );

  G4RotationMatrix* rotTrd = new G4RotationMatrix;
  rotTrd-> rotateY(90.*degree);
  G4DisplacedSolid* VDsolid = 
    new G4DisplacedSolid( elemName_, Q1Trd, rotTrd,
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
