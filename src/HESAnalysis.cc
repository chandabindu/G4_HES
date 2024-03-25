#include "HESAnalysis.hh"
#include "HESTargetHit.hh"
#include "HESVDHit.hh"
#include "EDC1Hit.hh"
#include "EDC2Hit.hh"
#include "HESEH1Hit.hh"
#include "HESEH2Hit.hh"

#include "HESTrajectory.hh"
#include "HESSteppingAction.hh"
#include "RootHelper.hh"

#include "HESParamMan.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4String.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <CLHEP/Random/RandGaussQ.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>

//#ifdef USE_CSTREAM
#include <cstdio>
//#endif


///////////////////////////////////////
HESAnalysis::HESAnalysis()
  : fTriggered(false), fActive_(true), fOutput(false)
///////////////////////////////////////
{
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  filename_ = paramMan->GetHbookFileName();

  defineHistograms();
}

///////////////////////////
HESAnalysis::~HESAnalysis()
///////////////////////////
{
  SaveFile();
}


//////////////////////////////////////////////////
void HESAnalysis::BeginOfRun( const G4Run *)
//////////////////////////////////////////////////
{
  trigNum=0;
  
}

////////////////////////////////////////////////
void HESAnalysis::EndOfRun( const G4Run *aRun )
////////////////////////////////////////////////
{
  G4cout << "#events generated = " << aRun->GetNumberOfEvent() << G4endl;
  G4cout << "#events triggered = " << trigNum << G4endl;
}

/////////////////////////////////////////////////////////
void HESAnalysis::BeginOfEvent( const G4Event *)
/////////////////////////////////////////////////////////
{
  //G4cout << "Begin Of Event" <<G4endl;
}

////////////////////////////////////////////////////////////////
void HESAnalysis::PrimaryGeneration( const G4ThreeVector &pos,
				     const G4ThreeVector &mom,
				     const G4int ID)
////////////////////////////////////////////////////////////////
{
  //G4cout << "Primary Generation" <<G4endl;
  gPos_=pos; gMom_=mom;
  evID = ID;
}

////////////////////////////////////////////////////////
void HESAnalysis::EndOfEvent( const G4Event *anEvent  )
////////////////////////////////////////////////////////
{
  G4int eventID= anEvent->GetEventID()+1;
  if(eventID%1000 ==0){
    G4cout << "Event Number :" << std::setw(6) << eventID <<G4endl;
    G4cout << "Triggered Event :" << std::setw(6) << trigNum <<G4endl;
  }
  fTriggered = false;
  NormalAnalysis( anEvent );
  //G4cout << "End Of Event" <<G4endl;
}

