#include "HESDetectorConstruction.hh"
#include "HESGlobalSize.hh"

//=== Sensitive Detector ===//
#include "G4SDManager.hh"
#include "HESTargetSD.hh"
#include "HESVDSD.hh"
#include "EDC1SD.hh"
#include "EDC2SD.hh"
#include "HESEH1SD.hh"
#include "HESEH2SD.hh"

//=== Material Define ===//
#include "MaterialList.hh"

//=== Geometry Define ===//
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4AffineTransform.hh"

//=== Beam Line Element ===//
#include "Target.hh"
#include "PCS.hh"
#include "Q1Magnet.hh"
#include "Q2Magnet.hh"
#include "DMagnet.hh"
#include "VirtualDetector.hh"
#include "VDetectorPB.hh"
#include "VDetectorQ1.hh"
#include "VDetectorQ2.hh"
#include "EDC1.hh"
#include "EDC2.hh"
#include "EHodo1.hh"
#include "EHodo2.hh"
#include "Collimator.hh"
//#include "TargetChamber.hh"
#include "SieveSlit.hh"

//=== ElectoMagnetic Field ===//
#include "HESField.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>
#include <iomanip>
#include <string>

#include "HESParamMan.hh"
//#include "HESPrimaryGeneratorAction.hh"

using namespace std;

//=== Program Start ===//
///////////////////////////////////////////////////
HESDetectorConstruction::HESDetectorConstruction()
  : mList_(0), EMField_(0)
///////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////
HESDetectorConstruction::~HESDetectorConstruction()
///////////////////////////////////////////////////
{
  delete mList_;
  delete EMField_;
}

/////////////////////////////////////////////////////////
MaterialList *HESDetectorConstruction::DefineMaterials()
/////////////////////////////////////////////////////////
{
  MaterialList *ml = new MaterialList;

  return ml;
}

////////////////////////////////////////////////////////
G4VPhysicalVolume *HESDetectorConstruction::Construct()
///////////////////////////////////////////////////////
{
  mList_ = DefineMaterials();

  G4VPhysicalVolume *world = ConstructPayload();

  return world;
}

///////////////////////////////////////////////////////
HESField *HESDetectorConstruction::MakeDetectorField()
{
  return new HESField();
}

