/*
  EDC1SD.cc

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

#include "EDC1SD.hh"
#include "BeamLineElement.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>

const double EnergyDepositThreshold = 10.*keV;

///////////////////////////////////
EDC1SD::EDC1SD( G4String name )
  : G4VSensitiveDetector( name ), 
    thEdep( EnergyDepositThreshold )
////////////////////////////////////
{
  G4String Name = "EDC1Collection";
  collectionName.insert( Name );
  elem_ = 0;
}

////////////////////
EDC1SD::~EDC1SD()
////////////////////
{
}

/////////////////////////////////////////////////
void EDC1SD::Initialize( G4HCofThisEvent *HCE )
/////////////////////////////////////////////////
{
  DCCollection = 
    new EDC1HitsCollection( SensitiveDetectorName,
			       collectionName[0] );
  G4int HCID = GetCollectionID(0);
  HCE->AddHitsCollection( HCID, DCCollection );
}

//////////////////////////////////////////////////////////////////
G4bool EDC1SD::ProcessHits( G4Step *aStep, G4TouchableHistory* )
//////////////////////////////////////////////////////////////////
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  G4Track *aTrack = aStep->GetTrack();
  const G4VTouchable *theTouchable = 
    aStep->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume *DClayer = theTouchable->GetVolume();

  G4int hitLayer = DClayer->GetCopyNo();
  G4ThreeVector hitPos = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector hitMom =  aTrack->GetMomentum();
  G4int trackNo = aTrack->GetTrackID();
  G4double hitTime = aTrack->GetGlobalTime();
  G4double tLength = aTrack->GetTrackLength();


  G4ThreeVector lPos = elem_->GetLocalPosition( hitPos );
  G4ThreeVector lMom = elem_->GetLocalDirection( hitMom );

  G4double theta = 30.*degree;
  G4double X = lPos.x();
  G4double U = lPos.x()*cos(theta)+lPos.y()*sin(theta);
  G4double V = lPos.x()*cos(-theta)+lPos.y()*sin(-theta);

  G4int nHits = DCCollection->entries();
  for( G4int i=0; i<nHits; i++ ){
  		EDC1Hit *Hit = (*DCCollection)[i];
  	   if( hitLayer == Hit->GetLayerID() ){
  			Hit->AddEdep( edep );
			return true;
      }
  }

  EDC1Hit *aHit = 
    	new EDC1Hit( 0, hitLayer, hitPos, hitMom, trackNo, hitTime );
      //new EDC1Hit( DCno, hitLayer, hitPos, hitMom, trackNo, hitTime );
  aHit->SetEdep( edep );
  aHit->SetLPos( lPos );
  aHit->SetLMom( lMom );
  aHit->SetXplane( X );
  aHit->SetUplane( U );
  aHit->SetVplane( V );
  aHit->SettLength( tLength );

  DCCollection->insert( aHit );

  return true;
  

}

////////////////////////////////////////////
void EDC1SD::EndOfEvent( G4HCofThisEvent* )
////////////////////////////////////////////
{
}

/////////////////////
void EDC1SD::clear()
/////////////////////
{
  G4int nHits = DCCollection->entries();
  for( G4int j=0; j<nHits; j++ ){
    delete (*DCCollection)[j];
  }
}

////////////////////////
void EDC1SD::DrawAll()
///////////////////////
{
  G4VVisManager *pVisMan = G4VVisManager::GetConcreteInstance();

  if( pVisMan ){
    G4int nHits = DCCollection->entries();
    for( G4int j=0; j<nHits; j++ ){
      (*DCCollection)[j]->Draw();
    }
  }

}

////////////////////////
void EDC1SD::PrintAll()
////////////////////////
{
  G4int nHits = DCCollection->entries();
  for( G4int j=0; j<nHits; j++ ){
    (*DCCollection)[j]->Print();
  }
  
}
