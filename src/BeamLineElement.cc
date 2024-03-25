/*
  BeamLineElement.cc

  D.Kawama
*/

#include "BeamLineElement.hh"

#include "G4AffineTransform.hh"

///////////////////////////////////////////////////////////////////
BeamLineElement::BeamLineElement( const G4String &name,
				  const G4ThreeVector &pos,
				  const G4RotationMatrix *rotMtx )
  : elemName_(name), gPos_(pos),
    rotMtx_( rotMtx ? (*rotMtx) : G4RotationMatrix() ),
    pGtoL( new G4AffineTransform( rotMtx_, gPos_ ) ),
    pLtoG( new G4AffineTransform( rotMtx_, gPos_ ) )
//////////////////////////////////////////////////////////////////
{ 
  pGtoL-> Invert();
}

///////////////////////////////////
BeamLineElement::~BeamLineElement()
///////////////////////////////////
{
  delete pGtoL;
  delete pLtoG;
}

//////////////////////////////////////////////
G4ThreeVector BeamLineElement::
GetMagneticField( const G4ThreeVector &) const
//////////////////////////////////////////////
{
  return G4ThreeVector(0.,0.,0.);
}

///////////////////////////////////////////////
G4ThreeVector BeamLineElement::
GetElectricField( const G4ThreeVector &) const
///////////////////////////////////////////////
{
  return G4ThreeVector(0.,0.,0.);
}

/////////////////////////////////////////////////////
G4ThreeVector BeamLineElement::
GetGlobalPosition( const G4ThreeVector &lPos ) const
/////////////////////////////////////////////////////
{
  return pLtoG-> TransformPoint(lPos);
}

////////////////////////////////////////////////////
G4ThreeVector BeamLineElement::
GetLocalPosition( const G4ThreeVector &gPos ) const
///////////////////////////////////////////////////
{
  return pGtoL-> TransformPoint(gPos);
}

//////////////////////////////////////////////////////
G4ThreeVector BeamLineElement::
GetGlobalDirection( const G4ThreeVector &lDir ) const
//////////////////////////////////////////////////////
{
  return pLtoG-> TransformAxis(lDir);
}

/////////////////////////////////////////////////////
G4ThreeVector BeamLineElement::
GetLocalDirection( const G4ThreeVector &gDir ) const
/////////////////////////////////////////////////////
{
  return pGtoL-> TransformAxis(gDir);
}
