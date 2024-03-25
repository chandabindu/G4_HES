#include "HESPrimaryGeneratorAction.hh"
#include "HESAnalysis.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleMomentum.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>
#include <iostream>
#include <cstdio>

#include "HESParamMan.hh"

using namespace CLHEP;
G4ParticleDefinition* particle;
/////////////////////////////////////////////////////////////
HESPrimaryGeneratorAction::
HESPrimaryGeneratorAction( HESDetectorConstruction* , HESAnalysis* analysisManager )
  :G4VUserPrimaryGeneratorAction(), anaMan(analysisManager)
/////////////////////////////////////////////////////////////
{
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  int pID = paramMan->GetParticleID();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  if ( pID==1 ){
    particle = particleTable->FindParticle("e-");
  }else if (pID==2){
    particle = particleTable->FindParticle("kaon+");
  }else{
    particle = particleTable->FindParticle("geantino");
  }
  dataFile=fopen( paramMan->GetInputDataName(), "r");

  CentMom = paramMan->GetCentMom()*GeV;
  AcptMom = paramMan->GetAcptMom()*GeV;
  CentTheta = paramMan->GetCentTheta()*radian;
  AcptTheta = paramMan->GetAcptTheta()*radian;
  CentPhi = paramMan->GetCentPhi()*radian;
  AcptPhi = paramMan->GetAcptPhi()*radian;
  Xi = paramMan->GetGenerateX()*cm;
  Yi = paramMan->GetGenerateY()*cm;  
  Zi = paramMan->GetGenerateZ()*cm;
  xR = paramMan->GetRasterX()*cm;
  yR = paramMan->GetRasterY()*cm;
  zR = paramMan->GetRasterZ()*cm;
  bsizex = paramMan->GetBSizeX()*mm;
  bsizey = paramMan->GetBSizeY()*mm;

  angFlag = paramMan->GetAngFlag(); // added by ishige 2023Feb21
  momFlag = paramMan->GetMomFlag(); // added by ishige 2023Feb21

}

/////////////////////////////////////////////////////////
HESPrimaryGeneratorAction::~HESPrimaryGeneratorAction()
////////////////////////////////////////////////////////
{
  if ( dataFile ) fclose( dataFile );
}

//Generator event action
//////////////////////////////////////////////////////////////////////
void HESPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
////////////////////////////////////////////////////////////////////
{
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  int generateID = paramMan->GetGenerationID();
  if ( generateID == 0 ) {
    return GenParUni( anEvent );
//    return GenParCalib( anEvent );
  }else if(generateID == 2){
    return GenerateBeam( anEvent );
  }else {
    return GenFromFile( anEvent );
  }
}

///////////////////////////////////////////////////////////////
void HESPrimaryGeneratorAction::GenerateBeam( G4Event* anEvent)
///////////////////////////////////////////////////////////////
{

  G4double prepx,prepy,prepz; 
  G4double px,py,pz; 
  G4double E_Theta = 0. *degree;
  
  G4double Mom = (CentMom + (G4UniformRand()-0.5)*2.0*AcptMom);
  G4double min = cos(CentTheta+AcptTheta);
  G4double max = cos(CentTheta-AcptTheta);
  G4double da = (max-min)/2. ;
  G4double a = min + da ;
  
  G4double theta=acos( a + ( 2.*da*(G4UniformRand()-0.5) ) );
  G4double phi=CentPhi+( 2*AcptPhi*( G4UniformRand()-0.5 ) );

  G4double xp=sin(theta)*cos(phi)/cos(theta);
  G4double yp=sin(theta)*sin(phi)/cos(theta);
  
  prepz = Mom/sqrt(1+xp*xp+yp*yp);
  prepx = prepz*xp;
  prepy = prepz*yp;
  px = prepx*cos(E_Theta)-prepz*sin(E_Theta);
  py = prepy;
  pz = prepx*sin(E_Theta)+prepz*cos(E_Theta);
  G4double prex = G4RandGauss::shoot(Xi, bsizex);
  G4double prey = G4RandGauss::shoot(Yi, bsizey);
  G4double prez = Zi + zR*(G4UniformRand()-0.5)*2./2.;
  G4double x = prex*cos(E_Theta)-prez*sin(E_Theta);
  G4double y = prey;
  G4double z = prex*sin(E_Theta)+prez*cos(E_Theta);
  G4ThreeVector gPos(x, y, z);
  G4ThreeVector gMom(px, py, pz);
  
  SetMom(gMom,gPos,anEvent,0);
}

