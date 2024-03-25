/*
  Q2Magnet.cc

  D.Kawama
*/

#include "Q2Magnet.hh"
#include "Q2FieldMan.hh"

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "HESParamMan.hh"
#include <cmath>


/////////////////////////////////////////////////////
Q2Magnet::Q2Magnet( const G4String &name,
		    const G4ThreeVector &gPos,
		    const G4RotationMatrix *rotMtx,
		    const std::string deffilename)
  : BeamLineElement( name, gPos, rotMtx ),
    solidField(0), yokeLog_(0),gapLog_(0), 
    fMan(deffilename)
/////////////////////////////////////////////////////
{
  fMan.Initialize();
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  MapID = paramMan->GetQ2MapID();
  Scale = paramMan->GetQ2ScaleFactor();
}

/////////////////////
Q2Magnet::~Q2Magnet()
/////////////////////
{
}

////////////////////////////////////////////////
bool Q2Magnet::ExistMagneticField( void ) const
////////////////////////////////////////////////
{
  return true;
}

///////////////////////////////////////////////////////////
bool Q2Magnet::IsInside( const G4ThreeVector &gPos ) const
///////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition(gPos);
//  if( solidField->Inside(lPos)!=kOutside ){
    if( lPos.x()>-35.*cm && lPos.x()<35.*cm &&
        lPos.y()>-50.*cm && lPos.y()<50.*cm &&
        lPos.z()>-50.*cm && lPos.z()<50.*cm 
    )
  {  
    return true;
  }
  else return false;
}

