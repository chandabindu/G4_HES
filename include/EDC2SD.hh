/*
  EDC2SD.hh

  D.Kawama
*/

#ifndef EDC2SD_h

#define EDC2SD_h 1

#include "G4VSensitiveDetector.hh"
#include "EDC2Hit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class BeamLineElement;

class EDC2SD : public G4VSensitiveDetector
{
public:
  EDC2SD( G4String name );
  ~EDC2SD();

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
  EDC2HitsCollection* DCCollection;
  BeamLineElement *elem_;
  G4double thEdep;

public:
  void SetElement( BeamLineElement *elem)
  { elem_ = elem; }

};

#endif
