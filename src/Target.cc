/*
  Target.cc

  D.Kawama
*/

#include "Target.hh"

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

/////////////////////////////////////////////////////////
Target::Target( const G4String &name,
			  const G4ThreeVector &gPos,
			  const G4RotationMatrix *rotMtx,
			  int id )
  : BeamLineElement(name,gPos,rotMtx),
    Id_(id), mVol_(0), logDetector_(0), targetsolid_(0)
/////////////////////////////////////////////////////////
{
}

/////////////////
Target::~Target()
/////////////////
{
}

//////////////////////////////////////////////////////////
bool Target::IsInside( const G4ThreeVector &gPos ) const
/////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( targetsolid_->Inside(lPos)!=kOutside ) return true;
  else return false;
}

/////////////////////////////////////////////////////////////
G4VPhysicalVolume *Target::Place(G4VPhysicalVolume *pMother)
/////////////////////////////////////////////////////////////
{
  G4RotationMatrix* rotTrd = new G4RotationMatrix;
  rotTrd-> rotateY(0.*degree);
  
  //solid definition
  G4Box* tsolid = new G4Box( "Target",
			     1.*cm, 1.*cm, 0.0164*cm );


  //logical volume definition
  G4LogicalVolume *tlogical =
    new G4LogicalVolume( tsolid, mVol_, elemName_, 0, 0, 0 );
  
  //physical volume definition
  G4VPhysicalVolume *target =
    new G4PVPlacement( &rotMtx_, gPos_, elemName_, tlogical,
		       pMother, false, 0 );

  G4VisAttributes *VisAtt = new G4VisAttributes();
    //new G4VisAttributes( G4Colour::G4Colour(0.,1.,1.) );
  VisAtt->SetColour(0.0,1.0,1.0);
  tlogical-> SetVisAttributes( VisAtt );
    
  logDetector_ = tlogical;
  targetsolid_ = tsolid;

  return target;
}
