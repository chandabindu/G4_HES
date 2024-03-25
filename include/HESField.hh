/*
  HESField.hh

  D.Kawama
*/

#ifndef HESField_h

#define HESField_h 1

//#include "G4ElectroMagneticField.hh"
#include "G4MagneticField.hh"
#include "globals.hh"


#include "G4ThreeVector.hh"
#include <vector>

class BeamLineElement;

// class G4ChordFinder;
// class G4EqMagElectricField;
// class G4MagIntegratorStepper;

//class HESField : public G4ElectroMagneticField
class HESField : public G4MagneticField
{
public:
  HESField();
  //  explicit HESField( const G4String &fileName );
  ~HESField();
private:
  HESField( const HESField & );
  HESField & operator = ( const HESField & );

public:
  void GetFieldValue(const double Point[4], double* Bfield) const;

  void cleanupElemList( void );
  void AddElement( BeamLineElement *elem );

  void ResetQueue( void ) const;
  void PushInQueue( int val, const G4ThreeVector &pos ) const;
  void PrintQueue( void ) const;

private:
  typedef std::vector <BeamLineElement *> BLEContainer;
  typedef std::vector <BeamLineElement *>::const_iterator BLEIterator;

  BLEContainer elemList;

//   G4ChordFinder *fChordFinder;
//   G4EqMagElectricField *fEquation;
//   G4MagIntegratorStepper *fStepper;

  void MakeStepper( void );
  void DeleteStepper( void );

  mutable std::vector<int> fFlag;
  mutable std::vector<double> fPointX, fPointY, fPointZ;

};

#endif
