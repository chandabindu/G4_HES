/*
  EDC1.cc

  D.Kawama
*/

#include "EDC1.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4RotationMatrix.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

///////////////////////////////////////////////////////////
EDC1::EDC1( const G4String &name,
			    const G4ThreeVector &gPos,
			    const G4RotationMatrix *rotMtx,
			    int id )
  : BeamLineElement( name, gPos, rotMtx ),
    mBox_(0), mGas_(0), mMylar_(0), mLayer_(0),
    mAnode_(0), mCathode_(0), mShield_(0),
    DetectorLV_(0), vSolid_(0)
//////////////////////////////////////////////////////////
{
  Id_ = id;
}

/////////////////////////////
EDC1::~EDC1()
/////////////////////////////
{
}

//////////////////////////////////////////////////////////////
bool EDC1::IsInside( const G4ThreeVector &gPos ) const
//////////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );
  if( vSolid_->Inside(lPos) != kOutside ) return true;
  else return false;
}

////////////////////////////////////////////////////////////////////
G4VPhysicalVolume *EDC1::Place( G4VPhysicalVolume *pMother )
///////////////////////////////////////////////////////////////////
{
  G4String nmBox = "EDC1Box";
  G4String nmMylar = "EDC1Mylar";
  G4String nmCu = "Cu";
  G4String nmGas = "EDC1Gas";
  G4String nmLayer = "EDC1Layer";
  G4String nmAnode = "EDC1Anode";
  G4String nmCathode = "EDC1Cathode";
  G4String nmShield = "EDC1Shield";


  G4ThreeVector dxyz_Box( 200.*cm, 30.0*cm, 40.0*cm );//Larger than real Box
  G4ThreeVector dxyz_Mylar(100.*cm, 12.0*cm, 0.00125*cm );
  G4ThreeVector dxyz_Gas( 100.*cm, 12.0*cm, 32.0*cm );
  G4ThreeVector dxyz_Layer( 100.*cm, 12.0*cm, 0.001*cm);

  G4Box *BoxSolid = new G4Box( nmBox, dxyz_Box.x()/2,
			       dxyz_Box.y()/2, dxyz_Box.z()/2 );
  G4Box *MylarSolid = new G4Box( nmMylar, dxyz_Mylar.x()/2,
				dxyz_Mylar.y()/2, dxyz_Mylar.z()/2 );
  G4Box *GasSolid = new G4Box( nmGas, dxyz_Gas.x()/2,
			       dxyz_Gas.y()/2, dxyz_Gas.z()/2 );
  G4Box *LayerSolid = new G4Box( nmLayer, dxyz_Layer.x()/2,
				 dxyz_Layer.y()/2, dxyz_Layer.z()/2 );
  G4Tubs *AnodeSolid = new G4Tubs( nmAnode, 0.*cm, 20e-6/2.*m, 
  					20./2.*cm, 0.*degree, 360.*degree);
  G4Tubs *CathodeSolid = new G4Tubs( nmCathode, 0.*cm, 100e-6/2.*m, 
  					20./2.*cm, 0.*degree, 360.*degree);
  G4Tubs *ShieldSolid = new G4Tubs( nmShield, 0.*cm, 100e-6/2.*m, 
  					100./2.*cm, 0.*degree, 360.*degree);


  G4LogicalVolume *BoxLV =
    new G4LogicalVolume( BoxSolid, mBox_, "EDC1" );
  G4LogicalVolume *MylarLV = 
    new G4LogicalVolume( MylarSolid, mMylar_, nmMylar );
  G4LogicalVolume *GasLV =
    new G4LogicalVolume( GasSolid, mGas_, nmGas );
  G4LogicalVolume *LayerLV =
    new G4LogicalVolume( LayerSolid, mLayer_, nmGas );
  G4LogicalVolume *AnodeLV =
    new G4LogicalVolume( AnodeSolid, mAnode_, nmAnode );
  G4LogicalVolume *CathodeLV =
    new G4LogicalVolume( CathodeSolid, mCathode_, nmCathode );
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

  G4VPhysicalVolume *DCLayer[11];
  G4VPhysicalVolume *DCMylar[2];
  G4double z;
//   G4int ID = Id_+1;
  G4RotationMatrix *Rot = new G4RotationMatrix();
  G4RotationMatrix *RotX = new G4RotationMatrix();
  RotX->rotateX(90.*degree);
  G4RotationMatrix *RotU = new G4RotationMatrix();
  RotU->rotateX(90.*degree);
  RotU->rotateY(30.*degree);
  G4RotationMatrix *RotV = new G4RotationMatrix();
  RotV->rotateX(90.*degree);
  RotV->rotateY(-30.*degree);
  G4RotationMatrix *RotS = new G4RotationMatrix();
  RotS->rotateY(90.*degree);

  // DC-Layer
  for( G4int i=0; i<5; i++ ){
    G4double z1 = ( -15. + 7.5*i -0.375 )*cm;
    G4double z2 = ( -15. + 7.5*i +0.375 )*cm;
    G4int j = 2*i;
    G4int k = 2*i+1;
	 G4double dx=0.;

    G4ThreeVector posLayer1( 0.*cm, 0.*cm, z1 );
    DCLayer[j] = new G4PVPlacement( 0, posLayer1, "EDC1",
				    LayerLV, DCGas, false, j );
    G4ThreeVector posLayer2( 0.*cm, 0.*cm, z2 );
    DCLayer[k] = new G4PVPlacement( 0, posLayer2, "EDC1",
				    LayerLV, DCGas, false, k );
	 if (i==1){
		Rot=RotU;
		dx=1.;
	 }
	 else if (i==3){
		Rot=RotV;
		dx=1.;
	 }
	 else {
		Rot=RotX;
		dx=0.86603;
	 }

	 for( G4int l=0; l<115; l++){
    	G4double x1=-dx*57.5+dx*l;
    	G4double x2=-dx*57.5+dx*(l+0.5);
		G4ThreeVector posAnode1( x1*cm, 0.*cm, z1-0.01*cm );
    	G4ThreeVector posAnode2( x2*cm, 0.*cm, z2-0.01*cm );
    	G4ThreeVector posCathode1( x1*cm, 0.*cm, z1-0.5*cm );
    	G4ThreeVector posCathode2( x1*cm, 0.*cm, z1+0.5*cm );
    	G4ThreeVector posCathode3( x1*cm, 0.*cm, z1+1.0*cm );
    	G4ThreeVector posCathode4( x2*cm, 0.*cm, z2-1.0*cm );
    	G4ThreeVector posCathode5( x2*cm, 0.*cm, z2-0.5*cm );
    	G4ThreeVector posCathode6( x2*cm, 0.*cm, z2+0.5*cm );
    	new G4PVPlacement( Rot, posAnode1, "EDC1",
				    AnodeLV, DCBox, false, 10000+1000*i+l );
    	new G4PVPlacement( Rot, posAnode2, "EDC1",
				    AnodeLV, DCBox, false, 20000+1000*i+l );
    	new G4PVPlacement( Rot, posCathode1, "EDC1",
				    CathodeLV, DCBox, false, 30000+1000*i+l );
    	new G4PVPlacement( Rot, posCathode2, "EDC1",
				    CathodeLV, DCBox, false, 40000+1000*i+l );
    	new G4PVPlacement( Rot, posCathode3, "EDC1",
				    CathodeLV, DCBox, false, 50000+1000*i+l );
    	new G4PVPlacement( Rot, posCathode4, "EDC1",
				    CathodeLV, DCBox, false, 60000+1000*i+l );
    	new G4PVPlacement( Rot, posCathode5, "EDC1",
				    CathodeLV, DCBox, false, 70000+1000*i+l );
    	new G4PVPlacement( Rot, posCathode6, "EDC1",
				    CathodeLV, DCBox, false, 80000+1000*i+l );
	 }
  }
  //11th layer (virtual) for FP
  G4ThreeVector posLayer3( 0.*cm, 0.*cm, 0.*cm );
  DCLayer[10] = new G4PVPlacement( 0, posLayer3, "EDC1",
			    LayerLV, DCGas, false, 10 );

  for( G4int i=0; i<6; i++ ){
   z = -18.75 + 7.5*i ;
	 for(G4int j=0; j<27; j++){
		G4double y=-13.5/2.+0.5*j;
		G4ThreeVector posShield( 0.*cm, y*cm, z*cm );
    	new G4PVPlacement( RotS, posShield, "EDC1",
				    ShieldLV, DCBox, false, 90000+1000*i+j );
	 }
  }
  // DC-Mylar
  z = -32./2*cm;
  DCMylar[0] = new G4PVPlacement( 0, 
				  G4ThreeVector( 0.*cm, 0.*cm, z ),
				  nmMylar, MylarLV, DCGas, false, 0 );
  z = 32./2*cm;
  DCMylar[1] = new G4PVPlacement( 0,
				  G4ThreeVector( 0.*cm, 0.*cm, z ),
				  nmMylar, MylarLV, DCGas, false, 9 );

  G4VisAttributes *DCVis = new G4VisAttributes(G4Color(0,0,0));
  BoxLV-> SetVisAttributes( DCVis );
  GasLV-> SetVisAttributes( G4VisAttributes::Invisible );
  MylarLV-> SetVisAttributes( G4VisAttributes::Invisible );
  LayerLV-> SetVisAttributes( G4VisAttributes::Invisible );

  DetectorLV_ = LayerLV;
  vSolid_ = BoxSolid;
  return DCBox;
}
