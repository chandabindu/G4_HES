/*
  HESTransportation.hh

  D.Kawama
*/

#ifndef HESTransportation_h

#define HESTransportation_h 1

#include "G4Transportation.hh"

class HESTransportation : public G4Transportation
{
public:
  HESTransportation()
    : G4Transportation() {}
  ~HESTransportation() {}

  G4double AlongStepGetPhysicalInteractionLength( const G4Track & track, 
						  G4double previousStepSize,
						  G4double currentMinimumStep, 
						  G4double & currentSafety,
						  G4GPILSelection* selection );

};

#endif
