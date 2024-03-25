/*
  SieveSlit.cc
  2004/Sep/21 H.Nomura
*/

#include "SieveSlit.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include <cmath>

//////////////////////////////////////////////////////
SieveSlit::SieveSlit( const G4String &name,
		      const G4ThreeVector &gPos,
		      const G4RotationMatrix *rotMtx )
  : BeamLineElement( name, gPos, rotMtx ),
    mBox_(0), mHole_(0)
//////////////////////////////////////////////////////
{
}

///////////////////////
SieveSlit::~SieveSlit()
///////////////////////
{
}

///////////////////////////////////////////////////////////
bool SieveSlit::IsInside( const G4ThreeVector &gPos ) const
///////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( vSolid_->Inside(lPos) != kOutside ) return true;
  else return false;
}

///////////////////////////////////////////////////////////////
G4VPhysicalVolume *SieveSlit::Place(G4VPhysicalVolume *pMother)
///////////////////////////////////////////////////////////////
{
  G4String nmHole = elemName_+" hole";
  const G4int nHole = 55; 
  const G4int cHole = 26; 

  G4ThreeVector dxyzBox(22.*cm, 10.*cm, 2.54/2.*cm);
  G4Box *SlitBox =
    new G4Box( "SS solid", dxyzBox.x(),
	       dxyzBox.y(), dxyzBox.z() );
  G4Tubs *HOLE = new G4Tubs( "hole", 0.*cm, 3./16.*2.54/2*cm,
			     3.*cm, 0.*degree, 360.*degree );
  G4Tubs *HOLE0 = new G4Tubs( "hole", 0.*cm, 3./16.*2.54/2.*cm,
			     3.*cm, 0.*degree, 360.*degree );
  G4Tubs *HOLE1 = new G4Tubs( "hole", 0.*cm, 0.25*2.54/2.*cm,
			     3.*cm, 0.*degree, 360.*degree );
  G4DisplacedSolid *holeSolid;
  G4SubtractionSolid *TmpSolid[nHole];
  G4double holeX[nHole]={
			 -9.525,-9.525,-9.525,
			 -7.620,-7.620,-7.620,
			 -5.715,-5.715,-5.715,-5.715,-5.715,
			 -3.810,-3.810,-3.810,-3.810,-3.810,-3.810,-3.810,
			 -1.905,-1.905,-1.905,-1.905,-1.905,-1.905,
			  0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
			  1.905, 1.905, 1.905, 1.905, 1.905, 1.905, 1.905, 
			  3.810, 3.810, 3.810, 3.810, 3.810, 3.810, 3.810,
			  5.715, 5.715, 5.715, 5.715, 5.715,
			  7.620, 7.620, 7.620,
			  9.525, 9.525, 9.525
			 };
	G4double holeY[nHole]={
			  2.54,0,-2.54,
			  2.54,0,-2.54,
			  5.08,2.54,0,-2.54,-5.08,
			  7.62,5.08,2.54,0,-2.54,-5.08,-7.62,
			  7.62,5.08,2.54,0,-5.08,-7.62,
			  7.62,5.08,0,-2.54,-5.08,-7.62,
			  7.62,5.08,2.54,0,-2.54,-5.08,-7.62,
			  7.62,5.08,2.54,0,-2.54,-5.08,-7.62,
			  5.08,2.54,0,-2.54,-5.08,
			  2.54,0,-2.54,
			  2.54,0,-2.54
			  };
  for( G4int i=0; i<nHole; i++ ){
    //if( i!=34 ){
      /*G4int Column = i%8;
      G4int Row = G4int( i/8 );
      G4double x =(2.0*(Column-4)+1.)*cm;
      G4double y =(2.0*(Row-4)+1.)*cm;*/
      G4double x =holeX[i]*cm;
      G4double y =holeY[i]*cm;
      //G4double theta = atan2( x/(d-dxyzBox.z()), 1. );
      //G4double phi = atan2( y/(d-dxyzBox.z()), 1. );
      G4double theta = 0.;
      G4double phi = 0.;
      G4RotationMatrix *rotH = new G4RotationMatrix();
      rotH->rotateY(theta);
      rotH->rotateX(phi);
      if (i==cHole){
			holeSolid = 
			new G4DisplacedSolid( nmHole+" solid", HOLE0, rotH,
		            			  G4ThreeVector(x,y,0.) );
		}
		else if (y/cm>2){
			holeSolid = 
			new G4DisplacedSolid( nmHole+" solid", HOLE1, rotH,
		            			  G4ThreeVector(x,y,0.) );
		}
		else{
			holeSolid = 
			new G4DisplacedSolid( nmHole+" solid", HOLE, rotH,
		            			  G4ThreeVector(x,y,0.) );
		}
      if (i==0){
        TmpSolid[i] = 
         new G4SubtractionSolid("TmpSolid", SlitBox, holeSolid);
      }
      else {
        TmpSolid[i] = 
         new G4SubtractionSolid("TmpSolid", TmpSolid[i-1], holeSolid);
      }
    /*}
    else{
      holeSolid[i] =
	new G4Tubs( nmHole+" solid", 0.*cm, 0.15875*cm, dxyzBox.z(),
		    0.*degree, 360.*degree );
    }*/
  }


  G4LogicalVolume *SlitLV =
    new G4LogicalVolume( TmpSolid[nHole-1], mBox_, "SSLV" );
  //  G4LogicalVolume *holeLV[70];
  G4VPhysicalVolume *Slit =
    new G4PVPlacement( &rotMtx_, gPos_, "SS",
		       SlitLV, pMother, false, 0 );

  ///////////////////
  // Visualisation //
  ///////////////////
  G4Colour colourSlit(0.5, 0.5, 0.5); // gray
  G4VisAttributes* SlitVisAtt = new G4VisAttributes(true, colourSlit);
  SlitLV-> SetVisAttributes(SlitVisAtt);
  //SlitLV-> SetVisAttributes(G4VisAttributes::Invisible);

  vSolid_ = SlitBox;
//   logDetector1_ = holeLV[0];
//   logDetector2_ = holeLV[1];

  return Slit;
}

//////////////////////////////////////////////////////////
void SieveSlit::SetSD( G4VSensitiveDetector *pSDetector )
//////////////////////////////////////////////////////////
{
  for( G4int i=0; i<70; i++ ){
    holeLV[i]->SetSensitiveDetector( pSDetector );
  }
}
