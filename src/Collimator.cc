/*
  Collimator.cc
  D.Kawama
*/

#include "Collimator.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

Collimator::Collimator( const G4String &name,
			const G4ThreeVector &gPos,
			const G4RotationMatrix *rotMtx,
			int id)
  : BeamLineElement(name,gPos,rotMtx),
    mBox_(0), mGap_(0), DetectorLV_(0), vSolid_(0)
{
	Id_ = id;
}

Collimator::~Collimator()
{
}

bool Collimator::IsInside( const G4ThreeVector &gPos ) const
{
  G4ThreeVector lPos = GetLocalPosition(gPos);
  if( vSolid_-> Inside(lPos) != kOutside ) return true;
  else return false;
}

G4VPhysicalVolume *Collimator::Place( G4VPhysicalVolume *pMother )
{
  G4ThreeVector dxyz_Col( 56.*cm, 60.*cm, 2.54*cm );
  G4Box* Colbox = new G4Box( "Col box",
			    dxyz_Col.x()/2,
			    dxyz_Col.y()/2,
			    dxyz_Col.z()/2 );
  
  G4double ColL = dxyz_Col.z()+1.*cm;
  G4double Box1x = 6.38*cm;
  G4double Box1y = 20.2*cm;
  G4double Trd1x = 5.98*cm;
  G4double Trd1y1 = Box1y;
  G4double Trd1y2  = 8.20*cm;
  G4double Trd2x = 11.0*cm;
  G4double Trd2y1 = Trd1y2;
  G4double Trd2y2  = 3.00*cm;
  G4double Box2x = 1.78*cm;
  G4double Box2y = Trd2y2;
  G4double Box3x = 30.*cm;
  G4double Box3y = 10.*cm;
  G4RotationMatrix *rotL = new G4RotationMatrix();
  rotL-> rotateY(-90.*degree);
  G4RotationMatrix *rotR = new G4RotationMatrix();
  rotR-> rotateY(90.*degree);
  
  G4Box *HoleBox1 = new G4Box("HoleBox1", Box1x/2.,Box1y/2.,ColL/2.);

  G4Trd  *HoleTrd1 = new G4Trd("HoleTrd1", 
                          ColL/2., ColL/2.,
                          Trd1y1/2., Trd1y2/2., Trd1x/2.);
  G4DisplacedSolid *HoleTrd1L = new G4DisplacedSolid(
                           "HoleTrd1L", HoleTrd1, rotL,
			   G4ThreeVector((Box1x+Trd1x)/2.,0.,0.));
  G4DisplacedSolid *HoleTrd1R = new G4DisplacedSolid(
                           "HoleTrd1R", HoleTrd1, rotR,
			   G4ThreeVector(-(Box1x+Trd1x)/2.,0.,0.));
  G4Trd  *HoleTrd2 = new G4Trd("HoleTrd2", 
                          ColL/2., ColL/2.,
                          Trd2y1/2., Trd2y2/2., Trd2x/2.);
  G4DisplacedSolid *HoleTrd2L = new G4DisplacedSolid(
                           "HoleTrd2L", HoleTrd2, rotL,
			   G4ThreeVector((Box1x+Trd1x+Trd2x)/2.,0.,0.));
  G4DisplacedSolid *HoleTrd2R = new G4DisplacedSolid(
                           "HoleTrd2R", HoleTrd2, rotR,
			   G4ThreeVector(-(Box1x+Trd1x+Trd2x)/2.,0.,0.));
  
  G4Box *HoleBox2 = new G4Box("HoleBox2", Box2x/2.,Box2y/2.,ColL/2.);
  G4DisplacedSolid *HoleBox2L = new G4DisplacedSolid(
                           "HoleBox2L", HoleBox2, 0,
			   G4ThreeVector((Box1x+Trd1x+Trd2x+Box2x)/2.,0.,0.));
  G4DisplacedSolid *HoleBox2R = new G4DisplacedSolid(
                           "HoleBox2R", HoleBox2, 0,
			   G4ThreeVector(-(Box1x+Trd1x+Trd2x+Box2x)/2.,0.,0.));
  
  G4Box *Box3 = new G4Box("Box3", Box3x/2.,Box3y/2.,(ColL-1.*cm)/2.);
  G4DisplacedSolid *HoleBox3 = new G4DisplacedSolid(
                           "HoleBox3", Box3, 0,
			   G4ThreeVector(0.,-(Box3y/2.+3.*cm),0.));

  G4SubtractionSolid* Col1 =
    new G4SubtractionSolid( "Col1", Colbox, HoleBox1 );
  G4SubtractionSolid* Col2 =
    new G4SubtractionSolid( "Col2", Col1, HoleTrd1L );
  G4SubtractionSolid* Col3 =
    new G4SubtractionSolid( "Col3", Col2, HoleTrd1R );
  G4SubtractionSolid* Col4 =
    new G4SubtractionSolid( "Col4", Col3, HoleTrd2L );
  G4SubtractionSolid* Col5 =
    new G4SubtractionSolid( "Col5", Col4, HoleTrd2R );
  G4SubtractionSolid* Col6 =
    new G4SubtractionSolid( "Col6", Col5, HoleBox2L );
  G4SubtractionSolid* Col7 =
    new G4SubtractionSolid( "Col7", Col6, HoleBox2R );
  G4UnionSolid* Col8 =
    new G4UnionSolid( "Col8", Col7, HoleBox3 );

  G4LogicalVolume* ColLV =
    new G4LogicalVolume( Col8, mBox_, "Col LV");

  G4VPhysicalVolume *Col = 
    new G4PVPlacement( &rotMtx_, gPos_, "Col",
		       ColLV, pMother, false, 0);

  ///////////////////
  // Visualisation //
  ///////////////////
  G4Colour colourCol(0.5, 0.5, 0.5); // magenta
  G4VisAttributes* ColVisAtt = new G4VisAttributes(true, colourCol);
  ColLV-> SetVisAttributes(ColVisAtt);


  //gapLog_ = HoleapLV;

  return Col;
}


