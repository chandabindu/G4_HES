/*
  HESTransportation.cc

  D.Kawama
*/

#include "HESTransportation.hh"
#include "G4SystemOfUnits.hh"
using namespace CLHEP;
////////////////////////////////////////////////////////////////////
G4double HESTransportation::
AlongStepGetPhysicalInteractionLength( const G4Track & track, 
				       G4double previousStepSize,
				       G4double currentMinimumStep, 
				       G4double & currentSafety,
				       G4GPILSelection *selection )
////////////////////////////////////////////////////////////////////		  
{
  //if( this-> DoesGlobalFieldExist() &&
  if( this-> DoesAnyFieldExist() &&
      currentMinimumStep>1.*cm){
//    currentMinimumStep = 0.5*cm;
    currentMinimumStep = 2.0*mm;
  }

  return G4Transportation::
    AlongStepGetPhysicalInteractionLength( track, previousStepSize, 
					   currentMinimumStep,
					   currentSafety, selection );
  
}
