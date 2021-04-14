/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskForwardO2Parallel_H
#define AliAnalysisTaskForwardO2Parallel_H

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskForwardO2Parallel : public AliAnalysisTaskSE
{
    public:
                                AliAnalysisTaskForwardO2Parallel();
                                AliAnalysisTaskForwardO2Parallel(const char *name);
        virtual                 ~AliAnalysisTaskForwardO2Parallel();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

    private:
        AliESDEvent*            fESD;           //! input event
        TList*                  fOutputList;    //! output list
        TH1F*                   fHistPt;        //! dummy histogram
        TH1F*                   fHistM;
        TH1I*                   fHistCounter;

        AliAnalysisTaskForwardO2Parallel(const AliAnalysisTaskForwardO2Parallel&); // not implemented
        AliAnalysisTaskForwardO2Parallel& operator=(const AliAnalysisTaskForwardO2Parallel&); // not implemented

        ClassDef(AliAnalysisTaskForwardO2Parallel, 2);
};

#endif
