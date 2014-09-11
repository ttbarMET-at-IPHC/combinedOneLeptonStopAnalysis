
#------------------------------------------------------------------------------

everything: MTtailCorrection checkCR4CR5modeling backgroundPrediction copyResults summaryPlots reports

#------------------------------------------------------------------------------

include common.make

.PHONY:MTtailCorrection checkCR4CR5modeling backgroundPrediction summaryPlots

MTtailCorrection:
	@make -C ../MTtailCorrection 	 default

checkCR4CR5modeling:
	@make -C ../checkCR4CR5modeling default

backgroundPrediction:
	@make -C ../prediction 		 default
	
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
	#cd results/latest/report/ && pdflatex $(REPORT_TAG).tex 

reports_secondLep:
	@mkdir -p ./results/latest/reportSecondLep/
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="preselection"    REPORT_TAG="preselection" REPORT_LABEL="Preselection"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(CnC_T2tt)"     REPORT_TAG="CnC_T2tt"     REPORT_LABEL="Cut \& count, T2tt"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(CnC_T2bw025)"  REPORT_TAG="CnC_T2bw025"  REPORT_LABEL="Cut \& count, T2bw, x=0.25"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(CnC_T2bw050)"  REPORT_TAG="CnC_T2bw050"  REPORT_LABEL="Cut \& count, T2bw, x=0.50"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(CnC_T2bw075)"  REPORT_TAG="CnC_T2bw075"  REPORT_LABEL="Cut \& count, T2bw, x=0.75"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(BDT_T2tt)"     REPORT_TAG="BDT_T2tt"     REPORT_LABEL="BDT, T2tt"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(BDT_T2bw025)"  REPORT_TAG="BDT_T2bw025"  REPORT_LABEL="BDT, T2bw, x=0.25"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(BDT_T2bw050)"  REPORT_TAG="BDT_T2bw050"  REPORT_LABEL="BDT, T2bw, x=0.50"
	@make --no-print-directory report_secondLep SIGNAL_REGIONS="$(BDT_T2bw075)"  REPORT_TAG="BDT_T2bw075"  REPORT_LABEL="BDT, T2bw, x=0.75"

report_secondLep: reportProducer_secondLep.$(ExeSuf) 
	./reportProducer_secondLep.$(ExeSuf) $(REPORT_TAG) "$(REPORT_LABEL)" $(SIGNAL_REGIONS) > results/latest/reportSecondLep/$(REPORT_TAG).tex;
	#cd results/latest/report/ && pdflatex $(REPORT_TAG).tex 


copyResults:
	@DATE=$(shell date +%Y_%m_%d_%H:%M:%S);			\
	 mkdir -p ./results/$$DATE/;					\
	 cp -r ../MTtailCorrection/results    			./results/$$DATE/MTtailCorrection/;						\
	 cp -r ../MTtailCorrection/inputPlots 			./results/$$DATE/MTtailCorrection/inputPlots/;			\
	 cp -r ../checkCR4CR5modeling/results			./results/$$DATE/checkCR4CR5modeling/;					\
	 cp -r ../checkCR4CR5modeling/rawYieldTables	./results/$$DATE/checkCR4CR5modeling/rawYieldTables/;	\
	 cp -r ../checkCR4CR5modeling/scaleFactors		./results/$$DATE/checkCR4CR5modeling/scaleFactors/;		\
	 cp -r ../prediction/prediction					./results/$$DATE/prediction/;							\
	 cp -r ../prediction/rawYieldTables				./results/$$DATE/prediction/rawYieldTables/;			\
	 cp -r ../prediction/secondLeptonInAcceptance	./results/$$DATE/prediction/secondLeptonInAcceptance/;	\
	 cp -r ../prediction/scaleFactors				./results/$$DATE/prediction/scaleFactors/;				\
	 cp -r ../prediction/systematics				./results/$$DATE/prediction/systematics/;				\
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
	@make --no-print-directory produceSummaryPlotsMTtailCorrection SIGNAL_REGIONS="BDT_T2tt_1 BDT_T2tt_2 BDT_T2tt_5 BDT_T2bw075_1 BDT_T2bw075_2 BDT_T2bw075_3 BDT_T2bw075_5 BDT_T2bw050_1 BDT_T2bw050_3 BDT_T2bw050_4 BDT_T2bw050_5 BDT_T2bw050_6 BDT_T2bw025_1 BDT_T2bw025_3 BDT_T2bw025_4 BDT_T2bw025_6" TAG="BDT" LABEL="BDT"
	@make --no-print-directory produceSummaryPlotsMTtailCorrection SIGNAL_REGIONS="MT_100 MT_120 MT_125 MT_130 MT_135 MT_140" TAG="individualMTcuts" LABEL="Individual MT cuts"
	@make --no-print-directory produceSummaryPlotsMTtailCorrection SIGNAL_REGIONS="MET_200 MET_250 MET_300" TAG="individualMETcuts" LABEL="Individual MET cuts"
	@make --no-print-directory produceSummaryPlotsMTtailCorrection SIGNAL_REGIONS="METoverSqrtHT_6 METoverSqrtHT_7 METoverSqrtHT_8 METoverSqrtHT_9 METoverSqrtHT_10 METoverSqrtHT_12" TAG="individualMETsqrtHTcuts" LABEL="Individual MET/sqrt(H_{T}) cuts"

produceSummaryPlots: yieldsSummaryProducer.$(ExeSuf) scaleFactorsSummaryProducer.$(ExeSuf)
	./yieldsSummaryProducer.$(ExeSuf) 		"$(TAG)" "$(LABEL)" $(SIGNAL_REGIONS)
	./scaleFactorsSummaryProducer.$(ExeSuf) "$(TAG)" "$(LABEL)" $(SIGNAL_REGIONS)

produceSummaryPlotsMTtailCorrection: MTtailCorrectionSummaryProducer.$(ExeSuf)
	./MTtailCorrectionSummaryProducer.$(ExeSuf) "$(TAG)" "$(LABEL)" $(SIGNAL_REGIONS)

cleanAll:
	@make -C ../MTtailCorrection 	 	clean
	@make -C ../checkCR4CR5modeling 	clean
	@make -C ../prediction 		 		clean
	@make -C ./ 						clean


BDTCutsWithCustomRequirements: computeBDTCutsWithCustomRequirements.$(ExeSuf)
	./computeBDTCutsWithCustomRequirements.$(ExeSuf) 

#syst: reportGenSystProducer.$(ExeSuf)
#	./reportGenSystProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportGenSystProducer.$(LogSuf) > reports/reportWithSyst.tex && cd reports && pdflatex report.tex