///////////////////////////////////////////////////////////
void HESAnalysis::NormalAnalysis( const G4Event *anEvent )
///////////////////////////////////////////////////////////
{
  TTree *tree = dynamic_cast<TTree *>(gFile->Get("tree"));
  InitializeEvent();
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  int flag = paramMan->GetEventFlag();
  int EDCFlag = paramMan->GetEDCFlag();
  G4double FPAng = paramMan->GetFPAngle()*degree;
  pBeam =  paramMan->GetpBeam0();
  pVD   =  paramMan->GetpBeam1();
  
  G4HCofThisEvent *HCE = anEvent->GetHCofThisEvent();
  G4SDManager *SDMan = G4SDManager::GetSDMpointer();
  G4int colIdTarget = SDMan->GetCollectionID( "TargetCollection" );
  G4int colIdVD[n_vd];
  G4double pt  = -1000., pxt = -1000.;
  G4double pyt = -1000., pzt = -1000.;
  G4double theta = -1000., phi   = -1000.;
  G4double xpt = -1000., ypt = -1000.;
  G4double xt = -1000., yt = -1000., zt = -1000.;
  G4double VDEdep = 0.;
  G4double DCEdep = 0.;
  G4double EH1Edep = 0.;
  G4double EH2Edep = 0.;
  G4int nHits = 0;G4double TargetEdep = 0.0;
  G4int VDhit[n_vd];
  HESVDHitsCollection *VDHC[n_vd];

  //=== Target ===//
  HESTargetHitsCollection *TargetHC; 
  TargetHC = dynamic_cast<HESTargetHitsCollection *>(HCE->GetHC(colIdTarget));
  if( TargetHC ){
    nHits = TargetHC->entries();
    for( G4int i=0; i<nHits; i++ ){
      HESTargetHit *aHit = (*TargetHC)[i];
      TargetEdep += aHit->GetEdep();
    }
  } // if( HESTargetHC )

  
  if( flag==0 ){
    
    //=== Virtual Detector ===//
   for( int i=1 ; i<n_vd; ++i){
     std::ostringstream id;
     id << i;
     G4String Name = G4String("VD")+id.str().c_str()+"-Collection";
     colIdVD[i] = SDMan-> GetCollectionID( Name );
     VDHC[i] = dynamic_cast<HESVDHitsCollection *> (HCE->GetHC(colIdVD[i]));

     if( VDHC[i] ){
       G4int nHits = VDHC[i]->entries();
       G4ThreeVector lPos;
       G4ThreeVector lMom;
       G4ThreeVector Vertex;
       for( G4int j=0; j<nHits; j++ ){
         HESVDHit *aHit = (*VDHC[i])[j];
         VDhit[i] = nHits;
         Vertex = aHit->GetVertex();
         Yv[i] = Vertex.y()/cm;
         Xv[i] = Vertex.x()/cm;
         Zv[i] = Vertex.z()/cm;
         L[i] = aHit->GettLength()/cm;
         VDEdep = aHit->GetEdep();
         //For Local Coordinates
         lPos = aHit->GetPos();
         X[i] = lPos.x()/cm;
         Z[i] = lPos.z()/cm;
         Y[i] = lPos.y()/cm;
         lMom = aHit->GetMom();
         G4double pxVD = lMom.x();
         G4double pyVD = lMom.y();
         G4double pzVD = lMom.z();
         P[i] = sqrt(pxVD*pxVD+pyVD*pyVD+pzVD*pzVD)/GeV;
         /*G4double pxVD = lMom.x()/GeV;
         G4double pyVD = lMom.y()/GeV;
         G4double pzVD = lMom.z()/GeV;
         P[i] = sqrt(pxVD*pxVD+pyVD*pyVD+pzVD*pzVD);*/
         //Xp[i] = atan2(pxVD, pzVD)/radian;
         //Yp[i] = atan2(pyVD, pzVD)/radian;
         Xp[i] = pxVD/pzVD;
         Yp[i] = pyVD/pzVD;
       }
     }//VDHC
   }

   //===Drift Chamber===//
  
  if (EDCFlag!=0){
  //=== EDC1 ===//
  G4int colIdEDC1 = SDMan->GetCollectionID( "EDC1Collection" );
  EDC1HitsCollection *EDC1HC;
  EDC1HC = dynamic_cast<EDC1HitsCollection *>
                        (HCE->GetHC(colIdEDC1));
    if(EDC1HC){
       G4int nHits = EDC1HC->entries();
       G4ThreeVector lPos;
       G4ThreeVector lMom;
       G4ThreeVector Vertex;
       G4int LID;
		 EDC1Trig=true;
       for( G4int j=0; j<nHits; j++ ){
         EDC1Hit *aHit = (*EDC1HC)[j];
         DCEdep = aHit->GetEdep();
         LID = aHit->GetLayerID();
         if (LID==10){
				EDCFPL = aHit->GettLength()/cm;
         	//For Local Coordinates
         	lPos = aHit->GetLPos();
         	EDCFPX = lPos.x()/cm;
         	EDCFPZ = lPos.z()/cm;
         	EDCFPY = lPos.y()/cm;
         	lMom = aHit->GetLMom();
         	G4double pxDC = lMom.x()/GeV;
         	G4double pyDC = lMom.y()/GeV;
         	G4double pzDC = lMom.z()/GeV;
         	EDCFPP = sqrt(pxDC*pxDC+pyDC*pyDC+pzDC*pzDC);
         	//EDCFPXp = atan2(pxDC, pzDC)/radian;
         	//EDCFPYp = atan2(pyDC, pzDC)/radian;
         	EDCFPXp = pxDC/pzDC;
         	EDCFPYp = pyDC/pzDC;
			}
         else {
         	//For Local Coordinates
         	lPos = aHit->GetLPos();
         	EDC1X[LID] = lPos.x()/cm;
         	EDC1Z[LID] = lPos.z()/cm;
         	EDC1Y[LID] = lPos.y()/cm;
         	lMom = aHit->GetLMom();
         	G4double pxDC = lMom.x()/GeV;
         	G4double pyDC = lMom.y()/GeV;
         	G4double pzDC = lMom.z()/GeV;
         	EDC1P[LID] = sqrt(pxDC*pxDC+pyDC*pyDC+pzDC*pzDC);
         	//EDC1Xp[LID] = atan2(pxDC, pzDC)/radian;
         	//EDC1Yp[LID] = atan2(pyDC, pzDC)/radian;
         	EDC1Xp[LID] = pxDC/pzDC;
         	EDC1Yp[LID] = pyDC/pzDC;
			}
       }
    }
  //=== EDC2 ===//
  G4int colIdEDC2 = SDMan->GetCollectionID( "EDC2Collection" );
  EDC2HitsCollection *EDC2HC;
  EDC2HC = dynamic_cast<EDC2HitsCollection *>
                        (HCE->GetHC(colIdEDC2));
    if(EDC2HC){
       G4int nHits = EDC2HC->entries();
       G4ThreeVector lPos;
       G4ThreeVector lMom;
       G4ThreeVector Vertex;
       G4int LID;
		 EDC2Trig=true;
       for( G4int j=0; j<nHits; j++ ){
         EDC2Hit *aHit = (*EDC2HC)[j];
         DCEdep = aHit->GetEdep();
         LID = aHit->GetLayerID();
         //For Local Coordinates
         lPos = aHit->GetLPos();
         EDC2X[LID] = lPos.x()/cm;
         EDC2Z[LID] = lPos.z()/cm;
         EDC2Y[LID] = lPos.y()/cm;
         lMom = aHit->GetLMom();
         G4double pxDC = lMom.x()/GeV;
         G4double pyDC = lMom.y()/GeV;
         G4double pzDC = lMom.z()/GeV;
         EDC2P[LID] = sqrt(pxDC*pxDC+pyDC*pyDC+pzDC*pzDC);
         //EDC2Xp[LID] = atan2(pxDC, pzDC)/radian;
         //EDC2Yp[LID] = atan2(pyDC, pzDC)/radian;
         EDC2Xp[LID] = pxDC/pzDC;
         EDC2Yp[LID] = pyDC/pzDC;
       }
    }

  }
  
  //=== HES HodoScope ===//
  //=== EH1 ===//
  G4int colIdEH1 = SDMan->GetCollectionID( "EH1Collection" );
  EH1HitsCollection *EH1HC;
  EH1HC = dynamic_cast<EH1HitsCollection *>
    (HCE->GetHC(colIdEH1));
  if(EH1HC){
    G4int nHits = EH1HC->entries();
    G4ThreeVector lPos;
    G4ThreeVector lMom;
    G4ThreeVector Vertex;
    G4int ELID;
    EH1Trig=true;
    if (nHits>0){
	 	for( G4int j=0; j<nHits; j++ ){
     	 	EH1Hit *aHit = (*EH1HC)[j];
			ELID = aHit->GetLayerID();
      	EH1Edep = aHit->GetEdep();
      	EH1A[ELID] += EH1Edep;
			G4double EHTime = aHit->GetTime()/ns;
      	EH1T[ELID] = EHTime;
      	//For Local Coordinates
      	lPos = aHit->GetLPos();
      	EH1X[ELID] = lPos.x()/cm;
      	EH1Z[ELID] = lPos.z()/cm;
      	EH1Y[ELID] = lPos.y()/cm;
      	lMom = aHit->GetLMom();
      	G4double pxDC = lMom.x()/GeV;
      	G4double pyDC = lMom.y()/GeV;
      	G4double pzDC = lMom.z()/GeV;
      	EH1P[ELID] = sqrt(pxDC*pxDC+pyDC*pyDC+pzDC*pzDC);
      	//EH1Xp[ELID] = atan2(pxDC, pzDC)/radian;
      	//EH1Yp[ELID] = atan2(pyDC, pzDC)/radian;
      	EH1Xp[ELID] = pxDC/pzDC;
      	EH1Yp[ELID] = pyDC/pzDC;
    	}
	 }
	 else{
		for( G4int j=0; j<nHits; j++){
			EH1A[j]=-1000.;
		}
	 }
  }
  //=== EH2 ===//
  G4int colIdEH2 = SDMan->GetCollectionID( "EH2Collection" );
  EH2HitsCollection *EH2HC;
  EH2HC = dynamic_cast<EH2HitsCollection *>
    (HCE->GetHC(colIdEH2));
  if(EH2HC){
    G4int nHits = EH2HC->entries();
    G4ThreeVector lPos;
    G4ThreeVector lMom;
    G4ThreeVector Vertex;
    G4int ELID;
    EH2Trig=true;
    if (nHits>0){
	 	for( G4int j=0; j<nHits; j++ ){
      	EH2Hit *aHit = (*EH2HC)[j];
     	 	ELID = aHit->GetLayerID();
     	 	EH2Edep = aHit->GetEdep();
     		EH2A[ELID] += EH2Edep;
			G4double EHTime = aHit->GetTime()/ns;
      	EH2T[ELID] = EHTime;
     	 	//For Local Coordinates
     	 	lPos = aHit->GetLPos();
     	 	EH2X[ELID] = lPos.x()/cm;
     	 	EH2Z[ELID] = lPos.z()/cm;
     	 	EH2Y[ELID] = lPos.y()/cm;
      	lMom = aHit->GetLMom();
      	G4double pxDC = lMom.x()/GeV;
      	G4double pyDC = lMom.y()/GeV;
      	G4double pzDC = lMom.z()/GeV;
      	EH2P[ELID] = sqrt(pxDC*pxDC+pyDC*pyDC+pzDC*pzDC);
      	//EH2Xp[ELID] = atan2(pxDC, pzDC)/radian;
      	//EH2Yp[ELID] = atan2(pyDC, pzDC)/radian;
      	EH2Xp[ELID] = pxDC/pzDC;
      	EH2Yp[ELID] = pyDC/pzDC;
    	}
	 }
	 else{
		for( G4int j=0; j<nHits; j++){
			EH2A[j]=-1000.;
		}
	 }
  }
  
  
  
  }//flag
  
  if ( EDC1X[0]>-1000.
       && EDC1X[1]>-1000.
       && EDC1X[2]>-1000.
       && EDC1X[3]>-1000.
       && EDC1X[4]>-1000.
       && EDC1X[5]>-1000.
       && EDC1X[6]>-1000.
       && EDC1X[7]>-1000.
       && EDC1X[8]>-1000.
       && EDC1X[9]>-1000.)
    {EDC1Trig=true;}
  else{EDC1Trig=false;}
  if ( EDC2X[0]>-1000.
       && EDC2X[1]>-1000.
       && EDC2X[2]>-1000.
       && EDC2X[3]>-1000.
       && EDC2X[4]>-1000.
		  && EDC2X[5]>-1000.)
    {EDC2Trig=true;}
  else{EDC2Trig=false;}

  if( VDhit[4]>0 && VDhit[5]>0 && VDhit[6]>0 && VDhit[7]>0 && VDhit[8]>0 && VDhit[9]>0 ){ QQDTrig = true; }else{ QQDTrig = false; }
  if( VDhit[10]>0 && VDhit[11]>0 && VDhit[12]>0 && VDhit[13]>0 && VDhit[14]>0 ){ FPTrig = true;}else{ FPTrig = false; }
  if( QQDTrig && EDC1Trig && EDC2Trig){ EDCTrig = true;}else{ EDCTrig=false;}
  if( EH1Trig && EH2Trig ){ EHTrig = true; }else{ EHTrig = false; }
  if( QQDTrig && FPTrig && abs(X[13])<58.5 && abs(Y[13])<15. && abs(X[14])<58.5 && abs(Y[14])<15.){ ETrig = true; }else{ ETrig = false; }
  if( QQDTrig && EDCTrig && VDhit[11]>0 &&abs(X[13])<58.5 && abs(Y[13])<15. && abs(X[14])<58.5 && abs(Y[14])<15.){ ETrig2 = true; }else{ ETrig2 = false; }
  if( QQDTrig && VDhit[11]>0 ){ ETrig3 = true; trigNum++; }
  fTriggered = ETrig3;
//  std::cout<<"test9"<<std::endl;
  
  xt    = gPos_.x();
  yt    = gPos_.y();
  zt    = gPos_.z();
  pt    = gMom_.mag();
  pxt   = gMom_.x();
  pyt   = gMom_.y();
  pzt   = gMom_.z();
  theta = acos( pzt/pt );
  phi   = atan2( pyt, pxt );
  xpt   = atan2( pxt, pzt );
  ypt   = atan2( pyt, pzt );
  
  Mom   = pt/GeV;
  Theta = theta/degree;
  Phi   = phi/degree;
  Xpt=xpt;Ypt=ypt;
  //   Xpt=xpt/radian;Ypt=ypt/radian;
  Xt=xt/cm;Yt=yt/cm;Zt=zt/cm;
  
  // ==== Toshi, 17/Sep/2012 ===
  tree->Fill();
  //if(fTriggered){
  //tree->Fill();
  //}
  // ===========================
  
}  

