/*
   Copyright 2012 University of Washington

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef _COMETDATAINTERNAL_H_
#define _COMETDATAINTERNAL_H_

#include "CometData.h"
#include "Threading.h"
#include <chrono>

class CometSearchManager;

#define PROTON_MASS                 1.00727646688
#define C13_DIFF                    1.00335483

#define FLOAT_ZERO                  1e-6     // 0.000001
#define MIN_PEPTIDE_LEN             1        // min # of AA for a petpide
#define MAX_PEPTIDE_LEN             64       // max # of AA for a peptide; one less to account for terminating char
#define MAX_PEPTIDE_LEN_P2          66       // max # of AA for a peptide plus 2 for N/C-term
#define MAX_PEFFMOD_LEN             16
#define SIZE_MASS                   128      // ascii value size
#define SIZE_NATIVEID               256      // max length of nativeID string
#define NUM_SP_IONS                 1000     // num ions for preliminary scoring
#define NUM_ION_SERIES              7        // a,b,c,x,y,z,z1
#define DECOY_SIZE                  3000     // number of decoy entries in CometDecoys.h
#define BIN_MOD_COUNT               10       // size of 4th dimension of uiBinnedIonMasses; covers unmodified ions (0), mod NL (1-9)

#define WIDTH_REFERENCE             512      // length of the protein accession field to store
#define MAX_PROTEINS                50       // maximum number of proteins to return for each query; for index search only right now

#define HISTO_SIZE                  152      // some number greater than 150; chose 152 for byte alignment?

#define NO_PEFF_VARIANT             -127

#define VMODS                       9
#define VMOD_1_INDEX                0
#define VMOD_2_INDEX                1
#define VMOD_3_INDEX                2
#define VMOD_4_INDEX                3
#define VMOD_5_INDEX                4
#define VMOD_6_INDEX                5
#define VMOD_7_INDEX                6
#define VMOD_8_INDEX                7
#define VMOD_9_INDEX                8

#define ENZYME_SINGLE_TERMINI       1
#define ENZYME_DOUBLE_TERMINI       2
#define ENZYME_N_TERMINI            8
#define ENZYME_C_TERMINI            9

#define ION_SERIES_A                0
#define ION_SERIES_B                1
#define ION_SERIES_C                2
#define ION_SERIES_X                3
#define ION_SERIES_Y                4
#define ION_SERIES_Z                5
#define ION_SERIES_Z1               6  //z+1

#ifdef CRUX
#define XCORR_CUTOFF                -999.0
#else
#define XCORR_CUTOFF                1E-8   // some near-zero cutoff
#endif

struct msdata                    // used in the preprocessing
{
   double dIon;
   double dIntensity;
};

struct Options             // output parameters
{
   int iNumPeptideOutputLines;
   int iWhichReadingFrame;
   int iEnzymeTermini;
   int iNumStored;               // # of search results to store for xcorr analysis
   int iMaxDuplicateProteins;    // maximum number of duplicate proteins to report or store in idx file
   int iSpectrumBatchSize;       // # of spectra to search at a time within the scan range
   int iStartCharge;
   int iEndCharge;
   int iMaxFragmentCharge;
   int iMaxPrecursorCharge;
   int iMSLevel;                 // mzXML only
   int iMinPeaks;
   int iRemovePrecursor;         // 0=no, 1=yes, 2=ETD precursors, 3=phosphate neutral loss
   int iDecoySearch;             // 0=no, 1=concatenated search, 2=separate decoy search
   int iNumThreads;              // 0=poll CPU else set # threads to spawn
   int bOutputSqtStream;
   int bOutputSqtFile;
   int bOutputTxtFile;
   int bOutputPepXMLFile;
   int bOutputMzIdentMLFile;
   int bOutputPercolatorFile;
   int bOutputOutFiles;
   int bClipNtermMet;            // 0=leave protein sequences alone; 1=also consider w/o N-term methionine
   int bClipNtermAA;             // 0=leave peptide sequences as-is; 1=clip N-term amino acid from every peptide
   int bPinModProteinDelim;      // 0=default pin output format; 1=change protein delimiter to comma
   int bSkipAlreadyDone;         // 0=search everything; 1=don't re-search if .out exists
// int bSkipUpdateCheck;         // 0=do not check for updates; 1=check for updates
   int bMango;                   // 0=normal; 1=Mango x-link ms2 input
   int bScaleFragmentNL;         // 0=no; 1=scale fragment NL for each modified residue contained in fragment
   int bCreateIndex;             // 0=normal search; 1=create peptide index file
   int bVerboseOutput;
   int bShowFragmentIons;
   int bExplicitDeltaCn;         // if set to 1, do not use sequence similarity logic 
   int bPrintExpectScore;
   int bOverrideCharge;
   int bCorrectMass;             // use selectionMZ instead of monoMZ if monoMZ is outside selection window
   int bTreatSameIL;
   int iMaxIndexRunTime;         // max run time of index search in milliseconds
   long lMaxIterations;          // max # of modification permutations for each iStart position
   double dMinIntensity;
   double dRemovePrecursorTol;
   double dPeptideMassLow;       // MH+ mass
   double dPeptideMassHigh;      // MH+ mass
   double dMinimumXcorr;         // set the minimum xcorr to report (default is 1e-8)
   IntRange scanRange;
   IntRange peptideLengthRange;
   DoubleRange clearMzRange;
   char szActivationMethod[24];  // mzXML only

   Options& operator=(Options& a)
   {
      iNumPeptideOutputLines = a.iNumPeptideOutputLines;
      iWhichReadingFrame = a.iWhichReadingFrame;
      iEnzymeTermini = a.iEnzymeTermini;
      iNumStored = a.iNumStored;
      iMaxDuplicateProteins = a.iMaxDuplicateProteins;
      iSpectrumBatchSize = a.iSpectrumBatchSize;
      iStartCharge = a.iStartCharge;
      iEndCharge = a.iEndCharge;
      iMaxFragmentCharge = a.iMaxFragmentCharge;
      iMaxPrecursorCharge = a.iMaxPrecursorCharge ;
      iMSLevel = a.iMSLevel;
      iMinPeaks = a.iMinPeaks;
      iRemovePrecursor = a.iRemovePrecursor;
      iDecoySearch = a.iDecoySearch;
      iNumThreads = a.iNumThreads;
      bOutputSqtStream = a.bOutputSqtStream;
      bOutputSqtFile = a.bOutputSqtFile;
      bOutputTxtFile = a.bOutputTxtFile;
      bOutputPepXMLFile = a.bOutputPepXMLFile;
      bOutputMzIdentMLFile = a.bOutputMzIdentMLFile;
      bOutputPercolatorFile = a.bOutputPercolatorFile;
      bOutputOutFiles = a.bOutputOutFiles;
      bClipNtermMet = a.bClipNtermMet;
      bClipNtermAA = a.bClipNtermAA;
      bPinModProteinDelim = a.bPinModProteinDelim;
      bSkipAlreadyDone = a.bSkipAlreadyDone;
//    bSkipUpdateCheck = a.bSkipUpdateCheck;
      bMango = a.bMango;
      bScaleFragmentNL = a.bScaleFragmentNL;
      bCreateIndex = a.bCreateIndex;
      bVerboseOutput = a.bVerboseOutput;
      bShowFragmentIons = a.bShowFragmentIons;
      bExplicitDeltaCn = a.bExplicitDeltaCn;
      bPrintExpectScore = a.bPrintExpectScore;
      bOverrideCharge = a.bOverrideCharge;
      bCorrectMass = a.bCorrectMass;
      bTreatSameIL = a.bTreatSameIL;
      iMaxIndexRunTime = a.iMaxIndexRunTime;
      lMaxIterations = a.lMaxIterations;
      dMinIntensity = a.dMinIntensity;
      dRemovePrecursorTol = a.dRemovePrecursorTol;
      dPeptideMassLow = a.dPeptideMassLow;
      dPeptideMassHigh = a.dPeptideMassHigh;
      dMinimumXcorr = a.dMinimumXcorr;
      scanRange = a.scanRange;
      peptideLengthRange = a.peptideLengthRange;
      clearMzRange = a.clearMzRange;
      strcpy(szActivationMethod, a.szActivationMethod);

      return *this;
   }
};

struct Results
{
   double dPepMass;
   double dExpect;
   float  fScoreSp;
   float  fXcorr;
   int    iLenPeptide;
   int    iRankSp;
   int    iMatchedIons;
   int    iTotalIons;
   comet_fileoffset_t   lProteinFilePosition;
   int    piVarModSites[MAX_PEPTIDE_LEN_P2];   // store variable mods encoding, +2 to accomodate N/C-term
   double pdVarModSites[MAX_PEPTIDE_LEN_P2];   // store variable mods mass diffs, +2 to accomodate N/C-term
   char   pszMod[MAX_PEPTIDE_LEN][MAX_PEFFMOD_LEN];    // store PEFF mod string
   char   szPeptide[MAX_PEPTIDE_LEN];
   char   szPrevNextAA[2];                    // [0] stores prev AA, [1] stores next AA
   bool   bClippedM;                          // true if new N-term protein due to clipped methionine
   string strSingleSearchProtein;             // used only in single spectrum search to return protein name from index file
   string sPeffOrigResidues;                  // original residue(s) of a PEFF variant
   int    iPeffOrigResiduePosition;           // position of PEFF variant substitution; -1 = n-term, iLenPeptide = c-term; -9=unused
   int    iPeffNewResidueCount;               // more than 0 new residues is a substitution (if iPeffOrigResidueCount=1) or insertion (if iPeffOrigResidueCount>1)
   vector<struct ProteinEntryStruct> pWhichProtein;       // file positions of matched protein entries
   vector<struct ProteinEntryStruct> pWhichDecoyProtein;  // keep separate decoy list (used for separate decoy matches and combined results)
};

struct PepMassInfo
{
   double dCalcPepMass;
   double dExpPepMass;                        // protonated MH+ experimental mass
   double dPeptideMassTolerance;
   double dPeptideMassToleranceMinus;
   double dPeptideMassTolerancePlus;
};

struct SpectrumInfoInternal
{
   int    iArraySize;         // m/z versus intensity array
   int    iHighestIon;
   int    iScanNumber;
   int    iChargeState;
   int    iMaxFragCharge;
   double dTotalIntensity;
   double dRTime;
   char   szMango[32];                // Mango encoding
   char   szNativeID[SIZE_NATIVEID];  // nativeID string from mzML
};

// The minimum and maximum mass range of all peptides to consider
// i.e. lowestPepMass - tolerance to highestPepMass + tolerance
struct MassRange
{
   double dMinMass;
   double dMaxMass;
   int    iMaxFragmentCharge;  // global maximum fragment charge
   bool   bNarrowMassRange;    // used to determine how to parse peptides in SearchForPeptides
};

extern MassRange g_massRange;

// PreprocessStruct stores information used in preprocessing
// each spectrum.  Information not kept around otherwise
struct PreprocessStruct
{
   int    iHighestIon;
   double dHighestIntensity;
   struct msdata pTmpSpData[NUM_SP_IONS];
};

struct OBOStruct           // stores info read from OBO file
{
   double dMassDiffAvg;    // this is looked up from strMod string from OBO
   double dMassDiffMono;
   string strMod;          // mod string, PSI-MOD, Unimod or custom

   bool operator<(const OBOStruct& a) const
   {
      return (strMod < a.strMod);
   }

};

struct ProteinEntryStruct
{
   comet_fileoffset_t   lWhichProtein;     // file pointer to protein
   int    iStartResidue;      // start residue position in protein (1-based)
   char   cPrevAA;
   char   cNextAA;

   bool operator<(const ProteinEntryStruct& a) const
   {
      return (lWhichProtein < a.lWhichProtein);
   }
};

struct PeffModStruct       // stores info read from PEFF header
{
   double dMassDiffAvg;    // this is looked up from strMod string from OBO
   double dMassDiffMono;
   int    iPosition;       // position of modification
   char   szMod[MAX_PEFFMOD_LEN];

   bool operator<(const PeffModStruct& a) const
   {
      return (iPosition < a.iPosition);
   }
};

struct PeffVariantSimpleStruct  // stores info read from PEFF header
{
   int    iPosition;       // position of variant
   char   cResidue;        // new variant

   bool operator<(const PeffVariantSimpleStruct& a) const
   {
      return (iPosition < a.iPosition);
   }
};

struct PeffVariantComplexStruct  // stores info read from PEFF header
{
  int    iPositionA;       // start position of variant
  int    iPositionB;       // end position of variant
  string sResidues;        // if !empty(), insertion replacing aa from pos A to B; 
                           // if empty(), deletion of aa from pos A to B

  bool operator<(const PeffVariantComplexStruct& a) const
  {
    return (iPositionA < a.iPositionA);
  }
};

struct PeffProcessedStruct
{
   int iBeginResidue;
   int iEndResidue;
};

struct PeffPositionStruct  // collate PEFF mods by position in sequence
{
   int iPosition;  // position within the sequence
   vector<int>    vectorWhichPeff;  // which specific peff entry from PeffModStruct
   vector<double> vectorMassDiffAvg;
   vector<double> vectorMassDiffMono;
};

struct PeffSearchStruct  // variant info passed to SearchForPeptides
{
   int    iPosition;
   bool   bBeginCleavage;
   bool   bEndCleavage;
   char   cOrigResidue;
};

//-->MH
typedef struct sDBEntry
{
   string strName;           // might be able to delete this here
   string strSeq;
   comet_fileoffset_t lProteinFilePosition;
   vector<PeffModStruct> vectorPeffMod;
   vector<PeffVariantSimpleStruct> vectorPeffVariantSimple;
   vector<PeffVariantComplexStruct> vectorPeffVariantComplex;
   vector<PeffProcessedStruct> vectorPeffProcessed;
} sDBEntry;

struct DBInfo
{
   char   szDatabase[SIZE_FILE];
   char   szFileName[SIZE_FILE];
   int    iTotalNumProteins;
   unsigned long int uliTotAACount;

   DBInfo& operator=(DBInfo& a)
   {
      strcpy(szDatabase, a.szDatabase);
      strcpy(szFileName, a.szFileName);
      iTotalNumProteins = a.iTotalNumProteins;
      uliTotAACount = a.uliTotAACount;

      return *this;
   }
};

struct DBIndex
{
   char   szPeptide[MAX_PEPTIDE_LEN];
   char   szPrevNextAA[2];
   char   pcVarModSites[MAX_PEPTIDE_LEN_P2]; // encodes 0-9 indicating which var mod at which position
   comet_fileoffset_t   lIndexProteinFilePosition;         // file position index to protein reference
   double dPepMass;                          // MH+ pep mass

   bool operator==(const DBIndex &rhs) const
   {
      if (!strcmp(szPeptide, rhs.szPeptide))
      {
         // peptides are same here

         // different masses should equate to different mod state
         if (fabs(dPepMass - rhs.dPepMass) > FLOAT_ZERO)
            return false;

         // masses are the same at this point
         // next compare modification states
         int iLen = (int)strlen(szPeptide)+2;
         for (int i=0; i<iLen; i++)
         {
            if (pcVarModSites[i] != rhs.pcVarModSites[i])
            {
               return false;
            }
         }

         // if it gets here, same peptide, same mass, same mod state
         return true;
      }

      // peptides are different
      return false;
   }
};

struct IndexProteinStruct  // for indexed database
{
   char szProt[WIDTH_REFERENCE];
   comet_fileoffset_t lProteinFilePosition;
   int  iWhichProtein;
};

struct PEFFInfo
{
   char   szPeffOBO[SIZE_FILE];
   int    iPeffSearch;               // 0=no, 1=PSI-MOD, 2=Unimod, 3=PSI-MOD only, 4=Unimod only, 5=variants only
};

struct StaticMod
{
   double dAddCterminusPeptide;
   double dAddNterminusPeptide;
   double dAddCterminusProtein;
   double dAddNterminusProtein;
   double pdStaticMods[SIZE_MASS];

   StaticMod& operator=(StaticMod& a)
   {
      dAddCterminusPeptide = a.dAddCterminusPeptide;
      dAddNterminusPeptide = a.dAddNterminusPeptide;
      dAddCterminusProtein = a.dAddCterminusProtein;
      dAddNterminusProtein = a.dAddNterminusProtein;

      for (int i = 0; i < SIZE_MASS; i++)
      {
         pdStaticMods[i] = a.pdStaticMods[i];
      }

      return *this;
   }
};

struct PrecalcMasses
{
   double dNtermProton;          // dAddNterminusPeptide + PROTON_MASS
   double dCtermOH2Proton;       // dAddCterminusPeptide + dOH2fragment + PROTON_MASS
   double dOH2ProtonCtermNterm;  // dOH2parent + PROTON_MASS + dAddCterminusPeptide + dAddNterminusPeptide
   int    iMinus17;              // BIN'd value of mass(NH3)
   int    iMinus18;              // BIN'd value of mass(H2O)

   PrecalcMasses& operator=(PrecalcMasses& a)
   {
      dNtermProton = a.dNtermProton;
      dCtermOH2Proton = a.dCtermOH2Proton;
      dOH2ProtonCtermNterm = a.dOH2ProtonCtermNterm;
      iMinus17 = a.iMinus17;
      iMinus18 = a.iMinus18;

      return *this;
   }
};

struct VarModParams
{
   bool    bVarModSearch;            // set to true if variable mods are specified
   bool    bBinaryModSearch;         // set to true if any of the variable mods are of binary mod variety
   bool    bUseFragmentNeutralLoss;  // set to true if any custom NL is set; applied only to 1+ and 2+ fragments
   bool    bRequireVarMod;           // also set to true if any individual bRequireThisMod is true
   int     iMaxVarModPerPeptide;
   int     iMaxPermutations;
   VarMods varModList[VMODS];
   char    cModCode[VMODS];          // mod characters

   VarModParams& operator=(VarModParams& a)
   {
      bVarModSearch = a.bVarModSearch;
      iMaxVarModPerPeptide = a.iMaxVarModPerPeptide;
      iMaxPermutations = a.iMaxPermutations;
      bUseFragmentNeutralLoss = a.bUseFragmentNeutralLoss;

      for (int i = 0; i < VMODS; i++)
      {
         varModList[i] = a.varModList[i];
         cModCode[i] = a.cModCode[i];
      }

      return *this;
   }
};

struct MassUtil
{
   int    bMonoMassesParent;
   int    bMonoMassesFragment;
   double dCO;
   double dNH3;
   double dNH2;
   double dH2O;
   double dCOminusH2;
   double dOH2fragment;
   double dOH2parent;
   double pdAAMassParent[SIZE_MASS];
   double pdAAMassFragment[SIZE_MASS];

   MassUtil& operator=(MassUtil& a)
   {
      bMonoMassesParent = a.bMonoMassesParent;
      bMonoMassesFragment = a.bMonoMassesFragment;
      dCO = a.dCO;
      dNH3 = a.dNH3;
      dNH2 = a.dNH2;
      dH2O = a.dH2O;
      dCOminusH2 = a.dCOminusH2;
      dOH2fragment = a.dOH2fragment;
      dOH2parent = a.dOH2parent;

      for (int i = 0; i < SIZE_MASS; i++)
      {
         pdAAMassParent[i] = a.pdAAMassParent[i];
         pdAAMassFragment[i] = a.pdAAMassFragment[i];
      }

      return *this;
   }
};

struct ToleranceParams
{
   int    iMassToleranceUnits;    // 0=amu, 1=mmu, else ppm (2)
   int    iMassToleranceType;     // 0=MH+ (default), 1=precursor m/z; only valid if iMassToleranceUnits > 0
   int    iIsotopeError;
   double dInputTolerance;        // tolerance from param file
   double dFragmentBinSize;
   double dFragmentBinStartOffset;
   double dMatchPeakTolerance;

   ToleranceParams& operator=(ToleranceParams& a)
   {
      iMassToleranceUnits = a.iMassToleranceUnits;
      iMassToleranceType = a.iMassToleranceType;
      iIsotopeError = a.iIsotopeError;
      dInputTolerance = a.dInputTolerance;
      dFragmentBinSize = a.dFragmentBinSize;
      dFragmentBinStartOffset = a.dFragmentBinStartOffset;
      dMatchPeakTolerance = a.dMatchPeakTolerance;

      return *this;
   }
};

struct PeaksInfo
{
   int iNumMatchPeaks;
   int iNumAllowedMatchPeakErrors;

   PeaksInfo& operator=(PeaksInfo& a)
   {
      iNumMatchPeaks = a.iNumMatchPeaks;
      iNumAllowedMatchPeakErrors = a.iNumAllowedMatchPeakErrors;

      return *this;
   }
};

struct IonInfo
{
   int iNumIonSeriesUsed;
   int piSelectedIonSeries[NUM_ION_SERIES];
   int bUseWaterAmmoniaLoss;    // ammonia, water loss
   int iTheoreticalFragmentIons;
   int iIonVal[NUM_ION_SERIES];

   IonInfo& operator=(IonInfo& a)
   {
      iNumIonSeriesUsed = a.iNumIonSeriesUsed;
      bUseWaterAmmoniaLoss = a.bUseWaterAmmoniaLoss;
      iTheoreticalFragmentIons = a.iTheoreticalFragmentIons;

      for (int i = 0; i < NUM_ION_SERIES; i++)
      {
         piSelectedIonSeries[i] = a.piSelectedIonSeries[i];
         iIonVal[i] = a.iIonVal[i];
      }

      return *this;
   }
};

// static user params, won't change per thread - can make global!
struct StaticParams
{
   char            szHostName[SIZE_FILE];
   char            szOutFileTimeString[256];
   char            szIonSeries[256];   // used for .out files
   char            szDisplayLine[256]; // used for .out files
   char            szMod[512];         // used for .out files
   char            szDecoyPrefix[256]; // used for prefix to indicate decoys
   string          sDecoyPrefix;       // escaped version of szDecoyPrefix for output within XML files
   char            szOutputSuffix[256]; // used for suffix to append to output file base names
   char            szTxtFileExt[256];  // text file extension; default "txt"
   int             iElapseTime;
   char            szDate[32];
   Options         options;
   DBInfo          databaseInfo;
   PEFFInfo        peffInfo;
   InputFileInfo   inputFile;
   int             bPrintDuplReferences;
   VarModParams    variableModParameters;
   ToleranceParams tolerances;
   StaticMod       staticModifications;
   PrecalcMasses   precalcMasses;
   EnzymeInfo      enzymeInformation;
   MassUtil        massUtility;
   double          dInverseBinWidth;    // this is used in BIN() many times so use inverse binWidth to do multiply vs. divide
   double          dOneMinusBinOffset;  // this is used in BIN() many times so calculate once
   PeaksInfo       peaksInformation;
   IonInfo         ionInformation;
   int             iXcorrProcessingOffset;
   int             bIndexDb;            // 0 = normal fasta; 1 = indexed database
   vector<double>  vectorMassOffsets;
   vector<double>  precursorNLIons;
   int             iPrecursorNLSize;
   int             iOldModsEncoding;
   char            szDIAWindowsFile[SIZE_FILE];
   bool            bSkipToStartScan;
   std::chrono::high_resolution_clock::time_point tRealTimeStart;     // track run time of real-time index search

   StaticParams()
   {
       RestoreDefaults();
   }

   StaticParams& operator=(StaticParams& a)
   {
       strcpy(szHostName, a.szHostName);
       strcpy(szOutFileTimeString, a.szOutFileTimeString);
       strcpy(szIonSeries, a.szIonSeries);
       strcpy(szDisplayLine, a.szDisplayLine);
       strcpy(szMod, a.szMod);
       strcpy(szDecoyPrefix, a.szDecoyPrefix);
       strcpy(szOutputSuffix, a.szOutputSuffix);
       strcpy(szTxtFileExt, a.szTxtFileExt);
       vectorMassOffsets = a.vectorMassOffsets;
       precursorNLIons= a.precursorNLIons;
       iPrecursorNLSize = a.iPrecursorNLSize;
       iOldModsEncoding = a.iOldModsEncoding;
       iElapseTime = a.iElapseTime;
       strcpy(szDate, a.szDate);
       options = a.options;
       databaseInfo = a.databaseInfo;
       inputFile = a.inputFile;
       bPrintDuplReferences = a.bPrintDuplReferences;
       variableModParameters = a.variableModParameters;
       tolerances = a.tolerances;
       staticModifications = a.staticModifications;
       precalcMasses = a.precalcMasses;
       enzymeInformation = a.enzymeInformation;
       massUtility = a.massUtility;
       dInverseBinWidth = a.dInverseBinWidth;
       dOneMinusBinOffset = a.dOneMinusBinOffset;
       iXcorrProcessingOffset = a.iXcorrProcessingOffset;
       peaksInformation = a.peaksInformation;
       ionInformation = a.ionInformation;
       return *this;
   }

   void RestoreDefaults()
   {
      int i;

      inputFile.iInputType = InputType_MS2;

      szMod[0] = '\0';

      iXcorrProcessingOffset = 75;
      bIndexDb = 0;

      databaseInfo.szDatabase[0] = '\0';

      strcpy(szDecoyPrefix, "DECOY_");
      strcpy(szTxtFileExt, "txt");
      szOutputSuffix[0] = '\0';

      peffInfo.szPeffOBO[0] = '\0';
      peffInfo.iPeffSearch = 0;

      szDIAWindowsFile[0]='\0';
      iPrecursorNLSize = 0;

      for (i=0; i<SIZE_MASS; i++)
      {
         massUtility.pdAAMassParent[i] = 999999.;
         massUtility.pdAAMassFragment[i] = 999999.;
         staticModifications.pdStaticMods[i] = 0.0;
      }

      massUtility.bMonoMassesFragment = 1;
      massUtility.bMonoMassesParent = 1;

#ifdef CRUX
      staticModifications.pdStaticMods[(int)'C'] = 57.021464;
#endif


      enzymeInformation.iAllowedMissedCleavage = 2;

      for (i=0; i<VMODS; i++)
      {
         variableModParameters.varModList[i].iMaxNumVarModAAPerMod = 3;
         variableModParameters.varModList[i].iMinNumVarModAAPerMod = 0;
         variableModParameters.varModList[i].iBinaryMod = 0;
         variableModParameters.varModList[i].bRequireThisMod = false;
         variableModParameters.varModList[i].iVarModTermDistance = -1;   // distance from N or C-term distance
         variableModParameters.varModList[i].iWhichTerm = 0;             // specify N (0) or C-term (1)
         variableModParameters.varModList[i].dVarModMass = 0.0;
         variableModParameters.varModList[i].dNeutralLoss = 0.0;
         strcpy(variableModParameters.varModList[i].szVarModChar, "X");

#ifdef CRUX
         if (i==0)
         {
            variableModParameters.varModList[i].dVarModMass = 15.9949;
            strcpy(variableModParameters.varModList[i].szVarModChar, "M");
         }
#endif
      }

      variableModParameters.cModCode[0] = '*';
      variableModParameters.cModCode[1] = '#';
      variableModParameters.cModCode[2] = '@';
      variableModParameters.cModCode[3] = '^';
      variableModParameters.cModCode[4] = '~';
      variableModParameters.cModCode[5] = '$';
      variableModParameters.cModCode[6] = '%';
      variableModParameters.cModCode[7] = '!';
      variableModParameters.cModCode[8] = '+';

      variableModParameters.iMaxVarModPerPeptide = 5;
      variableModParameters.iMaxPermutations = MAX_PERMUTATIONS;
      variableModParameters.bUseFragmentNeutralLoss = false;

      ionInformation.bUseWaterAmmoniaLoss = 0;
      ionInformation.iTheoreticalFragmentIons = 1;      // 0 = flanking peaks; 1 = no flanking peaks
      ionInformation.iIonVal[ION_SERIES_A] = 0;
      ionInformation.iIonVal[ION_SERIES_B] = 1;
      ionInformation.iIonVal[ION_SERIES_C] = 0;
      ionInformation.iIonVal[ION_SERIES_X] = 0;
      ionInformation.iIonVal[ION_SERIES_Y] = 1;
      ionInformation.iIonVal[ION_SERIES_Z] = 0;
      ionInformation.iIonVal[ION_SERIES_Z1] = 0;

      options.iNumPeptideOutputLines = 5;
      options.iWhichReadingFrame = 0;
      options.iEnzymeTermini = 2;
      options.iNumStored = 100;                         // default # of search results to store for xcorr analysis.
      options.iMaxDuplicateProteins = 20;               // maximum number of duplicate proteins to report or store in idx file

      options.bShowFragmentIons = 0;
      options.bExplicitDeltaCn = 0;
      options.bPrintExpectScore = 1;
      options.bOverrideCharge = 0;
      options.bCorrectMass = 0;
      options.bTreatSameIL = 1;
      options.iMaxIndexRunTime = 0;                     // index run time limit in milliseconds; 0=no time limit
      options.iRemovePrecursor = 0;
      options.dRemovePrecursorTol = 1.5;

      options.bOutputSqtStream = 0;
      options.bOutputSqtFile = 0;
      options.bOutputTxtFile = 0;
      options.bOutputPepXMLFile = 1;
      options.bOutputMzIdentMLFile = 0;
      options.bOutputPercolatorFile = 0;
      options.bOutputOutFiles = 0;

      options.bSkipAlreadyDone = 1;
//    options.bSkipUpdateCheck = 0;
      options.bMango = 0;
      options.bScaleFragmentNL = 0;
      options.bCreateIndex = 0;
      options.bVerboseOutput = 0;
      options.iDecoySearch = 0;
      options.iNumThreads = 0;
      options.bClipNtermMet = 0;
      options.bClipNtermAA = 0;
      options.bPinModProteinDelim = 0;

      options.lMaxIterations = 0;

      // These parameters affect mzXML/RAMP spectra only.
      options.scanRange.iStart = 0;
      options.scanRange.iEnd = 0;
      options.iSpectrumBatchSize = 0;
      options.iMinPeaks = 10;
      options.iStartCharge = 0;
      options.iEndCharge = 0;
      options.iMaxFragmentCharge = 3;
      options.iMaxPrecursorCharge = 6;
      options.iMSLevel = 2;
      options.dMinIntensity = 0.0;
      options.dPeptideMassLow = 600.0;
      options.dPeptideMassHigh = 8000.0;
      options.dMinimumXcorr = XCORR_CUTOFF;
      strcpy(options.szActivationMethod, "ALL");
      // End of mzXML specific parameters.

      options.clearMzRange.dStart = 0.0;
      options.clearMzRange.dEnd = 0.0;

      options.peptideLengthRange.iStart = MIN_PEPTIDE_LEN;
      options.peptideLengthRange.iEnd = MAX_PEPTIDE_LEN-1;  // -1 accounts for terminating char

      staticModifications.dAddCterminusPeptide = 0.0;
      staticModifications.dAddNterminusPeptide = 0.0;
      staticModifications.dAddCterminusProtein = 0.0;
      staticModifications.dAddNterminusProtein = 0.0;

      tolerances.iMassToleranceUnits = 0;
      tolerances.iMassToleranceType = 0;
      tolerances.iIsotopeError = 0;
      tolerances.dInputTolerance = 3.0;                     // peptide_mass_tolerance
      tolerances.dFragmentBinSize = 1.0005;
      tolerances.dFragmentBinStartOffset = 0.4;
      tolerances.dMatchPeakTolerance = 0.5;

      bSkipToStartScan = true;
   }
};

extern StaticParams    g_staticParams;

extern vector<double>  g_pvDIAWindows;    // vector of start-end masses for DIA window; even number start mass, odd number end mass

extern string g_psGITHUB_SHA;             // grab the GITHUB_SHA environment variable and trim to 7 chars; null if environment variable not present

extern map<long long, IndexProteinStruct>  g_pvProteinNames;

extern vector<DBIndex> g_pvDBIndex;

extern vector<vector<comet_fileoffset_t>> g_pvProteinsList;

// Query stores information for peptide scoring and results
// This struct is allocated for each spectrum/charge combination
struct Query
{
   int   iXcorrHistogram[HISTO_SIZE];
   int   iHistogramCount;   // # of entries in histogram
   float fPar[4];           // parameters of LMA regression

   int iMatchPeptideCount;        // # of peptides that get stored (i.e. are greater than lowest score)
   int iDecoyMatchPeptideCount;   // # of decoy peptides that get stored (i.e. are greater than lowest score)

   short siMaxXcorr;        // index of maximum correlation score in iXcorrHistogram

   short siLowestSpScoreIndex;
   short siLowestDecoySpScoreIndex;

   float fLowestSpScore;
   float fLowestDecoySpScore;

   double dLowestXcorrScore;
   double dLowestDecoyXcorrScore;

   int iMinXcorrHisto;    // min xcorr score for xcorr histogram to address good E-values for poor/sparse spectra

   double dMangoIndex;      // scan number decimal precursor value i.e. 2401.001 for scan 2401, first precursor/z pair

   unsigned long int  _uliNumMatchedPeptides;  // # of peptides that get scored
   unsigned long int  _uliNumMatchedDecoyPeptides;

   // Sparse matrix representation of data
   int iSpScoreData;    //size of sparse matrix
   int iFastXcorrDataSize;
   float **ppfSparseSpScoreData;
   float **ppfSparseFastXcorrData;
   float **ppfSparseFastXcorrDataNL;

   // Standard array representation of data
   float *pfSpScoreData;
   float *pfFastXcorrData;
   float *pfFastXcorrDataNL;  // pfFastXcorrData with NH3, H2O contributions

   PepMassInfo          _pepMassInfo;
   SpectrumInfoInternal _spectrumInfoInternal;
   Results              *_pResults;
   Results              *_pDecoys;

   Mutex accessMutex;

   Query()
   {
      memset(iXcorrHistogram, 0, sizeof(int)*HISTO_SIZE);

      iMatchPeptideCount = 0;
      iDecoyMatchPeptideCount = 0;
      iHistogramCount = 0;

      fPar[0]=0.0;
      fPar[1]=0.0;
      fPar[2]=0.0;
      fPar[3]=0.0;

      siMaxXcorr = 0;                        // index of maximum correlation score in iXcorrHistogram
      siLowestSpScoreIndex = 0;
      siLowestDecoySpScoreIndex = 0;

      fLowestSpScore = 0.0;
      fLowestDecoySpScore = 0.0;

      dLowestXcorrScore = XCORR_CUTOFF;
      dLowestDecoyXcorrScore = XCORR_CUTOFF;

      dMangoIndex = 0.0;

      _uliNumMatchedPeptides = 0;
      _uliNumMatchedDecoyPeptides = 0;

      ppfSparseSpScoreData = NULL;
      ppfSparseFastXcorrData = NULL;
      ppfSparseFastXcorrDataNL = NULL;          // ppfSparseFastXcorrData with NH3, H2O contributions

      pfSpScoreData = NULL;
      pfFastXcorrData = NULL;
      pfFastXcorrDataNL = NULL;                 // pfFastXcorrData with NH3, H2O contributions

      _pepMassInfo.dCalcPepMass = 0.0;
      _pepMassInfo.dExpPepMass = 0.0;
      _pepMassInfo.dPeptideMassTolerance = 0.0;
      _pepMassInfo.dPeptideMassToleranceMinus = 0.0;
      _pepMassInfo.dPeptideMassTolerancePlus = 0.0;

      _spectrumInfoInternal.dTotalIntensity = 0.0;
      _spectrumInfoInternal.iArraySize = 0;
      _spectrumInfoInternal.iHighestIon = 0;
      _spectrumInfoInternal.iScanNumber = 0;
      _spectrumInfoInternal.dTotalIntensity = 0.0;

      _pResults = NULL;
      _pDecoys = NULL;

      Threading::CreateMutex(&accessMutex);
   }

   ~Query()
   {
      int i;
      for (i=0;i<iSpScoreData;i++)
      {
         if (ppfSparseSpScoreData[i] != NULL)
            delete[] ppfSparseSpScoreData[i];
      }
      delete[] ppfSparseSpScoreData;
      ppfSparseSpScoreData = NULL;

      if (g_staticParams.ionInformation.bUseWaterAmmoniaLoss
            && (g_staticParams.ionInformation.iIonVal[ION_SERIES_A]
               || g_staticParams.ionInformation.iIonVal[ION_SERIES_B]
               || g_staticParams.ionInformation.iIonVal[ION_SERIES_Y]))
      {
         for (i=0;i<iFastXcorrDataSize;i++)
         {
            if (ppfSparseFastXcorrData[i] != NULL)
               delete[] ppfSparseFastXcorrData[i];
            if (ppfSparseFastXcorrDataNL[i]!=NULL)
               delete[] ppfSparseFastXcorrDataNL[i];
         }
         delete[] ppfSparseFastXcorrDataNL;
         ppfSparseFastXcorrDataNL = NULL;
      }
      else
      {
         for (i=0;i<iFastXcorrDataSize;i++)
         {
            if (ppfSparseFastXcorrData[i] != NULL)
               delete[] ppfSparseFastXcorrData[i];
         }
      }
      delete[] ppfSparseFastXcorrData;
      ppfSparseFastXcorrData = NULL;

      _pResults->pWhichProtein.clear();
      if (g_staticParams.options.iDecoySearch == 1)
         _pResults->pWhichDecoyProtein.clear();
      delete[] _pResults;
      _pResults = NULL;

      if (g_staticParams.options.iDecoySearch == 2)
      {
         _pDecoys->pWhichDecoyProtein.clear();
         delete[] _pDecoys;
         _pDecoys = NULL;
      }

      Threading::DestroyMutex(accessMutex);
   }
};

extern vector<Query*>          g_pvQuery;
extern vector<InputFileInfo*>  g_pvInputFiles;
extern Mutex                   g_pvQueryMutex;
extern Mutex                   g_preprocessMemoryPoolMutex;
extern Mutex                   g_searchMemoryPoolMutex;
extern Mutex                   g_dbIndexMutex;

struct IonSeriesStruct         // defines which fragment ion series are considered
{
   int bPreviousMatch[8];
};


struct MatchedIonsStruct  // for SingleSpectrumSearch
{
   double dMass;
   double dInten;

   bool operator<(const MatchedIonsStruct& a) const
   {
      return dInten > a.dInten;
   }
};

#endif // _COMETDATAINTERNAL_H_