///////////////////////////////////////////////////////////////
G4VPhysicalVolume *HESDetectorConstruction::ConstructPayload()
//////////////////////////////////////////////////////////////
{ 
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  G4SDManager *SDManager = G4SDManager::GetSDMpointer();

  int flag    = paramMan->GetEventFlag();
  int EDCFlag = paramMan->GetEDCFlag();
  int SSFlag  = paramMan->GetSSFlag();

  //G4double Tilt   = paramMan->GetTiltAngle()*degree;      // Tilt angle
  G4double Theta  = paramMan->GetHesThetaAngle()*degree;  // HES opening angle with e- beam axis
  G4double Phi    = paramMan->GetHesPhiAngle()*degree;    // HES rotation with HES optical axis
  G4double PBend  = -6.13 * degree;     // PCS Bending Angle
  // G4double PTheta = -0. * degree;       // PCS Setting Angle
  // G4double PTheta = -2.0* degree;       // PCS Setting Angle
  G4double PTheta = paramMan->GetPTheta() *degree;       // PCS Setting Angle  //Added by ishige 2022Sep28


  G4double DBend  = 50.0 * degree;      // HES Dipole Bending Angle
  G4double Q1L    = 60. * cm;           // HES-Q1 Pole Length
  G4double Q2L    = 50. * cm;           // HES-Q2 Pole Length

  if( !EMField_ ) EMField_ = MakeDetectorField();
  else            EMField_->cleanupElemList();

  //=== World ===//
  G4Box *worldSolid = new G4Box( "World",WorldSizeX,WorldSizeY,WorldSizeZ );
  G4LogicalVolume *worldLV = new G4LogicalVolume( worldSolid, mList_->Vacuum, "World LV");
  G4VPhysicalVolume *world = new G4PVPlacement( 0, G4ThreeVector( 0.*cm, 0.*cm, 0.*cm ), worldLV, "World", 0, false, 0);

  //=== Target ===//
  G4double TargetThickness = paramMan->GetTargetThickness();
  string TargetMaterial = paramMan->GetTargetMaterial();
  G4Material *Target = mList_->Vacuum;
  
  if( TargetMaterial == "Vanadium"){
    Target = mList_->V51;
  } else if( TargetMaterial == "Yttrium"){
    Target = mList_->Y89;
  } else if( TargetMaterial == "Lead"){
    Target = mList_->Pb208;
  } else if( TargetMaterial == "Carbon"){
    Target = mList_->C12;
  } else if( TargetMaterial == "Silicon"){
    Target = mList_->Si28;
  } else if( TargetMaterial == "Polyethylene"){
    Target = mList_->CH2;
  } else if( TargetMaterial == "Lithium"){
    Target = mList_->Li7;
  } else if( TargetMaterial == "Beryllium"){
    Target = mList_->Be9;
  } else if( TargetMaterial == "Boron"){
    Target = mList_->B10;
  } else if( TargetMaterial == "Calcium"){
    Target = mList_->Ca40;
  } else if( TargetMaterial == "Water"){
    Target = mList_->Water;
  } else if( TargetMaterial == "Cromium"){
    Target = mList_->Cr52;
  } else if( TargetMaterial == "Vacuum"){
    Target = mList_->Vacuum;
  } else {
  G4cout << "You choose strange target '" <<TargetMaterial <<"' !! I don't know such a target..." <<G4endl;
  G4cout << "I decided to use no target!" <<G4endl;
  Target = mList_->Vacuum;
  }

  G4double TargetDensity = Target->GetDensity();
  G4double TargetSizeX = 1.*cm;
  G4double TargetSizeY = 1.*cm;
  G4double TargetSizeZ = (TargetThickness)/(TargetDensity/(g/cm3)*1000)*cm;
  G4RotationMatrix* rotTarget = new G4RotationMatrix();
  rotTarget->rotateY(0.0 * deg);
  G4Box *targetSolid = new G4Box( "Target",TargetSizeX/2,TargetSizeY/2,TargetSizeZ/2 );
  G4LogicalVolume *targetLV = new G4LogicalVolume( targetSolid, Target, "Target LV");
  
  if (TargetMaterial != "Vacuum"){
    new G4PVPlacement( rotTarget, G4ThreeVector(0*cm, 0*cm, 0*cm), targetLV, "Target" , worldLV , false, 0);
  }
  
  if( flag==0 ){ 
// Coordinate transformation (Spectrometer Coordinates)
    G4RotationMatrix *rotSpec = new G4RotationMatrix();
    rotSpec->rotateY(Theta);
    G4AffineTransform *SpecTrans = new G4AffineTransform( rotSpec, G4ThreeVector(0,0,0) );

// PCS Magnet
    string PCSmap = paramMan->GetPCSFieldMap();
    // G4double TtoPCSi = 80. * cm;// temporaly
    // G4double TtoPCSi = 160. * cm;//Latest Setup 2023May11
    G4double TtoPCSi = 120. * cm;//Latest Setup
    // G4double TtoPCSi = paramMan->GetT2P() * cm; // Added by ishige 2022Oct13
    G4ThreeVector posPCSi = SpecTrans->TransformPoint( G4ThreeVector(0.*cm, 0.*cm, TtoPCSi) );
    G4RotationMatrix *rotPCSi = new G4RotationMatrix();
    rotPCSi->rotateY(Theta);
    rotPCSi->rotateY(PTheta);
    PCS *Pcs = new PCS( "PCS", posPCSi, rotPCSi, PCSmap);
    Pcs->SetMaterials(mList_->Fe);
    Pcs->Place( world );
    EMField_->AddElement( Pcs );

// VD of PCS entrance
    G4double TtoPCSVDi = TtoPCSi - 1.*cm;
    G4ThreeVector posPCSVDi = SpecTrans->TransformPoint( G4ThreeVector(0.*cm, 0.*cm, TtoPCSVDi) );
    VirtualDetector *PCSVDi = new VirtualDetector( "PCSVDi", posPCSVDi, rotPCSi, 100.*cm, 100.*cm, 0.01*cm, 2 );
    PCSVDi->SetMaterials( mList_->Vacuum );
    PCSVDi->Place( world );

// Coordinate transformation (PCSe coordinates)
    G4AffineTransform *PCSTrans = new G4AffineTransform( rotPCSi, posPCSi );
    G4double PArm = 769. * cm;
    G4ThreeVector posPBend = PCSTrans->TransformPoint( G4ThreeVector( PArm, 0.*cm, 0. ) );
    G4RotationMatrix *rotPCSe = new G4RotationMatrix();
    rotPCSe->rotateY(Theta);
    rotPCSe->rotateY(PTheta);
    rotPCSe->rotateY(PBend);
    G4AffineTransform *PCSeTrans = new G4AffineTransform( rotPCSe, posPBend );
    
// VD of PCS exit
    G4double PCSetoPCSVDe = 1.*cm;
    G4ThreeVector posPCSVDe = PCSeTrans->TransformPoint( G4ThreeVector(-PArm, 0.*cm, PCSetoPCSVDe) );
    VirtualDetector *PCSVDe = new VirtualDetector( "PCSVDe", posPCSVDe, rotPCSe, 100.*cm, 100.*cm, 0.01*cm, 3 );
    PCSVDe->SetMaterials( mList_->Vacuum );
    PCSVDe->Place( world );

// Coordinate transformation (HESQ1 coordinates)
    G4ThreeVector posQBend = PCSeTrans->TransformPoint( G4ThreeVector( -PArm + 1.5*cm, 0.*cm, 0.*cm ) );
    // G4ThreeVector posQBend = PCSeTrans->TransformPoint( G4ThreeVector( -PArm + 0.5*cm, 0.*cm, 0.*cm ) ); //Added by ishige 2022Sep30
    G4RotationMatrix *rotQBend = new G4RotationMatrix();
    rotQBend->rotateY(Theta);
    rotQBend->rotateY(PTheta);
    rotQBend->rotateY(PBend);
    rotQBend->rotateZ(Phi);
    G4AffineTransform *HESQTrans = new G4AffineTransform( rotQBend, posQBend );

// Collimator & Sieve slit
    G4double TtoSS = TtoPCSi - 30.0 * cm;
    G4ThreeVector posSS = SpecTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, TtoSS ) );
    SieveSlit *Slit  = new SieveSlit( "Sieve Slit", posSS, rotSpec );
    Collimator *Coll = new Collimator( "Collimator" , posSS, rotSpec, 0);
    if( SSFlag == 1 ){ 
      Slit->SetMaterials( mList_->Heavymet, mList_->Vacuum );
      Slit->Place( world );
    } else if( SSFlag == 2 ) {
      Coll->SetMaterials( mList_->Heavymet, mList_->Vacuum );
      Coll->Place( world );
    }