//////////////////////////////////////////
void HESAnalysis::InitializeEvent( void )
//////////////////////////////////////////
{
 Mom   = -1000.0;
 Theta = -1000.0;
 Phi   = -1000.0;
 Xpt   = -1000.0;
 Ypt   = -1000.0;
 QQDTrig    = false;
 FPTrig     = false;
 EDCTrig    = false;
 EHTrig     = false;
 ETrig      = false;
 ETrig2     = false;
 ETrig3     = false;
 fTriggered = false;
 for (int i=0;i<n_vd;i++){
  X[i] = Y[i] = Z[i] = -1000.;
  Xp[i] = Yp[i] = -1000.;
  Xv[i] = Yv[i] = Zv[i] = -1000.;
  P[i] = L[i]=-1000;
 }
  
 EDCFPX = EDCFPY = EDCFPZ = -1000;
 EDCFPXp = EDCFPYp = -1000;
 EDCFPP = EDCFPL = -1000;
 for (int i=0;i<10;i++){
  EDC1X[i] = EDC1Y[i] = EDC1Z[i] = -1000;
  EDC1Xp[i] = EDC1Yp[i] = -1000;
  EDC1P[i] = -1000;
 }
 for (int i=0;i<6;i++){
  EDC2X[i] = EDC2Y[i] = EDC2Z[i] = -1000;
  EDC2Xp[i] = EDC2Yp[i] = -1000;
  EDC2P[i] = -1000;
 }
 for (int i=0;i<29;i++){
   EH1X[i] = EH1Y[i] = EH1Z[i] = -1000;
   EH1Xp[i]  = EH1Yp[i] = -1000;
   EH1P[i] = EH1A[i] = EH1T[i] = -1000;
   EH2X[i] = EH2Y[i] = EH2Z[i] = -1000;
   EH2Xp[i] = EH2Yp[i] = -1000;
   EH2P[i] = EH2A[i] = EH2T[i] = -1000;
 } 
}

