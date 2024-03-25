/*
  HESTargetSD.cc

  D.Kawama
*/

#include "G4HCofThisEvent.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4TouchableHandle.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "HESTargetSD.hh"
#include "BeamLineElement.hh"
#include "Target.hh"

/////////////////////////////////////////
HESTargetSD::HESTargetSD( G4String name )
  : G4VSensitiveDetector(name)
/////////////////////////////////////////
{
  G4String HCname = "TargetCollection";
  collectionName.insert( HCname );
}

/////////////////////////////
HESTargetSD::~HESTargetSD()
////////////////////////////
{
}

/////////////////////////////////////////////////////
void HESTargetSD::Initialize( G4HCofThisEvent* HCE )
/////////////////////////////////////////////////////
{
//    static G4int HCID = -1;
  TargetCollection = 
    new HESTargetHitsCollection( SensitiveDetectorName,
				 collectionName[0]);
  //  if( HCID<0 ){
   G4int HCID = GetCollectionID(0);
 //   }
  HCE-> AddHitsCollection( HCID, TargetCollection );
  edep=0.;
}

//////////////////////////////////////////////////////////////
G4bool HESTargetSD::ProcessHits( G4Step* aStep,
				 G4TouchableHistory* )
/////////////////////////////////////////////////////////////
{

  edep += aStep-> GetTotalEnergyDeposit();
  if( edep==0.0 ) return false;
//  if( edep==0.0 ) return true;

  G4int nHits = TargetCollection-> entries();
  G4Track* aTrack = aStep-> GetTrack();
  
  G4double edep = aStep-> GetTotalEnergyDeposit();
  G4ThreeVector hitPos = aStep-> GetPreStepPoint()-> GetPosition();
  G4ThreeVector hitMom = aTrack-> GetMomentum();
  G4double hitTime = aTrack-> GetGlobalTime();
  G4int trackNo = aTrack-> GetTrackID();
  

  for( G4int i=0; i<nHits; i++ ){
    HESTargetHit *aHit = (*TargetCollection)[i];
    G4double time = aHit-> GetTime();
    if( fabs(hitTime-time) <= 0.1*ns ){
      aHit-> AddEdep(edep);
      if( hitTime < time ){
	aHit-> SetTime( hitTime );
	aHit-> SetPos( hitPos );
	aHit-> SetMom( hitMom );
	aHit-> SetTrackNo( trackNo );
      }
      return true;
    }
  }
  
  HESTargetHit* aHit = new HESTargetHit();
  aHit-> SetEdep( edep );
  aHit-> SetTime( hitTime );
  aHit-> SetPos( hitPos );
  aHit-> SetMom( hitMom );
  aHit-> SetTrackNo( trackNo );
  TargetCollection-> insert( aHit );
  
  return true;
}

/////////////////////////////////////////////////////
void HESTargetSD::EndOfEvent( G4HCofThisEvent* )
/////////////////////////////////////////////////////
{
 if(edep>0){
   HESTargetHit *aHit = new HESTargetHit();
   TargetCollection->insert(aHit);
 }

}

//////////////////////////
void HESTargetSD::clear()
/////////////////////////
{
 /* G4int nHits = TargetCollection-> entries();
  for( G4int i=0; i<nHits; i++ ){
    delete (*TargetCollection)[i];
  }*/
}

////////////////////////////
void HESTargetSD::DrawAll() 
////////////////////////////
{
/*  G4VVisManager* pVisManager = G4VVisManager::GetConcreteInstance();
  if( pVisManager ){
    G4int nHits = TargetCollection-> entries();
    for( G4int i=0; i<nHits; i++ ){
      (*TargetCollection)[i]-> Draw();
    }
  }*/
}

/////////////////////////////
void HESTargetSD::PrintAll() 
/////////////////////////////
{
//  G4int nHits = TargetCollection-> entries();
//  for( G4int i=0; i<nHits; i++ ){
//    (*TargetCollection)[i]-> Print();
//  }
    TargetCollection-> PrintAllHits();
}