// VD of Sieve entrance
    G4double TtoSSVDi = TtoSS - 3.*cm;
    G4ThreeVector posSSVDi = SpecTrans->TransformPoint( G4ThreeVector(0.*cm, 0.*cm, TtoSSVDi) );
    VirtualDetector *SSVDi = new VirtualDetector( "SSVDi", posSSVDi, rotSpec, 50.*cm, 50.*cm, 0.01*cm, 0 );
    SSVDi->SetMaterials( mList_->Vacuum );
    SSVDi->Place( world );

// VD of Sieve exit 
    G4double TtoSSVDe = TtoSS + 3.*cm;
    G4ThreeVector posSSVDe = SpecTrans->TransformPoint( G4ThreeVector(0.*cm, 0.*cm, TtoSSVDe) );
    VirtualDetector *SSVDe = new VirtualDetector( "SSVDe", posSSVDe, rotSpec, 50.*cm, 50.*cm, 0.01*cm, 1 );
    SSVDe->SetMaterials( mList_->Vacuum );
    SSVDe->Place( world );

// Quadrupole 1
    string Q1map = paramMan->GetQ1FieldMap();
    G4double PCSetoQ1 = (30. + 16.7)*cm + Q1L/2.;
    G4ThreeVector posQ1 = HESQTrans->TransformPoint( G4ThreeVector(0.*cm, 0.*cm, PCSetoQ1 ) );
    G4RotationMatrix *rotQ1 = new G4RotationMatrix();
    rotQ1 = rotQBend;
    Q1Magnet *Q1 = new Q1Magnet( "Q1", posQ1, rotQ1, Q1map );
    Q1->SetMaterials( mList_->Fe, mList_->Vacuum );
    Q1->Place( world );
    EMField_->AddElement( Q1 );

