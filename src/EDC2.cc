/*
  EDC2.cc

  D.Kawama
*/

#include "EDC2.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

///////////////////////////////////////////////////////////
EDC2::EDC2( const G4String &name,
			    const G4ThreeVector &gPos,
			    const G4RotationMatrix *rotMtx,
			    int id )
  : BeamLineElement( name, gPos, rotMtx ),
    mBox_(0), mGas_(0), mMylar_(0), mLayer_(0), mAnode_(0), mShield_(0),
    DetectorLV_(0), vSolid_(0)
//////////////////////////////////////////////////////////
{
  Id_ = id;
}

/////////////////////////////
EDC2::~EDC2()
/////////////////////////////
{
}

//////////////////////////////////////////////////////////////
bool EDC2::IsInside( const G4ThreeVector &gPos ) const
//////////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( vSolid_->Inside(lPos) != kOutside ) return true;
  else return false;
}

////////////////////////////////////////////////////////////////////
G4VPhysicalVolume *EDC2::Place( G4VPhysicalVolume *pMother )
///////////////////////////////////////////////////////////////////
{
  G4String nmBox = "EDC2Box";
  G4String nmMylar = "EDC2Mylar";
  G4String nmCu = "Cu";
  G4String nmGas = "EDC2Gas";
  G4String nmLayer = "EDC2Layer";
  G4String nmAnode = "EDC2Anode";
  G4String nmShield = "EDC2Shield";


  G4ThreeVector dxyz_Box( 300.*cm, 60.0*cm, 5.13*cm );//Larger than real Box
  G4ThreeVector dxyz_Mylar(122.*cm, 30.0*cm, 0.00127*cm );
  G4ThreeVector dxyz_Gas( 122.*cm, 30.0*cm, 5.13*cm );
  G4ThreeVector dxyz_Layer( 122.*cm, 30.0*cm, 0.001*cm);

  G4Box *BoxSolid = new G4Box( nmBox, dxyz_Box.x()/2,
			       dxyz_Box.y()/2, dxyz_Box.z()/2 );
  G4Box *MylarSolid = new G4Box( nmMylar, dxyz_Mylar.x()/2,
				dxyz_Mylar.y()/2, dxyz_Mylar.z()/2 );
  G4Box *GasSolid = new G4Box( nmGas, dxyz_Gas.x()/2,
			       dxyz_Gas.y()/2, dxyz_Gas.z()/2 );
  G4Box *LayerSolid = new G4Box( nmLayer, dxyz_Layer.x()/2,
				 dxyz_Layer.y()/2, dxyz_Layer.z()/2 );
  G4Tubs *AnodeSolid = new G4Tubs( nmAnode, 0.*cm, 30e-6/2.*m,
				 60./2.*cm, 0.*degree, 360.*degree );
  G4Tubs *ShieldSolid = new G4Tubs( nmShield, 0.*cm, 60e-6/2.*m,
				 60./2.*cm, 0.*degree, 360.*degree );


  G4LogicalVolume *BoxLV =
    new G4LogicalVolume( BoxSolid, mBox_, "EDC2" );
  G4LogicalVolume *MylarLV = 
    new G4LogicalVolume( MylarSolid, mMylar_, nmMylar );
  G4LogicalVolume *GasLV =
    new G4LogicalVolume( GasSolid, mGas_, nmGas );
  G4LogicalVolume *LayerLV =
    new G4LogicalVolume( LayerSolid, mLayer_, nmGas );
  G4LogicalVolume *AnodeLV =
    new G4LogicalVolume( AnodeSolid, mAnode_, nmAnode );
  G4LogicalVolume *ShieldLV =
    new G4LogicalVolume( ShieldSolid, mShield_, nmShield );

  //G4VPhysicalVolume *DCBox = 
  DCBox = 
    new G4PVPlacement( &rotMtx_, gPos_, nmBox, BoxLV,
		       pMother, false, Id_ );
  //G4VPhysicalVolume *DCGas = 
  DCGas = 
    new G4PVPlacement( 0, G4ThreeVector(), nmGas, GasLV, 
		       DCBox, false, Id_ );

  G4VPhysicalVolume *DCLayer[6];
  G4VPhysicalVolume *DCMylar[10];
  G4double z=0;
  G4double dx=0;
  G4double x10=0;
  G4double x20=0;
  G4double x1=0.; 
  G4double x2=0.; 
//   G4int ID = Id_+1;
  G4RotationMatrix *Rot = new G4RotationMatrix();
  G4RotationMatrix *RotX = new G4RotationMatrix();
  RotX->rotateX(90.*degree);
  G4RotationMatrix *RotU = new G4RotationMatrix();
  RotU->rotateX(90.*degree);
  RotU->rotateY(60.*degree);
  G4RotationMatrix *RotV = new G4RotationMatrix();
  RotV->rotateX(90.*degree);
  RotV->rotateY(-60.*degree);
  G4RotationMatrix *RotS = new G4RotationMatrix();
  RotS->rotateY(90.*degree);

  // DC-Layer
  for( G4int i=0; i<3; i++ ){
    G4double z = ( -1.905 + 0.635*i )*cm;
    G4ThreeVector posLayer1( 0.*cm, 0.*cm, z );
    DCLayer[i] = new G4PVPlacement( 0, posLayer1, "EDC2Layer",
				    LayerLV, DCGas, false, i );
	 if (i==0){
		x10 = 61.5;
		x20 = 61.;
		dx = 2.;
		Rot=RotU;
	 }
	 else if (i==1){
		x10 = 61.;
		x20 = 61.5;
		dx = 2.;
		Rot=RotU;
	 }
	 else{
		x10 = 61.5;
		x20 = 61.;
		dx = 1.;
		Rot=RotX;
	 }
	 for (G4int j=0;j<122;j++){
		x1 = -dx*x10+dx*j;
		x2 = -dx*x20+dx*j;
	 	G4ThreeVector posWire1( x1*cm, 0.*cm, z-0.01*cm );
	 	G4ThreeVector posWire2( x2*cm, 0.*cm, z-0.01*cm );
    	new G4PVPlacement( Rot, posWire1, "EDC2Layer",
				    AnodeLV, DCBox, false, 10000+1000*i+j );
    	new G4PVPlacement( Rot, posWire2, "EDC2Layer",
				    ShieldLV, DCBox, false, 20000+1000*i+j );
	 }
  }
  for( G4int i=3; i<6; i++ ){
    G4double z = ( 0.3175 + 0.635*(i-3) )*cm;
    //G4double z = ( 0.635 + 0.635*(i-3) )*cm;
    G4ThreeVector posLayer2( 0.*cm, 0.*cm, z );
    DCLayer[i] = new G4PVPlacement( 0, posLayer2, "EDC2Layer",
				    LayerLV, DCGas, false, i );
	 if (i==3){
		x10 = 61.;
		x20 = 61.5;
		dx = 1.;
		Rot=RotX;
	 }
	 else if (i==4){
		x10 = 61.5;
		x20 = 61.;
		dx = 2.;
		Rot=RotV;
	 }
	 else{
		x10 = 61.;
		x20 = 61.5;
		dx = 2.;
		Rot=RotV;
	 }
	 for (G4int j=0;j<122;j++){
		x1 = -dx*x10+dx*j;
		x2 = -dx*x20+dx*j;
	 	G4ThreeVector posWire1( x1*cm, 0.*cm, z-0.01*cm );
	 	G4ThreeVector posWire2( x2*cm, 0.*cm, z-0.01*cm );
    	new G4PVPlacement( Rot, posWire1, "EDC2Layer",
				    AnodeLV, DCBox, false, 10000+1000*i+j );
    	new G4PVPlacement( Rot, posWire2, "EDC2Layer",
				    ShieldLV, DCBox, false, 20000+1000*i+j );
	 }
  }
  // DC-Mylar
  z = -2.54*cm;
  DCMylar[0] = new G4PVPlacement( 0, 
				  G4ThreeVector( 0.*cm, 0.*cm, z ),
				  nmMylar, MylarLV, DCGas, false, 0 );
  for (G4int i=1;i<4; i++){
    z = (-2.2225+0.635*(i-1))*cm;
    DCMylar[i] = new G4PVPlacement( 0, 
				  G4ThreeVector( 0.*cm, 0.*cm, z ),
				  nmMylar, MylarLV, DCGas, false, i );
  }
  for (G4int i=5;i<9; i++){
    z = (0.0+0.635*(i-5))*cm;
    DCMylar[i] = new G4PVPlacement( 0, 
				  G4ThreeVector( 0.*cm, 0.*cm, z ),
				  nmMylar, MylarLV, DCGas, false, i );
  }
  z = 2.54*cm;
  DCMylar[9] = new G4PVPlacement( 0,
				  G4ThreeVector( 0.*cm, 0.*cm, z ),
				  nmMylar, MylarLV, DCGas, false, 9 );

  G4VisAttributes *DCVis =new G4VisAttributes(G4Color(0,0,0));
  BoxLV-> SetVisAttributes( DCVis );
  GasLV-> SetVisAttributes( G4VisAttributes::Invisible );
  MylarLV-> SetVisAttributes( G4VisAttributes::Invisible );
  LayerLV-> SetVisAttributes( G4VisAttributes::Invisible );

  DetectorLV_ = LayerLV;
  vSolid_ = BoxSolid;
  return DCBox;
}
