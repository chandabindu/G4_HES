/*
  EHodo2.cc

  K.Yokota
  2008/Aug/26
*/

#include "EHodo2.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

EHodo2::EHodo2( const G4String &name,
		const G4ThreeVector &gPos,
		const G4RotationMatrix *rotMtx, int id )
  : BeamLineElement( name, gPos, rotMtx ),
    mVol_(0), DetectorLV_(0), vSolid(0), Id_(id)
{
}

EHodo2::~EHodo2()
{
}

bool EHodo2::IsInside( const G4ThreeVector &gPos ) const
{
  G4ThreeVector lPos = GetLocalPosition(gPos);
  if( vSolid-> Inside(lPos) != kOutside ) return true;
  else return false;
}


G4VPhysicalVolume *EHodo2::Place( G4VPhysicalVolume *pMother )
{
  G4String nmWall = elemName_+" Wall";
  G4String nmSeg = elemName_+" Segment";

  G4ThreeVector dxyz_1X_Wall( 117.*cm, 30.*cm, 10.*cm );
  G4ThreeVector dxyz_1X_Seg( 5.*cm, 30.*cm, 1.*cm );

  G4Box *WallSolid = new G4Box( nmWall, dxyz_1X_Wall.x()/2,
				dxyz_1X_Wall.y()/2, dxyz_1X_Wall.z()/2 );
  G4Box *SegSolid = new G4Box( nmSeg, dxyz_1X_Seg.x()/2,
			       dxyz_1X_Seg.y()/2, dxyz_1X_Seg.z()/2 );

  G4LogicalVolume *WallLV =
    new G4LogicalVolume( WallSolid, mVol_, nmWall );
  G4LogicalVolume *SegLV =
    new G4LogicalVolume( SegSolid, mVol_, nmSeg );

  G4VPhysicalVolume *Wall = 
    new G4PVPlacement( &rotMtx_, gPos_, elemName_, WallLV,
		       pMother, false, Id_ );

  //Placement Start 
  double Distance = 5.*cm;
  double whole_l =117.;
  G4VPhysicalVolume *Segment[29];
  for( G4int i=0; i<29; ++i ){
    if(i==0){
      G4double x = ( whole_l/2-2.5 )*cm;
      G4ThreeVector posSeg( x, 0., -Distance/2. );
      Segment[i] = 
	new G4PVPlacement( 0, posSeg, elemName_, SegLV,
			   Wall, false, i );
    }
    else if(i==1){
      G4double x = ( whole_l/2-4.-2.5 )*cm;
      G4ThreeVector posSeg( x, 0., Distance/2. );
      Segment[i] = 
	new G4PVPlacement( 0, posSeg, elemName_, SegLV,
			   Wall, false, i );
    }
    else if(!(i%2)){
      G4double x = ( whole_l/2-4.*i-2.5 )*cm;
      G4ThreeVector posSeg( x, 0., -Distance/2. );
      Segment[i] = 
	new G4PVPlacement( 0, posSeg, elemName_, SegLV,
			   Wall, false, i ); 
    }
    else if((i%2)){
      G4double x = ( whole_l/2-4.*i-2.5 )*cm;
      G4ThreeVector posSeg( x, 0., Distance/2. );    
      Segment[i] = 
	new G4PVPlacement( 0, posSeg, elemName_, SegLV,
			   Wall, false, i );
    }    
  }
  
  WallLV-> SetVisAttributes( G4VisAttributes::Invisible );
  G4VisAttributes *VisAtt = new G4VisAttributes();
    //new G4VisAttributes( G4Colour::G4Colour(0.,1.,1.) );
  VisAtt->SetColour(0.0,1.0,1.0);
  SegLV-> SetVisAttributes( VisAtt );
  
  DetectorLV_ = SegLV;
  vSolid = WallSolid;
  return Wall;
}
