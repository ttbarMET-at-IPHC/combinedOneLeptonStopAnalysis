

void ResoPlots(bool basic_cut = false, float met_cut = 0, float mt_cut = 0, bool active_veto = false){
	gStyle->SetOptFit();

	TFile* fin = new TFile("metResoTerm.root","OPEN");
	TTree* tree = (TTree*) fin->Get("T");
     	float massWl, massTl, massTm;
	float met, MT, dphi, hadchi2,topness;
	bool PassTauVeto, PassTrackVeto;
	tree->SetBranchAddress("massWl", &massWl);
     	tree->SetBranchAddress("massTl", &massTl);
     	tree->SetBranchAddress("massTm", &massTm);
     	tree->SetBranchAddress("met", &met);
     	tree->SetBranchAddress("MT", &MT);
     	tree->SetBranchAddress("dphi", &dphi);
     	tree->SetBranchAddress("hadchi2", &hadchi2);
     	tree->SetBranchAddress("topness", &topness);
     	tree->SetBranchAddress("PassTauvVeto", &PassTauVeto);
     	tree->SetBranchAddress("PassTrackVeto", &PassTrackVeto);


        TH1F* hmassWl = new TH1F("hmassWl","",50,0,200);
        TH1F* hmassTl = new TH1F("hmassTl","",50,0,500);
        TH1F* hmassTm = new TH1F("hmassTm","",50,0,500);
        TH1F* hmass2Wl = new TH1F("hmass2Wl","",50,0,15000);
        TH1F* hmass2Tl = new TH1F("hmass2Tl","",50,0,150000);
        TH1F* hmass2Tm = new TH1F("hmass2Tm","",50,0,300000);

	hmassWl->GetYaxis()->SetTitle("U.A");
	hmass2Wl->GetYaxis()->SetTitle("U.A");
	hmassTl->GetYaxis()->SetTitle("U.A");
	hmass2Tl->GetYaxis()->SetTitle("U.A");
	hmassTm->GetYaxis()->SetTitle("U.A");
	hmass2Tm->GetYaxis()->SetTitle("U.A");
	
	hmassWl->GetXaxis()->SetTitle("mass(W_{l}) [GeV]");
	hmass2Wl->GetXaxis()->SetTitle("mass^{2}(W_{l}) [GeV^{2}]");
	hmassTl->GetXaxis()->SetTitle("mass(top_{l}) [GeV]");
	hmass2Tl->GetXaxis()->SetTitle("mass^{2}(top_{l}) [GeV^{2}]");
	hmassTm->GetXaxis()->SetTitle("mass(top_{m}) [GeV]");
	hmass2Tm->GetXaxis()->SetTitle("mass^{2}(top_{m}) [GeV^{2}]");


   	// Loop over the tree
	for(int i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		
		if(basic_cut)
			if(hadchi2>10 || abs(dphi)<0.8) continue;
		if(active_veto)
			if(!PassTrackVeto || !PassTauVeto) continue;
		if(met<met_cut) continue;
		if(MT<mt_cut) continue;

		hmassWl->Fill(massWl);
		hmassTl->Fill(massTl);
		hmassTm->Fill(massTm);
		hmass2Wl->Fill(massWl*massWl);
		hmass2Tl->Fill(massTl*massTl);
		hmass2Tm->Fill(massTm*massTm);

	}
	
	char label[120];
	sprintf(label,"%d_%d_%d_%d",(int)basic_cut, (int)active_veto, (int)met_cut, (int)mt_cut);
	cout<<label<<endl;
	string cname;
	

	// Draw mass
	int sizeX = 1400;
	int sizeY = 500;
	TCanvas* c = new TCanvas("c1", "mass", sizeX, sizeY);
	c->Divide(3);
	c->cd(1);
	hmassWl->Fit("gaus",0,0,60,100);
	hmassWl->Draw();
	c->cd(2);
	hmassTl->Draw();
	hmassTl->Fit("gaus",0,0,130,200);
	c->cd(3);
	hmassTm->Draw();
	hmassTm->Fit("gaus",0,0,130,200);
	cname = "mass_Reso_"+string(label)+".png";
	c->Print(cname.c_str());
	
	// Draw mass^2
	TCanvas* c2 = new TCanvas("c2","mass^2", sizeX, sizeY);
	c2->Divide(3);
	c2->cd(1);
	hmass2Wl->Draw();
	hmass2Wl->Fit("gaus",0,0,5000,7500);
	c2->cd(2);
	hmass2Tl->Draw();
	hmass2Tl->Fit("gaus",0,0,15000,35000);
	c2->cd(3);
	hmass2Tm->Draw();
	hmass2Tm->Fit("gaus",0,0,10000,40000);
	cname = "mass_Reso_"+string(label)+".png";
	c2->Print(cname.c_str());

	cout<<"#################################"<<endl;
	cout<<"aWl \t"<<hmassWl->GetRMS()<<" "<<hmassWl->GetFunction("gaus")->GetParameter(2)<<endl;
	cout<<"aTl \t"<<hmassTl->GetRMS()<<" "<<hmassTl->GetFunction("gaus")->GetParameter(2)<<endl;
	cout<<"aTm \t"<<hmassTm->GetRMS()<<" "<<hmassTm->GetFunction("gaus")->GetParameter(2)<<endl;
	//cout<<"aWl-2 \t"<<hmass2Wl->GetRMS()<<" "<<hmass2Wl->GetFunction("gaus")->GetParameter(2)<<endl;
	//cout<<"aTl-2 \t"<<hmass2Tl->GetRMS()<<" "<<hmass2Tl->GetFunction("gaus")->GetParameter(2)<<endl;
	//cout<<"aTm-2 \t"<<hmass2Tm->GetRMS()<<" "<<hmass2Tm->GetFunction("gaus")->GetParameter(2)<<endl;
	cout<<"aWl-2 \t"<<sqrt(hmass2Wl->GetRMS())<<" "<<sqrt(hmass2Wl->GetFunction("gaus")->GetParameter(2))<<endl;
	cout<<"aTl-2 \t"<<sqrt(hmass2Tl->GetRMS())<<" "<<sqrt(hmass2Tl->GetFunction("gaus")->GetParameter(2))<<endl;
	cout<<"aTm-2 \t"<<sqrt(hmass2Tm->GetRMS())<<" "<<sqrt(hmass2Tm->GetFunction("gaus")->GetParameter(2))<<endl;
}

void All(){
	ResoPlots(false, 0,  0, false);
	ResoPlots(true, 0,  0, true);
	ResoPlots(true, 0,  150, true);
	ResoPlots(true, 200,  0, true);
	ResoPlots(true, 200,  150, true);
}