/////////////////////////////////////////////////////////////////////
void HESPrimaryGeneratorAction::GenParUni( G4Event* anEvent )
/////////////////////////////////////////////////////////////////////
{

  G4double prepx,prepy,prepz; 
  G4double px,py,pz; 
  G4double E_Theta = 0. *degree;
  
  
  G4double Mom = (CentMom + (G4UniformRand()-0.5)*2.0*AcptMom);
  // random seed for pattern branching
  G4double ptnMom = CentMom;
  // 6 patterns to reproduce central momentum 
  if(momFlag == 1){
    G4cout << "rand%6 :" << rand()%6 << G4endl;
    switch(rand()%6){
      case 0:
        ptnMom = ptnMom + (0. - 1.) * 0.1 *GeV;
        break;
      case 1:
        ptnMom = ptnMom + (1. - 1.) * 0.1 *GeV;
        break;
      case 2:
        ptnMom = ptnMom + (2. - 1.) * 0.1 *GeV;
        break;
      case 3:
        ptnMom = ptnMom + (3. - 1.) * 0.1 *GeV;
        break;
      case 4:
        ptnMom = ptnMom + (4. - 1.) * 0.1 *GeV;
        break;
      case 5:
        ptnMom = ptnMom + (5. - 1.) * 0.1 *GeV;
        break;
    }

    Mom = ptnMom;
  }
  

  // G4cout << "Momentum :" << Mom <<G4endl;

  G4double min = cos(CentTheta+AcptTheta);
  G4double max = cos(CentTheta-AcptTheta);
  G4double da = (max-min)/2. ;
  G4double a = min + da ;
  
  G4double theta=acos( a + ( 2.*da*(G4UniformRand()-0.5) ) );
  G4double phi=CentPhi+( 2*AcptPhi*( G4UniformRand()-0.5 ) );

  if(angFlag == 1){
    G4cout << "rand%6 :" << rand()%6 << G4endl;
    switch(rand()%6){
      case 0:
        theta = CentTheta + (0. - 2.) * (2.*pi/180.) *radian;
        break;
      case 1:
        theta = CentTheta + (1. - 2.) * (2.*pi/180.) *radian;
        break;
      case 2:
        theta = CentTheta + (2. - 2.) * (2.*pi/180.) *radian;
        break;
      case 3:
        theta = CentTheta + (3. - 2.) * (2.*pi/180.) *radian;
        break;
      case 4:
        theta = CentTheta + (4. - 2.) * (2.*pi/180.) *radian;
        break;
      case 5:
        theta = CentTheta + (5. - 2.) * (2.*pi/180.) *radian;
        break;
    }
  }
  // G4cout << "theta: " << theta / degree << G4endl;


  G4double xp=sin(theta)*cos(phi)/cos(theta);
  G4double yp=sin(theta)*sin(phi)/cos(theta);
  
  prepz = Mom/sqrt(1+xp*xp+yp*yp);
  prepx = prepz*xp;
  prepy = prepz*yp;
  px = prepx*cos(E_Theta)-prepz*sin(E_Theta);
  py = prepy;
  pz = prepx*sin(E_Theta)+prepz*cos(E_Theta);
  G4double prex = xR*(G4UniformRand()-0.5)*2./2.;
  G4double prey = yR*(G4UniformRand()-0.5)*2./2.;
  G4double prez = Zi + zR*(G4UniformRand()-0.5)*2./2.;
  G4double x = prex*cos(E_Theta)-prez*sin(E_Theta);
  G4double y = prey;
  G4double z = prex*sin(E_Theta)+prez*cos(E_Theta);
  G4ThreeVector gPos(x, y, z);
  G4ThreeVector gMom(px, py, pz);
  
  SetMom(gMom,gPos,anEvent,0);

}
////////////////////////////////////////////////////////////////////
void HESPrimaryGeneratorAction::GenParCalib( G4Event *anEvent )
////////////////////////////////////////////////////////////////////
{
  G4int ID = anEvent->GetEventID();
  G4double Mom = (CentMom + (1. + (int(ID/5) - 2.)*0.05));
  G4ThreeVector gMom(0.,0.,Mom);
  gMom.rotateY(CentTheta + double(ID%5 - 2.)*degree);
  gMom.rotateZ(CentPhi);

  G4ThreeVector gPos(0,0,0);
  SetMom(gMom,gPos,anEvent,0);
}

