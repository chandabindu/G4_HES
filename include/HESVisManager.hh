/*
  HESVisManager.hh

  2003/Nov/3
  H.Nomura
*/

#ifndef HES_VIS_MANAGER_H
#define HES_VIS_MANAGER_H

#include "G4VisManager.hh"

class HESVisManager : public G4VisManager {
public:
  HESVisManager();
  virtual ~HESVisManager();

private:
  virtual void RegisterGraphicsSystems();

};

#endif
