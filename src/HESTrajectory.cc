/*
  HESTrajectiry.cc

  D.Kawama
*/

#include "HESTrajectory.hh"

#include <algorithm>
#include "TemplateLib.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4Circle.hh"
#include "G4VisAttributes.hh"
#include <vector> 

G4Allocator<HESTrajectory> HESTrajectoryAllocator;

///////////////////////////////////////////////////////
HESTrajectory::HESTrajectory( const G4Track* aTrack)
  : positionRecord( new TrajectoryPointContainer ),
    trackID_( aTrack-> GetTrackID() ),
    parentID_( aTrack-> GetParentID() ),
    gpos_( aTrack-> GetPosition() ),
    gmom_( aTrack-> GetMomentum() )
//////////////////////////////////////////////////////
{
  G4ParticleDefinition* fpPD = aTrack-> GetDefinition();
  particleName_ = fpPD-> GetParticleName();
  PDGCharge_ = fpPD-> GetPDGCharge();
#ifdef GEANT44
  PDGEncoding_ = fpPD-> GetPDGEncoding();
#endif
}

////////////////////////////////
HESTrajectory::~HESTrajectory()
////////////////////////////////
{
  for_each( positionRecord-> begin(),
	    positionRecord-> end(), DeleteObject());
  delete positionRecord;
}

/////////////////////////////////////////////////////////
void HESTrajectory::ShowTrajectory( std::ostream& ) const
////////////////////////////////////////////////////////
{ 
  G4cout << G4endl << "TrackID = " << trackID_
	 << " : ParentID = " << parentID_ << G4endl;

  G4cout << "ParticleName : " << particleName_ << G4endl;

  G4cout << "Current trajectory has " << positionRecord-> size()
	 << " point" << G4endl;

  for( G4int i=0; G4int(positionRecord-> size()); ++i ){
    G4TrajectoryPoint* tp = 
      dynamic_cast<G4TrajectoryPoint*> ((*positionRecord)[i]);
    
    G4cout << "Point[" << i << "]"
	   << "Position = " << tp-> GetPosition() << G4endl;
  }
}

//////////////////////////////////////////////////////////
void HESTrajectory::DrawTrajectory( G4int i_mode ) const
//////////////////////////////////////////////////////////
{
  G4VVisManager* pVisManager = 
    G4VVisManager::GetConcreteInstance();

  if( pVisManager ){
    if(i_mode >= 0 ){
      G4Polyline pPolyline;
      for( G4int i=0 ; i<G4int(positionRecord-> size());++i){
	G4TrajectoryPoint* tp = 
	  dynamic_cast<G4TrajectoryPoint*> ((*positionRecord)[i]);
	pPolyline.push_back( tp-> GetPosition() );
      }
      G4Colour colour;
      if( PDGCharge_<0. )      colour = G4Colour( 1.0, 0.0, 0.0 );
      else if( PDGCharge_>0. ) colour = G4Colour( 0.0, 0.0, 1.0 );
      else                     colour = G4Colour( 0.0, 1.0, 0.0 );
      //colour = G4Colour( 1.0, 0.0, 0.0 );

      pPolyline.SetVisAttributes( G4VisAttributes(colour) );
      pVisManager-> Draw( pPolyline );
    }

    if( i_mode != 0 ){
      for( G4int i=0 ; i<G4int(positionRecord-> size());++i){
	G4TrajectoryPoint* tp = 
	  dynamic_cast<G4TrajectoryPoint*> ((*positionRecord)[i]);
	G4Circle circle( tp-> GetPosition() );
	circle.SetScreenSize( 0.001*i_mode );
	circle.SetFillStyle( G4Circle::filled );
	circle.SetVisAttributes( G4VisAttributes(G4Colour(0.,0.,0.)) );
	pVisManager-> Draw( circle );
      }
    }
  }
}

//////////////////////////////////////////////////////
void HESTrajectory::AppendStep( const G4Step* aStep )
//////////////////////////////////////////////////////
{
  positionRecord-> push_back
    ( new G4TrajectoryPoint
      ( aStep-> GetPostStepPoint()-> GetPosition() ) );
}

///////////////////////////////////////////////////////////////////////
void HESTrajectory::MergeTrajectory( G4VTrajectory* secondTrajectory )
///////////////////////////////////////////////////////////////////////
{
  HESTrajectory* second = 
    dynamic_cast<HESTrajectory*>( secondTrajectory );
  if( second ){
    G4int ent = second-> GetPointEntries();
    for( G4int i=0; i<ent; ++i ){
      positionRecord-> push_back((*(second-> positionRecord))[i]);
    }
    delete (*(second-> positionRecord))[0];
    second-> positionRecord-> clear();
    dpos_ = second-> dpos_;
    second-> dpos_ = second-> gpos_;
    dmom_ = second-> dmom_;
    second-> dmom_ = second-> gmom_;
  }
}

