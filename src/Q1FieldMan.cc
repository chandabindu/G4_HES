/*
  Q1FieldMan.cpp

  Field Map Manager for Tosca Fields

*/

#include "Q1FieldMan.hh"
#include <CLHEP/Random/RandGaussQ.h>

#include "stdlib.h" 
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cmath>

const double Deg2Rad = M_PI/180.;
const double Rad2Deg = 180./M_PI;

const int MaxChar = 144;

///////////////////////////////////
bool Q1FieldMan::Initialize( void )
//////////////////////////////////
{
  static const std::string funcname = "Q1FieldMan::Initialize";
  FILE *fp;
  char str[MaxChar];
  double x, y, z, bx, by, bz;
  
  if( (fp=fopen(FieldMapFileName.c_str(),"r"))==0 ){
    std::cerr << "[" << funcname << "]: file open fail" << std::endl;
    exit(-1);
  }
  cleanupMap();

  if( fgets(str,MaxChar,fp)==0 ){
    std::cerr << "[" << funcname << "]: Error in reading file " << std::endl;
    fclose(fp);
    exit(-1);
  }    
  else if( sscanf(str,"%d %d %d %lf %lf %lf %lf %lf %lf",&Nx,&Ny,&Nz,
	     &X0,&Y0,&Z0,&dX,&dY,&dZ)!=9 ){
    std::string mes=str;
    std::cerr << "[" << funcname << "]: Invalid format [0] ==> "
	      << mes << std::endl;
    fclose(fp);
    exit(-1);
  }

  B.resize(Nx);
  for( int ix=0; ix<Nx; ++ix ){
    B[ix].resize(Ny);
    for( int iy=0; iy<Ny; ++iy ){
      B[ix][iy].resize(Nz);
    }
  }

  while( fgets(str,MaxChar,fp)!=0){
    if(str[0]!='#'){
      if( sscanf(str," %lf %lf %lf %lf %lf %lf",
		 &x,&y,&z,&bx,&by,&bz) ==6 ){
	int ix = int((x-X0+0.5*dX)/dX);
	int iy = int((y-Y0+0.5*dY)/dY);
	int iz = int((z-Z0+0.5*dZ)/dZ);
	if( ix<0 || ix>=Nx || iy<0 || iy>=Ny || iz<0 || iz>=Nz ){
	  std::string mes=str;
	  std::cerr << "[" << funcname << "]: Invalid Range ==> "
		    << mes << std::endl;
	  std::cout << ix  << "," << iy << ","
		    << iz  << std::endl;
	  std::cout << Ny  << "," << dY << ","
		    << Y0  << std::endl;

	}
	else{
	  B[ix][iy][iz].x = bx;
	  B[ix][iy][iz].y = by;
	  B[ix][iy][iz].z = bz;
	}
      }
      else {
	std::string mes=str;
	std::cerr << "[" << funcname << "]: Invalid format [1] ==> "
		  << mes << std::endl;
      }	/* if( sscanf... ) */

    }   /* if(str[0]...) */

  }     /* while( fgets... ) */

  fclose(fp);

  return true;
}

