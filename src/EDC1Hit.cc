/*
  EDC1Hit.cc

  D.Kawama 
*/

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

#include "EDC1Hit.hh"

#include <iomanip>

/////////////////////////////////////////////////
EDC1Hit::EDC1Hit( G4int dcID, G4int layerID,
		    const G4ThreeVector &gPos,
		    const G4ThreeVector &gMom,
		    G4int track, G4double time )
  : edep_(0), lX(0), lU(0), lV(0),
    lPos_(0), lMom_(0), fSignal_(false)
/////////////////////////////////////////////////
{
  DC_ = dcID;
  Layer_ = layerID;
  gPos_ = gPos;
  gMom_ = gMom;
  trackID_ = track;
  Time_ = time;
}

/////////////////////
EDC1Hit::~EDC1Hit()
/////////////////////
{
}

G4Allocator<EDC1Hit> EDC1HitAllocator;

/////////////////////
void EDC1Hit::Draw()
/////////////////////
{
  G4VVisManager *pVisMan = G4VVisManager::GetConcreteInstance();
  if( pVisMan ){
    G4Circle circle(gPos_);
    circle.SetScreenSize( 3.0 ); // in pixels
    circle.SetFillStyle( G4Circle::filled );
    G4Colour colour( 0., 0., 1. ); //blue
    G4VisAttributes attribs( colour );
    circle.SetVisAttributes( attribs );
    pVisMan->Draw( circle );
  }
}

//////////////////////
void EDC1Hit::Print()
//////////////////////
{
  G4cout << "DC" << DC_+1 << "[" << Layer_ 
	 << "] Hit (" << gPos_ << ")" << G4endl;
}
