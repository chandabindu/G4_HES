#ifndef HESVDHit_h
#define HESVDHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class HESVDHit : public G4VHit
{
public:
  HESVDHit( G4int id, const G4ThreeVector &gPos, const G4ThreeVector &gMom,
	    G4double x, G4double y, G4double u, G4double v,
	    G4double edep, G4ThreeVector Vertex);
  ~HESVDHit();

private:
  HESVDHit( const HESVDHit &  );
  HESVDHit & operator = ( const HESVDHit & );

public:
  int operator == ( const HESVDHit & ) const
  { return 0; }

  inline void* operator new( size_t size );
  inline void operator delete( void* aHit );

  void Draw() ;
  void Print();

private:
  G4int VDId_;
  G4ThreeVector gPos_ , gMom_;
  G4double Xloc, Yloc, Uloc, Vloc;
  G4ThreeVector lPos_, lMom_;
  G4double Edep_;
  G4ThreeVector Vertex_;
  G4double tLength_;

public:
  void SetGPos( const G4ThreeVector & pos ) { gPos_ = pos; }
  void SetLPos( const G4ThreeVector & pos ) { lPos_ = pos; }
  void SetLMom( const G4ThreeVector & mom ) { lMom_ = mom; }
  void SetEdep( const G4double & Sedep ) { Edep_ = Sedep; }
  void AddEdep( const G4double & Aedep ) { Edep_ = Aedep; }
  void SetVertex( const G4ThreeVector & ver ) { Vertex_ = ver; }
  void SettLength( const G4double & tlen ) 
  { tLength_ = tlen; }

  const G4ThreeVector & GetPos() const { return lPos_; }
  const G4ThreeVector & GetGPos() const { return gPos_; }
  const G4ThreeVector & GetMom() const { return lMom_; }
  const G4ThreeVector & GetGMom() const { return gMom_; } 
  const G4double & GetEdep() const { return Edep_; } 
  G4int GetVDid( void ) const { return VDId_; }
  const G4ThreeVector & GetVertex() const { return Vertex_; } 
  const G4double & GettLength() const { return tLength_; } 

};

typedef G4THitsCollection<HESVDHit> HESVDHitsCollection;

extern G4Allocator<HESVDHit> HESVDHitAllocator;

inline void* HESVDHit::operator new( size_t )
{
  return static_cast<void*>( HESVDHitAllocator.MallocSingle() );
}

inline void HESVDHit::operator delete( void* aHit )
{
  HESVDHitAllocator.FreeSingle
    ( static_cast<HESVDHit *>( aHit ) );
}

#endif
