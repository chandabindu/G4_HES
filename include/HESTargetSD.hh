/*
  HESTargetSD.hh

  D.Kawama
*/

#ifndef HESTargetSD_h

#define HESTargetSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HESTargetHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class BeamLineElement;

class HESTargetSD : public G4VSensitiveDetector
{
public:
  HESTargetSD( G4String name );
  virtual ~HESTargetSD();

  virtual void Initialize( G4HCofThisEvent *HCE );
  virtual G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
  virtual void EndOfEvent( G4HCofThisEvent* HCE );

  virtual void DrawAll() ;
  virtual void PrintAll();
  void clear();

  void SetElement( BeamLineElement *elem )
  { elem_ = elem; }

private:
  HESTargetHitsCollection* TargetCollection;
  G4double edep;
  BeamLineElement *elem_;
};

#endif
