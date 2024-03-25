/*
  HESRunAction.cc

  D.Kawama
*/

#include "globals.hh"
#include "G4ios.hh"
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"

#include "HESRunAction.hh"
#include "HESAnalysis.hh"

/////////////////////////////////////////////////////////
HESRunAction::HESRunAction(HESAnalysis *analysisManager)
  : G4UserRunAction(), anaMan(analysisManager)
/////////////////////////////////////////////////////////
{
}

//////////////////////////////
HESRunAction::~HESRunAction()
//////////////////////////////
{
}

///////////////////////////////////////////////////////
void HESRunAction::BeginOfRunAction(const G4Run* aRun)
//////////////////////////////////////////////////////
{  
  /*if(G4VVisManager::GetConcreteInstance()){
    G4UImanager *UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }*/
  anaMan->BeginOfRun(aRun);
}

/////////////////////////////////////////////////////
void HESRunAction::EndOfRunAction(const G4Run* aRun)
/////////////////////////////////////////////////////
{
  /*if(G4VVisManager::GetConcreteInstance()){
    G4UImanager *UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/viewer/update");
  }*/
  if(anaMan) anaMan->EndOfRun(aRun);
}

