#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"
#include "GetNumberFromKernelEntropyPool.hh"

#include "HESDetectorConstruction.hh"
//#include "HESPhysicsList.hh"
#include "HESPrimaryGeneratorAction.hh"
#include "HESRunAction.hh"
#include "HESEventAction.hh"
#include "HESAnalysis.hh"
#include "HESTrackingAction.hh"
#include "HESSteppingAction.hh"
#include "HESParamMan.hh"
#include "QGSP_BERT_HP.hh"
#include "G4String.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main( int argc, char** argv)
{

  //  ====== Choose the Random Engine ========= //
  //CLHEP::HepRandom::setTheSeed(time(NULL));
  int initSeed=GetIntFromKernelEntropyPool()&0x7FFFFFFF;
  CLHEP::HepRandom::setTheSeed(initSeed);
  int startSeed=CLHEP::HepRandom::getTheSeed();
  G4cout << "*** Initial Seed = " << startSeed << G4endl;
  CLHEP::HepRandom::showEngineStatus();
  //HepRandom::setTheEngine(new RanecuEngine);

  G4String macFile = "mac/gun.mac";
  const char *paramFile = "input/sample.in";
  // const char *paramFile = "input/config_vert.in"; //Added by ishige 2022Oct06
  // const char *paramFile = "input/config_pattern.in"; //Added by ishige 2023Feb21

  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }  else if( argc<3 ){
    G4cout << "program [macro file] [parameter file]" << G4endl;
    exit(-1);
  }  else{
    macFile = argv[1];
    paramFile = argv[2];
  }

  // parameter manager
  HESParamMan *paramMan = new HESParamMan( paramFile );

  // run manager
  G4RunManager *runManager = new G4RunManager;

 // particle & physics processes
  //HESPhysicsList *physicsList = new HESPhysicsList;
  //runManager->SetUserInitialization( physicsList );
  auto *physicsList = new QGSP_BERT_HP;
  runManager->SetUserInitialization( physicsList );

  // initialize visualization
#ifdef G4VIS_USE
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  HESDetectorConstruction *detector = new HESDetectorConstruction;
  runManager->SetUserInitialization( detector );
  HESAnalysis *analysisManager = new HESAnalysis();
  
  HESPrimaryGeneratorAction *primaryGenerator = new HESPrimaryGeneratorAction( detector, analysisManager );
  HESRunAction *runAction = new HESRunAction( analysisManager );
  HESEventAction *eventAction = new HESEventAction( analysisManager );
  HESTrackingAction *trackAction = new HESTrackingAction();
  HESSteppingAction *steppingAction = new HESSteppingAction();

  runManager->SetUserAction( primaryGenerator );
  runManager->SetUserAction( runAction );
  runManager->SetUserAction( eventAction );
  runManager->SetUserAction( trackAction );
  runManager->SetUserAction( steppingAction );
  runManager->Initialize();

  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  if ( ! ui ) {
    std::cout<<"batch mode"<<std::endl;
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macFile);
  }  else {
    std::cout<<"execute macro/init_vis.mac"<<std::endl;
    UImanager->ApplyCommand("/control/execute macro/init_vis.mac");
    if (ui->IsGUI()) {
      G4cout<<"execute macro/gui.mac"<<G4endl;
      UImanager->ApplyCommand("/control/execute macro/gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  delete analysisManager;

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;
  delete paramMan;

  return 0;
}

