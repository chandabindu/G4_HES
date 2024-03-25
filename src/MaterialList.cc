/*
  MaterialList.cc

  D.Kawama 
*/

#include "MaterialList.hh"
#include "G4SystemOfUnits.hh"
using namespace CLHEP;
MaterialList::MaterialList()
{
  G4double a, iz, z, density;
  G4String name, symbol;
  G4int nel, nAtoms;
  G4double fractionMass;

  //=== Elements ===//
  name = "Hydrogen"; symbol = "H";
  a = 1.00794*g/mole;
  elH = new G4Element( name, symbol, iz=1., a);

  name = "Lithium"; symbol = "Li";
  a = 6.941*g/mole;
  elLi = new G4Element( name, symbol, iz=3., a);
  
  name = "Beryllium"; symbol = "Be";
  a = 9.012182*g/mole;
  elBe = new G4Element( name, symbol, iz=4., a);
  
  name = "Boron"; symbol = "B";
  a = 10.811*g/mole;
  elB = new G4Element( name, symbol, iz=5., a);

  name = "Carbon"; symbol = "C";
  a = 12.011*g/mole;
  elC = new G4Element( name, symbol, iz=6., a);

  name = "Nitrogen"; symbol = "N";
  a = 14.00674*g/mole;
  elN = new G4Element( name, symbol, iz=7., a);

  name = "Oxygen"; symbol = "O";
  a = 15.9994*g/mole;
  elO = new G4Element( name, symbol, iz=8., a);

  name = "Silicon"; symbol = "Si";
  a = 28.0855*g/mole;
  elSi = new G4Element( name, symbol, iz=14., a);

  name = "Argon"; symbol = "Ar";
  a = 39.948*g/mole;
  elAr = new G4Element( name, symbol, iz=18., a);
  
  name = "Calcium"; symbol = "Ca";
  a = 40.078*g/mole;
  elCa = new G4Element( name, symbol, iz=20., a);
  
  name = "Chromium"; symbol = "Cr"; //Added by Gogami
  a = 51.996*g/mole;
  elCr = new G4Element( name, symbol, iz=24., a);

  name = "Manganese"; symbol = "Mn";//Added by Gogami
  a = 54.938*g/mole;
  elMn = new G4Element( name, symbol, iz=25., a);

  name = "Iron"; symbol = "Fe";    //Added by Gogami
  a = 55.845*g/mole;
  elFe = new G4Element( name, symbol, iz=26., a);

  name = "Cobalt"; symbol = "Co";  //Added by Gogami
  a = 58.933*g/mole;
  elCo = new G4Element( name, symbol, iz=27., a);

  name = "Nickel"; symbol = "Ni";
  a = 58.69*g/mole;
  elNi = new G4Element( name, symbol, iz=28., a);

  name = "Copper"; symbol = "Cu";
  a = 63.546*g/mole;
  elCu= new G4Element( name, symbol, iz=29., a);

  name = "Molybd"; symbol = "Mo";  //Added by Gogami
  a = 95.94*g/mole;
  elMo = new G4Element( name, symbol, iz=42., a);

  name = "Tungsten"; symbol = "W";
  a = 183.84*g/mole;
  elW = new G4Element( name, symbol, iz=74., a );

  //=== Simple Materials ==//
  name = "Tungsten";
  a = 183.84*g/mole;
  density = 19.3*g/cm3;
  W = new G4Material( name, z=74., a, density);
  
  name = "Aluminuium";
  a = 26.98*g/mole;
  density = 2.70*g/cm3;
  Al = new G4Material( name, z=13., a, density);

  name = "Iron";
  a = 55.843*g/mole;
  density = 7.86*g/cm3;
  Fe = new G4Material( name, z=26., a, density);

  name = "Cupper";
  a = 63.546*g/mole;
  density = 8.93*g/cm3;
  Cu = new G4Material( name, z=29., a, density);

  name = "Carbon";
  a = 12.0*g/mole;
  //density = 2.25*g/cm3; // original
  density = 1.75*g/cm3; // real measurement in E05-115, Toshi(16Dec2013)
  C12 = new G4Material( name, z=6., a, density);

  name = "Silicon";
  a = 28.0*g/mole;
  density = 2.33*g/cm3;
  Si28 = new G4Material( name, z=14., a, density);

  name = "Vanadium";
  a = 51.0*g/mole;
  density = 6.11*g/cm3;
  V51 = new G4Material( name, z=23., a, density);

  name = "Cromium";
  a = 52.0*g/mole;
  density = 7.15*g/cm3;
  Cr52 = new G4Material( name, z=24., a, density);

  name = "Yttrium";
  a = 89.0*g/mole;
  density = 4.469*g/cm3;
  Y89 = new G4Material( name, z=39., a, density);
  
  name = "Lithium";
  a = 7.0*g/mole;
  density = 0.534*g/cm3;
  Li7 = new G4Material( name, z=3., a, density);
  
  name = "Berylium"; //Added by Gogami
  a = 9.012*g/mole;
  density = 1.848*g/cm3;
  Be9 = new G4Material( name, z=4., a, density);
  
  name = "Boron";
  a = 10.0*g/mole;
  density = 2.340*g/cm3;
  B10 = new G4Material( name, z=5., a, density);
  
  name = "Calcium";
  a = 40.0*g/mole;
  density = 1.55*g/cm3;
  Ca40 = new G4Material( name, z=20., a, density);

  name = "Lead";
  a = 208.0*g/mole;
  density = 11.342*g/cm3;
  Pb208 = new G4Material( name, z=82., a, density);

  name = "HeliumGas";
  a = 4.002602*g/mole;
  //  density = 0.167*g/cm3;
  density = 0.1786*mg/cm3;
  HeGas = new G4Material( name, z=2., a, density );

  name = "Argon Gas";
  a = 39.948*g/mole;
  density = 0.001782*g/cm3;
  ArGas = new G4Material( name, z=18., a, density );

  name = "Nitrogen Gas";
  //a = 28.01*g/mole; // wrong
  a = 14.01*g/mole; // correct, Noticed by S.Nagao (2014/11/13)
  density = 1.251*mg/cm3;
  N2Gas = new G4Material( name, z=7., a, density );

  name = "Oxygen Gas";
  //a = 32.00*g/mole; // wrong
  a = 16.00*g/mole; // correct, Noticed by S.Nagao (2014/11/13)
  density = 1.429*mg/cm3;
  O2Gas = new G4Material( name, z=8., a, density );

  //=== Componds Materials ===//
  name = "Mylar";
  density = 1.39*g/cm3;
  Mylar = new G4Material( name, density, nel=3 );
  Mylar->AddElement( elH, nAtoms=8 );
  Mylar->AddElement( elC, nAtoms=10 );
  Mylar->AddElement( elO, nAtoms=4 );
  
  name = "Kevlar";
  density = 0.74*g/cm3;
  Kevlar = new G4Material( name, density, nel=4 );
  Kevlar->AddElement( elH, nAtoms=10 );
  Kevlar->AddElement( elC, nAtoms=14 );
  Kevlar->AddElement( elO, nAtoms=2 );
  Kevlar->AddElement( elN, nAtoms=2 );

  name = "Scintillator";
  density = 1.032*g/cm3;
  Scinti = new G4Material( name, density, nel=2);
  Scinti->AddElement(elH, nAtoms=8);
  Scinti->AddElement(elC, nAtoms=8);

  name = "Aerogel";
  density = 0.06*g/cm3;
  AC = new G4Material( name, density, nel=2);
  AC->AddElement(elSi, nAtoms=1);
  AC->AddElement(elO,  nAtoms=2);

  name = "Water";
  density = 1.*g/cm3;
  Water = new G4Material( name, density, nel=2);
  Water->AddElement(elH, nAtoms=1);
  Water->AddElement(elO, nAtoms=2);

  name = "Etane";
  //  density = 0.509*g/cm3;
  density = 1.356*mg/cm3;
  C2H6 = new G4Material( name, density, nel=2);
  C2H6->AddElement(elC, nAtoms=2);
  C2H6->AddElement(elH, nAtoms=6);
  
  name = "Polyethylene";
  density = 0.93*g/cm3;
  CH2 = new G4Material( name, density, nel=2);
  CH2->AddElement(elC, nAtoms=1);
  CH2->AddElement(elH, nAtoms=2);

  density= universe_mean_density;
  Vacuum= new G4Material(name="Vacuum", z=1, a=1.01*g/mole, density, kStateGas, 300*kelvin, 3.e-18*pascal);
  //Vacuum= new G4Material(name="Vacuum", density, nel=2);
  //   Vacuum-> AddElement(elN, .7);
  //   Vacuum-> AddElement(elO, .3);
  //Vacuum->AddMaterial( N2Gas, .78 );
  //Vacuum->AddMaterial( O2Gas, .22 );
  
  name = "Air";
  density = 1.293*mg/cm3;
  Air = new G4Material( name, density, nel=2);
  //   Air->AddElement(elN, .7);
  //   Air->AddElement(elO, .3);
  Air->AddMaterial( N2Gas, .78 );
  Air->AddMaterial( O2Gas, .22 );
  
  name = "Ar+C2H6";
  density = (0.5*39.948+0.5*(2*12.011+6*1.00794))/22.3*mg/cm3;
  DCGas = new G4Material( name, density, nel=2);
  fractionMass = 39.948/(39.948+2*12.011+6*1.00794);
  DCGas->AddMaterial(ArGas, fractionMass);
  fractionMass = (2*12.011+6*1.00794)/(39.948+2*12.011+6*1.00794);
  DCGas->AddMaterial(C2H6, fractionMass);

  /*name = "EDCLayer";
  density = 1.39*g/cm3;
  EDCLayer = new G4Material( name, density, nel=3 );
  EDCLayer->AddElement( elH, nAtoms=8 );
  EDCLayer->AddElement( elC, nAtoms=10 );
  EDCLayer->AddElement( elO, nAtoms=4 );*/

  name = "Heavy Metal";
  density = 16.9*g/cm3;
  Heavymet = new G4Material( name, density, nel=3 );
  Heavymet->AddElement( elW, .9 );
  Heavymet->AddElement(elNi, .06 );
  Heavymet->AddElement(elCu, .04 );

  name = "Harver";    //Added by Gogami
  density = 8.3*g/cm3;
  Harver = new G4Material( name, density, nel=7 );
  Harver->AddElement(elCr, .200 );
  Harver->AddElement(elMn, .016 );
  Harver->AddElement(elFe, .181 );
  Harver->AddElement(elCo, .425 );
  Harver->AddElement(elNi, .130 );
  Harver->AddElement(elMo, .020 );
  Harver->AddElement(elW , .028 );
}

MaterialList::~MaterialList()
{
  delete Harver;//Added by Gogami
  delete Heavymet;
  delete DCGas;
  delete Air;
  delete Vacuum;

  delete C2H6;
  delete CH2;
  delete Water;
  delete AC;
  delete Scinti;

  delete Li7;//Added by Gogami
  delete Be9;//Added by Gogami
  delete B10;//Added by Gogami
  delete C12;
  delete Si28;
  delete V51;
  delete Cr52;
  delete Cu;
  delete Fe;
  delete Al;
  delete Y89;
  delete Pb208;
  delete W;
  delete Mylar;
  delete HeGas;
  delete Kevlar;
  delete ArGas;
  delete N2Gas;
  delete O2Gas;

  delete elH;
  delete elLi;
  delete elBe;
  delete elB;
  delete elC;
  delete elN;
  delete elO;
  delete elSi;
  delete elAr;
  delete elCa;
  delete elCr;//Added by Gogami
  delete elMn;//Added by Gogami
  delete elFe;//Added by Gogami
  delete elCo;//Added by Gogami
  delete elNi;
  delete elCu;
  delete elMo;//Added by Gogami
  delete elW;
}
