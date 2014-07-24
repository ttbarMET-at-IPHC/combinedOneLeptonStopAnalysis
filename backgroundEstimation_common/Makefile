
#------------------------------------------------------------------------------

#everything: MTtailCorrection checkCR4CR5modeling backgroundPrediction copyResults reports
everything: backgroundPrediction copyResults summaryPlots reports

#------------------------------------------------------------------------------

include common.make

.PHONY:MTtailCorrection checkCR4CR5modeling backgroundPrediction summaryPlots

MTtailCorrection:
	@make -C ../backgroundEstimation_MTtailCorrection 	 default

checkCR4CR5modeling:
	@make -C ../backgroundEstimation_checkCR4CR5modeling default

backgroundPrediction:
	@make -C ../backgroundEstimation_prediction 		 default
	
reports:
	@mkdir -p ./results/latest/report/
	@make --no-print-directory report SIGNAL_REGIONS="preselection"    REPORT_TAG="preselection" REPORT_LABEL="Preselection"
	@make --no-print-directory report SIGNAL_REGIONS="$(CnC_T2tt)"     REPORT_TAG="CnC_T2tt"     REPORT_LABEL="Cut \& count, T2tt"
	@make --no-print-directory report SIGNAL_REGIONS="$(CnC_T2bw025)"  REPORT_TAG="CnC_T2bw025"  REPORT_LABEL="Cut \& count, T2bw, x=0.25"
	@make --no-print-directory report SIGNAL_REGIONS="$(CnC_T2bw050)"  REPORT_TAG="CnC_T2bw050"  REPORT_LABEL="Cut \& count, T2bw, x=0.50"
	@make --no-print-directory report SIGNAL_REGIONS="$(CnC_T2bw075)"  REPORT_TAG="CnC_T2bw075"  REPORT_LABEL="Cut \& count, T2bw, x=0.75"
	@make --no-print-directory report SIGNAL_REGIONS="$(BDT_T2tt)"     REPORT_TAG="BDT_T2tt"     REPORT_LABEL="BDT, T2tt"
	@make --no-print-directory report SIGNAL_REGIONS="$(BDT_T2bw025)"  REPORT_TAG="BDT_T2bw025"  REPORT_LABEL="BDT, T2bw, x=0.25"
	@make --no-print-directory report SIGNAL_REGIONS="$(BDT_T2bw050)"  REPORT_TAG="BDT_T2bw050"  REPORT_LABEL="BDT, T2bw, x=0.50"
	@make --no-print-directory report SIGNAL_REGIONS="$(BDT_T2bw075)"  REPORT_TAG="BDT_T2bw075"  REPORT_LABEL="BDT, T2bw, x=0.75"

report: reportProducer.$(ExeSuf) 
	./reportProducer.$(ExeSuf) $(REPORT_TAG) "$(REPORT_LABEL)" $(SIGNAL_REGIONS) > results/latest/report/$(REPORT_TAG).tex;
	cd results/latest/report/ && pdflatex $(REPORT_TAG).tex 
	
copyResults:
	@DATE=$(shell date +%Y_%m_%d_%H:%M:%S);			\
	 mkdir -p ./results/$$DATE/;					\
	 cp -r ../backgroundEstimation_MTtailCorrection/results    			./results/$$DATE/MTtailCorrection/;						\
	 cp -r ../backgroundEstimation_MTtailCorrection/inputPlots 			./results/$$DATE/MTtailCorrection/inputPlots/;			\
	 cp -r ../backgroundEstimation_checkCR4CR5modeling/results			./results/$$DATE/checkCR4CR5modeling/;					\
	 cp -r ../backgroundEstimation_checkCR4CR5modeling/rawYieldTables	./results/$$DATE/checkCR4CR5modeling/rawYieldTables/;	\
	 cp -r ../backgroundEstimation_checkCR4CR5modeling/scaleFactors		./results/$$DATE/checkCR4CR5modeling/scaleFactors/;		\
	 cp -r ../backgroundEstimation_prediction/prediction				./results/$$DATE/prediction/;							\
	 cp -r ../backgroundEstimation_prediction/rawYieldTables			./results/$$DATE/prediction/rawYieldTables/;			\
	 cp -r ../backgroundEstimation_prediction/scaleFactors				./results/$$DATE/prediction/scaleFactors/;				\
	 cp -r ../backgroundEstimation_prediction/systematics				./results/$$DATE/prediction/systematics/;				\
	 rm -f ./results/latest; cd results; ln -s ./$$DATE/ ./latest

summaryPlots:
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(CnC_T2tt)"    TAG="CnC_T2tt"    LABEL="Cut & count, T2tt"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(CnC_T2bw025)" TAG="CnC_T2bw025" LABEL="Cut & count, T2bw, x=0.25"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(CnC_T2bw050)" TAG="CnC_T2bw050" LABEL="Cut & count, T2bw, x=0.50"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(CnC_T2bw075)" TAG="CnC_T2bw075" LABEL="Cut & count, T2bw, x=0.75"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(BDT_T2tt)"    TAG="BDT_T2tt"    LABEL="BDT, T2tt"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(BDT_T2bw025)" TAG="BDT_T2bw025" LABEL="BDT, T2bw, x=0.25"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(BDT_T2bw050)" TAG="BDT_T2bw050" LABEL="BDT, T2bw, x=0.50"
	@make --no-print-directory produceSummaryPlots SIGNAL_REGIONS="$(BDT_T2bw075)" TAG="BDT_T2bw075" LABEL="BDT, T2bw, x=0.75"

produceSummaryPlots: yieldsSummaryProducer.$(ExeSuf) scaleFactorsSummaryProducer.$(ExeSuf)
	./yieldsSummaryProducer.$(ExeSuf) 		"$(TAG)" "$(LABEL)" $(SIGNAL_REGIONS)
	./scaleFactorsSummaryProducer.$(ExeSuf) "$(TAG)" "$(LABEL)" $(SIGNAL_REGIONS)

cleanAll:
	@make -C ../backgroundEstimation_MTtailCorrection 	 clean
	@make -C ../backgroundEstimation_checkCR4CR5modeling clean
	@make -C ../backgroundEstimation_prediction 		 clean
	@make -C ./ 										 clean


#computeBDTCuts: computeBDTCuts.$(ExeSuf)
#	./computeBDTCuts.$(ExeSuf) 

#syst: reportGenSystProducer.$(ExeSuf)
#	./reportGenSystProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportGenSystProducer.$(LogSuf) > reports/reportWithSyst.tex && cd reports && pdflatex report.tex