// VD of Q1 Entrance
    G4double PCSetoQ1VDi = PCSetoQ1 - (Q1L/2. + 1.*cm);
    G4ThreeVector posQ1VDi = HESQTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, PCSetoQ1VDi ) );
    VirtualDetector *Q1VDi = new VirtualDetector( "Q1VDi", posQ1VDi, rotQ1, 100.*cm, 50.*cm, 0.01*cm, 4 );
    Q1VDi->SetMaterials( mList_->Vacuum );
    Q1VDi->Place( world );
    
// VD of Q1 Exit
    G4double PCSetoQ1VDe = PCSetoQ1 + (Q1L/2. + 1.*cm);
    G4ThreeVector posQ1VDe = HESQTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, PCSetoQ1VDe ) );
    VirtualDetector *Q1VDe = new VirtualDetector( "Q1VDe", posQ1VDe, rotQ1, 100.*cm, 50.*cm, 0.01*cm, 5 );
    Q1VDe->SetMaterials( mList_->Vacuum );
    Q1VDe->Place( world );
    
// Quadrupole 2
    string Q2map = paramMan->GetQ2FieldMap();
    G4double PCSetoQ2 = PCSetoQ1 + Q1L/2. + 30.*cm + Q2L/2.;
    G4ThreeVector posQ2 = HESQTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, PCSetoQ2 ) );
    G4RotationMatrix *rotQ2 = new G4RotationMatrix();
    rotQ2 = rotQBend;
    Q2Magnet *Q2 = new Q2Magnet( "Q2", posQ2, rotQ2, Q2map );
    Q2->SetMaterials( mList_->Fe, mList_->Vacuum );
    Q2->Place( world );
    EMField_->AddElement( Q2 );
    
// VD of Q2 Entrance
    G4double PCSetoQ2VDi = PCSetoQ2 - (Q2L/2. + 1.*cm);
    G4ThreeVector posPCSQ1i = HESQTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, PCSetoQ2VDi ) );
    VirtualDetector *Q2VDi = new VirtualDetector( "Q1VDi", posPCSQ1i, rotQBend, 100.*cm, 50.*cm, 0.01*cm, 6 );
    Q2VDi->SetMaterials( mList_->Vacuum );
    Q2VDi->Place( world );
    
// VD of Q2 Exit
    G4double PCSetoQ2VDe = PCSetoQ2 + (Q2L/2. + 1.*cm);
    G4ThreeVector posVD4 = HESQTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, PCSetoQ2VDe ) );
    VirtualDetector *Q2VDe = new VirtualDetector( "Q2VDe", posVD4, rotQBend, 100.*cm, 50.*cm, 0.01*cm, 7 );
    Q2VDe->SetMaterials( mList_->Vacuum );
    Q2VDe->Place( world );
    
// Dipole Magnet
    G4double PCSetoDi = PCSetoQ2 + 105.*cm;
    G4double DArm = 220.0 * cm;
    G4ThreeVector preDen( DArm, 0., PCSetoDi  );
    const G4ThreeVector posD = HESQTrans->TransformPoint( G4ThreeVector( DArm, 0., PCSetoDi ) );
    string Dmap = paramMan->GetDFieldMap();
    G4RotationMatrix *rotDmag = new G4RotationMatrix();
    rotDmag->rotateY(Theta);
    rotDmag->rotateY(PBend);
    rotDmag->rotateY(PTheta);
    rotDmag->rotateZ(180*degree+Phi);
    rotDmag->rotateY(DBend/2.);
    DMagnet *Dipole = new DMagnet("D", posD, rotDmag ,Dmap );
    Dipole->SetMaterials( mList_->Fe, mList_->Vacuum );
    Dipole->Place( world );
    EMField_->AddElement( Dipole );
    
