


double Reweight_Stop_to_TopChi0 (std::vector<IPHCTree::NTGenParticle> genParticles, double referenceTopPolarization, double requestedTopPolarization) 
{

    double weight = 1.;
    int nFoundStops = 0;

    unsigned int ngen = genParticles.size();

    for (unsigned int ig=0; ig<ngen; ++ig) 
    {
        const IPHCTree::NTGenParticle& gen = genParticles[ig];
        if (gen.motherIndex_<0) continue;
        if (abs(gen.id)>20) continue; // expect quarks or leptons from W decay

        // Navigate upwards in the stop->top->W->fermion decay chain
        const IPHCTree::NTGenParticle& genW = genParticles[gen.motherIndex_];
        if (genW.motherIndex_<0) continue;
        if (abs(genW.id)!=24) continue;
        const IPHCTree::NTGenParticle& genTop = genParticles[genW.motherIndex_];
        if (abs(genTop.id)!=6) continue;

        // We only care about the down-type fermion
        if (genTop.id*gen.id>0) continue;

        // We also need a stop
        if (genTop.motherIndex_<0) continue;
        const IPHCTree::NTGenParticle& genStop = genParticles[genTop.motherIndex_];
        if (abs(genStop.id)!=1000006) continue;

        // Move top and fermion to the stop center-of-mass frame
        TLorentzVector stop4 = genStop.p4;
        TVector3 betaV(-stop4.Px()/stop4.Energy(),-stop4.Py()/stop4.Energy(),-stop4.Pz()/stop4.Energy());

        TLorentzVector top4 = genTop.p4;
        top4.Boost(betaV);

        TLorentzVector ferm4;
        ferm4.SetPtEtaPhiE(gen.p4.Pt(), gen.p4.Eta(), gen.p4.Phi(), gen.p4.E());
        ferm4.Boost(betaV);

        // Do not reweight if by any reason top/fermion directions are undefined
        // This should be pathological if things are fine
        if (top4.P()<=0 || ferm4.P()<=0) {
            printf("Warning: particles at rest, no weight applied: ptop: %.3e, pf: %.3e\n", top4.P(), ferm4.P());
            continue; 
        }

        double costh = (top4.Px()*ferm4.Px()+top4.Py()*ferm4.Py()+top4.Pz()*ferm4.Pz())/top4.P()/ferm4.P();

        double weight_L = (top4.Energy()+top4.P())*(1-costh);
        double weight_R = (top4.Energy()-top4.P())*(1+costh);
        weight *= ((1+requestedTopPolarization)*weight_R+(1-requestedTopPolarization)*weight_L)/((1+referenceTopPolarization)*weight_R+(1-referenceTopPolarization)*weight_L);

        nFoundStops++;
    }

    if( nFoundStops!=2 ) cout << __FILE__ << " " << __LINE__ << " WARNING: found " << nFoundStops << " stops, should be 2." << endl;

    return weight;

};

double Norm(double M1, double M2, double MV, double CL, double CR) 
{
    double lambda = pow(M1,4) + pow(M2,4) + pow(MV,4) - 2*pow(M1*M2,2) - 2*pow(M1*MV,2) - 2*pow(M2*MV,2);
    double norm = (CL*CL+CR*CR)*(lambda + 3*MV*MV*(M1*M1+M2*M2-MV*MV)) - 12*CL*CR*M1*M2*MV*MV;
    norm /= 3.;
    return norm;
}

void Boost_To_Stop_Rest_Frame(TLorentzVector& stop4, TLorentzVector& chargino4, TLorentzVector& b4, TLorentzVector& neutralino4, TLorentzVector& W4, TLorentzVector& up4, TLorentzVector& down4, TLorentzVector& s4)
{
    TVector3 betaV(-stop4.Px()/stop4.Energy(),-stop4.Py()/stop4.Energy(),-stop4.Pz()/stop4.Energy());
    stop4.Boost(betaV);
    chargino4.Boost(betaV);
    b4.Boost(betaV);
    neutralino4.Boost(betaV);
    W4.Boost(betaV);
    up4.Boost(betaV);
    down4.Boost(betaV);
    s4.SetE(chargino4.P()/chargino4.M());
    s4.SetVect(chargino4.Vect().Unit()*chargino4.Gamma());
}

