/*
  HESEH1SD.hh

  K.Yokota
  2008/Aug/26
*/

#ifndef EH1SD_h

#define EH1SD_h 1

#include "G4VSensitiveDetector.hh"
#include "HESEH1Hit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class BeamLineElement;

class EH1SD : public G4VSensitiveDetector
{
public:
  EH1SD( G4String name );
  ~EH1SD();

  void Initialize( G4HCofThisEvent *HCE );
  G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
  void EndOfEvent( G4HCofThisEvent* HCE );

  void DrawAll();
  void PrintAll();
  void clear();

  //  void SetElement( int id, BeamLineElement *elem);

private:
  EH1HitsCollection* EH1Collection;
  BeamLineElement *elem_;
  G4double thEdep;

public:
  void SetElement( BeamLineElement *elem)
  { elem_ = elem; }

};

#endif
