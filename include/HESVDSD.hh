#ifndef HESVDSD_h

#define HESVDSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HESVDHit.hh"
#include "Define.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class BeamLineElement;

class HESVDSD : public G4VSensitiveDetector
{
public:
  HESVDSD( G4String name );
  virtual ~HESVDSD();

  void Initialize( G4HCofThisEvent *HCE );
  G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
   void EndOfEvent( G4HCofThisEvent* HCE );

   void DrawAll() ;
   void PrintAll();
  void clear();

  void SetElement( int id, BeamLineElement *elem);

private:
  HESVDHitsCollection* VDCollection[n_vd];
  BeamLineElement *elem_[n_vd];
};

#endif
