/*
  HESVDHit.cc

  D.Kawama
*/

#include "HESVDHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>

////////////////////////////////////////////////////////////////////
HESVDHit::HESVDHit( G4int id, const G4ThreeVector &gPos,
		    const G4ThreeVector &gMom,
		    G4double x, G4double y, G4double u, G4double v, 
		    G4double edep, G4ThreeVector vertex)
  : VDId_(id), gPos_(gPos), gMom_(gMom), 
    Xloc(x), Yloc(y), Uloc(u), Vloc(v), Edep_(edep), Vertex_(vertex)
////////////////////////////////////////////////////////////////////
{
}

/////////////////////
HESVDHit::~HESVDHit()
/////////////////////
{
}


G4Allocator<HESVDHit> HESVDHitAllocator;

//////////////////////
void HESVDHit::Draw() 
//////////////////////
{
  G4VVisManager* pVisManager = 
    G4VVisManager::GetConcreteInstance();
  if( pVisManager ){
    G4Circle circle(gPos_);
    circle.SetScreenSize( 3.0 ); // in pixels
    circle.SetFillStyle( G4Circle::filled );
    G4Colour colour( 1., 0., 0. ); //blue
    G4VisAttributes attribs( colour );
    circle.SetVisAttributes( attribs );
    pVisManager-> Draw( circle );
  }
}

///////////////////////
void HESVDHit::Print() 
///////////////////////
{
  G4cout << "VD : " << VDId_ << " : ( "
	 << gPos_.x()/cm << ", " << gPos_.y()/cm << ", "
	 << gPos_.z()/cm << " ) " << G4endl;

}
