struct HighestPt
{
  bool operator()( TLorentzVector p1, TLorentzVector p2 ) const {
        return p1.Pt() > p2.Pt() ;
  }
};

// Return true for multilepton events  with true truth-level track and events with true truth-level tau
bool IsMultilepEvtWithTrueTrkFromTau(){

	 bool hastruetrk = false;
	 bool hastruetau = false;
	 
	 //--- store index of genLeptons ---//
	 vector<int> genLeptons; 
	 for(unsigned int ip=0;ip<myEvent.genParticles.size();ip++){
		if(fabs(myEvent.genParticlesPDGId[ip])==11 || fabs(myEvent.genParticlesPDGId[ip])==13 || fabs(myEvent.genParticlesPDGId[ip])==15){
			if(abs(myEvent.genParticlesPDGId[myEvent.genParticlesMother[ip]])==23 || abs(myEvent.genParticlesPDGId[myEvent.genParticlesMother[ip]])==24){
				genLeptons.push_back(ip);
			}
		}
	 }
	 //---------------------------------//
	 //sort the vector in Pt
	 std::sort(genLeptons.begin(),genLeptons.end(),HighestPt());

	 // Event have at least 2 generated leptons and second lepton has [Eta| < 2.5
	 if (genLeptons.size()>1 && abs(myEvent.genParticles[genLeptons[1]].Eta())<2.5) {
	 	//check if second lepton is e/mu pT>5GeV
	 	// Second lepton is an electron or muon with pT > 5
	 	if (abs(myEvent.genParticlesPDGId[genLeptons[1]])<14 && myEvent.genParticles[genLeptons[1]].Pt()>5.) hastruetrk = true;
	 	//if second lepton is tau
	 	//check if daughter lepton or single track has pT>10GeV
		 if (abs(myEvent.genParticlesPDGId[genLeptons[1]])==15) {
	 		//access info of tau decay
			vector<int> tauDecay;
			bool tauHad = false;
			TLorentzVector taudvis;
			for(unsigned int td=genLeptons[1]+1;td<myEvent.genParticles.size();td++){
			if(myEvent.genParticlesMother[td]==genLeptons[1]){
				if(abs(myEvent.genParticlesPDGId[td])>16) tauHad = true;
				if(abs(myEvent.genParticlesPDGId[td])!=16 && abs(myEvent.genParticlesPDGId[td])!=14 && abs(myEvent.genParticlesPDGId[td])!=12) taudvis+=myEvent.genParticles[td];
				tauDecay.push_back(td);
				//int aid = abs(myEvent.genParticlesPDGId[td]);
				//if((aid>20) &&(aid!=22 && aid!=130 && aid!=111 && aid!=211)) cout<<" - "<<myEvent.genParticlesPDGId[td]<<endl;
			}
		}
		//check for hadronic tau
		if(tauHad){
			int ntracks = 0;
			float trackPt = 0;
			for(unsigned int vtd=0;vtd<tauDecay.size();vtd++){
				int aid = abs(myEvent.genParticlesPDGId[tauDecay[vtd]]);
				if(aid==211 || aid==321) {
					trackPt = myEvent.genParticles[tauDecay[vtd]].Pt();
					ntracks++;
				}
			}
			//if(ntracks!=1 && ntracks!=3) cout<<"had tau: "<<ntracks<<" tracks"<<endl;
			if (ntracks==1 && trackPt>10.) hastruetrk = true;
		}
		//check for leptonic decay
		else{
			for(unsigned int vtd=0;vtd<tauDecay.size();vtd++){
				if(abs(myEvent.genParticlesPDGId[tauDecay[vtd]]==11 || abs(myEvent.genParticlesPDGId[tauDecay[vtd]])==13))
					if(myEvent.genParticles[tauDecay[vtd]].Pt()>5.) hastruetrk = true;
			}
		}
	 	if (taudvis.Pt()>20.) hastruetau = true;
	  }
	 }
	 if (hastruetrk && hastruetau) return true;
	 return false;
}

//Return value of Mt(l,nu) for event with a W (default value = -9999.) 
double MtGen(){
 	double mtgen = -9999.;
	  // Find a lepton coming from a W
	  TLorentzVector Wcand;
	  bool foundNu = false;
	  bool foundLep = false;
	  int iNu = 0;
	  int iLep = 0;
	  //if(myEvent.genParticlesPDGId.size()==0 && myEvent.genParticlesMother.size()==0)
	  //	cout<<"MtGen()::size of the MC collections null  !!!"<<endl;

	  for(unsigned int ip=0;ip<myEvent.genParticles.size();ip++){
		if(fabs(myEvent.genParticlesPDGId[ip])==11 || fabs(myEvent.genParticlesPDGId[ip])==13 || fabs(myEvent.genParticlesPDGId[ip])==15){
			if(abs(myEvent.genParticlesPDGId[myEvent.genParticlesMother[ip]])==24){
				foundLep = true;
				iLep = ip;
			}
		}
	  	if(fabs(myEvent.genParticlesPDGId[ip])==12 || fabs(myEvent.genParticlesPDGId[ip])==14 || fabs(myEvent.genParticlesPDGId[ip])==16){
			if(abs(myEvent.genParticlesPDGId[myEvent.genParticlesMother[ip]])==24){
				foundNu = true;
				iNu = ip;
			}
		}
		if(foundNu && foundLep) break;
	  }
	  if(foundNu && foundLep){
		Wcand = myEvent.genParticles[iNu]+myEvent.genParticles[iLep];
		Wcand = TLorentzVector(0,myEvent.genParticles[iNu].Px()+myEvent.genParticles[iLep].Px(),myEvent.genParticles[iNu].Py()+myEvent.genParticles[iLep].Py(),myEvent.genParticles[iNu].E()+myEvent.genParticles[iLep].E());
		//cout<<Wcand.E()<<" "<<Wcand.M()<<" "<<Wcand.Mt()<<endl;
		//MGen = Wcand.M();
		mtgen = sqrt(Wcand.E()*Wcand.E()-Wcand.Pt()*Wcand.Pt());
	  }
	  return mtgen;
}

//compute the ratio |sum_i Pt_i|/sum|Pt_i| - default value is -9999.
//summation run over leptons && jets
double R(){
	TLorentzVector sum;
	vector<TLorentzVector> objects;
	
	//--- list interesting objects --//
	objects.insert(objects.begin(),myEvent.jets.begin(),myEvent.jets.end());
	//objects.push_back(myEvent.leadingLepton);

	double denom = 0;
	for(unsigned int i=0;i<objects.size();i++){
		//sum of Pt objects norm
		denom+=objects[i].Pt();
		//build the sum of objects
		sum+=objects[i];
	}
	//cout<<sum.Pt()<<" "<<denom<<endl;
	if(denom!=0) return sum.Pt()/denom;
	return -9999.;
}