////////////////////////////////////////////////////////////////////
void HESPrimaryGeneratorAction::GenFromFile( G4Event *anEvent )
////////////////////////////////////////////////////////////////////
{
  HESParamMan *paramMan = HESParamMan::GetParamMan();
  G4double E_Theta = 0.*degree;
  G4double CentMom = paramMan->GetCentMom();
  G4double xR = paramMan->GetRasterX();
  G4double yR = paramMan->GetRasterY();
  G4double zR = paramMan->GetRasterZ();
  char str[MAXCHAR];
  fgets( str, MAXCHAR, dataFile );
  
  G4double px, py, pz;
  G4double Mom=0.;
  G4double prepx; 
  G4double prepy; 
  G4double prepz; 
  G4double x0, y0, z0;
  G4double t, dp,pe, prob;
  G4double xp, yp;
  G4double xp0, yp0;
  G4double xpk,ypk,pk;
  G4double pbeam0,pbeam1;
  G4double Kpx, Kpy, Kpz;
  G4int evID = anEvent->GetEventID();

  if(evID == 0){
    for(int i=0; i<skipLine; i++){
      fgets(str, MAXCHAR, dataFile);
    }
  } 
  
  if(sscanf(str, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",&x0, &y0, &z0, &Kpx, &Kpy, &Kpz, &px, &py, &pz)==9){

  }else{
    std::cout << "*****I can't read seed file ...*****"<< std::endl;
  }

//   if (sscanf( str, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&evID,  &x0, &xp0, &y0, &yp0, &pe, &z0, &xpk, &ypk, &pk, &pbeam1, &pbeam0)==12){
//     //G4cout <<  "  ; ; ;; ;" << G4endl;
//     //G4cout << x0 << " " << y0 << " " << z0 << " "  << G4endl;
//     Mom = pe;
//     paramMan->SetpBeam(pbeam0,pbeam1);
//   }
//  else if (sscanf( str, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf",&evID,  &x0, &xp0, &y0, &yp0, &pe, &z0, &xpk, &ypk, &pk)==10){
//    //G4cout << " 10" << G4endl;
//     Mom = pe;
//   }
//   else if(sscanf( str, "%lf %lf %lf %lf %lf %lf %lf %lf",&x0, &xp0, &y0, &yp0, &z0, &t, &dp, &prob)==8){
//     Mom = CentMom*(1.+(dp/100.));
//   }
//   else if(sscanf( str, "%lf %lf %lf %lf %lf %lf %lf %lf",&x0, &y0, &z0, &Kpx, &Kpy, &Kpz, &px, &py, &pz)==9){

//   }
//   else{std::cout << "*****I can't read seed file ...*****"<< std::endl;}
 //G4cout << pbeam1 << " " << pbeam0 << G4endl;

  // xp = xp0/1000.;
  // yp = yp0/1000.;
  // prepz = (Mom/sqrt(1+xp*xp+yp*yp));
  // prepx = prepz*xp;
  // prepy = prepz*yp;
  // px = prepx*cos(E_Theta)-prepz*sin(E_Theta);
  // py = prepy;
  // pz = prepx*sin(E_Theta)+prepz*cos(E_Theta);
 
  // ~~~~~~~~ Original ~~~~~~~~~~~~~~~~
  //G4double x = xR*(x0*cos(E_Theta)-z0*sin(E_Theta));
  //G4double y = yR*y0;
  //G4double z = zR*(x0*sin(E_Theta)+z0*cos(E_Theta));
  
  // G4ThreeVector gMom( px*GeV, py*GeV, pz*GeV);
  // G4ThreeVector gPos( x0*cm, y0*cm, z0*cm);
  G4ThreeVector gMom( px*MeV, py*MeV, pz*MeV);
  G4ThreeVector gPos( x0*mm, y0*mm, z0*mm);
  // G4cout << "@ GenFromFile()" << G4endl;
  // G4cout << "gPos: " << gPos << G4endl;
  // G4cout << "gMom: " << gMom << G4endl;
  SetMom(gMom,gPos,anEvent,evID);
}

//////////////////////////////////////////
void HESPrimaryGeneratorAction::SetMom
( G4ParticleMomentum gMom, G4ThreeVector gPos, G4Event* anEvent, G4int evID)
//////////////////////////////////////////
{
  //G4cout << gMom.x()/GeV << gMom.y()/GeV << gMom.z()/GeV <<G4endl;

  G4int n_particle = 1;
  gunParticle = new G4ParticleGun(n_particle);
  gunParticle-> SetParticleEnergy(0.0); //reset
  gunParticle-> SetParticleDefinition(particle);
  gunParticle-> SetParticleMomentum(gMom);
  gunParticle-> SetParticlePosition(gPos);
  gunParticle-> GeneratePrimaryVertex(anEvent);

  if( anaMan ){
    anaMan->PrimaryGeneration( gPos, gMom, evID );
    //G4cout << gPos.x() << G4endl;
  }
  delete gunParticle;

}
