/*
  HESTrajectory.hh

  D.Kawama
*/

#ifndef HESTrajectory_h

#define HESTrajectory_h 1

#include "globals.hh"
#include "G4VTrajectory.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
//#include "g4std/vector"
#include <vector>

class G4Track;
class G4Step;

typedef std::vector <G4VTrajectoryPoint*> TrajectoryPointContainer;

class HESTrajectory : public G4VTrajectory
{
public:
  HESTrajectory();
  HESTrajectory( const G4Track *aTrack );
  ~HESTrajectory();

  inline void* operator new( std::size_t );
  inline void operator delete( void* );
  int operator == ( const HESTrajectory &right ) const
  { return ( this == &right ); }


private:
  TrajectoryPointContainer* positionRecord;
  G4int trackID_;
  G4int parentID_;
  G4String particleName_;
  G4double PDGCharge_;
  G4double particleMass_;
#ifndef Geant44
  G4int PDGEncoding_;
#endif

  G4ThreeVector gpos_, dpos_;
  G4ThreeVector gmom_, dmom_;

public:
  void ShowTrajectory( std::ostream& os ) const; 
  void DrawTrajectory( G4int i_mode=0 ) const;
  void AppendStep( const G4Step *aStep );
  int GetPointEntries() const
  { return positionRecord-> size(); }
  G4VTrajectoryPoint* GetPoint( int i ) const
  { return (*positionRecord)[i]; }
  void MergeTrajectory( G4VTrajectory* secondTrajectory );

  G4int GetTrackID( void )  const { return trackID_;  }
  G4int GetParentID( void ) const { return parentID_; }
#ifdef GEANT44
  const G4String & GetParticleName( void ) const
  { return particleName_; }
  G4double GetPDGCharge( void ) const { return PDGCharge_; }
  G4double GetParticleMass( void ) const { return particleMass_; }
#else
  G4String GetParticleName( void ) const { return particleName_; }
  G4double GetCharge( void ) const { return PDGCharge_; }
  G4int GetPDGEncoding( void ) const { return PDGEncoding_; }
  G4ThreeVector GetInitialMomentum() const { return gmom_; }
#endif

  const G4ThreeVector & GetPositionAtFirst( void ) const 
  { return gpos_; }
  const G4ThreeVector & GetPositionAtLast( void ) const 
  { return dpos_; }
  const G4ThreeVector & GetMomentumAtFirst( void ) const 
  { return gmom_; }
  const G4ThreeVector & GetMomentumAtLast( void ) const 
  { return dmom_; }

  void SetPositionAtLast( const G4ThreeVector &pos ){ dpos_=pos; }
  void SetMomentumAtLast( const G4ThreeVector &mom ){ dmom_=mom; }

};

extern G4Allocator<HESTrajectory> HESTrajectoryAllocator;

inline void* HESTrajectory::operator new( std::size_t )
{
  return static_cast<void*>( HESTrajectoryAllocator.MallocSingle() );
}

inline void HESTrajectory::operator delete( void *aHit )
{
  HESTrajectoryAllocator.FreeSingle
    ( static_cast<HESTrajectory*>( aHit ) );
}

#endif

