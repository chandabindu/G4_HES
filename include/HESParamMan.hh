#ifndef HESParamMan_h
#define HESParamMan_h 1

#define MAXCHAR 144

#include "globals.hh"

class HESParamMan
{
public:
  HESParamMan( const char *fileName );

private:
  static HESParamMan *thisPointer;

  bool ReadParam( void );

private:
  // Input file name
  const char *fileName;

  /////////////////////
  // FOR HESAnalysis //
  /////////////////////

  // Hbook file name
  char HbookFileName[MAXCHAR];
  char LogFileName[MAXCHAR];
  // Output file name
  char ErikaFileName[MAXCHAR]; // for ERIKA study
  char TulFileName[MAXCHAR]; // for grouping study
  char dumFileName[MAXCHAR]; // for dummy data

  ///////////////////////////////////
  // FOR HESPrimaryGeneratorAction //
  ///////////////////////////////////

  // Generation Type
  int GenerationID;
  // Particle Type
  int ParticleID;
  // input for dummy data
  char InputDataName[MAXCHAR];

  ///////////////
  // Field Map //
  ///////////////

  std::string DFieldMap;
  std::string PCSFieldMap;
  std::string Q1FieldMap;
  std::string Q2FieldMap;
  double PTheta; //Added by ishige 2022Sep28
  // double T2P;  //Added by ishige 2022Oct13
  double Pscale, Dscale, Q1scale, Q2scale;
  int Q1mapID, Q2mapID;
  double TiltAngle;
  double HesThetaAngle;
  double HesPhiAngle;
  double FPAngle;
  double CentMom;
  double AcptMom;
  double CentTheta;
  double AcptTheta;
  double CentPhi;
  double AcptPhi;

  double GeneX, GeneY, GeneZ;
  double RasterX, RasterY, RasterZ;
  double BSizeX, BSizeY; //mm
  double TargetThickness;
  std::string TargetMaterial;
  int EventFlag;
  int PhysicsFlag;
  int EDCFlag;
  int SSFlag;
  int AngFlag; // To make particles emitted at several-case fixed angle, Added by ishige 2023Feb21  
  int MomFlag; // To make particles emitted at several-case fixed momentum, Added by ishige 2023Feb21
  double p0,p1;// incident beam momentum

public:
  static HESParamMan *GetParamMan(){ return thisPointer; }

  char *GetHbookFileName( void ){ return HbookFileName; }
  char *GetErikaFileName( void ){ return ErikaFileName; }
  char *GetTulFileName  ( void ){ return TulFileName;   }
  char *GetDumFileName  ( void ){ return dumFileName;   }
  char *GetInputDataName( void ){ return InputDataName; }

  int GetGenerationID          ( void ){ return GenerationID;    }
  int GetParticleID            ( void ){ return ParticleID;      }
  std::string GetDFieldMap     ( void ){ return DFieldMap;       }
  double GetPCSScaleFactor     ( void ){ return Pscale;          }
  double GetPTheta             ( void ){ return PTheta;          } // Added by ishige 2022Sep28
  // double GetT2P                ( void ){ return T2P;             } // Added by ishige 2022Oct13
  double GetDScaleFactor       ( void ){ return Dscale;          }
  double GetQ1ScaleFactor      ( void ){ return Q1scale;         }
  double GetQ2ScaleFactor      ( void ){ return Q2scale;         }
  std::string GetPCSFieldMap   ( void ){ return PCSFieldMap;     }
  std::string GetQ1FieldMap    ( void ){ return Q1FieldMap;      }
  std::string GetQ2FieldMap    ( void ){ return Q2FieldMap;      }
  int GetQ1MapID               ( void ){ return Q1mapID;         }
  int GetQ2MapID               ( void ){ return Q2mapID;         }
  double GetTiltAngle          ( void ){ return TiltAngle;       }
  double GetHesThetaAngle      ( void ){ return HesThetaAngle;   }
  double GetHesPhiAngle        ( void ){ return HesPhiAngle;     }
  double GetFPAngle            ( void ){ return FPAngle;         }
  double GetCentMom            ( void ){ return CentMom;         }
  double GetAcptMom            ( void ){ return AcptMom;         }
  double GetCentTheta          ( void ){ return CentTheta;       }
  double GetAcptTheta          ( void ){ return AcptTheta;       }
  double GetCentPhi            ( void ){ return CentPhi;         }
  double GetAcptPhi            ( void ){ return AcptPhi;         }
  double GetRasterX            ( void ){ return RasterX;         }
  double GetRasterY            ( void ){ return RasterY;         }
  double GetRasterZ            ( void ){ return RasterZ;         }
  double GetGenerateX          ( void ){ return GeneX;           }
  double GetGenerateY          ( void ){ return GeneY;           }
  double GetGenerateZ          ( void ){ return GeneZ;           }
  double GetBSizeX             ( void ){ return BSizeX;           }
  double GetBSizeY             ( void ){ return BSizeY;           }
  double GetTargetThickness    ( void ){ return TargetThickness; }
  std::string GetTargetMaterial( void ){ return TargetMaterial;  }
  int GetEventFlag             ( void ){ return EventFlag;       }
  int GetPhysicsFlag           ( void ){ return PhysicsFlag;     }
  int GetEDCFlag               ( void ){ return EDCFlag;         }
  int GetSSFlag                ( void ){ return SSFlag;          }
  int GetAngFlag               ( void ){ return AngFlag;         } // added by ishige 2023Feb21
  int GetMomFlag               ( void ){ return MomFlag;         } // added by ishige 2023Feb21
  void SetpBeam(double bp0,double bp1){
    p0 = bp0;
    p1 = bp1;
  };
  double GetpBeam0(){ return p0; };
  double GetpBeam1(){ return p1; };

};

#endif
