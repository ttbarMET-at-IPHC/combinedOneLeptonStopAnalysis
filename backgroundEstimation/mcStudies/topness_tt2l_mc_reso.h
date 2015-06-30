#ifndef topness_tt2l_mc_res
#define topness_tt2l_mc_res


#define top_status_code 62
#define W_status_code 52
#define lepton_status_code 22

// check MC info
//
struct tt2l_mc_info{
    // Global variables
    float genMET;
    float Pz_ttbar;
    float Pt_ttbar;
    float CM_Q2;
    //Lost branch
    TLorentzVector lostW;
    TLorentzVector lostLepton;
    TLorentzVector lostNeutrino;
    TLorentzVector lostB;
    //Visible branch
    TLorentzVector visLepton;
    TLorentzVector visNeutrino;
    TLorentzVector visB;
    
    //Top and its decay
    TLorentzVector top;
    bool top_lepdecay;
    int top_lepid;
    TLorentzVector top_W;
    TLorentzVector top_b;
    TLorentzVector top_l;
    TLorentzVector top_nu;
    TLorentzVector top_q;
    TLorentzVector top_qbar;
    
    //annti-Top and its decay
    TLorentzVector atop;
    bool atop_lepdecay;
    int atop_lepid;
    TLorentzVector atop_W;
    TLorentzVector atop_b;
    TLorentzVector atop_l;
    TLorentzVector atop_nu;
    TLorentzVector atop_q;
    TLorentzVector atop_qbar;

    //ttbar system:
    TLorentzVector ttbar;
    int ttbar_decay; //0: had, 1: lep, 2: dilep
    
    // ISR
    vector<int> isr_id;
    vector<TLorentzVector> isr_p4;
};

ostream&  PrintP4(const TLorentzVector& a, ostream& os = cout){
	os<<"Pt = "<<a.Pt()<<" "<<"Eta = "<<a.Eta()<<" Phi = "<<a.Phi()<<" E = "<<a.E();//<<endl;
	return os;
}

void DumpMCinfo(tt2l_mc_info& mcinfo){
	cout<<"ttbar: \tpt = "<<mcinfo.ttbar.Pt()<<"\tpz = "<<mcinfo.ttbar.Pz()<<endl;
	cout<<"decay";
	switch(mcinfo.ttbar_decay){
		case 0: cout<<" hadronic"; break;
		case 1: cout<<" semi-lep"; break;
		case 2: cout<<" di-lept "; break;
		default: cout<<" unknwon "; break;
	}
	cout<<endl;
	// top info
	cout<<"### top ";
	if(mcinfo.top_lepdecay) cout<<"(lep)";
	else cout<<"(had)";
	cout<<PrintP4(mcinfo.top)<<endl;
	// top decay
	cout<<" > bquark ";
	cout<<PrintP4(mcinfo.top_b)<<endl;
	if(mcinfo.top_lepdecay){
		cout<<" > lepton ("<<mcinfo.top_lepid<<") ";
		cout<<PrintP4(mcinfo.top_l)<<endl;
		cout<<" > neutrino ";
		cout<<PrintP4(mcinfo.top_nu)<<endl;
	}
	else{
		cout<<" > quark  ";
		cout<<PrintP4(mcinfo.top_q)<<endl;
		cout<<" > aquark ";
		cout<<PrintP4(mcinfo.top_qbar)<<endl;
	}
	// anti-top
	cout<<"### anti-top ";
	if(mcinfo.atop_lepdecay) cout<<"(lep) ";
	else cout<<"(had) ";
	cout<<PrintP4(mcinfo.atop)<<endl;
	// atop decy
	cout<<" > bquark ";
	cout<<PrintP4(mcinfo.atop_b)<<endl;
	if(mcinfo.atop_lepdecay){
		cout<<" > lepton (";
		cout<<mcinfo.atop_lepid<<") ";
		cout<<PrintP4(mcinfo.atop_l)<<endl;
		cout<<" > neutrino ";
		cout<<PrintP4(mcinfo.atop_nu)<<endl;
	}
	else{
		cout<<" > quark  ";
		cout<<PrintP4(mcinfo.atop_q)<<endl;
		cout<<" > aquark ";
		cout<<PrintP4(mcinfo.atop_qbar)<<endl;
	}

	cout<<"### ISR: "<<mcinfo.isr_id.size()<<endl;
	for(unsigned int i=0;i<mcinfo.isr_id.size();i++){
		cout<<" > "<<mcinfo.isr_id[i]<<" ";
		cout<<PrintP4(mcinfo.isr_p4[i])<<endl;
	}
}

