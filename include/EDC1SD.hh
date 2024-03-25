/*
  EDC1SD.hh

  D.Kawama
*/

#ifndef EDC1SD_h

#define EDC1SD_h 1

#include "G4VSensitiveDetector.hh"
#include "EDC1Hit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class BeamLineElement;

class EDC1SD : public G4VSensitiveDetector
{
public:
  EDC1SD( G4String name );
  ~EDC1SD();

  void Initialize( G4HCofThisEvent *HCE );
  G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
  void EndOfEvent( G4HCofThisEvent* HCE );

  void DrawAll();
  void PrintAll();
  void clear();

  //  void SetElement( int id, BeamLineElement *elem);

private:
//   HESDCHitsCollection* DCCollection[2];
//   BeamLineElement *elem_[2];
  EDC1HitsCollection* DCCollection;
  BeamLineElement *elem_;
  G4double thEdep;

public:
  void SetElement( BeamLineElement *elem)
  { elem_ = elem; }

};

#endif
