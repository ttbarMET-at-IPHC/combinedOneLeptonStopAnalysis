
// ########################################################################
// # Credits goes to Wouter Verkerke for original code                    #
// #                                                                      #
// # 'ORGANIZATION AND SIMULTANEOUS FITS' RooFit tutorial macro #501      #
// # Using simultaneous p.d.f.s to describe simultaneous fits to multiple #
// # datasets                                                             #
// #                                                                      #
// # 07/2008                                                              #
// ########################################################################

#include "MTTailCorrectionProducer.h"

std::pair<double,double>  GetSF(RooFitResult* res, string param)
{

    RooRealVar* par_init = (RooRealVar*) res->floatParsInit().find(param.c_str());
    RooRealVar* par_final = (RooRealVar*) res->floatParsFinal().find(param.c_str());
    double vinit = par_init->getVal();
    double vfinal = par_final->getVal();
    double SF = 1.;
    if(par_init!=0) SF = vfinal/vinit;

    double SFerror = 0.;
    if(vfinal!=0) SFerror = SF*par_final->getError()/vfinal;
    cout << "vfinal, parfinalGetErro, SF = " << vfinal << " ; " << par_final->getError() << " ; " << SF << endl;
    cout << "SF error = " << SFerror << endl;
    std::pair<double,double> pSF(SF,SFerror);
    return pSF;
}

//----------  Retrieve pdf (norm distrib) -----------------//

TH1F* GetHisto(TFile* fin, string region, string process, string varname, float& norm)
{
    string cname = "singleLepton/"+region+"/"+varname;
    TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
    string hname = "v:"+varname+"|p:"+process+"|r:"+region+"|c:singleLepton|t:1DEntries";
    TH1F* h = 0;
    TList* l = c->GetListOfPrimitives();
    TPad* pad = (TPad*) l->At(0);
    THStack* stack = (THStack*) pad->GetPrimitive("");
    h = (TH1F*) stack->GetHists()->FindObject(hname.c_str());
    norm = h->Integral();
    return (TH1F*) h->Clone();
}

RooHistPdf* GetRooHistPdf(TFile* fin, string region, string process, string varname, RooRealVar* var, float& norm, bool do_mcstat)
{
    TH1F* h  =  GetHisto(fin,region,process,varname, norm);
    if(do_mcstat)
    {
        //randomisation of the histo ...
        for(int i=1;i<=h->GetNbinsX();i++)
        {
            h->SetBinContent(i,randomnessGenerator->Gaus(h->GetBinContent(i),h->GetBinError(i)));
        }
    }
    string rdhname = "rdh_"+region+"_"+process;
    RooDataHist *rdh  = new RooDataHist(rdhname.c_str(),rdhname.c_str(),RooArgList(*var),Import(*h));
    string pdfname = "pdf_"+region+"_"+process;
    RooHistPdf *pdf  = new  RooHistPdf(pdfname.c_str(),pdfname.c_str(),RooArgSet(*var),*rdh);
    return pdf;
}
//---------------------------------------------------------//

//----------  Retrieve data histo  -----------------
TH1F* GetData(TFile* fin, string region, string varname)
{
    string cname = "singleLepton/"+region+"/"+varname;
    TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
    TList* l = c->GetListOfPrimitives();
    TPad* pad = (TPad*) l->At(0);
    string hname = "v:"+varname+"|r:"+region+"|c:singleLepton|t:1DSumData";
    TH1F* h = (TH1F*) pad->GetPrimitive(hname.c_str());
    return (TH1F*) h->Clone();
}

RooDataHist* GetRooData(TFile* fin, string region, string varname, RooRealVar* var)
{
    TH1F* h = GetData(fin, region, varname);
    string dname = "data_"+region;
    RooDataHist *datah = new RooDataHist(dname.c_str(),dname.c_str(), RooArgList(*var), Import(*h));
    return datah;
}

