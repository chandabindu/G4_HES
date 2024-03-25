#include "HESSteppingAction.hh"
#include "HESGlobalSize.hh"
#include "HESField.hh"
#include "HESParamMan.hh"
#include "HESAnalysis.hh"

#include "G4Step.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"

#include <fstream>
#include <iostream>
using namespace std;

//ofstream TrackFile;
G4int n;
////////////////////////////////////////
HESSteppingAction::HESSteppingAction()
  : G4UserSteppingAction(), field_(0)
///////////////////////////////////////
{
// TrackFile.open("test.track");
}

///////////////////////////////////////
HESSteppingAction::~HESSteppingAction()
///////////////////////////////////////
{
// TrackFile.close();
}

//////////////////////////////////////////////////////////////////
void HESSteppingAction::UserSteppingAction( const G4Step *aStep )
/////////////////////////////////////////////////////////////////
{
  G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
  G4Track *track = aStep->GetTrack();
   
  /*n++;
  G4ThreeVector mom = aStep->GetPostStepPoint()->GetMomentum();
  G4ThreeVector vpos = aStep->GetTrack()->GetVertexPosition();
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  G4double E_Theta=paramMan->GetRotAngle()*degree;
  double Xg=(pos.x()*cos(E_Theta)+pos.z()*sin(E_Theta))/cm;
  double Yg=pos.y()/cm;
  double Zg=(-pos.x()*sin(E_Theta)+pos.z()*cos(E_Theta))/cm;
  double P=sqrt(mom.x()*mom.x()+mom.y()*mom.y()+mom.z()*mom.z())/GeV;
  if( (Xg>400 && Xg<410 && Zg < 300) ||
      (Xg>400 && Xg<410 && Zg > 400) ){
    track->SetTrackStatus(fStopAndKill);
  }
  if(vpos.z()*cm<1. && Xg>400 && Xg<800 && n%5==0){
  TrackFile<<Xg<< " "<<Yg<<" "<< Zg<<" "<< P<<G4endl;
  }*/

  const G4VTouchable *theTouchable =
      aStep->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume *Vol = theTouchable->GetVolume();
  G4String volName = Vol->GetName();
  
  if( fabs(pos.x()) > WorldSizeX ||
      fabs(pos.y()) > WorldSizeY ||
      fabs(pos.z()) > WorldSizeZ  ) 
    {
        track->SetTrackStatus(fStopAndKill);
    }
#ifdef DEBUG
    if( volName != "World" ){
       if( !field_ ){
 	G4FieldManager *fieldMan =
 	  G4TransportationManager::GetTransportationManager()
 	  ->GetFieldManager();
 	field_ = dynamic_cast<const HESField *>
 	  (fieldMan->GetDetectorField());
       }
      G4cout << pos/cm << " " << volName << G4endl;
      if( field_ ) field_->PrintQueue()
      G4cout << pos/cm << " " << volName << G4endl;
      G4Exception( "Wrong Volume" );
    }
#endif

#if 0
//     G4ParticleDefinition *particle =
//       trac->GetDynamicparticle()->GetDefinition();
//     G4cout << "HESSteppingAction::UserSteppingAction() : \n"
// 	   << " Trac(" << particle->GetParticleName()
// 	   << ") is killed at " << pos/cm << "[cm]" <<G4endl;
#endif
    G4double kin=track->GetKineticEnergy();
	 //G4cout << kin << G4endl;
	 if ( 
			   kin<10.
         || volName == "Splitter"
         || volName == "SS"
         || volName == "PCS"
         || volName == "Q1"
         || volName == "Q2"
         || volName == "D"
       )
    {
      track->SetTrackStatus(fStopAndKill);
      //track->SetTrackStatus(fKillTrackAndSecondaries);
    }
  

#ifdef DEBUG
//   if( !field_ ){
//     G4FieldManager *fieldMan =
//       G4TransportationManager::GetTransportationManager()->GetFieldManager();
//     field_ = dynamic_cast<const HESField *>(fieldMan->GetDetectorField());
//   }
  if( field_ ) field_->PushInQueue( 1, pos );
#endif
}
