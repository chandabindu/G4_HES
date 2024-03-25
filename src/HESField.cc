/*
  HESField.cc

  D.Kawama
*/

#include "HESField.hh"
#include "HESGlobalSize.hh"
#include "BeamLineElement.hh"
#include <string>

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
// #include "G4ClassicalRK4.hh"
// #include "G4SimpleHeum.hh"
// #include "G4EqMagElectricField.hh"


/////////////////////
HESField::HESField()
////////////////////
{
  MakeStepper();
  elemList.reserve(32);
}

//////////////////////
HESField::~HESField()
/////////////////////
{
  cleanupElemList();
  //  DeleteStepper();
}

//////////////////////////////////////
void HESField::cleanupElemList( void )
//////////////////////////////////////
{
  elemList.clear();
}

///////////////////////////////////////////////////
void HESField::AddElement( BeamLineElement *elem )
//////////////////////////////////////////////////
{
  elemList.push_back(elem);
}

/////////////////////////////////////////////////////////////////////////
void HESField::GetFieldValue(const double Point[3], double *Bfield) const
/////////////////////////////////////////////////////////////////////////
{
  G4ThreeVector gPos(Point[0], Point[1], Point[2]);
#ifdef DEBUG
  fFlag.push_back(0);
  fPointX.push_back(gPos.x());
  fPointY.push_back(gPos.y());
  fPointZ.push_back(gPos.z());

  if( fabs(Point[0]) > FieldSizeX ||
      fabs(Point[1]) > FieldSizeY ||
      fabs(Point[2]) > FieldSizeZ ){
    G4cout << "HESField::GetFieldValue :" <<""
	  << " Out of World !!" << G4endl;
    int ns = fFlag.size();
    for( int i=0 ; i<ns ; ++i ){
      G4ThreeVector pos(fPointX[i], fPointY[i], fPointZ[i]);
      int flag = fFlag[i];
      G4cout << flag < " : " < pos/cm << G4endl;
    }
    G4Exception( "HESField::Field : Out of World" );
  }
#endif

  G4ThreeVector B(0., 0., 0.);
  
  BLEIterator end = elemList.end();
  for( BLEIterator i=elemList.begin(); i!=end; ++i){
    if( (*i)->ExistMagneticField() ){
      B += (*i)->GetMagneticField( gPos );
    }
  }
  Bfield[0] = B.x(); Bfield[1] = B.y(); Bfield[2] = B.z();

#if 0
// #if 1
  //  G4cout << gPos << " B=" << B << " E=" << E << G4endl;
   G4cout << gPos << " B=" << B.getR()/tesla << "[T]" << G4endl;
#endif
}

///////////////////////////////////
void HESField::MakeStepper( void )
//////////////////////////////////
{
  //  fEquation = new G4EqMagElectricField( this );
  //  fStepper = new G4SimpleHeum( fEquation );
  G4FieldManager *fieldMan =
    G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fieldMan->SetDetectorField( this );
  fieldMan->CreateChordFinder( this );
  //  fChordFinder = new G4ChordFinder( this, 1.0e-2*mm, fStepper);

   /*fieldMan->SetMinimumEpsilonStep( 1.0e-4*mm );
   fieldMan->SetMaximumEpsilonStep( 1.0e-3*mm );
   fieldMan->SetDeltaOneStep( 0.5e-3*mm );*/
//   fieldMan->SetDeltaIntersection( 0.01*mm );

//   fieldMan->SetChordFinder( fChordFinder );
}

/////////////////////////////////////
void HESField::DeleteStepper( void )
/////////////////////////////////////
{
//   delete fChordFinder;
//   delete fStepper;
//   delete fEquation;
}

////////////////////////////////////////
void HESField::ResetQueue( void ) const
////////////////////////////////////////
{
  fFlag.clear();
  fPointX.clear();
  fPointY.clear();
  fPointZ.clear();
  if( fFlag.capacity()>1024 ){
    std::vector<int>(fFlag).swap(fFlag);
    std::vector<double>(fPointX).swap(fPointX);
    std::vector<double>(fPointY).swap(fPointY);
    std::vector<double>(fPointZ).swap(fPointZ);
  }
}

/////////////////////////////////////////////////////////////////////
void HESField::PushInQueue( int , const G4ThreeVector & ) const
//////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
  fFlag.push_back(val);
  fPintX.push_back(pos.x());
  fPintY.push_back(pos.y());
  fPintZ.push_back(pos.z());
#endif
}

////////////////////////////////////////
void HESField::PrintQueue( void ) const
////////////////////////////////////////
{
  int ns = fFlag.size();
  for( int i=0; i<ns ; ++i ){
    G4ThreeVector pos(fPointX[i], fPointY[i], fPointZ[i]);
    int flag = fFlag[i];
    G4cout << flag << " : " << pos/cm << G4endl;
  }
}