void Fill_tt2l_mc_info(tt2l_mc_info& mc_info, babyEvent& myevent){
   //cout<<"SIZE = "<<myevent.gen_id.size()<<endl;
   vector<int> isr;
   for(unsigned int i=0;i<myevent.gen_id.size();i++){
      /*
      cout<<i<<" "<<myevent.gen_id[i]<<" "<<myevent.gen_status[i]<<" "<<myevent.gen_mother_index[i]<<endl;
      for(unsigned int j=0;j<myevent.gen_daughter_index[i].size();j++){
	cout<<"\t"<<myevent.gen_daughter_index[i][j]<<"\t";
      }
      cout<<endl;
      */
       

      //looking for radiations
      if(myevent.gen_id[i] == 2212){
	for(unsigned int d = 0;d<myevent.gen_daughter_index[i].size();d++){
		int indice = myevent.gen_daughter_index[i][d];
		//search for radiation (quarks or gluons - they should be in the 20st particles and pt>0
		if((abs(myevent.gen_id[indice])<=5 || myevent.gen_id[indice]==21) && indice<20 && myevent.gen_pt[indice]>0){
			//search if we do not duplicate it
			bool found = false;
			for(unsigned iv=0;iv<isr.size();iv++){
				if(isr[iv]==indice){
					found = true;
					break;
				}
			}
			if(!found) isr.push_back(indice);
		}
	}
      }

      //if(i>50) break;
      //found tops
      if (abs(myevent.gen_id[i]) == 6  && myevent.gen_status[i] == top_status_code && myevent.gen_daughter_index[i].size() == 2){
	//cout<<"we found a top !"<<endl;
	int sign = 1;
	if (myevent.gen_id[i]<0) sign = -1;
	//found a top
	if(sign>0) mc_info.top.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
	//found an anti-top
	if(sign<0) mc_info.atop.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
      	for(unsigned int d = 0; d<myevent.gen_daughter_index[i].size();d++){
		// finding b from top decay
		int indice = myevent.gen_daughter_index[i][d];
		//cout<<indice<<endl;
		//cout<<myevent.gen_id[indice]<<endl;
		if(myevent.gen_id[indice] == sign*5){
		    if(sign>0) mc_info.top_b.SetPtEtaPhiM(myevent.gen_pt[indice], myevent.gen_eta[indice], myevent.gen_phi[indice], myevent.gen_m[indice]);
		    else  mc_info.atop_b.SetPtEtaPhiM(myevent.gen_pt[indice], myevent.gen_eta[indice], myevent.gen_phi[indice], myevent.gen_m[indice]);
		}
		// finding W from top decay
		if(myevent.gen_id[indice] == sign*24){
			if(myevent.gen_daughter_index[indice].size()!=0){
				int indice2 = myevent.gen_daughter_index[indice][0];
				//finding a W after raditation
				if(myevent.gen_id[indice2] == sign*24 &&  myevent.gen_status[indice2]== W_status_code){
					if(sign>0) mc_info.top_W.SetPtEtaPhiM(myevent.gen_pt[indice2], myevent.gen_eta[indice2], myevent.gen_phi[indice2], myevent.gen_m[indice2]);
					else mc_info.atop_W.SetPtEtaPhiM(myevent.gen_pt[indice2], myevent.gen_eta[indice2], myevent.gen_phi[indice2], myevent.gen_m[indice2]);
					for(unsigned dd = 0;dd<myevent.gen_daughter_index[indice2].size();dd++){
						int indice3 = myevent.gen_daughter_index[indice2][dd];
						//looking for W decays (lnu or qqbar)
						// find lepton
						if(myevent.gen_id[indice3] == sign*(-11) || myevent.gen_id[indice3] == sign*(-13) || myevent.gen_id[indice3] == sign*(-15)){
							if(sign>0) mc_info.top_l.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
							else mc_info.atop_l.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
							if(sign>0){
								mc_info.top_lepdecay = true;
								mc_info.top_lepid = myevent.gen_id[indice3];
							}
							else{
								mc_info.atop_lepdecay = true;
								mc_info.atop_lepid = myevent.gen_id[indice3];
							}
						}
						// find neutrino
						if(myevent.gen_id[indice3] == sign*12 || myevent.gen_id[indice3] == sign*14 || myevent.gen_id[indice3] == sign*16){
							 if(sign>0) mc_info.top_nu.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
							 else mc_info.atop_nu.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
						}
						// find quark
						if(myevent.gen_id[indice3]> 0 &&  myevent.gen_id[indice3] <5){
							if(sign>0) mc_info.top_q.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
							else mc_info.atop_q.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
							 if(sign>0) mc_info.top_lepdecay = false;
							 else mc_info.atop_lepdecay = false;
						}
						// find anti-quark
						if(myevent.gen_id[indice3]< 0 &&  abs(myevent.gen_id[indice3]) <5){
							if(sign>0) mc_info.top_qbar.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
							else mc_info.atop_qbar.SetPtEtaPhiM(myevent.gen_pt[indice3], myevent.gen_eta[indice3], myevent.gen_phi[indice3], myevent.gen_m[indice3]);
						}
					}
				}
			}
		}
	}
      }

      }
      // adding the ISR
      for(unsigned int is=0;is<isr.size();is++){
      	mc_info.isr_id.push_back(myevent.gen_id[isr[is]]);
      	TLorentzVector part;
	int indice = isr[is];
	part.SetPtEtaPhiM(myevent.gen_pt[indice], myevent.gen_eta[indice], myevent.gen_phi[indice], myevent.gen_m[indice]);
        mc_info.isr_p4.push_back(part);
      }

      //if you've found ttbar
      mc_info.ttbar_decay = -1;
      if(mc_info.top != TLorentzVector() && mc_info.atop != TLorentzVector()){
      		mc_info.ttbar_decay = 0;
      		mc_info.ttbar = mc_info.top+mc_info.atop;
		//---------------------------
    		//Invisble part of the event
		//---------------------------
		mc_info.genMET = 0;
		TLorentzVector inv;
		//sum neutrino
		if (mc_info.top_lepdecay){ 
      			mc_info.ttbar_decay++;
			inv.SetPx(mc_info.top_l.Px());
			inv.SetPy(mc_info.top_l.Py());
		}
		if (mc_info.atop_lepdecay){ 
      			mc_info.ttbar_decay++;
			inv.SetPx(mc_info.top_l.Px());
			inv.SetPy(mc_info.top_l.Py());
		}
		mc_info.genMET = inv.Pt();
		//---------------------------

    		mc_info.Pz_ttbar = mc_info.ttbar.Pz();
    		mc_info.Pt_ttbar = mc_info.ttbar.Pt();
      /*
      if (myevent.gen_id[i] == -6 && myevent.gen_status[i] == top_status_code)  itbar = i;
      //found
      // top - assigned temporary to lost branch
      if ( (myevent.gen_id[i] == -11 || myevent.gen_id[i] == -13 || myevent.gen_id[i] == -15) && myevent.gen_status[i] == lepton_status_code && myevent.gen_id[myevent.gen_mother_index[i]] == 24 
      	&& myevent.gen_id[myevent.gen_mother_index[myevent.gen_mother_index[i]]] == 6) {
	mc_info.lostLepton.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
        nof_leptons++;
      }
      if ( (myevent.gen_id[i] == 12 || myevent.gen_id[i] == 14 || myevent.gen_id[i] == 16) && myevent.gen_status[i] == lepton_status_code && myevent.gen_id[myevent.gen_mother_index[i]] == 24 
      	&& myevent.gen_id[myevent.gen_mother_index[myevent.gen_mother_index[i]]] == 6) 
      	mc_info.lostNeutrino.SetPtEtaPhiE(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
      if ( myevent.gen_id[i] == 5 && myevent.gen_status[i] == lepton_status_code && myevent.gen_id[myevent.gen_mother_index[i]] == 6 )
      	mc_info.lostB.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
      // top - assigned temporary to lost branch
      if ( (myevent.gen_id[i] == 11 || myevent.gen_id[i] == 13 || myevent.gen_id[i] == 15) && myevent.gen_status[i] == lepton_status_code && myevent.gen_id[myevent.gen_mother_index[i]] == -24  
      	&& myevent.gen_id[myevent.gen_mother_index[myevent.gen_mother_index[i]]] == -6) {
	mc_info.visLepton.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
        nof_leptons++;
      }
      if ( (myevent.gen_id[i] == -12 || myevent.gen_id[i] == -14 || myevent.gen_id[i] == -16) && myevent.gen_status[i] == lepton_status_code && myevent.gen_id[myevent.gen_mother_index[i]] == -24 
      	&& myevent.gen_id[myevent.gen_mother_index[myevent.gen_mother_index[i]]] == -6) 
      	mc_info.visNeutrino.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
      if ( myevent.gen_id[i] == -5 && myevent.gen_status[i] == lepton_status_code && myevent.gen_id[myevent.gen_mother_index[i]] == -6 ){
	mc_info.visB.SetPtEtaPhiM(myevent.gen_pt[i], myevent.gen_eta[i], myevent.gen_phi[i], myevent.gen_m[i]);
	cout<<"pass !"<<endl;
	*/
	//}
   
   }
   // Check ttbar
   /*
   if(itop!=-1 && itbar !=-1){
       TLorentzVector top_p4;
       TLorentzVector tbar_p4;
       top_p4.SetPtEtaPhiM(myevent.gen_pt[itop], myevent.gen_eta[itop], myevent.gen_phi[itop], myevent.gen_m[itop]);
       tbar_p4.SetPtEtaPhiM(myevent.gen_pt[itop], myevent.gen_eta[itop], myevent.gen_phi[itop], myevent.gen_m[itop]);
       mc_info.Pz_ttbar = (top_p4+tbar_p4).Pz();
       mc_info.CM_Q2 = pow((top_p4+tbar_p4).E(),2);
       cout<<"found ttbar"<<endl;
       cout<<mc_info.Pz_ttbar<<endl;
   }
   */

}

void Fill_tt2l_res(tt2l_mc_info){
    //partage resolution MET (2 neutrinos - )
    //fraction evt correct jet assignation
}
#endif