double Reweight_T2bW (double thetaChi_eff, double thetaW_eff, std::vector<IPHCTree::NTGenParticle> genParticles) {
    double weight = 1.;

    unsigned int ngen = genParticles.size();

    for (unsigned int i_stop=0; i_stop<ngen; ++i_stop) {
        // Look for stops
        const IPHCTree::NTGenParticle& gen = genParticles[i_stop];
        if (abs(gen.id)!=1000006) continue;

        // Look for stop decay products
        int i_b = -1;
        int i_chargino = -1;
        for (unsigned int ig=i_stop+1; ig<ngen; ++ig) 
        {
            const IPHCTree::NTGenParticle& gen = genParticles[ig];
            if (abs(gen.motherIndex_)!=i_stop) continue;
            if (abs(gen.id)==5) i_b = ig;
            else if (abs(gen.id)==1000024) i_chargino = ig;
            if (i_b>=0 && i_chargino>=0) break;
        }
        if (i_b<0 || i_chargino<0) continue;

        int i_neutralino = -1;
        int i_W = -1;
        for (unsigned int ig=i_chargino+1; ig<ngen; ++ig) {
            const IPHCTree::NTGenParticle& gen = genParticles[ig];
            if (abs(gen.motherIndex_)!=i_chargino) continue;
            if (abs(gen.id)==24) i_W = ig;
            else if (abs(gen.id)==1000022) i_neutralino = ig;
            if (i_W>=0 && i_neutralino>=0) break;
        }
        if (i_W<0 || i_neutralino<0) continue;

        int i_up = -1;
        int i_down = -1;
        for (unsigned int ig=i_W+1; ig<ngen; ++ig) {
            const IPHCTree::NTGenParticle& gen = genParticles[ig];
            if (abs(gen.motherIndex_)!=i_W) continue;
            if (abs(gen.id)%2==0) i_up = ig;
            else if (abs(gen.id)%2==1) i_down = ig;
            if (i_up>=0 && i_down>=0) break;
        }
        if (i_up<0 || i_down<0) continue;

        const IPHCTree::NTGenParticle& gen_stop = genParticles[i_stop];
        const IPHCTree::NTGenParticle& gen_b = genParticles[i_b];
        const IPHCTree::NTGenParticle& gen_chargino = genParticles[i_chargino];
        const IPHCTree::NTGenParticle& gen_W = genParticles[i_W];
        const IPHCTree::NTGenParticle& gen_neutralino = genParticles[i_neutralino];
        const IPHCTree::NTGenParticle& gen_up = genParticles[i_up];
        const IPHCTree::NTGenParticle& gen_down = genParticles[i_down];

        // Fill Lorentz four-vectors
        TLorentzVector stop4       = gen_stop.p4;
        TLorentzVector chargino4   = gen_chargino.p4;
        TLorentzVector b4          = gen_b.p4;
        TLorentzVector neutralino4 = gen_neutralino.p4;
        TLorentzVector W4          = gen_W.p4;
        TLorentzVector up4         = gen_up.p4;
        TLorentzVector down4       = gen_down.p4;

        // Reference spin four-vector along the chargino direction (filled in after boost)
        TLorentzVector s4;

        // Move everything to the stop center-of-mass frame
        Boost_To_Stop_Rest_Frame(stop4, chargino4, b4, neutralino4, W4, up4, down4, s4);

        double c_L = sin(thetaW_eff);
        double c_R = cos(thetaW_eff);
        double norm_target = Norm(chargino4.M(), neutralino4.M(), W4.M(), c_L, c_R);
        double target = 0;
        for (int hel = -1; hel<2; hel += 2) 
        {
            TLorentzVector t4 = s4*hel;
            TLorentzVector chargino4_plus = chargino4 + t4*chargino4.M();
            TLorentzVector chargino4_minus = chargino4 - t4*chargino4.M();
            target += (1. - chargino4.M()*cos(2*thetaChi_eff)*(b4*t4)/((b4*chargino4) - (b4.M()*chargino4.M())*sin(2*thetaChi_eff)))/2 *
                (8*c_L*c_L*(down4*chargino4_minus)*(up4*neutralino4)
                 + 8*c_R*c_R*(up4*chargino4_plus)*(down4*neutralino4)
                 - 4*c_L*c_R*neutralino4.M()*(pow(W4.M(),2)*chargino4.M()-2*(down4*t4)*(up4*chargino4)+2*(down4*chargino4)*(up4*t4)))/norm_target; 
        }

        weight *= target;

    }

    return weight;

};





