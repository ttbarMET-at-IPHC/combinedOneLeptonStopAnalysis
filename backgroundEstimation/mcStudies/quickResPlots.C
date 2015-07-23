{
 //TFile* fin = new TFile("/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_30_06/skimming2/ttbar-madgraph.root","OPEN");
 TFile* fin = new TFile("/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_30_06/skimming_noMETcut/ttbar-madgraph.root","OPEN");
 TTree* tree = (TTree*) fin->Get("babyTuple");
 float genMET = 0;
 float pfmet = 0;
 tree->SetBranchAddress("genMET", &genMET);
 tree->SetBranchAddress("pfmet",&pfmet);
 int nbins = 5;
 double* xbins = new double[nbins+1];
 xbins[0] = 80;
 xbins[1] = 100;
 xbins[2] = 150;
 xbins[3] = 200;
 xbins[4] = 250;
 xbins[5] = 500;
 TProfile* profReso = new TProfile("pMETReso", "", 5, xbins,"s");
 TProfile* profBias = new TProfile("pMETBias", "", 5, xbins);
 for(unsigned int i=0;i<tree->GetEntries();i++){
 	tree->GetEntry(i);
	//cout<<genMET<<endl;
 	profBias->Fill(pfmet, (pfmet-genMET)/pfmet);
 	profReso->Fill(pfmet, (pfmet-genMET));
 }
 TCanvas* c = new TCanvas();
 c->Divide(2);
 c->cd(1);
 //profBias->Draw();
 TH1F* hReso = profReso->DrawCopy("hReso");
 for(int b=1;b<hReso.GetNbinsX()+1;b++){
 	cout<<profReso->GetBinLowEdge(b)<<" "<<profBias->GetBinContent(b)<<" "<<profReso->GetBinError(b)<<" "<<profReso->GetBinContent(b)<<endl;
 	hReso->SetBinContent(b,profReso->GetBinError(b));
 	hReso->SetBinError(b,0);
 }
 c->cd(2);
 hReso->Draw();
 //profReso->Draw(); 
}
