/*
  HESTargetHit.hh

  D.Kawama
*/

#ifndef HESTargetHit_h

#define HESTargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class HESTargetHit : public G4VHit
{
public:
  HESTargetHit();
  ~HESTargetHit();

private:
  HESTargetHit( const HESTargetHit &right );
//  HESTargetHit( const HESTargetHit & );
//  const HESTargetHit& operator = ( const HESTargetHit &right );
  HESTargetHit & operator = ( const HESTargetHit &right );


public:
  int operator == ( const HESTargetHit &) const
  { return 0; }

  inline void* operator new( size_t size );
  inline void operator delete( void* aHit );

  void Draw() ;
  void Print();

private:
  G4double edep_;
  G4double time_;
  G4ThreeVector pos_;
  G4ThreeVector mom_;
  G4int trackNo_;
  G4bool fSignal_;

  G4ThreeVector lPos_, lMom_;
  G4ThreeVector gPos_, gMom_;

public:
  void SetLPos( const G4ThreeVector & pos ) { lPos_ = pos; }
  void SetLMom( const G4ThreeVector & mom ) { lMom_ = mom; }
  void SetGPos( const G4ThreeVector & pos ) { gPos_ = pos; }
  void SetGMom( const G4ThreeVector & mom ) { gMom_ = mom; }
  void SetEdep( G4double Sedep ) { edep_ = Sedep; }
  void AddEdep( G4double Aedep ) { edep_ +=Aedep; }
  void SetTime( G4double Stime ) { time_ = Stime; }
  void SetPos( const G4ThreeVector & pos ) { pos_ = pos; }
  void SetMom( const G4ThreeVector & mom ) { mom_ = mom; }
  void SetTrackNo( G4int No ) { trackNo_ = No; }
  void SetTrueSignal() { fSignal_ = true; }
  void SetFalseSignal() { fSignal_ = false; }

  const G4double & GetEdep() const { return edep_; }
  G4double GetTime() const { return time_; }
  const G4ThreeVector & GetGPos() const { return gPos_; }
  const G4ThreeVector & GetGMom() const { return gMom_; }
  const G4ThreeVector & GetLPos() const { return lPos_; }
  const G4ThreeVector & GetLMom() const { return lMom_; }
  G4int GetTrackNo() const { return trackNo_; }
  G4bool IsTrueSignal() const { return fSignal_; }

};

typedef G4THitsCollection<HESTargetHit> HESTargetHitsCollection;

extern G4Allocator<HESTargetHit> HESTargetHitAllocator;

//inline functions
//inline HESTargetHit::HESTargetHit(const HESTargetHit &right)
//{
// edep_=right.edep_;
//}

//inline const HESTargetHit& HESTargetHit::operator=(const HESTargetHit &right)
//{
// edep_=right.edep_;
// return *this;
//}

inline void* HESTargetHit::operator new( size_t )
{
  return static_cast<void*>( HESTargetHitAllocator.MallocSingle() );
}

inline void HESTargetHit::operator delete( void* aHit )
{
  HESTargetHitAllocator.FreeSingle
    ( static_cast<HESTargetHit *>( aHit ) );
}

#endif