////////////////////////////////////////////////////
G4ThreeVector Q2Magnet::
GetMagneticField( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////
{
  if( IsInside(gPos) ){
    G4ThreeVector lPos = GetLocalPosition( gPos );
    return GetGlobalDirection( Field(lPos) );
    //return GetGlobalDirection( KatoEq(lPos) );
  }
  else return G4ThreeVector();
}


/////////////////////////////////////////////////////////////////
G4ThreeVector Q2Magnet::KatoEq( const G4ThreeVector &pos ) const
/////////////////////////////////////////////////////////////////
{
  G4double G = 25.*cm; // Q2 Bore diameter
  G4double x, y, z;
  x = pos.x(); y = pos.y(); z = pos.z();
  G4double Q2L = 50.*cm; // Q2 Length
  G4double s = (fabs(z)-Q2L/2)/G;
  G4double par[4];
  par[0] = -0.9842;
  par[1] = 6.3375;
  par[2] = -3.5134;
  par[3] = 0.9895;
  G4double ds[3];
  ds[0] = par[0] + par[1]*s +   par[2]*s*s +   par[3]*s*s*s;
  ds[1] =        + par[1]   + 2*par[2]*s   + 3*par[3]*s*s;
  ds[2] =                     2*par[2]     + 6*par[3]*s;
  G4double h = 1/(1+exp(ds[0]));
  G4double dh[2];
  dh[0] = -ds[1]*exp(ds[0])*h*h;
  dh[1] = 
    (-ds[2]-ds[1]*ds[1])*exp(ds[0])*h*h+2*pow((ds[1]*exp(ds[0])),2)*h*h*h;

  G4double Bq1 = -0.576*1000*Scale*gauss/cm; // Q2 Field Gradient
  G4double Bfield[3];
  //Bfield[0] = Bq1*( y*h -y*(y*y+3*x*y)/(12*G*G)*dh[1]);
  Bfield[0] = Bq1*( y*h -y*(y*y+3*x*x)/(12*G*G)*dh[1]);
  Bfield[1] = Bq1*( x*h -x*(x*x+3*y*y)/(12*G*G)*dh[1]);
  Bfield[2] = Bq1*x*y*dh[0]/G;
  G4ThreeVector B( Bfield[0], Bfield[1], Bfield[2] );

  return B;

}

////////////////////////////////////////////////////////////////
G4ThreeVector Q2Magnet::Field( const G4ThreeVector &pos ) const
////////////////////////////////////////////////////////////////
{

  double X[3];
  double Bfield[3];
  X[0] = pos.x()/cm;
  X[1] = pos.y()/cm;
  X[2] = pos.z()/cm;

  if( fMan.GetFieldValue(X, Bfield) ){
    Bfield[0] *= -Scale*gauss;
    Bfield[1] *= -Scale*gauss;
    Bfield[2] *= -Scale*gauss;
  }
  else{
    Bfield[0] = Bfield[1] = Bfield[2] = 0;
  }
  G4ThreeVector B( Bfield[0], Bfield[1], Bfield[2] );

  return B;
}

////////////////////////////////////////////////////////////////
G4VPhysicalVolume *Q2Magnet::Place( G4VPhysicalVolume *pMother )
////////////////////////////////////////////////////////////////
{
  G4ThreeVector dxyz_Q2Yoke( 72.*cm, 115.*cm, 50.*cm );
  G4Box* Q2box = new G4Box( "Q2 box",
			    dxyz_Q2Yoke.x()/2,
			    dxyz_Q2Yoke.y()/2,
			    dxyz_Q2Yoke.z()/2 );

  G4double Q2L = dxyz_Q2Yoke.z();
  G4double Box1x = 8.98*cm;
  G4double Box1y = 24.4*cm;
  G4double Trd1x = 7.23*cm;
  G4double Trd1y1 = Box1y;
  G4double Trd1y2  = 10.1*cm;
  G4double Trd2x = 14.0*cm;
  G4double Trd2y1 = Trd1y2;
  G4double Trd2y2  = 3.60*cm;
  G4double Box2x = 3.79*cm;
  G4double Box2y = Trd2y2;
  G4RotationMatrix *rotL = new G4RotationMatrix();
  rotL-> rotateY(-90.*degree);
  G4RotationMatrix *rotR = new G4RotationMatrix();
  rotR-> rotateY(90.*degree);
  
  G4Box *Q2GBox1 = new G4Box("Q2GBox1", Box1x/2.,Box1y/2.,Q2L/2.);

  G4Trd  *Q2GTrd1 = new G4Trd("Q2GTrd1", 
                          Q2L/2., Q2L/2.,
                          Trd1y1/2., Trd1y2/2., 
			  Trd1x/2.);
  G4DisplacedSolid *Q2GTrd1L = new G4DisplacedSolid(
                           "Q2GTrd1L", Q2GTrd1, rotL,
			   G4ThreeVector((Box1x+Trd1x)/2.,0.,0.));
  G4DisplacedSolid *Q2GTrd1R = new G4DisplacedSolid(
                           "Q2GTrd1R", Q2GTrd1, rotR,
			   G4ThreeVector(-(Box1x+Trd1x)/2.,0.,0.));
  G4Trd  *Q2GTrd2 = new G4Trd("Q2GTrd2", 
                          Q2L/2., Q2L/2.,
                          Trd2y1/2., Trd2y2/2., 
			  Trd2x/2.);
  G4DisplacedSolid *Q2GTrd2L = new G4DisplacedSolid(
                           "Q2GTrd2L", Q2GTrd2, rotL,
			   G4ThreeVector((Box1x+Trd1x+Trd2x)/2.,0.,0.));
  G4DisplacedSolid *Q2GTrd2R = new G4DisplacedSolid(
                           "Q2GTrd2R", Q2GTrd2, rotR,
			   G4ThreeVector(-(Box1x+Trd1x+Trd2x)/2.,0.,0.));
  
  G4Box *Q2GBox2 = new G4Box("Q2GBox2", Box2x/2.,Box2y/2.,Q2L/2.);
  G4DisplacedSolid *Q2GBox2L = new G4DisplacedSolid(
                           "Q2GBox2L", Q2GBox2, rotL,
			   G4ThreeVector((Box1x+Trd1x+Trd2x+Box2x)/2.,0.,0.));
  G4DisplacedSolid *Q2GBox2R = new G4DisplacedSolid(
                           "Q2GBox2R", Q2GBox2, rotR,
			   G4ThreeVector(-(Box1x+Trd1x+Trd2x+Box2x)/2.,0.,0.));

  G4SubtractionSolid* Q2Yoke1 =
    new G4SubtractionSolid( "Q2Yoke1", Q2box, Q2GBox1 );
  G4SubtractionSolid* Q2Yoke2 =
    new G4SubtractionSolid( "Q2Yoke2", Q2Yoke1, Q2GTrd1L );
  G4SubtractionSolid* Q2Yoke3 =
    new G4SubtractionSolid( "Q2Yoke3", Q2Yoke2, Q2GTrd1R );
  G4SubtractionSolid* Q2Yoke4 =
    new G4SubtractionSolid( "Q2Yoke4", Q2Yoke3, Q2GTrd2L );
  G4SubtractionSolid* Q2Yoke5 =
    new G4SubtractionSolid( "Q2Yoke5", Q2Yoke4, Q2GTrd2R );
  G4SubtractionSolid* Q2Yoke6 =
    new G4SubtractionSolid( "Q2Yoke6", Q2Yoke5, Q2GBox2L );
  G4SubtractionSolid* Q2Yoke7 =
    new G4SubtractionSolid( "Q2Yoke7", Q2Yoke6, Q2GBox2R );

  G4LogicalVolume* Q2YokeLV =
    new G4LogicalVolume( Q2Yoke7, mYoke_, "Q2Yoke LV");
  //G4LogicalVolume *Q2GapLV =
  //  new G4LogicalVolume( Q2GapSolid, mGap_, "Q2 Gap LV" );

  G4VPhysicalVolume *Q2Yoke = 
    new G4PVPlacement( &rotMtx_, gPos_, "Q2",
		       Q2YokeLV, pMother, false, 0);
  //G4VPhysicalVolume *Q2Gap = 
  //G4PVPlacement( &rotMtx_, gPos_, elemName_+" Gap",
	//	       Q2GapLV, pMother, false, 0);

  ///////////////////
  // Visualisation //
  ///////////////////
  G4Colour colourQ2Yoke(1., 0., 1.); // magenta
  G4VisAttributes* Q2YokeVisAtt = new G4VisAttributes(true, colourQ2Yoke);
  Q2YokeLV-> SetVisAttributes(Q2YokeVisAtt);
  //Q2GapLV-> SetVisAttributes(G4VisAttributes::Invisible);
  //Q2YokeLV-> SetVisAttributes(G4VisAttributes::Invisible);
  //Q2GapLV-> SetVisAttributes(Q2YokeVisAtt);

  solidField = new G4Box( "Q2 field", 50.*cm, 50.*cm, 50.*cm );


  //gapLog_ = Q2GapLV;
  yokeLog_ = Q2YokeLV;

  return Q2Yoke;
}


//////////////////////////////////////////////////////
void Q2Magnet::SetUserLimits( G4UserLimits *limitYoke,
			      G4UserLimits *limitGap )
//////////////////////////////////////////////////////			      
{
  if( yokeLog_ ) yokeLog_-> SetUserLimits(limitYoke);
  if( gapLog_ && limitGap )
    gapLog_-> SetUserLimits(limitGap);
}
