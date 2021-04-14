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

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliESDInputHandler.h"
#include "AliAnalysisTaskForwardO2Parallel.h"
#include "TLorentzVector.h"

class AliAnalysisTaskForwardO2Parallel;

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskForwardO2Parallel) // classimp: necessary for root

AliAnalysisTaskForwardO2Parallel::AliAnalysisTaskForwardO2Parallel() : AliAnalysisTaskSE(),
    fESD(0), fOutputList(0), fHistPt(0), fHistM(0),fHistCounter(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskForwardO2Parallel::AliAnalysisTaskForwardO2Parallel(const char* name) : AliAnalysisTaskSE(name),
    fESD(0), fOutputList(0), fHistPt(0), fHistM(0),fHistCounter(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it,
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskForwardO2Parallel::~AliAnalysisTaskForwardO2Parallel()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskForwardO2Parallel::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)

    // example of a histogram

    fHistCounter = new TH1I("fHistCounter","Counter",15,0,15);
    fOutputList->Add(fHistCounter);
    fHistPt = new TH1F("fHistPt", "fHistPt",500, 0., 5.);       // create your histogram
    fOutputList->Add(fHistPt);
    fHistM = new TH1F("fHistM", "fHistM",500,0.,10.);            //

    fOutputList->Add(fHistM);          // don't forget to add it to the list! the list will be written to file, so if you want
                                        // your histogram in the output file, add it to the list!
    TString SelectionCuts[6] = {"NoSelection", "CMup11Trigger", "twotracks", "oppositecharge", "-2.5<Eta<-4", "Pt<1"};
    for (int i = 0; i < 6; i++) {
      fHistCounter->GetXaxis()->SetBinLabel(i + 1, SelectionCuts[i].Data());
    }
    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
}
//_____________________________________________________________________________
void AliAnalysisTaskForwardO2Parallel::UserExec(Option_t *)
{

    fESD = dynamic_cast<AliESDEvent*>(InputEvent());    // get an event (called fESDVertex) from the input file
                                                        // there's another event format (ESD) which works in a similar wya
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's
    if(!fESD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event
    fHistCounter->Fill(0);
    TString trigger = fESD->GetFiredTriggerClasses();
    if  (!trigger.Contains("CMUP11")||!trigger.Contains("CMUP10")){
      return;
    }
    fHistCounter->Fill(1);

        // example part: i'll show how to loop over the tracks in an event
        // and extract some information from them which we'll store in a histogram
    AliESDtrack* track1;
    AliESDtrack* track2;
    Int_t trackcounter =0;
    Int_t iTracks(fESD->GetNumberOfTracks());           // see how many tracks there are in the event
    for(Int_t i(0); i < iTracks; i++) {                 // loop ove rall these tracks

        for(Int_t j=i+1;j < iTracks; j++) {                       // if we failed, skip this track
          track1 = static_cast<AliESDtrack*>(fESD->GetTrack(i));
          track2 = static_cast<AliESDtrack*>(fESD->GetTrack(j));

          trackcounter++;
          //if(trackcounter>1){break;}
        }
    }
    if (trackcounter != 1) {
      return;
    }

    fHistCounter->Fill(2);
    if (track1->Charge()*track2->Charge()<=0) {
      return;
    }
    fHistCounter->Fill(3);

    TLorentzVector d1;
    TLorentzVector d2;
    TLorentzVector p;
    d1.SetPtEtaPhiM(track1->Pt(),track1->Eta(),track1->Phi(),0.1057);
    d2.SetPtEtaPhiM(track2->Pt(),track2->Eta(),track2->Phi(),0.1057);
    p = d1+d2;
    if (-4 > d1.Eta() > -2.5 || -4 > d2.Eta() > -2.5) {
      return;
    }
    fHistCounter->Fill(4);

    if (p.Pt() > 1) {
      return;
    }
    fHistCounter->Fill(5);



    fHistPt->Fill(p.Pt());
    fHistM->Fill(p.M());
                                                   // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file
}
//_____________________________________________________________________________
void AliAnalysisTaskForwardO2Parallel::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
