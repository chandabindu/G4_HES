/*
  HESEH1Hit.cc

  K.Yokota
  2008/Aug/26
*/

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

#include "HESEH1Hit.hh"

#include <iomanip>

/////////////////////////////////////////////////
EH1Hit::EH1Hit( G4int ehID, G4int layerEHID,
		    const G4ThreeVector &gPos,
		    const G4ThreeVector &gMom,
		    G4int track, G4double time )
  : edep_(0), lX(0), lU(0), lV(0),
    lPos_(0), lMom_(0), fSignal_(false)
/////////////////////////////////////////////////
{
  EH_ = ehID;
  LayerEH_ = layerEHID;
  gPos_ = gPos;
  gMom_ = gMom;
  trackID_ = track;
  Time_ = time;
}

/////////////////////
EH1Hit::~EH1Hit()
/////////////////////
{
}

G4Allocator<EH1Hit> EH1HitAllocator;

/////////////////////
void EH1Hit::Draw()
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
void EH1Hit::Print()
//////////////////////
{
  G4cout << "EH" << EH_+1 << "[" << LayerEH_ 
	 << "] Hit (" << gPos_ << ")" << G4endl;
}
