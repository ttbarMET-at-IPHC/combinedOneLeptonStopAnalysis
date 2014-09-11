#------------------------------------------------------------------------------

default: plots MTTailCorrection

#------------------------------------------------------------------------------

include ../backgroundEstimation_common/common.make

plots: plotsProducer.$(ExeSuf)
	./plotsProducer.$(ExeSuf)

ROOT_WITHROOFIT="/opt/sbg/data/safe1/cms/echabert/root_v5.34.05"

MTTailCorrectionProducer:
	@make --no-print-directory MTTailCorrectionProducer.run 									\
	ROOTCFLAGS="$(shell $(ROOT_WITHROOFIT)/bin/root-config --cflags)" 							\
	ROOTLIBS="$(shell   $(ROOT_WITHROOFIT)/bin/root-config --libs) -lRooFit -lRooFitCore" 

MTTailCorrection: MTTailCorrectionProducer
	@echo ""
	@echo " [Producing scale factors to correct MT tail...]"
	@echo ""
	@LD_LIBRARY_PATH="$(shell $(ROOT_WITHROOFIT)/bin/root-config --libdir):$$LD_LIBRARY_PATH" ./MTTailCorrectionProducer.$(ExeSuf) > MTTailCorrectionProducer.$(LogSuf)
	@cat results/SF_MTtail.tab

#------------------------------------------------------------------------------


