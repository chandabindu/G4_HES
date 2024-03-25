#include "G4HCofThisEvent.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4TouchableHandle.hh"
#include "G4VVisManager.hh"

#include "HESVDSD.hh"
#include "BeamLineElement.hh"

#include <sstream>

//////////////////////////////////
HESVDSD::HESVDSD( G4String name )
  : G4VSensitiveDetector(name)
/////////////////////////////////
{
  for( int i=0; i<n_vd; ++i ){
    std::ostringstream id;
    id << i;
    G4String Name = G4String("VD")+id.str().c_str()+"-Collection";
    collectionName.insert( Name );
    elem_[i] = 0;
  }
}

////////////////////
HESVDSD::~HESVDSD()
///////////////////
{
}

/////////////////////////////////////////////////////////
void HESVDSD::SetElement( int id, BeamLineElement *elem )
/////////////////////////////////////////////////////////
{
  if( id >= 0 && id < n_vd ) elem_[id] = elem;
}

/////////////////////////////////////////////////
void HESVDSD::Initialize( G4HCofThisEvent *HCE )
/////////////////////////////////////////////////
{
  for( int i=0; i<n_vd ; ++i ){
    VDCollection[i] = new HESVDHitsCollection( SensitiveDetectorName,collectionName[i]);
    G4int HCID = GetCollectionID(i);
    HCE-> AddHitsCollection( HCID, VDCollection[i] );
  }
}

//////////////////////////////////////////////////////////////
G4bool HESVDSD::ProcessHits( G4Step* aStep,
				 G4TouchableHistory* )
/////////////////////////////////////////////////////////////	
{
  G4Track* aTrack = aStep-> GetTrack();
  const G4VTouchable* theTouchable =
    aStep-> GetPreStepPoint()-> GetTouchable();
  G4VPhysicalVolume *Vol = theTouchable-> GetVolume();
  G4int id = Vol->GetCopyNo();

  if( id>=0 && id<n_vd && elem_[id] ){
    G4ThreeVector hitPos = aStep-> GetPreStepPoint()-> GetPosition();
    G4double Charge=aStep->GetTrack()->GetDefinition()->GetPDGCharge();
    if( Charge != 0 ){
    G4ThreeVector hitMom = aTrack-> GetMomentum();
    //Kawama added
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4double tLength = aTrack->GetTrackLength();

    G4ThreeVector lPos = elem_[id]-> GetLocalPosition( hitPos );
    G4ThreeVector lMom = elem_[id]-> GetLocalDirection( hitMom );
    G4double u = lMom.x()/lMom.z(), v = lMom.y()/lMom.z();
    G4double x = lPos.x()-u*lPos.z(), y = lPos.y()-v*lPos.z();

    HESVDHit *aHit =
      new HESVDHit( id, hitPos, hitMom, x, y, u, v, edep, vertex );
    aHit-> SetGPos( hitPos );
    aHit-> SetLPos( lPos );
    aHit-> SetLMom( lMom );
    //Kawama added
    aHit-> SetEdep( edep );
    aHit-> SetVertex( vertex );
    aHit-> SettLength( tLength );

    VDCollection[id]-> insert( aHit );
    }
  }
  return true;
}

/////////////////////////////////////////////////
void HESVDSD::EndOfEvent( G4HCofThisEvent* )
/////////////////////////////////////////////////
{
}

//////////////////////
void HESVDSD::clear()
/////////////////////
{
  for( int i=0; i<n_vd; ++i ){
    G4int nHits = VDCollection[i]-> entries();
    for( G4int j=0; j<nHits; ++j ){
      delete (*VDCollection[i])[j];
    }
  }
}

///////////////////////
void HESVDSD::DrawAll()
///////////////////////
{
  G4VVisManager* pVisManager = G4VVisManager::GetConcreteInstance();
  if( pVisManager ){
    for( int j=0; j<n_vd; ++j ){
      G4int nHits = VDCollection[j]-> entries();
      for( G4int i=0; i<nHits; i++ ){
	(*VDCollection[j])[i]-> Draw();
      }
    }
  }
}

/////////////////////////
void HESVDSD::PrintAll() 
/////////////////////////
{
  for( int j=0; j<n_vd; ++j ){
    G4int nHits = VDCollection[j]-> entries();
    for( G4int i=0; i<nHits; i++ ){
      (*VDCollection[j])[i]-> Print();
    }
  }
}