struct FitSetup
{
    //--name of the root file --//
    string filename;
    //--- variable used to the fit --//
    string varname;
    float  varMin;
    float  varMax;
    //--- region used to the fit ---//
    string region;
    //--- Xsection uncertainties --//
    float  xs_sysfactor;
    bool   do_xs_tt2l_sys;
    bool   do_xs_rare_sys;
    //--- MC stat uncertainties --//
    bool do_mcstat;
    //-- algorithm used to the fit --//
    string type;
    string algo;
    //-- Init value uncert --//
    bool do_init_uncert;
    float init_1ltop;
    float init_Wjets;

    void Reset()
    {
        filename=string(INPUT_FOLDER)+"/1DDataMCComparison.root";
        varname=OBSERVABLE_FOR_FIT;
        varMin = 0;
        varMax = 1000;
        region = "0btag_MTtail";

        //--- Xsection uncertainties --// (or at least, the one who store them later since these variables don't say anything at all anyway...)
        xs_sysfactor=1.;
        do_xs_tt2l_sys = false;
        do_xs_rare_sys = false;

        //--- MC stat uncertainties --//
        do_mcstat = false;

        //-- algorithm used to the fit --//
        type = "Minuit2";
        algo = "MIGRAD";

        //-- Init value uncert --//
        do_init_uncert = false;
        init_1ltop=1.;
        init_Wjets=1.;
    }
};

struct FitResult
{
    string conditions;
    pair<float,float> SF_1ltop;
    pair<float,float> SF_Wjets;
    float norm_1ltop;
    float correlation;
    float edm;

    void Reset()
    {
        norm_1ltop = 0;
        edm = 0;
        SF_1ltop = pair<float,float>(0,0);
        SF_Wjets = pair<float,float>(0,0);
        correlation = 0;
    }
    void Print()
    {
        cout<<"#################"<<endl;
        cout<<"# "<<conditions<<"\t SF_1ltop = "<<SF_1ltop.first<<"+/-"<<SF_1ltop.second<<"\t SF_Wjets = "<<SF_Wjets.first<<"+/-"<<SF_Wjets.second<<endl;
        cout<<"#\t edm = "<<edm<<" correlation = "<<correlation<<endl;
        cout<<"#\t init 1ltop: "<<norm_1ltop<<" \t fitted 1ltop: "<<norm_1ltop*SF_1ltop.first<<endl;
        cout<<"#################"<<endl;
    }
};