//////////////////////////////////////////////////////////////
bool Q1FieldMan::
GetFieldValue( const double point[3], double *Bfield ) const
//////////////////////////////////////////////////////////////
{
  static const std::string funcname = "Q1FieldMan::GetFieldValue";
  double xt = point[0];
  double zt = point[2];
  double yt = point[1];
  int Xflag = 1;
  int Yflag = 1;
  int Zflag = 1;
  /*if( point[0]>=0 ){
    xt = point[0];
  }
  else if( point[0]<0 ){
    xt = -point[0];
  }*/
  if( point[1]>=0 ){
    yt = point[1];
  }
  else if( point[1]<0 ){
    yt = -point[1];
     Xflag = -1;
     Zflag = -1;
  }
  /*if( point[2]>=0 ){
    zt = point[2];
  }
  else if( point[2]<0 ){
    zt = -point[2];
  }*/
  
  int ix1, iy1, iz1, ix2, iy2, iz2;
  
  ix1=int( (xt-X0)/dX );
  iy1=int( (yt-Y0)/dY );
  iz1=int( (zt-Z0)/dZ );

  double wx1, wx2, wy1, wy2, wz1, wz2;
  if( ix1<0 ) {
    ix1=ix2=0; wx1=1.; wx2=0.;
  }
  else if( ix1>=Nx-1 ) {
    ix1=ix2=Nx-1; wx1=1.; wx2=0.;
  }
  else {
    ix2=ix1+1; wx1=(X0+dX*ix2-xt)/dX; wx2=1.-wx1;
  }
  if( iy1<0 ) {
    iy1=iy2=0; wy1=1.; wy2=0.;
  }
  else if( iy1>=Ny-1 ) {
    iy1=iy2=Ny-1; wy1=1.; wy2=0.;
  }
  else {
    iy2=iy1+1; wy1=(Y0+dY*iy2-yt)/dY; wy2=1.-wy1;
  }
  if( iz1<0 ) {
    iz1=iz2=0; wz1=1.; wz2=0.;
  }
  else if( iz1>=Nz-1 ) {
    iz1=iz2=Nz-1; wz1=1.; wz2=0.;
  }
  else {
    iz2=iz1+1; wz1=(Z0+dZ*iz2-zt)/dZ; wz2=1.-wz1;
  }

  double bx1=wx1*wy1*B[ix1][iy1][iz1].x+wx1*wy2*B[ix1][iy2][iz1].x
            +wx2*wy1*B[ix2][iy1][iz1].x+wx2*wy2*B[ix2][iy2][iz1].x;
  double bx2=wx1*wy1*B[ix1][iy1][iz2].x+wx1*wy2*B[ix1][iy2][iz2].x
            +wx2*wy1*B[ix2][iy1][iz2].x+wx2*wy2*B[ix2][iy2][iz2].x;
  double bx=wz1*bx1+wz2*bx2;

  double by1=wx1*wy1*B[ix1][iy1][iz1].y+wx1*wy2*B[ix1][iy2][iz1].y
            +wx2*wy1*B[ix2][iy1][iz1].y+wx2*wy2*B[ix2][iy2][iz1].y;
  double by2=wx1*wy1*B[ix1][iy1][iz2].y+wx1*wy2*B[ix1][iy2][iz2].y
            +wx2*wy1*B[ix2][iy1][iz2].y+wx2*wy2*B[ix2][iy2][iz2].y;
  double by=wz1*by1+wz2*by2;
  double bz1=wx1*wy1*B[ix1][iy1][iz1].z+wx1*wy2*B[ix1][iy2][iz1].z
            +wx2*wy1*B[ix2][iy1][iz1].z+wx2*wy2*B[ix2][iy2][iz1].z;
  double bz2=wx1*wy1*B[ix1][iy1][iz2].z+wx1*wy2*B[ix1][iy2][iz2].z
            +wx2*wy1*B[ix2][iy1][iz2].z+wx2*wy2*B[ix2][iy2][iz2].z;
  double bz=wz1*bz1+wz2*bz2;
  


  Bfield[0]= Xflag*bx;
  Bfield[1]= Yflag*by;
  Bfield[2]= Zflag*bz;

  return true;
}



const double RadiiEffectiveEdge = 60.0;

/////////////////////////////////////////////////////////
double Q1FieldMan::GetReff( double, double) const
////////////////////////////////////////////////////////
{
  return RadiiEffectiveEdge;
}

///////////////////////////////////
void Q1FieldMan::cleanupMap( void )
///////////////////////////////////
{
  for( int ix=0; ix<Nx; ++ix ){
    for( int iy=0; iy<Ny; ++iy ){
      B[ix][iy].clear();
    }
    B[ix].clear();
  }
  B.clear();
}


