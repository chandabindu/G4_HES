/*
  HESEventAction.cc

  D.Kawama
*/

#include "G4RunManager.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

//=== Sensitive Detector ===//
#include "HESEventAction.hh"
#include "HESTargetHit.hh"
#include "HESTargetSD.hh"
#include "HESVDHit.hh"
#include "EDC1Hit.hh"
#include "EDC2Hit.hh"

#include "HESAnalysis.hh"


#include <iomanip>
#include <sstream>

/////////////////////////////////////////////////////////////
HESEventAction::HESEventAction(HESAnalysis *analysisManager)
  : G4UserEventAction(), anaMan(analysisManager)
/////////////////////////////////////////////////////////////
{
  //for( int i=1; i<=14; ++i ) VDColID[i] = -1;
  //TargetColID = -1;
}

////////////////////////////////////
HESEventAction::~HESEventAction()
///////////////////////////////////
{
}

////////////////////////////////////////////////////////////////
void HESEventAction::BeginOfEventAction(const G4Event* anEvent)
///////////////////////////////////////////////////////////////
{
  /*G4SDManager* SDManager= G4SDManager::GetSDMpointer();

  if( TargetColID<0 ){
    TargetColID = SDManager->GetCollectionID(G4String("TargetCollection"));
  }
  
  for( int i=0; i<=14; ++i ){
    if( VDColID[i]<0 ){
      std::ostringstream id;
      id << i;
      G4String Name = G4String("VD")+id.str().c_str()+"-Collection";
      VDColID[i] = SDManager->GetCollectionID( Name );
    }
  }*/

  if(anaMan) anaMan->BeginOfEvent(anEvent);
    //G4cout << "EventAction::BeginOfEvent" << G4endl;  
}

///////////////////////////////////////////////////////////////
void HESEventAction::EndOfEventAction(const G4Event* anEvent)
//////////////////////////////////////////////////////////////
{

  G4VVisManager *visManager = G4VVisManager::GetConcreteInstance();
  anaMan->EndOfEvent(anEvent);

  //  G4cout << "EventAction::EndOfEvent" << G4endl;  
//  G4bool fTriggered = anaMan->GetTriggerStatus();
//  if(visManager && fTriggered){
//    // Trajectories
//    G4TrajectoryContainer *trajectoryContainer =
//      anEvent->GetTrajectoryContainer();
//    if(trajectoryContainer){
//      G4int nTrajectories = trajectoryContainer->entries();
//      for(G4int i=0; i<nTrajectories; i++){
//	G4VTrajectory *trj = 
//	((*(anEvent->GetTrajectoryContainer()))[i]);
//	//trj->DrawTrajectory(50);
//	visManager->Draw(*trj,50);
//      }
//    }
//
//    // get "Hit Collection of This Event"
////    G4HCofThisEvent* HCTE= anEvent->GetHCofThisEvent();
////    if(HCTE){
////      G4VHitsCollection *HESTargetHit = HCTE->GetHC( TargetColID );
////        if( HESTargetHit ) HESTargetHit-> DrawAllHits();
////     }
//    
//  }
}

