/*
  HESTrackingAction.cc

  D.Kawama
*/

#include "HESTrackingAction.hh"
#include "HESTrajectory.hh"
#include "HESField.hh"

#include "G4Track.hh"
#include "G4TrackingManager.hh"

///////////////////////////////////////
HESTrackingAction::HESTrackingAction()
  : G4UserTrackingAction()
///////////////////////////////////////  
{
}

////////////////////////////////////////
HESTrackingAction::~HESTrackingAction()
////////////////////////////////////////
{
}

///////////////////////////////////////////////////////////////////////
void HESTrackingAction::PreUserTrackingAction( const G4Track *aTrack )
///////////////////////////////////////////////////////////////////////
{
  G4bool stored = true;
  HESTrajectory *trj = new HESTrajectory( aTrack );
  fpTrackingManager-> SetTrajectory(trj );
  fpTrackingManager-> SetStoreTrajectory( stored );

}

////////////////////////////////////////////////////////////////////////
void HESTrackingAction::PostUserTrackingAction( const G4Track *aTrack )
////////////////////////////////////////////////////////////////////////
{
  HESTrajectory *trj = dynamic_cast<HESTrajectory*>
    ( fpTrackingManager-> GimmeTrajectory() );
  if( trj ){
    trj-> SetPositionAtLast( aTrack-> GetPosition() );
    trj-> SetMomentumAtLast( aTrack-> GetMomentum() );
  }

}
