/*
  HESSteppingAction.hh

  D.Kawama
*/

#ifndef HESSteppingAction_h
#define HESSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include <fstream>
#include <iostream>

class G4Step;
class HESField;

class HESSteppingAction : public G4UserSteppingAction
{
public:
  HESSteppingAction();
  ~HESSteppingAction();

  void UserSteppingAction( const G4Step *aStep );

private:
  const HESField *field_;


};
#endif
 
