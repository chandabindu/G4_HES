/*
  HESEH2SD.cc

  K.Yokota
  2008/Aug/26
*/

#include "G4HCofThisEvent.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4TouchableHandle.hh"
#include "G4VVisManager.hh"

#include "HESEH2SD.hh"
#include "BeamLineElement.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>

const double EnergyDepositThreshold = 10.*keV;

///////////////////////////////////
EH2SD::EH2SD( G4String name )
  : G4VSensitiveDetector( name ), 
    thEdep( EnergyDepositThreshold )
////////////////////////////////////
{
  G4String Name = "EH2Collection";
  collectionName.insert( Name );
  elem_ = 0;
}

////////////////////
EH2SD::~EH2SD()
////////////////////
{
}

/////////////////////////////////////////////////
void EH2SD::Initialize( G4HCofThisEvent *HCE )
/////////////////////////////////////////////////
{
  EH2Collection = 
    new EH2HitsCollection( SensitiveDetectorName,
			       collectionName[0] );
  G4int HCID = GetCollectionID(0);
  HCE->AddHitsCollection( HCID, EH2Collection );
}

//////////////////////////////////////////////////////////////////
G4bool EH2SD::ProcessHits( G4Step *aStep, G4TouchableHistory* )
//////////////////////////////////////////////////////////////////
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  G4Track *aTrack = aStep->GetTrack();
  const G4VTouchable *theTouchable = 
    aStep->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume *EHlayer = theTouchable->GetVolume();

    G4int hitLayer = EHlayer->GetCopyNo();
    G4ThreeVector hitPos = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector hitMom =  aTrack->GetMomentum();
    G4int trackNo = aTrack->GetTrackID();
    G4double hitTime = aTrack->GetGlobalTime();

    G4ThreeVector lPos = elem_->GetLocalPosition( hitPos );
    G4ThreeVector lMom = elem_->GetLocalDirection( hitMom );

//     G4double theta = 30.*degree;
//     G4double X = lPos.x();
//     G4double U = lPos.x()*cos(theta)+lPos.y()*sin(theta);
//     G4double V = lPos.x()*cos(-theta)+lPos.y()*sin(-theta);

    G4int nHits = EH2Collection->entries();
    for( G4int i=0; i<nHits; i++ ){
      EH2Hit *Hit = (*EH2Collection)[i];
      if( hitLayer == Hit->GetLayerID() ){
	Hit->AddEdep( edep );
	return true;
      }
    }

    EH2Hit *aHit = 
      new EH2Hit( 0, hitLayer, hitPos, hitMom, trackNo, hitTime );
      //new EH2Hit( EHno, hitLayer, hitPos, hitMom, trackNo, hitTime );
    aHit->SetEdep( edep );
    aHit->SetLPos( lPos );
    aHit->SetLMom( lMom );
//     aHit->SetXplane( X );
//     aHit->SetUplane( U );
//     aHit->SetVplane( V );

    EH2Collection->insert( aHit );

    return true;
  

  return true;
}

////////////////////////////////////////////
void EH2SD::EndOfEvent( G4HCofThisEvent* )
////////////////////////////////////////////
{
}

/////////////////////
void EH2SD::clear()
/////////////////////
{
  G4int nHits = EH2Collection->entries();
  for( G4int j=0; j<nHits; j++ ){
    delete (*EH2Collection)[j];
  }
}

////////////////////////
void EH2SD::DrawAll()
///////////////////////
{
  G4VVisManager *pVisMan = G4VVisManager::GetConcreteInstance();

  if( pVisMan ){
    G4int nHits = EH2Collection->entries();
    for( G4int j=0; j<nHits; j++ ){
      (*EH2Collection)[j]->Draw();
    }
  }

}

////////////////////////
void EH2SD::PrintAll()
////////////////////////
{
  G4int nHits = EH2Collection->entries();
  for( G4int j=0; j<nHits; j++ ){
    (*EH2Collection)[j]->Print();
  }
  
}