// VD of Dipole Entrance
    G4ThreeVector posDVDi = HESQTrans->TransformPoint( G4ThreeVector( 0.*cm, 0.*cm, PCSetoDi - 1.*cm ) );
    VirtualDetector *DVDi = new VirtualDetector( "DVDi", posDVDi, rotQBend, 250.*cm, 100.*cm, 0.01*cm, 8 );
    DVDi->SetMaterials( mList_->Vacuum );
    DVDi->Place( world );
    
// Virtual Detector for Dipole exit
    G4ThreeVector posDBend = HESQTrans->TransformPoint( G4ThreeVector( DArm, 0.*cm, PCSetoDi ) );
    G4RotationMatrix *rotDe = new G4RotationMatrix();
    rotDe->rotateY(Theta);
    rotDe->rotateY(PTheta);
    rotDe->rotateY(PBend);
    rotDe->rotateZ(Phi);
    rotDe->rotateY(-DBend);
    G4AffineTransform *HESDeTrans = new G4AffineTransform( rotDe, posDBend );

// VD of Dipole Exit
    G4double DetoDVDe = 1. * cm;
    G4ThreeVector posDVDe = HESDeTrans->TransformPoint( G4ThreeVector( -DArm,0,DetoDVDe) );
    VirtualDetector *DVDe = new VirtualDetector( "DVDe", posDVDe, rotDe, 250.*cm, 100.*cm, 0.01*cm, 9 );
    DVDe->SetMaterials( mList_->Vacuum );
    DVDe->Place( world );
    
    G4ThreeVector posDet = HESDeTrans->TransformPoint( G4ThreeVector( -DArm, 0.*cm, 0.*cm ) );
    G4RotationMatrix *rotDet = new G4RotationMatrix(); // Rotation for HES Detectors
    rotDet->rotateY(Theta);
    rotDet->rotateY(PBend);
    rotDet->rotateY(PTheta);
    rotDet->rotateZ(Phi);
    rotDet->rotateY(-DBend);
    G4AffineTransform *DetTrans = new G4AffineTransform( rotDet, posDet );

    if (EDCFlag!=0){

// Window 1 //
      G4double VD6toW1z = 107.*cm;
      G4ThreeVector VD6toW1( 0.*cm, 0.*cm,  VD6toW1z);
      G4ThreeVector preposW1 = DetTrans->TransformPoint(VD6toW1);
      G4ThreeVector posW1 = preposW1;
      G4Box *W1Solid = new G4Box( "Window1",105./2.*cm,17.5/2.*cm,0.02/2.*cm );
      G4LogicalVolume *W1LV = new G4LogicalVolume( W1Solid, mList_->Kevlar, "Window1LV");
      new G4PVPlacement( rotDet, posW1, W1LV, "Window1" , worldLV , false, 0);

// Window 2 //
      G4double VD6toW2z = (107.+0.02+0.01/2.)*cm;
      G4ThreeVector VD6toW2( 0.*cm, 0.*cm,  VD6toW2z);
      G4ThreeVector preposW2 = DetTrans->TransformPoint(VD6toW2);
      G4ThreeVector posW2 = preposW2;
      G4Box *W2Solid = new G4Box( "Window2",105./2.*cm,17.5/2.*cm,0.01/2.*cm );
      G4LogicalVolume *W2LV = new G4LogicalVolume( W2Solid, mList_->Mylar, "Window2LV");
      new G4PVPlacement( rotDet, posW2, W2LV, "Window2" , worldLV , false, 0);

// Drift Chamber //
      G4double VD6toDC1 = 135.*cm;
      G4ThreeVector VD6toDCD1( 0.*cm, 0.*cm,  VD6toDC1);
      G4ThreeVector preposDC1 = DetTrans->TransformPoint(VD6toDCD1);
      G4ThreeVector posDC1 = preposDC1; 
      EDC1 *DC1 = new EDC1( "EDC1", posDC1, rotDet, 0 );
      DC1->SetMaterials( mList_->Vacuum, mList_->DCGas, mList_->Mylar, mList_->DCGas, mList_->W, mList_->Cu, mList_->Cu );
      DC1->Place(world);
      
      G4double VD6toDC2 = 166.*cm;
      G4ThreeVector VD6toDCD2( 0.*cm, 0.*cm,  VD6toDC2);
      G4ThreeVector preposDC2 = DetTrans->TransformPoint(VD6toDCD2);
      G4ThreeVector posDC2 = preposDC2;
      EDC2 *DC2 = new EDC2( "EDC2", posDC2, rotDet, 0 );
      DC2->SetMaterials( mList_->Vacuum, mList_->DCGas, mList_->Mylar, mList_->DCGas, mList_->W, mList_->W);
      DC2->Place(world );

      EDC1SD *DC1SD = new EDC1SD( "/HES/DC1" );
      SDManager->AddNewDetector( DC1SD );
      DC1->GetDetectorLV()->SetSensitiveDetector( DC1SD );
      DC1SD->SetElement( DC1 );
      EDC2SD *DC2SD = new EDC2SD( "/HES/DC2" );
      SDManager->AddNewDetector( DC2SD );
      DC2->GetDetectorLV()->SetSensitiveDetector( DC2SD );
      DC2SD->SetElement( DC2 );

      G4double VD6toAirz = 154.85*cm;
      G4ThreeVector VD6toAir( 0.*cm, 0.*cm,  VD6toAirz);
      G4ThreeVector preposAir = DetTrans->TransformPoint(VD6toAir);
      G4ThreeVector posAir = preposAir;
      G4Box *AirSolid = new G4Box( "Air",100.*cm,50.*cm,7.7/2*cm );
      G4LogicalVolume *AirLV = new G4LogicalVolume( AirSolid, mList_->Air, "AirLV");
      new G4PVPlacement( rotDet, posAir, AirLV, "Air" , worldLV , false, 0);
    }  // if EDCFlag!=0

