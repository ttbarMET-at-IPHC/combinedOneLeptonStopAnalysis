#ifndef ProofBabyTupler_h
#define ProofBabyTupler_h

// C headers
#include <iostream>
#include <string>
using namespace std;

// ROOT headers
#include <TApplication.h>
#include <TGClient.h>
#include <TProof.h>
#include <TEnv.h>
#include <TChain.h>
#include <TFileCollection.h>
#include <TDrawFeedback.h>

// IPHC headers
#include "Tools/interface/Dataset.h"
#include "Tools/interface/AnalysisEnvironmentLoader.h"

// ###################
// Proof configuration
// ###################

// Number of nodes to be used
#define NUMBER_OF_NODES 10

// Name of the file describing what each worker have to do
#define JOB_DESCRIPTION "ProofJob.C+"

// XML configuration to load
#define XML_CONFIG "/opt/sbg/data/data4/cms/aaubin/stopAnalysis/babyTuples/production/config_1lep.xml"

// Sandbox for PROOF
// NB : the content of this folder will be deleted each time you launch proof !
//#define PROOF_SANDBOX "/opt/sbg/data/data4/cms/aaubin/proofBox/"
#define PROOF_SANDBOX "/opt/sbg/data/sbgse24/data1/cms/aaubin/proofBox/"

// Name for the final output
#define OUTPUT_NAME "babyTuple.root"

// Format definition of the babytuples
#define BABYTUPLE_FORMAT "formats/Format_newFinal0516.h"

#endif
