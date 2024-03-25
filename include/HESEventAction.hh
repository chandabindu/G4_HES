/*
  HESEventAction.hh

  D.Kawama
*/

#ifndef HES_EVENT_ACTION_H
#define HES_EVENT_ACTION_H 

#include "globals.hh"
#include "G4UserEventAction.hh"

class G4Event;
class HESAnalysis;

class HESEventAction : public G4UserEventAction {
public:
  HESEventAction(HESAnalysis *analysisManager=0 );
  virtual ~HESEventAction();

  virtual void BeginOfEventAction(const G4Event* anEvent);
  virtual void EndOfEventAction(const G4Event* anEvent);

private:
  HESAnalysis *anaMan;
  G4int TargetColID;
  G4int VDColID[15];
  //  G4int HT1xColID, HT1yColID;

};

#endif
