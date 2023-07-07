/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// This task is meant to acquire MC-level predictions for 2pc analyses
//
// Please report any bugs, complaints, suggestions to:
// --- david.dobrigkeit.chinellato@cern.ch
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#ifndef AliAnalysisTaskMCPredictions2pc_H
#define AliAnalysisTaskMCPredictions2pc_H

class TTree;
class TList;
class TH1F;
class TH2F;
class TH3F;
class THnF;
class TVector3;
class THnSparse;

class AliESDpid;
class AliESDtrackCuts;
class AliAnalysisUtils;
class AliPPVsMultUtils;
class AliESDEvent;
class AliPhysicsSelection;
class AliCFContainer;

//#include "TString.h"
//#include "AliESDtrackCuts.h"
//#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskMCPredictions2pc : public AliAnalysisTaskSE {
public:
  AliAnalysisTaskMCPredictions2pc();
  AliAnalysisTaskMCPredictions2pc(const char *name, Int_t lNSmallBinning, Int_t lNLargeBinning, Int_t lRebinFactor, Int_t lNEtaBins = 80);
  virtual ~AliAnalysisTaskMCPredictions2pc();
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  Double_t MyRapidity(Double_t rE, Double_t rPz) const;
  
  Double_t ComputeDeltaPhi( Double_t phi1, Double_t phi2) const;
  
  void SetDo2pc( Bool_t lOpt = kTRUE ) { fkDo2pc = lOpt; }
  void SetPtTrigger( Float_t l1, Float_t l2 ) { fkMinPtTrigger = l1; fkMaxPtTrigger = l2; }
  void SetSelectINELgtZERO ( Bool_t lOpt ) { fkSelectINELgtZERO = lOpt; }
  void SetMinimumMultiplicity ( Long_t lMinMult ) { fkMinimumMultiplicity = lMinMult; } ;
  
  //configure intervals
  void ClearEtaIntervals() { fkNIntervals = 0; };
  void AddEtaInterval( Float_t lMin, Float_t lMax, Float_t lWeight = 1.0 ) {
    fkNIntervals++;
    fkIntervalMinEta[fkNIntervals] = lMin;
    fkIntervalMaxEta[fkNIntervals] = lMax;
    fkIntervalWeight[fkNIntervals] = lWeight;
  } ;
  void PrintEtaIntervals(); 
  
  //---------------------------------------------------------------------------------------
  
private:
  // Note : In ROOT, "//!" means "do not stream the data from Master node to Worker node" ...
  // your data member object is created on the worker nodes and streaming is not needed.
  // http://root.cern.ch/download/doc/11InputOutput.pdf, page 14
  TList  *fListHist;  //! List of Cascade histograms
  
  //Histograms (Desired objects in this cross-checking task)
  TH1D *fHistEventCounter; //! histogram for event counting
  TH1D *fHistChargedEta; //! histogram for event counting
  
  Int_t fSmallMultRange;
  Int_t fLargeMultRange;
  Int_t fRebinFactor;
  
  Int_t fkNIntervals; //number of eta intervals in "SPD" estimator (def 1)
  Float_t fkIntervalMinEta[10];
  Float_t fkIntervalMaxEta[10];
  Float_t fkIntervalWeight[10]; 
  
  Bool_t fkSelectINELgtZERO;
  Long_t fkMinimumMultiplicity;
  
  //Basic Histograms for counting events as a function of Forward percentiles...
  TH1D *fHistForwardMult; //!
  TH2D *fHistNchVsForwardMult; //!
  
  Bool_t fkDo2pc;
  Float_t fkMinPtTrigger; //for xi trigger
  Float_t fkMaxPtTrigger; //for xi trigger
  Float_t fkMinEta;
  Float_t fkMaxEta;
  Int_t fkNEtaBins;
  TH2D *fHistEtaVsPtTrigger; //!
  
  THnSparseD *fHist4d2pcK0Short; //!
  THnSparseD *fHist4d2pcLambda; //!
  THnSparseD *fHist4d2pcAntiLambda; //!
  THnSparseD *fHist4d2pcXiMinus; //!
  THnSparseD *fHist4d2pcXiPlus; //!
  THnSparseD *fHist4d2pcOmegaMinus; //!
  THnSparseD *fHist4d2pcOmegaPlus; //!

  THnSparseD *fHist4d2pcMixedK0Short; //!
  THnSparseD *fHist4d2pcMixedLambda; //!
  THnSparseD *fHist4d2pcMixedAntiLambda; //!
  THnSparseD *fHist4d2pcMixedXiMinus; //!
  THnSparseD *fHist4d2pcMixedXiPlus; //!
  THnSparseD *fHist4d2pcMixedOmegaMinus; //!
  THnSparseD *fHist4d2pcMixedOmegaPlus; //!

  //for event mixing
  Bool_t fEMBufferFull[20];
  Int_t fEMBufferCycle[20];
  Int_t fEMBufferSize[20];
  Float_t fEMBufferEta[50][20];
  Float_t fEMBufferPhi[50][20];
  
  AliAnalysisTaskMCPredictions2pc(const AliAnalysisTaskMCPredictions2pc&);            // not implemented
  AliAnalysisTaskMCPredictions2pc& operator=(const AliAnalysisTaskMCPredictions2pc&); // not implemented
  
  ClassDef(AliAnalysisTaskMCPredictions2pc, 1);
};

#endif
