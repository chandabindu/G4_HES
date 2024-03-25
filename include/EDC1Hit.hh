/*
  EDC1Hit.hh

  D.Kawama 
*/

#ifndef EDC1Hit_h

#define EDC1Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class EDC1Hit : public G4VHit
{
public:
  EDC1Hit( G4int dcID, G4int layerID,
	    const G4ThreeVector &gPos,
	    const G4ThreeVector &lPos,
	    G4int track, G4double time );
  ~EDC1Hit();

private:
  EDC1Hit( const EDC1Hit & right );
  EDC1Hit & operator == ( const EDC1Hit & );

public:
  int operator == ( const EDC1Hit & ) const
  { return 0; }

  inline void * operator new( size_t size );
  inline void operator delete( void *aHit );

  void Draw();
  void Print();

private:
  G4int DC_, Layer_, trackID_;
  G4double edep_, Time_, lX, lU, lV;
  G4ThreeVector gPos_, gMom_, lPos_, lMom_;
  G4bool fSignal_;
  G4double tLength_;

public:
  void SetDCID( G4int id ) { DC_ = id; }
  void SetLayerID( G4int id ) { Layer_ = id; }
  void SetTrackNo( G4int id ) { trackID_ = id; }
  void SetGPos( const G4ThreeVector & pos ) { gPos_ = pos; }
  void SetGMom( const G4ThreeVector & mom ) { gMom_ = mom; }
  void SetLPos( const G4ThreeVector & pos ) { lPos_ = pos; }
  void SetLMom( const G4ThreeVector & mom ) { lMom_ = mom; }
  void SettLength( G4double & tlen ) { tLength_ = tlen; }
  void SetEdep( G4double E ) { edep_ = E; }
  void AddEdep( G4double E ) { edep_ += E; }
  void SetTime( G4double T ) { Time_ = T; }
  void SetXplane( G4double x ) { lX = x;};
  void SetUplane( G4double u ) { lU = u;};
  void SetVplane( G4double v ) { lV = v;};
  void SetTrueSignal() { fSignal_ = true; }
  void SetFalseSignal() { fSignal_ = false; }

  G4int GetDCID( void ) const { return DC_; }
  G4int GetLayerID( void ) const { return Layer_; }
  G4int GetTrackNo( void ) const { return trackID_; }
  const G4ThreeVector & GetGPos() const { return gPos_; }
  const G4ThreeVector & GetGMom() const { return gMom_; }
  const G4ThreeVector & GetLPos() const { return lPos_; }
  const G4ThreeVector & GetLMom() const { return lMom_; }
  const G4double & GettLength() const { return tLength_; }
  G4double GetEdep() const { return edep_; }
  G4double GetTime() const { return Time_; }
  G4double GetXplane() const { return lX; }
  G4double GetUplane() const { return lU; }
  G4double GetVplane() const { return lV; }
  G4bool IsTrueSignal() const { return fSignal_; }

};

typedef G4THitsCollection<EDC1Hit> EDC1HitsCollection;

extern G4Allocator<EDC1Hit> EDC1HitAllocator;

inline void* EDC1Hit::operator new( size_t )
{
  return static_cast<void*>( EDC1HitAllocator.MallocSingle() );
}

inline void EDC1Hit::operator delete( void* aHit )
{
  EDC1HitAllocator.FreeSingle
    ( static_cast<EDC1Hit *>( aHit ) );
}

#endif