// EHODO 1 //
    G4double DetoDetH1 = 178.2*cm;
    G4ThreeVector posEH1 = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetH1));
    EHodo1 *DetH1 = new EHodo1( "EHODO 1", posEH1, rotDet, 0 );
    DetH1->SetMaterial( mList_->Scinti );
    DetH1->Place(world);
    EMField_->AddElement(DetH1);
 
// EHODO 2 //
    G4double DetoDetH2 = 208.2*cm;
    G4ThreeVector posEH2 = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetH2));
    EHodo2 *DetH2 = new EHodo2( "EHODO 2", posEH2, rotDet, 0 );
    DetH2->SetMaterial( mList_->Scinti );
    DetH2->Place(world);
    EMField_->AddElement(DetH2);

  //=== Virtual Detectors for Detector Plane ===//
//VD of EDC1
    G4double DetoDetDC1 = 135.*cm;
    G4ThreeVector posDetDC1 = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetDC1));
    VirtualDetector *DetDC1 = new VirtualDetector( "DetDC1", posDetDC1, rotDet, 300.*cm, 100.*cm, 0.1*cm, 10 );
    DetDC1->SetMaterials( mList_->Vacuum );
    DetDC1->Place( world );

//VD of Reference Plane 
    G4double DetoDetRPVD = 155.*cm;
    G4ThreeVector posDetRPVD = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetRPVD));
    VirtualDetector *DetRPVD = new VirtualDetector( "DetRPVD", posDetRPVD, rotDet, 300.*cm, 100.*cm, 0.1*cm, 11 );
    DetRPVD->SetMaterials( mList_->Vacuum );
    DetRPVD->Place( world );

//VD of EDC2
    G4double DetoDetDC2 = 166.*cm;
    G4ThreeVector posDetDC2 = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetDC2));
    VirtualDetector *DetDC2 = new VirtualDetector( "DetDC2", posDetDC2, rotDet, 300.*cm, 100.*cm, 0.1*cm, 12 );
    DetDC2->SetMaterials( mList_->Vacuum );
    DetDC2->Place( world );

//VD of EH1
    G4double DetoDetH1VD = 172.2*cm;
    G4ThreeVector posDetH1VD = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetH1VD));
    VirtualDetector *DetH1VD = new VirtualDetector( "DetH1VD", posDetH1VD, rotDet, 300.*cm, 100.*cm, 0.1*cm, 13 );
    DetH1VD->SetMaterial( mList_->Scinti );
    DetH1VD->Place( world );
    EMField_->AddElement(DetH1VD);

