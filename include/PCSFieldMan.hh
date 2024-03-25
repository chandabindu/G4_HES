#ifndef PCSFieldMan_h

#define PCSFieldMan_h 1

#include <string>
#include <vector>

#include "globals.hh"
#include "G4MagneticField.hh"

//class Q1FieldMan : public G4MagneticField
//{
//  public:
//    Q1FieldMan();
//    ~Q1FieldMan();
//
//
//    void GetFieldValue( const  double Point[3],
//                               double *Bfield ) const;
//
//  private:
//    G4double Bz;
//    };
//
//#endif
//original (FieldMap) 
class PCSFieldMan
{
private:
  std::string FieldMapFileName;
public:
  PCSFieldMan( const std::string & filename )
    : FieldMapFileName(filename), Nx(0), Ny(0), Nz(0)
  {}
  ~PCSFieldMan()  { cleanupMap(); }

  PCSFieldMan() {}
private:
  PCSFieldMan( const PCSFieldMan & );
  PCSFieldMan & operator = ( const PCSFieldMan & );

public:
  bool Initialize( void );
  double GetReff( double phi, double z=0. ) const;
  bool GetFieldValue( const double point[3], double *Bfield ) const; 

private:
  struct FD {
    float x, y, z;
  }; 
  typedef std::vector < std::vector < std::vector < FD > > > FDContainer;

  FDContainer B;
  // Initial point
  double X0, Y0, Z0;
  // cell size
  double dX, dY, dZ;
  // Number of cell
  int Nx, Ny, Nz;

  //  FILE *out;

  void cleanupMap( void );
};
#endif



