#include "AliAnalysisTaskAO2Dconverter2.h"

AliAnalysisTaskAO2Dconverter2* AddTaskAO2Dconverter(TString suffix = "")
{
  return AliAnalysisTaskAO2Dconverter2::AddTask(suffix);
}
