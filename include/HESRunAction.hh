/*
  HESRunAction.hh

  D.Kawama
*/

#ifndef HES_RUN_ACTION_H
#define HES_RUN_ACTION_H

#include "G4UserRunAction.hh"


class G4Run;
class HESAnalysis;

class HESRunAction : public G4UserRunAction {
public:
  HESRunAction(HESAnalysis *analysisManager=0);
  virtual ~HESRunAction();

  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

private:
  HESAnalysis *anaMan;
};

#endif
