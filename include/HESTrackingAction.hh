/*
  HESTrackingAction.hh

  D.Kawama
*/

#ifndef HESTrackingAction_h

#define HESTrackingAction_h

#include "G4UserTrackingAction.hh"

class G4Track;

class HESTrackingAction : public G4UserTrackingAction
{
public:
  HESTrackingAction();
  HESTrackingAction( const G4Track* aTrack );
  ~HESTrackingAction();

  void PreUserTrackingAction( const G4Track* aTrack );
  void PostUserTrackingAction( const G4Track* aTrack );

};

#endif