////////////////////////////////////////
void HESAnalysis::SaveFile( void ) const
////////////////////////////////////////
{
 if( fActive_ )
   gFile->Write();
}

////////////////////////////////////////
void HESAnalysis::Terminate( void ) const
////////////////////////////////////////
{
 if( fActive_ )
   gFile->Write();
   gFile->Close();
}

//////////////////////////////////////////////
void HESAnalysis::outputData( const G4Event *)
//////////////////////////////////////////////
{
  if( fDummy ){
    fprintf( fDummy, "Kine: %5d %4.3f %4.3f %4.3f %7.3f %7.3f %7.3f \n",
	     evID, gPos_.x()/cm, gPos_.y()/cm, gPos_.z(),
	     gMom_.x()/MeV, gMom_.y()/MeV, gMom_.z()/MeV);
  }

}

///////////////////////////////////////////
void HESAnalysis::defineHistograms( void )
///////////////////////////////////////////
{
  new TFile( filename_, "recreate","",0 );
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  int flag = paramMan->GetEventFlag();
  char sPEDC[100];
  char sXEDC[100];
  char sYEDC[100];
  char sZEDC[100];
  char sXpEDC[100];
  char sYpEDC[100];
  char sPEDCD[100];
  char sXEDCD[100];
  char sYEDCD[100];
  char sZEDCD[100];
  char sXpEDCD[100];
  char sYpEDCD[100];
  
  char sPEH[100];
  char sXEH[100];
  char sYEH[100];
  char sZEH[100];
  char sXpEH[100];
  char sYpEH[100];
  char sAEH[100];
  char sTEH[100];
  char sPEHD[100];
  char sXEHD[100];
  char sYEHD[100];
  char sZEHD[100];
  char sXpEHD[100];
  char sYpEHD[100];
  char sAEHD[100];
  char sTEHD[100];
  
  if( flag==0 ){ 
  
  HBTree("tree","tree of HES");
  TTree *tree = dynamic_cast<TTree *>(gFile->Get("tree"));
  tree->Branch("evID"     , &evID ,   "evID/I"   );
  tree->Branch("pBeam"    , &pBeam,   "pBeam/D"  );
  tree->Branch("pVD"      , &pVD  ,   "pDV/D"    );
  tree->Branch("EMom"     , &Mom  ,   "EMom/D"   );
  tree->Branch("ETheta"   , &Theta,   "ETheta/D" );
  tree->Branch("EPhi"     , &Phi  ,   "EPhi/D"   );
  tree->Branch("EXpt"     , &Xpt  ,   "EXpt/D"   );
  tree->Branch("EYpt"     , &Ypt  ,   "EYpt/D"   );
  tree->Branch("EXt"      , &Xt   ,   "EXt/D"    );
  tree->Branch("EYt"      , &Yt   ,   "EYt/D"    );
  tree->Branch("EZt"      , &Zt   ,   "EZt/D"    );

    // VD of SS entrance
    tree->Branch("EXSSi"   ,  &X[0]     , "EXSSi/D"   );
    tree->Branch("EYSSi"   ,  &Y[0]     , "EYSSi/D"   );
    tree->Branch("EZSSi"   ,  &Z[0]     , "EZSSi/D"   );
    tree->Branch("XpSSi"   ,  &Xp[0]    , "XpSSi/D"   );
    tree->Branch("YpSSi"   ,  &Yp[0]    , "YpSSi/D"   );
    // VD of SS exit
    tree->Branch("EXSSe"   ,  &X[1]     , "EXSSe/D"   );
    tree->Branch("EYSSe"   ,  &Y[1]     , "EYSSe/D"   );
    tree->Branch("EZSSe"   ,  &Z[1]     , "EZSSe/D"   );
    tree->Branch("XpSSe"   ,  &Xp[1]    , "XpSSe/D"   );
    tree->Branch("YpSSe"   ,  &Yp[1]    , "YpSSe/D"   );
    // VD of PCS entrance
    tree->Branch("EXPCSi"  ,  &X[2]     , "EXPCSi/D"  );
    tree->Branch("EYPCSi"  ,  &Y[2]     , "EYPCSi/D"  );
    tree->Branch("EZPCSi"  ,  &Z[2]     , "EZPCSi/D"  );
    tree->Branch("XpPCSi"  ,  &Xp[2]    , "XpPCSi/D"  );
    tree->Branch("YpPCSi"  ,  &Yp[2]    , "YpPCSi/D"  );
    // VD of PCS exit
    tree->Branch("EXPCSe"  ,  &X[3]     , "EXPCSe/D"  );
    tree->Branch("EYPCSe"  ,  &Y[3]     , "EYPCSe/D"  );
    tree->Branch("EZPCSe"  ,  &Z[3]     , "EZPCSe/D"  );
    tree->Branch("XpPCSe"  ,  &Xp[3]    , "XpPCSe/D"  );
    tree->Branch("YpPCSe"  ,  &Yp[3]    , "YpPCSe/D"  );
    // VD of Q1 entrance
    tree->Branch("EXQ1i"   ,  &X[4]     , "EXQ1i/D"   );
    tree->Branch("EYQ1i"   ,  &Y[4]     , "EYQ1i/D"   );
    tree->Branch("EZQ1i"   ,  &Z[4]     , "EZQ1i/D"   );
    tree->Branch("EXpQ1i"  ,  &Xp[4]    , "EXpQ1i/D"  );
    tree->Branch("EYpQ1i"  ,  &Yp[4]    , "EYpQ1i/D"  );
    // VD of Q1 exit
    tree->Branch("EXQ1e"   ,  &X[5]     , "EXQ1e/D"   );
    tree->Branch("EYQ1e"   ,  &Y[5]     , "EYQ1e/D"   );
    tree->Branch("EZQ1e"   ,  &Z[5]     , "EZQ1e/D"   );
    tree->Branch("EXpQ1e"  ,  &Xp[5]    , "EXpQ1e/D"  );
    tree->Branch("EYpQ1e"  ,  &Yp[5]    , "EYpQ1e/D"  );
    // VD of Q2 entrance
    tree->Branch("EXQ2i"   ,  &X[6]     , "EXQ2i/D"   );
    tree->Branch("EYQ2i"   ,  &Y[6]     , "EYQ2i/D"   );
    tree->Branch("EZQ2i"   ,  &Z[6]     , "EZQ2i/D"   );
    tree->Branch("EXpQ2i"  ,  &Xp[6]    , "EXpQ2i/D"  );
    tree->Branch("EYpQ2i"  ,  &Yp[6]    , "EYpQ2i/D"  );
    // VD of Q2 exit
    tree->Branch("EXQ2e"   ,  &X[7]     , "EXQ2e/D"   );
    tree->Branch("EYQ2e"   ,  &Y[7]     , "EYQ2e/D"   );
    tree->Branch("EZQ2e"   ,  &Z[7]     , "EZQ2e/D"   );
    tree->Branch("EXpQ2e"  ,  &Xp[7]    , "EXpQ2e/D"  );
    tree->Branch("EYpQ2e"  ,  &Yp[7]    , "EYpQ2e/D"  );
    // VD of D entrance
    tree->Branch("EPDi"    ,  &P[8]     , "EPDi/D"    );
    tree->Branch("EXDi"    ,  &X[8]     , "EXDi/D"    );
    tree->Branch("EYDi"    ,  &Y[8]     , "EYDi/D"    );
    tree->Branch("EZDi"    ,  &Z[8]     , "EZDi/D"    );
    tree->Branch("EXpDi"   ,  &Xp[8]    , "EXpDi/D"   );
    tree->Branch("EYpDi"   ,  &Yp[8]    , "EYpDi/D"   );
    // VD of D exit
    tree->Branch("EPDe"    ,  &P[9]     , "EPDe/D"    );
    tree->Branch("EXDe"    ,  &X[9]     , "EXDe/D"    );
    tree->Branch("EYDe"    ,  &Y[9]     , "EYDe/D"    );
    tree->Branch("EZDe"    ,  &Z[9]     , "EZDe/D"    );
    tree->Branch("EXpDe"   ,  &Xp[9]    , "EXpDe/D"   );
    tree->Branch("EYpDe"   ,  &Yp[9]    , "EYpDe/D"   );
    // VD of EDC1
    tree->Branch("ELFP1"   ,  &L[10]    , "ELFP1/D"   );
    tree->Branch("EPFP1"   ,  &P[10]    , "EPFP1/D"   );
    tree->Branch("EXFP1"   ,  &X[10]    , "EXFP1/D"   );
    tree->Branch("EYFP1"   ,  &Y[10]    , "EYFP1/D"   );
    tree->Branch("EZFP1"   ,  &Z[10]    , "EZFP1/D"   );
    tree->Branch("EXpFP1"  ,  &Xp[10]   , "EXpFP1/D"  );
    tree->Branch("EYpFP1"  ,  &Yp[10]   , "EYpFP1/D"  );
    tree->Branch("EXvFP1"  ,  &Xv[10]   , "EXvFP1/D"  );
    tree->Branch("EYvFP1"  ,  &Yv[10]   , "EYvFP1/D"  );
    tree->Branch("EZvFP1"  ,  &Zv[10]   , "EZvFP1/D"  );
    // VD of Reference Plane
    tree->Branch("ELFP2"   ,  &L[11]    , "ELFP2/D"   );
    tree->Branch("EPFP2"   ,  &P[11]    , "EPFP2/D"   );
    tree->Branch("EXFP2"   ,  &X[11]    , "EXFP2/D"   );
    tree->Branch("EYFP2"   ,  &Y[11]    , "EYFP2/D"   );
    tree->Branch("EZFP2"   ,  &Z[11]    , "EZFP2/D"   );
    tree->Branch("EXpFP2"  ,  &Xp[11]   , "EXpFP2/D"  );
    tree->Branch("EYpFP2"  ,  &Yp[11]   , "EYpFP2/D"  );
    tree->Branch("EXvFP2"  ,  &Xv[11]   , "EXvFP2/D"  );
    tree->Branch("EYvFP2"  ,  &Yv[11]   , "EYvFP2/D"  );
    tree->Branch("EZvFP2"  ,  &Zv[11]   , "EZvFP2/D"  );
    // VD of EDC2
    tree->Branch("ELFP3"   ,  &L[12]    , "ELFP3/D"   );
    tree->Branch("EPFP3"   ,  &P[12]    , "EPFP3/D"   );
    tree->Branch("EXFP3"   ,  &X[12]    , "EXFP3/D"   );
    tree->Branch("EYFP3"   ,  &Y[12]    , "EYFP3/D"   );
    tree->Branch("EZFP3"   ,  &Z[12]    , "EZFP3/D"   );
    tree->Branch("EXpFP3"  ,  &Xp[12]   , "EXpFP3/D"  );
    tree->Branch("EYpFP3"  ,  &Yp[12]   , "EYpFP3/D"  );
    tree->Branch("EXvFP3"  ,  &Xv[12]   , "EXvFP3/D"  );
    tree->Branch("EYvFP3"  ,  &Yv[12]   , "EYvFP3/D"  );
    tree->Branch("EZvFP3"  ,  &Zv[12]   , "EZvFP3/D"  );
    // VD of EHODO1
    tree->Branch("ELFP4"   ,  &L[13]    , "ELFP4/D"   );
    tree->Branch("EPFP4"   ,  &P[13]    , "EPFP4/D"   );
    tree->Branch("EXFP4"   ,  &X[13]    , "EXFP4/D"   );
    tree->Branch("EYFP4"   ,  &Y[13]    , "EYFP4/D"   );
    tree->Branch("EZFP4"   ,  &Z[13]    , "EZFP4/D"   );
    tree->Branch("EXpFP4"  ,  &Xp[13]   , "EXpFP4/D"  );
    tree->Branch("EYpFP4"  ,  &Yp[13]   , "EYpFP4/D"  );
    tree->Branch("EXvFP4"  ,  &Xv[13]   , "EXvFP4/D"  );
    tree->Branch("EYvFP4"  ,  &Yv[13]   , "EYvFP4/D"  );
    tree->Branch("EZvFP4"  ,  &Zv[13]   , "EZvFP4/D"  );
    // VD of EHODO2
    tree->Branch("ELFP5"   ,  &L[14]    , "ELFP5/D"   );
    tree->Branch("EPFP5"   ,  &P[14]    , "EPFP5/D"   );
    tree->Branch("EXFP5"   ,  &X[14]    , "EXFP5/D"   );
    tree->Branch("EYFP5"   ,  &Y[14]    , "EYFP5/D"   );
    tree->Branch("EZFP5"   ,  &Z[14]    , "EZFP5/D"   );
    tree->Branch("EXpFP5"  ,  &Xp[14]   , "EXpFP5/D"  );
    tree->Branch("EYpFP5"  ,  &Yp[14]   , "EYpFP5/D"  );
    tree->Branch("EXvFP5"  ,  &Xv[14]   , "EXvFP5/D"  );
    tree->Branch("EYvFP5"  ,  &Yv[14]   , "EYvFP5/D"  );
    tree->Branch("EZvFP5"  ,  &Zv[14]   , "EZvFP5/D"  );


  //===Drift Chamber===//
  tree->Branch("PEDCFP"   ,&EDCFPP ,   "PEDCFP/D" );
  tree->Branch("LEDCFP"   ,&EDCFPL ,   "LEDCFP/D" );
  tree->Branch("XEDCFP"   ,&EDCFPX ,   "XEDCFP/D" );
  tree->Branch("YEDCFP"   ,&EDCFPY ,   "YEDCFP/D" );
  tree->Branch("ZEDCFP"   ,&EDCFPZ ,   "ZEDCFP/D" );
  tree->Branch("XpEDCFP"  ,&EDCFPXp,   "XpEDCFP/D");
  tree->Branch("YpEDCFP"  ,&EDCFPYp,   "YpEDCFP/D");

  for (int i=0;i<10;i++){
    sprintf(sPEDC,"PEDC%d",i+1); 
    sprintf(sPEDCD,"PEDC%d/D",i+1); 
    sprintf(sXEDC,"XEDC%d",i+1); 
    sprintf(sXEDCD,"XEDC%d/D",i+1); 
    sprintf(sYEDC,"YEDC%d",i+1); 
    sprintf(sYEDCD,"YEDC%d/D",i+1); 
    sprintf(sZEDC,"ZEDC%d",i+1); 
    sprintf(sZEDCD,"ZEDC%d/D",i+1); 
    sprintf(sXpEDC,"XpEDC%d",i+1); 
    sprintf(sXpEDCD,"XpEDC%d/D",i+1); 
    sprintf(sYpEDC,"YpEDC%d",i+1); 
    sprintf(sYpEDCD,"YpEDC%d/D",i+1); 
    tree->Branch(sPEDC,    &EDC1P[i],   sPEDCD);
    tree->Branch(sXEDC,    &EDC1X[i],   sXEDCD);
    tree->Branch(sYEDC,    &EDC1Y[i],   sYEDCD);
    tree->Branch(sZEDC,    &EDC1Z[i],   sZEDCD);
    tree->Branch(sXpEDC,   &EDC1Xp[i],   sXpEDCD);
    tree->Branch(sYpEDC,   &EDC1Yp[i],   sYpEDCD);
  }
  for (int i=0;i<6;i++){
    sprintf(sPEDC,"PEDC%d",i+11); 
    sprintf(sPEDCD,"PEDC%d/D",i+11); 
    sprintf(sXEDC,"XEDC%d",i+11); 
    sprintf(sXEDCD,"XEDC%d/D",i+11); 
    sprintf(sYEDC,"YEDC%d",i+11); 
    sprintf(sYEDCD,"YEDC%d/D",i+11); 
    sprintf(sZEDC,"ZEDC%d",i+11); 
    sprintf(sZEDCD,"ZEDC%d/D",i+11); 
    sprintf(sXpEDC,"XpEDC%d",i+11); 
    sprintf(sXpEDCD,"XpEDC%d/D",i+11); 
    sprintf(sYpEDC,"YpEDC%d",i+11); 
    sprintf(sYpEDCD,"YpEDC%d/D",i+11); 
    tree->Branch(sPEDC,    &EDC2P[i],   sPEDCD);
    tree->Branch(sXEDC,    &EDC2X[i],   sXEDCD);
    tree->Branch(sYEDC,    &EDC2Y[i],   sYEDCD);
    tree->Branch(sZEDC,    &EDC2Z[i],   sZEDCD);
    tree->Branch(sXpEDC,   &EDC2Xp[i],   sXpEDCD);
    tree->Branch(sYpEDC,   &EDC2Yp[i],   sYpEDCD);
  }
  //===HES HodoScope===//
  for (int i=0;i<29;i++){

    sprintf(sPEH,"PEH%d",100+i+1); 
    sprintf(sPEHD,"PEH%d/D",100+i+1); 
    sprintf(sXEH,"XEH%d",100+i+1); 
    sprintf(sXEHD,"XEH%d/D",100+i+1); 
    sprintf(sYEH,"YEH%d",100+i+1); 
    sprintf(sYEHD,"YEH%d/D",100+i+1); 
    sprintf(sZEH,"ZEH%d",100+i+1); 
    sprintf(sZEHD,"ZEH%d/D",100+i+1); 
    sprintf(sXpEH,"XpEH%d",100+i+1); 
    sprintf(sXpEHD,"XpEH%d/D",100+i+1); 
    sprintf(sYpEH,"YpEH%d",100+i+1); 
    sprintf(sYpEHD,"YpEH%d/D",100+i+1); 
    sprintf(sAEH,"AEH%d",100+i+1); 
    sprintf(sAEHD,"AEH%d/D",100+i+1); 
    sprintf(sTEH,"TEH%d",100+i+1); 
    sprintf(sTEHD,"TEH%d/D",100+i+1); 
    tree->Branch(sPEH,    &EH1P[i],   sPEHD);
    tree->Branch(sXEH,    &EH1X[i],   sXEHD);
    tree->Branch(sYEH,    &EH1Y[i],   sYEHD);
    tree->Branch(sZEH,    &EH1Z[i],   sZEHD);
    tree->Branch(sXpEH,   &EH1Xp[i],   sXpEHD);
    tree->Branch(sYpEH,   &EH1Yp[i],   sYpEHD);
    tree->Branch(sAEH,    &EH1A[i],   sAEHD);
    tree->Branch(sTEH,    &EH1T[i],   sTEHD);

    sprintf(sPEH,"PEH%d",200+i+1); 
    sprintf(sPEHD,"PEH%d/D",200+i+1); 
    sprintf(sXEH,"XEH%d",200+i+1); 
    sprintf(sXEHD,"XEH%d/D",200+i+1); 
    sprintf(sYEH,"YEH%d",200+i+1); 
    sprintf(sYEHD,"YEH%d/D",200+i+1); 
    sprintf(sZEH,"ZEH%d",200+i+1); 
    sprintf(sZEHD,"ZEH%d/D",200+i+1); 
    sprintf(sXpEH,"XpEH%d",200+i+1); 
    sprintf(sXpEHD,"XpEH%d/D",200+i+1); 
    sprintf(sYpEH,"YpEH%d",200+i+1); 
    sprintf(sYpEHD,"YpEH%d/D",200+i+1); 
    sprintf(sAEH,"AEH%d",200+i+1); 
    sprintf(sAEHD,"AEH%d/D",200+i+1); 
    sprintf(sTEH,"TEH%d",200+i+1); 
    sprintf(sTEHD,"TEH%d/D",200+i+1); 
    tree->Branch(sPEH,    &EH2P[i],   sPEHD);
    tree->Branch(sXEH,    &EH2X[i],   sXEHD);
    tree->Branch(sYEH,    &EH2Y[i],   sYEHD);
    tree->Branch(sZEH,    &EH2Z[i],   sZEHD);
    tree->Branch(sXpEH,   &EH2Xp[i],   sXpEHD);
    tree->Branch(sYpEH,   &EH2Yp[i],   sYpEHD);
    tree->Branch(sAEH,    &EH2A[i],   sAEHD);
    tree->Branch(sTEH,    &EH2T[i],   sTEHD);
  }
  
  //===Trigger==// 
  tree->Branch("EQQDTrig",&QQDTrig ,"EQQDTrig/B");
  tree->Branch("EFPTrig" ,&FPTrig  ,"EFPTrig/B");
  tree->Branch("EDCTrig" ,&EDCTrig ,"EDCTrig/B");
  tree->Branch("EHTrig"  ,&EHTrig  ,"EHTrig/B");
  tree->Branch("ETrig"   ,&ETrig   ,"ETrig/B");
  tree->Branch("ETrig2"  ,&ETrig2  ,"ETrig2/B");
  tree->Branch("ETrig3"  ,&ETrig3  ,"ETrig3/B");
  
  }//if(Event Flag)
}

/////////////////////////////////////////////////////
void HESAnalysis::SetDataFile(const char *datafile )
/////////////////////////////////////////////////////
{
  DataFile_.open( datafile );
}

/////////////////////////////////////////////////////////////////
G4double HESAnalysis::RandGauss( G4double center, G4double dev )
/////////////////////////////////////////////////////////////////
{
  G4double rand1 = G4UniformRand();
  G4double rand2 = G4UniformRand();

  G4double a = sqrt(-2.0*log(rand1)) * cos(2.0*M_PI*rand2);

  return dev*a + center;
}
