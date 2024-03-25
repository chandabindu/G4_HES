/*
  HESEH1Hit.hh

  K.Yokota 
*/

#ifndef EH1Hit_h

#define EH1Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class EH1Hit : public G4VHit
{
public:
  EH1Hit( G4int ehID, G4int layerEHID,
	    const G4ThreeVector &gPos,
	    const G4ThreeVector &lPos,
	    G4int track, G4double time );
  ~EH1Hit();

private:
  EH1Hit( const EH1Hit & right );
  EH1Hit & operator == ( const EH1Hit & );

public:
  int operator == ( const EH1Hit & ) const
  { return 0; }

  inline void * operator new( size_t size );
  inline void operator delete( void *aHit );

  void Draw();
  void Print();

private:
  G4int EH_, LayerEH_, trackID_;
  G4double edep_, Time_, lX, lU, lV;
  G4ThreeVector gPos_, gMom_, lPos_, lMom_;
  G4bool fSignal_;

public:
  void SetEHID( G4int id ) { EH_ = id; }
  void SetLayerEHID( G4int id ) { LayerEH_ = id; }
  void SetTrackNo( G4int id ) { trackID_ = id; }
  void SetGPos( const G4ThreeVector & pos ) { gPos_ = pos; }
  void SetGMom( const G4ThreeVector & mom ) { gMom_ = mom; }
  void SetLPos( const G4ThreeVector & pos ) { lPos_ = pos; }
  void SetLMom( const G4ThreeVector & mom ) { lMom_ = mom; }
  void SetEdep( G4double E ) { edep_ = E; }
  void AddEdep( G4double E ) { edep_ += E; }
  void SetTime( G4double T ) { Time_ = T; }
  void SetXplane( G4double x ) { lX = x;};
  void SetUplane( G4double u ) { lU = u;};
  void SetVplane( G4double v ) { lV = v;};
  void SetTrueSignal() { fSignal_ = true; }
  void SetFalseSignal() { fSignal_ = false; }

  G4int GetEHID( void ) const { return EH_; }
  G4int GetLayerID( void ) const { return LayerEH_; }
  G4int GetTrackNo( void ) const { return trackID_; }
  const G4ThreeVector & GetGPos() const { return gPos_; }
  const G4ThreeVector & GetGMom() const { return gMom_; }
  const G4ThreeVector & GetLPos() const { return lPos_; }
  const G4ThreeVector & GetLMom() const { return lMom_; }
  G4double GetEdep() const { return edep_; }
  G4double GetTime() const { return Time_; }
  G4double GetXplane() const { return lX; }
  G4double GetUplane() const { return lU; }
  G4double GetVplane() const { return lV; }
  G4bool IsTrueSignal() const { return fSignal_; }

};

typedef G4THitsCollection<EH1Hit> EH1HitsCollection;

extern G4Allocator<EH1Hit> EH1HitAllocator;

inline void* EH1Hit::operator new( size_t )
{
  return static_cast<void*>( EH1HitAllocator.MallocSingle() );
}

inline void EH1Hit::operator delete( void* aHit )
{
  EH1HitAllocator.FreeSingle
    ( static_cast<EH1Hit *>( aHit ) );
}

#endif