//VD of EH2
    G4double DetoDetH2VD = 214.2*cm;
    G4ThreeVector posDetH2VD = DetTrans->TransformPoint(G4ThreeVector( 0.*cm, 0.*cm, DetoDetH2VD));
    VirtualDetector *DetH2VD = new VirtualDetector( "DetH2VD", posDetH2VD, rotDet, 300.*cm, 100.*cm, 0.1*cm, 14 );
    DetH2VD->SetMaterial( mList_->Scinti );
    DetH2VD->Place( world );
    EMField_->AddElement(DetH2VD);

  /////////////////////////
  // Sensitive Detectors //
  /////////////////////////
  //=== Virtual Detector ===//
    HESVDSD *VDSD = new HESVDSD( "/HES/VD" );
    SDManager->AddNewDetector( VDSD );
    SSVDi  ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    SSVDe  ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    PCSVDi ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    PCSVDe ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    Q1VDi  ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    Q1VDe  ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    Q2VDi  ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    Q2VDe  ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DVDi   ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DVDe   ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DetDC1 ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DetRPVD->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DetDC2 ->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DetH1VD->GetDetectorLV()->SetSensitiveDetector( VDSD );
    DetH2VD->GetDetectorLV()->SetSensitiveDetector( VDSD );
    VDSD->SetElement(  0, SSVDi   );
    VDSD->SetElement(  1, SSVDe   );
    VDSD->SetElement(  2, PCSVDi  );
    VDSD->SetElement(  3, PCSVDe  );
    VDSD->SetElement(  4, Q1VDi   );
    VDSD->SetElement(  5, Q1VDe   );
    VDSD->SetElement(  6, Q2VDi   );
    VDSD->SetElement(  7, Q2VDe   );
    VDSD->SetElement(  8, DVDi    );
    VDSD->SetElement(  9, DVDe    );
    VDSD->SetElement( 10, DetDC1  );
    VDSD->SetElement( 11, DetRPVD );
    VDSD->SetElement( 12, DetDC2  );
    VDSD->SetElement( 13, DetH1VD );
    VDSD->SetElement( 14, DetH2VD );

  //=== HES HodoScope ===//
    EH1SD *eh1SD = new EH1SD( "/HES/EH1" );
    SDManager->AddNewDetector( eh1SD );
    DetH1->GetDetectorLV()->SetSensitiveDetector( eh1SD );
    eh1SD->SetElement( DetH1 );
    EH2SD *eh2SD = new EH2SD( "/HES/EH2" );
    SDManager->AddNewDetector( eh2SD );
    DetH2->GetDetectorLV()->SetSensitiveDetector( eh2SD );
    eh2SD->SetElement( DetH2 );

  } //if(Event Flag)

  //=== Target ==//
  HESTargetSD *targetSD = new HESTargetSD( "/HES/Target/" );
  SDManager->AddNewDetector( targetSD );
  targetLV->SetSensitiveDetector( targetSD );

  ///////////////////
  // Visualisation //
  ///////////////////
  //=== World ===//
  worldLV->SetVisAttributes(G4VisAttributes::Invisible);

  //=== Target ===//
  G4Colour colourTarget(0., 1., 1.); // cyan
  G4VisAttributes *targetVisAtt = new G4VisAttributes(true, colourTarget);
  targetLV->SetVisAttributes(targetVisAtt);


  ///////////////////////
  // Special User Cuts //
  ///////////////////////
//  G4UserLimits  *userLimitsYoke =
//    new G4UserLimits( 5.*cm, 30.*cm, 100.*ns, 10.*MeV, 5.*cm);
//  pcs->SetUserLimits( userLimitsYoke );
//  Q1->SetUserLimits( userLimitsYoke );
//  Q2->SetUserLimits( userLimitsYoke );
//  Dipole->SetUserLimits( userLimitsYoke );
//  CExtension->SetUserLimits( userLimitsYoke );
//  targetC->SetUserLimits( userLimitsYoke );

  return world;
}