FitResult  doFit(const FitSetup& setup, string conditions, string fname=string(""))
{

    string varname = setup.varname;
    RooRealVar var(varname.c_str(),varname.c_str(),setup.varMin, setup.varMax);

    //string region="0btag_MTtail";
    string region= setup.region;

    //should it be an argument ?
    TFile* fin = 0;
    if(fname=="") fin = TFile::Open(setup.filename.c_str());
    //else fin = TFile::Open(fname.c_str());
    else fin = TFile::Open(fname.c_str());

    //-- normalisation in the MC --//
    float mc_norm_1ltop = 0;
    float mc_norm_tt2l = 0;
    float mc_norm_Wjets = 0;
    float mc_norm_rare = 0;

    // C r e a t e   m o d e l   f o r  CR1_peak_lowM3b
    // -------------------------------------------------------------
    // Construct pdfs for 1ltop, tt2l, Wjets and rare
    RooHistPdf *pdf_1ltop  = GetRooHistPdf(fin,region,"1ltop",varname,&var,mc_norm_1ltop, setup.do_mcstat);
    RooHistPdf *pdf_tt2l   = GetRooHistPdf(fin,region,"ttbar_2l",varname,&var,mc_norm_tt2l, setup.do_mcstat);
    RooHistPdf *pdf_Wjets  = GetRooHistPdf(fin,region,"W+jets",varname,&var,mc_norm_Wjets, setup.do_mcstat);
    RooHistPdf *pdf_rare   = GetRooHistPdf(fin,region,"rare",varname,&var,mc_norm_rare, setup.do_mcstat);

    // normalization factors (RooRealVar)
    float val_1ltop = mc_norm_1ltop;
    float val_Wjets = mc_norm_Wjets;
    if(setup.do_init_uncert)
    {
        val_1ltop = setup.init_1ltop*mc_norm_1ltop;
        val_Wjets = setup.init_Wjets*mc_norm_Wjets;
    }
    RooRealVar norm_1ltop("norm_1ltop","norm_1ltop",val_1ltop,0.25*mc_norm_1ltop,10.*mc_norm_1ltop);
    RooRealVar norm_Wjets("norm_Wjets","norm_Wjets",val_Wjets,0.25*mc_norm_Wjets,10.*mc_norm_Wjets);
    //RooRealVar norm_tt2l("norm_tt2l","norm_tt2l",mc_norm_tt2l,0.25*mc_norm_tt2l,2*mc_norm_tt2l);
    //RooRealVar norm_rare("norm_rare","norm_rare",mc_norm_rare,0.25*mc_norm_rare,2*mc_norm_rare);
    // possibility to study a systematic on it
    if(setup.do_xs_tt2l_sys) mc_norm_tt2l*=setup.xs_sysfactor;
    if(setup.do_xs_rare_sys) mc_norm_rare*=setup.xs_sysfactor;
    RooConstVar norm_tt2l("norm_tt2l","norm_tt2l",mc_norm_tt2l);
    RooConstVar norm_rare("norm_rare","norm_rare",mc_norm_rare);

    RooAddPdf model("model","model",
            RooArgList(*pdf_1ltop,*pdf_tt2l,*pdf_Wjets,*pdf_rare),
            RooArgList(norm_1ltop,norm_tt2l,norm_Wjets,norm_rare)) ;


    RooDataHist *data_CR1_peak_lowM3b = GetRooData(fin,region,varname,&var);

    fin->Close();


    //--  Constraints on single top and rare --//
    float RelUncert = 0.2;
    // Construct another Gaussian constraint p.d.f on "rare" bkg
    RooGaussian constr_rare("constr_rare","constr_rare",norm_rare,RooConst(mc_norm_rare),RooConst(RelUncert*mc_norm_rare)) ;
    // Construct another Gaussian constraint p.d.f on "tt2l" bkg
    RooGaussian constr_tt2l("constr_tt2l","constr_tt2l",norm_tt2l,RooConst(mc_norm_tt2l),RooConst(RelUncert*mc_norm_tt2l)) ;

    // P e r f o r m   t em p l a t e   f i t
    // ---------------------------------------------------

    //Minimizer(type,algo) -- Choose minimization package and algorithm to use. Default is MINUIT/MIGRAD through the RooMinimizer
    //                       interface, but rare can be specified (through RooMinimizer interface). Select OldMinuit to use
    //                       MINUIT through the old RooMinuit interface
    //
    //     Type         Algorithm
    //     ------       ---------
    //     OldMinuit    migrad, simplex, minimize (=migrad+simplex), migradimproved (=migrad+improve)
    //     Minuit       migrad, simplex, minimize (=migrad+simplex), migradimproved (=migrad+improve)
    //     Minuit2      migrad, simplex, minimize, scan
    //     GSLMultiMin  conjugatefr, conjugatepr, bfgs, bfgs2, steepestdescent
    //     GSLSimAn     -


    // ---  Perform simultaneous fit of model to data and model_ctl to data_ctl --//
    //RooFitResult* res = model.fitTo(*data_CR1_peak_lowM3b,Save());
    RooFitResult* res = model.fitTo(*data_CR1_peak_lowM3b,ExternalConstraints(constr_rare),ExternalConstraints(constr_tt2l),PrintLevel(-1),Save(),
            Minimizer(setup.type.c_str(),setup.algo.c_str()));

    //--- Writing the results ---///
    FitResult fitRes;
    fitRes.Reset();
    fitRes.norm_1ltop = mc_norm_1ltop;
    fitRes.SF_1ltop = GetSF(res,"norm_1ltop");
    fitRes.SF_Wjets = GetSF(res,"norm_Wjets");
    fitRes.edm = res->edm();
    fitRes.correlation = res->correlationMatrix()[0][1];
    fitRes.conditions = conditions;

    return fitRes;

}

