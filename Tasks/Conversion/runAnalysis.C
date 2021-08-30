// include the header of your analysis task here! for classes already compiled by aliBuild,
// precompiled header files (with extension pcm) are available, so that you do not need to
// specify includes for those. for your own task however, you (probably) have not generated a
// pcm file, so we need to include it explicitly
#include "AliAnalysisTaskAO2Dconverter2.h"
#include "TGrid.h"
#include <TMacro.h>





void runAnalysis()
{

  //  TGrid::Connect("alien://");

    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = kFALSE;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = kFALSE;

    // since we will compile a class, tell root where to look for headers
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT->ProcessLine(".include $ROOTSYS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif

    // create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTaskExample");
    AliESDInputHandler *esdH = new AliESDInputHandler();
    mgr->SetInputEventHandler(esdH);
    TMacro PIDadd(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"));
    AliAnalysisTaskPIDResponse* PIDresponseTask = reinterpret_cast<AliAnalysisTaskPIDResponse*>(PIDadd.Exec());
    TMacro multSelection(gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"));
    AliMultSelectionTask* multSelectionTask = reinterpret_cast<AliMultSelectionTask*>(multSelection.Exec());
    TMacro physicsSelection(gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"));
    AliPhysicsSelectionTask* physicsSelectionTask = reinterpret_cast<AliPhysicsSelectionTask*>(physicsSelection.Exec());



    // compile the class and load the add task macro
    // here we have to differentiate between using the just-in-time compiler
    // from root6, or the interpreter of root5
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->LoadMacro("AliAnalysisTaskAO2Dconverter2.cxx++g");
    AliAnalysisTaskAO2Dconverter2 *task = reinterpret_cast<AliAnalysisTaskAO2Dconverter2*>(gInterpreter->ExecuteMacro("AddTaskAO2Dconverter.C"));
#else
    gROOT->LoadMacro("AliAnalysisTaskAO2Dconverter2.cxx++g");
    gROOT->LoadMacro("AddTaskAO2Dconverter.C");
    AliAnalysisTaskAO2Dconverter2 *task = AddTaskAO2Dconverter();
#endif


    if(!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(1);
    mgr->PrintStatus();
    //mgr->SetUseProgressBar(1, 10);

    if(local) {
        // if you want to run locally, we need to define some input
        TChain* chain = new TChain("esdTree");
       chain->Add("alien:///alice/data/2015/LHC15o/000246392/pass2/15000246392019.100/AliESDs.root");

        // add a few files to the chain (change this so that your local files are added)
        //chain->Add("/home/agautam/Run3/UPC_Forward_O2/data/2018/AliESDs.root");
        //chain->Add("alien:///alice/sim/2016/LHC16b2a/246994/001/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000245692/muon_calo_pass1/15000245692023.310/AliESDs.root");  //15000245692023.310
        //chain->Add("alien:///alice/data/2015/LHC15o/000245692/muon_calo_pass1/15000245692023.311/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000245692/muon_calo_pass1/15000245692023.312/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000245692/muon_calo_pass1/15000245692023.313/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000245692/muon_calo_pass1/15000245692023.314/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000246949/pass1/15000246949019.100/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000246493/pass1/15000246493020.3611/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000246493/pass1/15000246493020.3612/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000246493/pass1/15000246493020.3613/AliESDs.root");
        //chain->Add("alien:///alice/data/2015/LHC15o/000246493/pass1/15000246493020.3614/AliESDs.root");
        //chain->Add("/home/agautam/Run3/UPC_Forward_O2/data/2015/AliESDs_muon_calo_pass1.root");
        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chain);
    } else {
        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();
        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
        // make sure your source files get copied to grid
        alienHandler->SetAdditionalLibs("AliAnalysisTaskAO2Dconverter2.cxx AliAnalysisTaskAO2Dconverter2.h");
        alienHandler->SetAnalysisSource("AliAnalysisTaskAO2Dconverter2.cxx");
        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        alienHandler->SetAliPhysicsVersion("vAN-20210828_ROOT6-1");
        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");
        // select the input data
        alienHandler->SetGridDataDir("/alice/data/2015/LHC15o");
        alienHandler->SetDataPattern("pass1/*AliESDs.root");
        // MC has no prefix, data has prefix 000
        alienHandler->SetRunPrefix("000");
        // runnumber
        alienHandler->AddRunNumber(246392);
        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(40);
        alienHandler->SetExecutable("myTask.sh");
        // specify how many seconds your job may take
        alienHandler->SetTTL(10000);
        alienHandler->SetJDLName("myTask.jdl");

        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);
        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate")
        // (see below) mode, set SetMergeViaJDL(kFALSE)
        // to collect final results
        alienHandler->SetMaxMergeStages(1);
        alienHandler->SetMergeViaJDL(kFALSE);

        // define the output folders
        alienHandler->SetGridWorkingDir("convert_4_1");
        alienHandler->SetGridOutputDir("convert_4_1");

        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);
        if(gridTest) {
            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        } else {
            // else launch the full grid analysis
            alienHandler->SetRunMode("terminate");
            mgr->StartAnalysis("grid");
        }
    }
}
