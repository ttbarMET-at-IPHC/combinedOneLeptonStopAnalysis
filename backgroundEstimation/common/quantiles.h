#ifndef _QUANTILES_
#define _QUANTILES_

//--- Access to quantiles of the plots   ---//
float QuantileVariabelCut(SonicScrewdriver& sonic, float quantile, string var, vector<string> processClass, string region, string channel){
	//retrieve plots for the all processClasses
	TH1F* hclone = 0;
	TH1F* hMCSum = 0; // sum of all processClasses
	for(unsigned int i=0;i<processClass.size();i++){
		hclone = sonic.Get1DHistoClone(var, processClass[i], region, channel);
		if(i==0) hMCSum = hclone;
		else hMCSum->Add(hclone);
	}
	//Search for the quantile
	float integral = hMCSum->Integral();
	float frac  = 0;
	int qbin = 0; // bin where quantile is found
	for(int i=0;i<hMCSum->GetNbinsX();i++){
		if(integral!=0) frac = hMCSum->Integral(0,i)/integral;
		if(frac>=quantile){
			qbin = i;
			break;
		}
	}
	if(integral!=0 && qbin!=hMCSum->GetNbinsX()){
		return hMCSum->GetBinCenter(qbin);
	}
	return -1;
}
  //
float MinNofEvtsVariabelCut(SonicScrewdriver& sonic, int nof, string var, vector<string> processClass, string region, string channel){
	//retrieve plots for the all processClasses
	TH1F* hclone = 0;
	TH1F* hMCSum = 0; // sum of all processClasses
	for(unsigned int i=0;i<processClass.size();i++){
		hclone = sonic.Get1DHistoClone(var, processClass[i], region, channel);
		if(i==0) hMCSum = hclone;
		else hMCSum->Add(hclone);
	}
	//Search for the quantile
	float integral = 0;
	int qbin = 0; // bin where quantile is found
	for(int i=hMCSum->GetNbinsX();i>0;i--){
		integral = hMCSum->Integral(i,hMCSum->GetNbinsX());
		if(integral>=nof){
			qbin = i;
			break;
		}
	}
	if(integral!=0 && qbin!=hMCSum->GetNbinsX()){
		return hMCSum->GetBinCenter(qbin);
	}
	return -1;
}

vector<float> EquiStatIndepIntervalInTail(SonicScrewdriver& sonic, int nofEvts, int nofInterval, string var, vector<string> processClass, string region, string channel){
	vector<float> Bounds(nofInterval);
	//retrieve plots for the all processClasses
	TH1F* hclone = 0;
	TH1F* hMCSum = 0; // sum of all processClasses
	for(unsigned int i=0;i<processClass.size();i++){
		hclone = sonic.Get1DHistoClone(var, processClass[i], region, channel);
		if(i==0) hMCSum = hclone;
		else hMCSum->Add(hclone);
	}
	//Search for bounds
	float integral = 0;
	int qbin = hMCSum->GetNbinsX()+1; // bin where quantile is found
	int iter = 0;
	for(int i=hMCSum->GetNbinsX();i>0;i--){
		integral = hMCSum->Integral(i,qbin-1);
		if(integral>=nofEvts){
			qbin = i;
			Bounds[iter] = hMCSum->GetBinCenter(qbin);
			iter++;
			if(iter==nofInterval) break;
		}
	}
	return Bounds;
}

#endif
