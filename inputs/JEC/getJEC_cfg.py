import FWCore.ParameterSet.Config as cms


process = cms.Process("jectxt")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')






process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.readAK5PF    = cms.EDAnalyzer('JetCorrectorDBReader',  
    # below is the communication to the database 
    payloadName    = cms.untracked.string('AK5PF'),
    # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
    # but it is recommended to use the GT name that you retrieved the files from.
    globalTag      = cms.untracked.string(''),  
    printScreen    = cms.untracked.bool(False),
    createTextFile = cms.untracked.bool(True)
)
process.p = cms.Path(process.readAK5PF)


# Used for the corrections
#process.GlobalTag.globaltag = 'GR_P_V39_AN3::All'
process.GlobalTag.globaltag = 'START53_V7G::All'


# Used only for the *name* of the printed txt files 
# (should be the same as GlobalTag.globaltag)
process.readAK5PF.globalTag = 'START53_V7G'