int main()
{
    randomnessGenerator = new TRandom();

    system((string("mkdir -p ")+OUTPUT_FOLDER).c_str());

    // ###########################
    // # Prepare final SFR table #
    // ###########################

    vector<string> columns = { "SFR_1ltop", "SFR_Wjets" };

    vector<string> listAllSignalRegion = listBDTSignalRegions;
                   listAllSignalRegion.insert(listAllSignalRegion.end(), listCutAndCounts.begin(), listCutAndCounts.end());

    vector<string> listRawRegion = listBDTSignalRegions;
                   listRawRegion.push_back("BDT_average");
                   listRawRegion.insert(listRawRegion.end(), listIndividualCuts.begin(), listIndividualCuts.end());

    Table tableSFRToBeUsed(columns,listAllSignalRegion);
    Table tableRawSFR(columns,listRawRegion);

    // Create observables
    RooRealVar var(OBSERVABLE_FOR_FIT,OBSERVABLE_FOR_FIT,0,600) ;
    string varname(OBSERVABLE_FOR_FIT);

    FitSetup setup;
    FitResult res;
    string conditions;

    // ########################
    // #  ____________ _____  #
    // #  | ___ \  _  \_   _| #
    // #  | |_/ / | | | | |   #
    // #  | ___ \ | | | | |   #
    // #  | |_/ / |/ /  | |   #
    // #  \____/|___/   \_/   #
    // #                      #
    // ########################

    // Create histos

    TH1F h_SF_MTpeak_BDT_1ltop ("h_SF_MTpeak_BDT_1ltop", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SF_MTtail_BDT_1ltop ("h_SF_MTtail_BDT_1ltop", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SFR_BDT_1ltop       ("h_SFR_BDT_1ltop",       "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SF_MTpeak_BDT_Wjets ("h_SF_MTpeak_BDT_Wjets", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SF_MTtail_BDT_Wjets ("h_SF_MTtail_BDT_Wjets", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SFR_BDT_Wjets       ("h_SFR_BDT_Wjets",       "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());

    float mean_SF1ltop_value = 0;
    float mean_SF1ltop_error = 0;
    float rms_SF1ltop_value = 0;

    float mean_SFWjets_value = 0;
    float mean_SFWjets_error = 0;
    float rms_SFWjets_value = 0;

    for(unsigned int i=0;i<listBDTSignalRegions_MTtail.size();i++)
    {
        cout<<"%%%%%%%%%%%%%%%%%% "<<listBDTSignalRegions_MTtail[i]<<endl;

        string label = listBDTSignalRegions[i];
        Figure SFR_1ltop;
        Figure SFR_Wjets;

        //MT tail
        setup.Reset(); conditions="sigRegions_tail";
        setup.region=listBDTSignalRegions_MTtail[i];
        setup.varname=varname;
        setup.varMin=0;
        setup.varMax=600;

        res = doFit(setup,conditions);

        Figure SF_1ltop_tail = Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        Figure SF_Wjets_tail = Figure(res.SF_Wjets.first,res.SF_Wjets.second);

        h_SF_MTtail_BDT_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTtail_BDT_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTtail_BDT_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTtail_BDT_Wjets.SetBinContent(i+1,res.SF_Wjets.first);
        h_SF_MTtail_BDT_Wjets.SetBinError(i+1,res.SF_Wjets.second);
        h_SF_MTtail_BDT_Wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //MT peak
        setup.Reset(); conditions="sigRegions_peak"; setup.region=listBDTSignalRegions_MTpeak[i];
        setup.varname=varname;
        setup.varMin=0;
        setup.varMax=600;

        res = doFit(setup,conditions);

        Figure SF_1ltop_peak = Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        Figure SF_Wjets_peak = Figure(res.SF_Wjets.first,res.SF_Wjets.second);

        h_SF_MTpeak_BDT_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTpeak_BDT_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTpeak_BDT_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTpeak_BDT_Wjets.SetBinContent(i+1,res.SF_Wjets.first);
        h_SF_MTpeak_BDT_Wjets.SetBinError(i+1,res.SF_Wjets.second);
        h_SF_MTpeak_BDT_Wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //Now compute the ration : SF_tail/SF_peak
        SFR_1ltop = SF_1ltop_tail / SF_1ltop_peak;
        SFR_Wjets = SF_Wjets_tail / SF_Wjets_peak;

        //-- do some additionnal test as function of the b-tag multiplicity

        //MT peak (no btag req)

        setup.Reset(); conditions="sigRegions_peak_NoBtag";
        setup.region=listBDTSignalRegions_MTpeak_NoBtag[i];
        setup.varMin=0;
        setup.varMax=600;

        res = doFit(setup,conditions);

        //MT peak (one btag req)

        setup.Reset(); conditions="sigRegions_peak_OneBtag";
        setup.region=listBDTSignalRegions_MTpeak_OneBtag[i];
        setup.varname=varname;
        setup.varMin=0;
        setup.varMax=600;

        res = doFit(setup,conditions);

        //Computation of mean/rms/ ..
        //It is based on  the ratio SF_tail/SF_peak

        //-- 1ltop
        mean_SF1ltop_value += SFR_1ltop.value();
        mean_SF1ltop_error += SFR_1ltop.error();
        rms_SF1ltop_value  += (SFR_1ltop.value()*SFR_1ltop.value());

        //-- W+jets
        mean_SFWjets_value += SFR_Wjets.value();
        mean_SFWjets_error += SFR_Wjets.error();
        rms_SFWjets_value  += (SFR_Wjets.value()*SFR_Wjets.value());

        //SFR
        h_SFR_BDT_1ltop.SetBinContent(i+1,SFR_1ltop.value());
        h_SFR_BDT_1ltop.SetBinError(i+1,SFR_1ltop.error());
        h_SFR_BDT_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SFR_BDT_Wjets.SetBinContent(i+1,SFR_Wjets.value());
        h_SFR_BDT_Wjets.SetBinError(i+1,SFR_Wjets.error());
        h_SFR_BDT_Wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        cout << listBDTSignalRegions[i] << " : SF_1ltop_peak: " << SF_1ltop_peak.Print() <<" SF_Wjets_peak: "<<SF_Wjets_tail.Print()<<endl;
        cout << listBDTSignalRegions[i] << " : SF_1ltop_tail: " << SF_1ltop_tail.Print() <<" SF_Wjets_tail: "<<SF_Wjets_peak.Print()<<endl;
        cout << listBDTSignalRegions[i] << " : SFR_1ltop: "     << SFR_1ltop.Print()     <<" SFR_Wjets: "<<SFR_Wjets.Print()<<endl;

        tableRawSFR.Set("SFR_1ltop",listBDTSignalRegions[i],SFR_1ltop);
        tableRawSFR.Set("SFR_Wjets",listBDTSignalRegions[i],SFR_Wjets);
    }

    //Save plots in roofile
    TFile fCR1_BDT((string(OUTPUT_FOLDER)+"/results_BDT.root").c_str(),"RECREATE");
    h_SF_MTpeak_BDT_1ltop.Write();
    h_SF_MTpeak_BDT_Wjets.Write();
    h_SF_MTtail_BDT_1ltop.Write();
    h_SF_MTtail_BDT_Wjets.Write();
    h_SFR_BDT_1ltop.Write();
    h_SFR_BDT_Wjets.Write();


    //---------------------------------------//

    mean_SF1ltop_value /= listBDTSignalRegions_MTtail.size();
    mean_SF1ltop_error /= listBDTSignalRegions_MTtail.size();

    rms_SF1ltop_value  /= listBDTSignalRegions_MTtail.size();
    rms_SF1ltop_value  -= (mean_SF1ltop_value*mean_SF1ltop_value);

    Figure BDT_SF1ltop(mean_SF1ltop_value,sqrt(rms_SF1ltop_value
                                              +mean_SF1ltop_error*mean_SF1ltop_error
                                              +pow(TEMPLATE_FIT_METHOD_UNCERTAINTY*mean_SF1ltop_value,2)));
    //---------------------------------------//

    mean_SFWjets_value /= listBDTSignalRegions_MTtail.size();
    mean_SFWjets_error /= listBDTSignalRegions_MTtail.size();

    rms_SFWjets_value  /= listBDTSignalRegions_MTtail.size();
    rms_SFWjets_value  -= (mean_SFWjets_value*mean_SFWjets_value);

    Figure BDT_SFWjets(mean_SFWjets_value,sqrt(rms_SFWjets_value
                                              +mean_SFWjets_error*mean_SFWjets_error
                                              +pow(TEMPLATE_FIT_METHOD_UNCERTAINTY*mean_SFWjets_value,2)));
    //---------------------------------------//

    tableRawSFR.Set("SFR_1ltop","BDT_average",BDT_SF1ltop);
    tableRawSFR.Set("SFR_Wjets","BDT_average",BDT_SFWjets);
    for(unsigned int i=0;i<listBDTSignalRegions.size();i++)
    {
        tableSFRToBeUsed.Set("SFR_1ltop",listBDTSignalRegions[i],BDT_SF1ltop);
        tableSFRToBeUsed.Set("SFR_Wjets",listBDTSignalRegions[i],BDT_SFWjets);
    }

    // ########################
    // #  _____        _____  #
    // # /  __ \ ___  /  __ \ #
    // # | /  \/( _ ) | /  \/ #
    // # | |    / _ \/\ |     #
    // # | \__/\ (_>  < \__/\ #
    // #  \____/\___/\/\____/ #
    // #                      #
    // ########################

    std::map<string,Figure> SFR_CC_1ltop_map;
    std::map<string,Figure> SFR_CC_Wjets_map;

    //Create histos
    TH1F h_SF_MTpeak_CC_1ltop ("h_SF_MTpeak_CC_1ltop",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SF_MTtail_CC_1ltop ("h_SF_MTtail_CC_1ltop",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SFR_CC_1ltop       ("h_SFR_CC_1ltop",        "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SF_MTpeak_CC_Wjets ("h_SF_MTpeak_CC_Wjets",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SF_MTtail_CC_Wjets ("h_SF_MTtail_CC_Wjets",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SFR_CC_Wjets       ("h_SFR_CC_Wjets",        "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());

    for(unsigned int i=0;i<listIndividualCuts.size();i++)
    {
        cout<<"%%%%%%%%%%%%%%%%%% "<<listIndividualCuts_MTtail[i]<<endl;

        string label = listIndividualCuts[i];
        Figure SFR_1ltop;
        Figure SFR_Wjets;

        //MT tail
        setup.Reset();
        conditions="sigRegions_CC_tail";
        setup.region=listIndividualCuts_MTtail[i];
        setup.varname=varname;
        setup.varMin=0;
        setup.varMax=600;

        res = doFit(setup,conditions);
        SFR_1ltop=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_Wjets=Figure(res.SF_Wjets.first,res.SF_Wjets.second);
        h_SF_MTtail_CC_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTtail_CC_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTtail_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTtail_CC_Wjets.SetBinContent(i+1,res.SF_Wjets.first);
        h_SF_MTtail_CC_Wjets.SetBinError(i+1,res.SF_Wjets.second);
        h_SF_MTtail_CC_Wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //MT peak
        setup.Reset();
        conditions="sigRegions_CC_peak";
        setup.region=listIndividualCuts_MTpeak[i];
        setup.varname=varname;
        setup.varMin=0;
        setup.varMax=600;

        res = doFit(setup,conditions);
        h_SF_MTpeak_CC_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTpeak_CC_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTpeak_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTpeak_CC_Wjets.SetBinContent(i+1,res.SF_Wjets.first);
        h_SF_MTpeak_CC_Wjets.SetBinError(i+1,res.SF_Wjets.second);
        h_SF_MTpeak_CC_Wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        // Now compute the ration : SF_tail/SF_peak
        SFR_1ltop /= Figure(res.SF_1ltop.first, res.SF_1ltop.second);
        SFR_Wjets /= Figure(res.SF_Wjets.first, res.SF_Wjets.second);

        // It is based on the ratio SF_tail/SF_peak
        SFR_CC_1ltop_map[label] = SFR_1ltop;
        SFR_CC_Wjets_map[label] = SFR_Wjets;

        // SFR
        h_SFR_CC_1ltop.SetBinContent(i+1,SFR_1ltop.value());
        h_SFR_CC_1ltop.SetBinError(i+1,SFR_1ltop.error());
        h_SFR_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SFR_CC_Wjets.SetBinContent(i+1,SFR_Wjets.value());
        h_SFR_CC_Wjets.SetBinError(i+1,SFR_Wjets.error());
        h_SFR_CC_Wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        cout<<"individual cut: " << listIndividualCuts[i] << " ; SFR_1ltop: "<<SFR_1ltop.Print()<<" SFR_Wjets: "<<SFR_Wjets.Print()<<endl;

        tableRawSFR.Set("SFR_1ltop",listIndividualCuts[i],SFR_1ltop);
        tableRawSFR.Set("SFR_Wjets",listIndividualCuts[i],SFR_Wjets);
    }

    //Save plots in roofile
    TFile fCR1_CC((string(OUTPUT_FOLDER)+"/results_CC.root").c_str(),"RECREATE");
    h_SF_MTpeak_CC_1ltop.Write();
    h_SF_MTpeak_CC_Wjets.Write();
    h_SF_MTtail_CC_1ltop.Write();
    h_SF_MTtail_CC_Wjets.Write();
    h_SFR_CC_1ltop.Write();
    h_SFR_CC_Wjets.Write();

    //---------------------------------------------
    // Results for C&C
    //---------------------------------------------
    initCutAndCountCuts();
    vector<string> cuts;

    for(unsigned int r=0;r<listCutAndCounts.size();r++)
    {
        cuts = listCutAndCounts_cuts[listCutAndCounts[r]];
        Figure SFR_CC_1ltop;
        Figure SFR_CC_Wjets;
        for(unsigned i=0;i<cuts.size();i++)
        {
            cout<<cuts[i]<<" "<<SFR_CC_1ltop_map[cuts[i]].Print()<<endl;
            if(i == 0) SFR_CC_1ltop = SFR_CC_1ltop_map[cuts[i]];
            else SFR_CC_1ltop = Figure(SFR_CC_1ltop.value(),
                                       sqrt(pow(SFR_CC_1ltop.error(),2)
                                           +pow(SFR_CC_1ltop.value()
                                                -SFR_CC_1ltop_map[cuts[i].c_str()].value(),2)
                                           ));

            if(i == 0) SFR_CC_Wjets = SFR_CC_Wjets_map[cuts[i]];
            else SFR_CC_Wjets = Figure(SFR_CC_Wjets.value(),
                                       sqrt(pow(SFR_CC_Wjets.error(),2)
                                           +pow(SFR_CC_Wjets.value()
                                               -SFR_CC_Wjets_map[cuts[i].c_str()].value(),2)
                                           ));
        }

        // Add 20% of uncertainty for fit itself (JES, MC stat. ...)
        SFR_CC_1ltop *= Figure(1.0,TEMPLATE_FIT_METHOD_UNCERTAINTY);
        SFR_CC_Wjets *= Figure(1.0,TEMPLATE_FIT_METHOD_UNCERTAINTY);

        tableSFRToBeUsed.Set("SFR_1ltop",listCutAndCounts[r],SFR_CC_1ltop);
        tableSFRToBeUsed.Set("SFR_Wjets",listCutAndCounts[r],SFR_CC_Wjets);
    }

    tableRawSFR     .Print(string(OUTPUT_FOLDER)+"/rawSFR.tab"   ,4);
    tableSFRToBeUsed.Print(string(OUTPUT_FOLDER)+"/SF_MTtail.tab",4);
}
