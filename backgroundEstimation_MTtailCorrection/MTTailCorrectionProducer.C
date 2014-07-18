
// ########################################################################
// #  Thanks to Wouter Verkerke, awesome stat teacher, for original code  #
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
    cout<<"SF("<<param<<"):\t"<<SF<<" +/- "<<SFerror<<" ("<<SFerror/SF*100<<"%)"<<endl;
    std::pair<double,double> pSF(SF,SFerror);
    return pSF;
}

//----------  Retrieve pdf (norm distrib) -----------------//

TH1F* GetHisto(TFile* fin, string region, string process, string varname, float& norm)
{
    bool normDistrib = false;
    string cname = "singleLepton/"+region+"/"+varname;
    //TCanvas* c = fin->Get("singleLepton/0btag/R");
    TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
    string hname = "v:"+varname+"|p:"+process+"|r:"+region+"|c:singleLepton|t:1DEntries";
    TH1F* h = 0;
    if(normDistrib)
    {
        h = (TH1F*) c->GetPrimitive(hname.c_str());
        norm = h->Integral();
    }
    else
    {
        TPad* pad = (TPad*) c->GetPrimitive("");
        THStack* stack = (THStack*) pad->GetPrimitive("");
        h = (TH1F*) stack->GetHists()->FindObject(hname.c_str());
        norm = h->Integral();
    }
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

//----------  Retrieve pdf combining several regions -----------------//

TH1F* GetMergedHisto(TFile* fin, vector<string> regions, string process, string varname, float& norm)
{
    vector<float> norms(regions.size());
    vector<TH1F*> histos(regions.size());
    norm = 0;
    for(unsigned int i=0;i<regions.size();i++)
    {
        histos[i] = GetHisto(fin, regions[i], process, varname, norms[i]);
        //sum the norms
        norm+=norms[i];
    }
    //Create the new merged histo
    string hname = varname+"_"+process+"_merged";
    int nbins = 0;
    float min = 0;
    float max = 0;
    if(regions.size()!=0)
    { 
        nbins=histos[0]->GetNbinsX()*regions.size();
        min = histos[0]->GetXaxis()->GetXmin();
        max = min+(histos[0]->GetXaxis()->GetXmax()-histos[0]->GetXaxis()->GetXmin())*regions.size();
    }
    TH1F* mergedHisto = new TH1F(hname.c_str(),hname.c_str(),nbins,min,max);
    int indice = 1;
    for(unsigned int i=0;i<histos.size();i++)
    {
        for(int ibin=1;ibin<=histos[i]->GetNbinsX();i++)
        {
            mergedHisto->SetBinContent(indice,histos[i]->GetBinContent(ibin));
            mergedHisto->SetBinError(indice,histos[i]->GetBinError(ibin));
            indice++;    
        }
    }
    return mergedHisto;
}

RooHistPdf* GetMergedRooHistPdf(TFile* fin, vector<string> region, string process, string varname, RooRealVar* var, float& norm)
{
    TH1F* h  =  GetMergedHisto(fin,region,process,varname, norm);
    string rdhname = "rdh_merged_"+process;
    RooDataHist *rdh  = new RooDataHist(rdhname.c_str(),rdhname.c_str(),RooArgList(*var),Import(*h));
    string pdfname = "pdf_merged_"+process;
    RooHistPdf *pdf  = new  RooHistPdf(pdfname.c_str(),pdfname.c_str(),RooArgSet(*var),*rdh);
    return pdf;
}

//----------  Retrieve data histo  -----------------
TH1F* GetData(TFile* fin, string region, string varname)
{
    // Protect againt using JES uncertainty
    /*
    size_t pos = varname.find("_JESup");
    if(pos!=string::npos)
    {
        varname = varname.substr(0,pos);
        cout<<"Modify variable name for data "<<varname<<endl;
    }
    pos = varname.find("_JESdown");
    if(pos!=string::npos)
    {
        varname = varname.substr(0,pos);
        cout<<"Modify variable name for data "<<varname<<endl;
    }
    */
    //-----------------------------------------//

    string cname = "singleLepton/"+region+"/"+varname;
    //TCanvas* c = fin->Get("singleLepton/0btag/R");
    cout<<"name = "<<cname<<endl;
    TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
    cout<<c<<endl;
    TPad* pad = (TPad*) c->GetPrimitive("");
    string hname = "v:"+varname+"|r:"+region+"|c:singleLepton|t:1DSumData";
    TH1F* h = (TH1F*) pad->GetPrimitive(hname.c_str());
    return (TH1F*) h->Clone();
}

RooDataHist* GetRooData(TFile* fin, string region, string varname, RooRealVar* var)
{
    TH1F* h = GetData(fin, region, varname);
    cout<<"data = "<<h->GetEntries()<<endl;
    string dname = "data_"+region;
    RooDataHist *datah = new RooDataHist(dname.c_str(),dname.c_str(), RooArgList(*var), Import(*h));
    return datah;
}

//---------- Perform Chi2 test to control variable shape in CR --/
void Chi2Test(TFile* fin, string region, string varname)
{
    //retrieve data histo
    TH1F* hData = GetData(fin, region, varname);
    cout<<hData->GetEntries()<<endl;
    //retrieve MC sum (THStack);
    string cname = "singleLepton/"+region+"/"+varname;
    TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
    TPad* pad = (TPad*) c->GetPrimitive("");
    THStack* stack = (THStack*) pad->GetPrimitive("");
    //TH1F* hMC = (TH1F*) stack->GetHistogram();
    TH1F* hMC = 0;
    TIter next(stack->GetHists());
    TObject* obj = 0;
    bool first = true;
    while ((obj = next()))
    {
        if(first)
        {
            hMC = (TH1F*) obj->Clone();
            first = false;
        }
        hMC->Add((TH1F*)obj);
    }
    cout<<hMC->GetEntries()<<endl;
    //--- perform tests --/
    cout<<"##################################################"<<endl;
    cout<<"#   Test - region:"<<region<<" var:"<<varname<<endl;
    cout<<"##################################################"<<endl;
    float pvalue = hData->Chi2Test(hMC,"UWp"); //"p CHI2/NDF
    cout<<"# Chi2 test\t"<<pvalue<<endl;
    pvalue = hData->KolmogorovTest(hMC);
    cout<<"# Kolmogorov test\t"<<pvalue<<endl;
    cout<<"##################################################"<<endl;
}

struct FitSetup{
    //--name of the root file --//
    string filename;
    //--- variable used to the fit --//
    string varname;
    float varMin;
    float varMax;
    //--- region used to the fit ---//
    string region;
    //--- Xsection uncertainties --//
    float xs_sysfactor;
    bool do_xs_tt2l_sys;
    bool do_xs_rare_sys;
    //--- MC stat uncertainties --//
    bool do_mcstat;
    //-- algorithm used to the fit --//
    string type;
    string algo;
    //-- Init value uncert --//
    bool do_init_uncert;
    float init_1ltop;
    float init_wjets;

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
        init_wjets=1.;
    }
};

struct FitResult{
    string conditions;
    pair<float,float> SF_1ltop;
    pair<float,float> SF_wjets;
    float norm_1ltop;
    float correlation;
    float edm;

    void Reset()
    {
        norm_1ltop = 0;
        edm = 0;
        SF_1ltop = pair<float,float>(0,0);
        SF_wjets = pair<float,float>(0,0);
        correlation = 0;
    }
    void Print()
    {
        cout<<"#################"<<endl;
        cout<<"# "<<conditions<<"\t SF_1ltop = "<<SF_1ltop.first<<"+/-"<<SF_1ltop.second<<"\t SF_wjets = "<<SF_wjets.first<<"+/-"<<SF_wjets.second<<endl;
        cout<<"#\t edm = "<<edm<<" correlation = "<<correlation<<endl;
        cout<<"#\t init 1ltop: "<<norm_1ltop<<" \t fitted 1ltop: "<<norm_1ltop*SF_1ltop.first<<endl;
        cout<<"#################"<<endl;
    }
};


TH1F* TailDescription(string region, string varname, string process, string file1, string file2, float vmin, float vmax)
{
    TFile* f1 = TFile::Open(file1.c_str());
    float norm1 = 0;
    TH1F* h1 = GetHisto(f1, region, process, varname, norm1);
    TFile* f2 = TFile::Open(file2.c_str());
    float norm2 = 0;
    TH1F* h2 = GetHisto(f2, region, process, varname, norm2);
    int imin = h1->FindBin(vmin);
    int imax = h1->FindBin(vmin);
    Double_t err1, err2 = 0;
    float v1, v2 = 0;
    float r1, r2 = 0;
    float r1_uncert, r2_uncert = 0;
    float ratio =1;
    float ratio_uncert =0;
    int ibin = 1;
    string hname = region+"_"+varname+"_"+process+"_ratio";
    TH1F* hratio = new TH1F(hname.c_str(),hname.c_str(),imax-imin,vmin,vmax);
    for(int i=imin;i<imax;i++)
    {
        v1 = h1->IntegralAndError(i,h1->GetNbinsX()+1,err1);
        r1=v1/h1->Integral();
        r1_uncert = err1/r1*v1;
        v2 = h2->IntegralAndError(i,h2->GetNbinsX()+1,err2);
        r2=v2/h2->Integral();
        r2_uncert = err2/r2*v2;
        ratio = r2/r1;
        ratio_uncert = sqrt(1./(r1*r1)*r1_uncert*r1_uncert+r2*r2*r2_uncert*r2_uncert);
        hratio->SetBinContent(ibin,ratio);
        hratio->SetBinError(ibin,ratio_uncert);
        ibin++;
    }
    return hratio;
}

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
    float mc_norm_wjets = 0;
    float mc_norm_rare = 0;

    // C r e a t e   m o d e l   f o r  CR1_peak_lowM3b
    // -------------------------------------------------------------
    // Construct pdf for 1ltop
    RooHistPdf *pdf_1ltop  = GetRooHistPdf(fin,region,"1ltop",varname,&var,mc_norm_1ltop, setup.do_mcstat);
    // Construct pdf for tt2l
    RooHistPdf *pdf_tt2l  = GetRooHistPdf(fin,region,"ttbar_2l",varname,&var,mc_norm_tt2l, setup.do_mcstat);
    // Construct pdf for Wjets
    RooHistPdf *pdf_Wjets  = GetRooHistPdf(fin,region,"W+jets",varname,&var,mc_norm_wjets, setup.do_mcstat);
    // Construct pdf for rare
    RooHistPdf *pdf_rare  = GetRooHistPdf(fin,region,"rare",varname,&var,mc_norm_rare, setup.do_mcstat);

    // normalization factors (RooRealVar)
    float val_1ltop = mc_norm_1ltop;
    float val_wjets = mc_norm_wjets;
    if(setup.do_init_uncert)
    {
        val_1ltop = setup.init_1ltop*mc_norm_1ltop;
        val_wjets = setup.init_wjets*mc_norm_wjets;
    }
    RooRealVar norm_1ltop("norm_1ltop","norm_1ltop",val_1ltop,0.25*mc_norm_1ltop,10.*mc_norm_1ltop);
    RooRealVar norm_wjets("norm_wjets","norm_wjets",val_wjets,0.25*mc_norm_wjets,10.*mc_norm_wjets);
    //RooRealVar norm_tt2l("norm_tt2l","norm_tt2l",mc_norm_tt2l,0.25*mc_norm_tt2l,2*mc_norm_tt2l);
    //RooRealVar norm_rare("norm_rare","norm_rare",mc_norm_rare,0.25*mc_norm_rare,2*mc_norm_rare);
    // possibility to study a systematic on it
    if(setup.do_xs_tt2l_sys) mc_norm_tt2l*=setup.xs_sysfactor;
    if(setup.do_xs_rare_sys) mc_norm_rare*=setup.xs_sysfactor;
    RooConstVar norm_tt2l("norm_tt2l","norm_tt2l",mc_norm_tt2l);
    RooConstVar norm_rare("norm_rare","norm_rare",mc_norm_rare);

    RooAddPdf model("model","model",
            RooArgList(*pdf_1ltop,*pdf_tt2l,*pdf_Wjets,*pdf_rare),
            RooArgList(norm_1ltop,norm_tt2l,norm_wjets,norm_rare)) ;
    //model.Print();


    RooDataHist *data_CR1_peak_lowM3b = GetRooData(fin,region,varname,&var);

    fin->Close();


    //--  Constraints on single top and rare --//
    float RelUncert = 0.2;
    // Construct another Gaussian constraint p.d.f on single top bkg
    // Construct another Gaussian constraint p.d.f on "rare" bkg
    RooGaussian constr_rare("constr_rare","constr_rare",norm_rare,RooConst(mc_norm_rare),RooConst(RelUncert*mc_norm_rare)) ; 
    // Construct another Gaussian constraint p.d.f on "tt2l" bkg
    RooGaussian constr_tt2l("constr_tt2l","constr_tt2l",norm_tt2l,RooConst(mc_norm_tt2l),RooConst(RelUncert*mc_norm_tt2l)) ; 

    // P e r f o r m   t em p l a t e   f i t
    // ---------------------------------------------------

    //Minimizer(type,algo)           -- Choose minimization package and algorithm to use. Default is MINUIT/MIGRAD through the RooMinimizer
    //                                 interface, but rare can be specified (through RooMinimizer interface). Select OldMinuit to use
    //                                 MINUIT through the old RooMinuit interface
    //
    //                                        Type         Algorithm
    //                                        ------       ---------
    //                                        OldMinuit    migrad, simplex, minimize (=migrad+simplex), migradimproved (=migrad+improve)
    //                                        Minuit       migrad, simplex, minimize (=migrad+simplex), migradimproved (=migrad+improve)
    //                                        Minuit2      migrad, simplex, minimize, scan
    //                                        GSLMultiMin  conjugatefr, conjugatepr, bfgs, bfgs2, steepestdescent
    //                                        GSLSimAn     -


    // ---  Perform simultaneous fit of model to data and model_ctl to data_ctl --//
    //RooFitResult* res = model.fitTo(*data_CR1_peak_lowM3b,Save());
    RooFitResult* res = model.fitTo(*data_CR1_peak_lowM3b,ExternalConstraints(constr_rare),ExternalConstraints(constr_tt2l),PrintLevel(-1),Save(),
            Minimizer(setup.type.c_str(),setup.algo.c_str()));

    //--- Writing the results ---///
    FitResult fitRes;
    fitRes.Reset();
    fitRes.norm_1ltop = mc_norm_1ltop;
    fitRes.SF_1ltop = GetSF(res,"norm_1ltop");
    fitRes.SF_wjets = GetSF(res,"norm_wjets");
    fitRes.edm = res->edm();
    fitRes.correlation = res->correlationMatrix()[0][1];
    fitRes.conditions = conditions;

    fitRes.Print();
    return fitRes;

    // Plot data and complete PDF overlaid
    /*
       RooPlot* xframe  = var.frame(Title("Component plotting of pdf=(sig1+sig2)+(bkg1+bkg2)")) ;
       data_CR1_peak_lowM3b->plotOn(xframe) ;
       model.plotOn(xframe) ;
       model.plotOn(xframe,Components("pdf_1ltop"),LineColor(kRed)) ;
       model.plotOn(xframe,Components("pdf_ttbar_2l"),LineColor(kCyan)) ;
       model.plotOn(xframe,Components("pdf_W+jets"),LineColor(kGreen)) ;
       model.plotOn(xframe,Components("pdf_rare"),LineColor(kPink)) ;
       TCanvas* c = new TCanvas();
       xframe->Draw();
       */

}

struct FitUncert{
    string name;
    float SF_1ltop_uncert;
    float SF_wjets_uncert;
};

struct SummaryResult{
    FitResult nominal;
    vector<FitUncert> uncert;
    void Print()
    {
        cout<<"#############################"<<endl;
        cout<<"#      Summary results      #"<<endl;
        nominal.Print();
        cout<<"#############################"<<endl;
        cout<<"# Systematic uncertainties #"<<endl;
        cout<<"# name\t SF_1ltop\t SF_wjets"<<endl;
        for(unsigned int i=0;i<uncert.size();i++)
        {
            cout<<"# "<<uncert[i].name<<"\t"<<uncert[i].SF_1ltop_uncert<<"\t"<<uncert[i].SF_wjets_uncert<<endl;
        }
        cout<<"#############################"<<endl;
    }
};


int main()
{
    randomnessGenerator = new TRandom();

    system((string("mkdir -p ")+OUTPUT_FOLDER).c_str());

    // Create observables
    RooRealVar var(OBSERVABLE_FOR_FIT,OBSERVABLE_FOR_FIT,0,600) ;
    string varname(OBSERVABLE_FOR_FIT);

    SummaryResult summary;

    FitSetup setup;
    FitResult res;
    string conditions;
    FitUncert uncert;

    float min=0;
    float max=0;

    float value_up_SF_1ltop = 0;
    float value_down_SF_1ltop = 0;
    float value_up_SF_wjets = 0;
    float value_down_SF_wjets = 0;


    summary.nominal = res;

    //--- tail --//
    setup.Reset(); conditions="tail", setup.region="0btag_MTtail";
    res = doFit(setup,conditions); 
    Figure SFR_1ltop=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
    Figure SFR_wjets=Figure(res.SF_wjets.first,res.SF_wjets.second);

    //--- nominal ---//
    setup.Reset(); conditions="peak", setup.region="0btag_MTpeak";
    res = doFit(setup,conditions); 
    SFR_1ltop/=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
    SFR_wjets/=Figure(res.SF_wjets.first,res.SF_wjets.second);

    cout<<"0btag-SFR SFR_1ltop = "<<SFR_1ltop.Print()<<" SFR_wjets = "<<SFR_wjets.Print()<<endl;

    //--- preselection --//
    //setup.Reset(); conditions="preselection", setup.region="presel_MTpeak";
    setup.Reset(); conditions="preselection", setup.region="MTpeak";
    res = doFit(setup,conditions); 

    //--- xs(tt2l) ---//
    setup.Reset(); conditions="xs_tt2l";
    setup.do_xs_tt2l_sys = true;
    setup.xs_sysfactor = 0.8;    res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first ; value_down_SF_wjets = res.SF_wjets.first; 
    setup.xs_sysfactor = 1.2; res = doFit(setup,conditions); value_up_SF_1ltop = res.SF_1ltop.first ; value_up_SF_wjets = res.SF_wjets.first;
    uncert.name = conditions;
    uncert.SF_1ltop_uncert = fabs((value_up_SF_1ltop-value_down_SF_1ltop)/2.);
    uncert.SF_wjets_uncert = fabs((value_up_SF_wjets-value_down_SF_wjets)/2.);
    summary.uncert.push_back(uncert);

    //--- xs(rare) ---//
    setup.Reset(); conditions="xs_rare";
    setup.do_xs_rare_sys = true;
    setup.xs_sysfactor = 0.8;    res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first ; value_down_SF_wjets = res.SF_wjets.first; 
    setup.xs_sysfactor = 1.2; res = doFit(setup,conditions); value_up_SF_1ltop = res.SF_1ltop.first ; value_up_SF_wjets = res.SF_wjets.first;
    uncert.name = conditions;
    uncert.SF_1ltop_uncert = fabs((value_up_SF_1ltop-value_down_SF_1ltop)/2.);
    uncert.SF_wjets_uncert = fabs((value_up_SF_wjets-value_down_SF_wjets)/2.);
    cout<<uncert.SF_1ltop_uncert<<" "<<uncert.SF_wjets_uncert<<" "<<value_up_SF_1ltop<<" "<<value_down_SF_1ltop<<endl;
    summary.uncert.push_back(uncert);

    //--- choice of the minimizer --//
    //                                        Minuit2      migrad, simplex, minimize, scan
    //                                        GSLMultiMin  conjugatefr, conjugatepr, bfgs, bfgs2, steepestdescent
    setup.Reset(); conditions="algo";
    //setup.type="Minuit2"; setup.algo="simplex",    res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first ; value_down_SF_wjets = res.SF_wjets.first; 
    //setup.type="Minuit2"; setup.algo="scan",    res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first ; value_down_SF_wjets = res.SF_wjets.first; 
    //setup.type="GSLMultiMin"; setup.algo="bfgs2",    res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first ; value_down_SF_wjets = res.SF_wjets.first; 
    //setup.type="GSLMultiMin"; setup.algo="conjugatepr",    res = doFit(setup,conditions); SF_1ltop_algo.push_back(res.SF_1ltop.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
    vector<float> SF_1ltop_algo;
    vector<float> SF_wjets_algo;
    setup.type="Minuit2"; setup.algo="minimize",    res = doFit(setup,conditions); SF_1ltop_algo.push_back(res.SF_1ltop.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
    setup.type="GSLMultiMin"; setup.algo="conjugatefr",    res = doFit(setup,conditions); SF_1ltop_algo.push_back(res.SF_1ltop.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
    setup.type="GSLMultiMin"; setup.algo="bfgs",    res = doFit(setup,conditions); SF_1ltop_algo.push_back(res.SF_1ltop.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
    setup.type="GSLMultiMin"; setup.algo="steepestdescent",    res = doFit(setup,conditions); SF_1ltop_algo.push_back(res.SF_1ltop.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
    uncert.name = conditions;
    min = *std::min_element(SF_1ltop_algo.data(),SF_1ltop_algo.data()+SF_1ltop_algo.size());
    max = *std::max_element(SF_1ltop_algo.data(),SF_1ltop_algo.data()+SF_1ltop_algo.size());
    uncert.SF_1ltop_uncert = fabs((max-min)/2.);
    min = *std::min_element(SF_wjets_algo.data(),SF_wjets_algo.data()+SF_wjets_algo.size());
    max = *std::max_element(SF_wjets_algo.data(),SF_wjets_algo.data()+SF_wjets_algo.size());
    uncert.SF_wjets_uncert = fabs((max-min)/2.);
    summary.uncert.push_back(uncert);

    //--- MC statistic --//
    setup.Reset(); conditions="MCstat"; uncert.name = conditions;
    setup.do_mcstat = true;
    SF_1ltop_algo.clear();
    SF_wjets_algo.clear();
    for(int i=0;i<200;i++)
    {
        res = doFit(setup,conditions); SF_1ltop_algo.push_back(res.SF_1ltop.first) ; SF_wjets_algo.push_back(res.SF_wjets.first);
    }
    uncert.SF_1ltop_uncert = TMath::RMS(SF_1ltop_algo.size(),SF_1ltop_algo.data());
    uncert.SF_wjets_uncert = TMath::RMS(SF_wjets_algo.size(),SF_wjets_algo.data());
    summary.uncert.push_back(uncert);

    //--- Conditions initial of the fit --//
    setup.Reset(); conditions="InitCond"; uncert.name = conditions;
    setup.do_init_uncert=true;
    setup.init_1ltop = 0.5; setup.init_wjets = 1.5;
    res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first*setup.init_1ltop ; value_down_SF_wjets = res.SF_wjets.first*setup.init_wjets;
    setup.init_1ltop = 1.5; setup.init_wjets = 0.5;
    res = doFit(setup,conditions); value_up_SF_1ltop = res.SF_1ltop.first*setup.init_1ltop ; value_up_SF_wjets = res.SF_wjets.first*setup.init_wjets;
    uncert.SF_1ltop_uncert = fabs((value_down_SF_1ltop-value_up_SF_1ltop)/2.); 
    uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
    summary.uncert.push_back(uncert);


    //--- JES uncertainty --//
    /*
        setup.Reset(); conditions="JES"; uncert.name = conditions;
        setup.varname=OBSERVABLE_FOR_FIT+"_JESdown"; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); value_down_SF_1ltop = res.SF_1ltop.first ; value_down_SF_wjets = res.SF_wjets.first;
        setup.varname=OBSERVABLE_FOR_FIT+"_JESup"; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); value_up_SF_1ltop = res.SF_1ltop.first ; value_up_SF_wjets = res.SF_wjets.first;
        uncert.SF_1ltop_uncert = fabs((value_down_SF_1ltop-value_up_SF_1ltop)/2.); 
        uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
        summary.uncert.push_back(uncert);
    */

    setup.Reset(); conditions="variables2"; setup.region="0btag_MTpeak"; uncert.name = conditions;
    setup.varname=OBSERVABLE_FOR_FIT; setup.varMin=0; setup.varMax=600;
    res = doFit(setup,conditions); 
    setup.varname="M3b"; setup.region="0btag_MTpeak";
    res = doFit(setup,conditions); 


    //-- Apply the template fit on Njets = 4 && Njets >=5  
    // This will change the fraction W+jets && ttbar
    setup.Reset(); conditions="Njets"; setup.region="0btag_MTpeak_4j"; uncert.name = conditions; res = doFit(setup,conditions); 
    setup.Reset(); conditions="Njets"; setup.region="0btag_MTpeak_5j"; uncert.name = conditions; res = doFit(setup,conditions); 
    setup.Reset(); conditions="Njets"; setup.region="0btag_MTtail_4j"; uncert.name = conditions; res = doFit(setup,conditions); 
    setup.Reset(); conditions="Njets"; setup.region="0btag_MTtail_5j"; uncert.name = conditions; res = doFit(setup,conditions); 

    //---  Perform the template for MT = 120 --//
    setup.Reset(); conditions="C&C"; setup.region="0btag_MTinverted120"; uncert.name = conditions; res = doFit(setup,conditions); 
    setup.Reset(); conditions="C&C"; setup.region="0btag_MTtail120";     uncert.name = conditions; res = doFit(setup,conditions); 


    //---------------------------------------//
    //-- Apply the template fit on each SR --//
    //---------------------------------------//

    //Create histos
    TH1F h_SF_MTpeak_BDT_1ltop ("h_SF_MTpeak_BDT_1ltop", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SF_MTtail_BDT_1ltop ("h_SF_MTtail_BDT_1ltop", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SFR_BDT_1ltop       ("h_SFR_BDT_1ltop",       "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SF_MTpeak_BDT_wjets ("h_SF_MTpeak_BDT_wjets", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SF_MTtail_BDT_wjets ("h_SF_MTtail_BDT_wjets", "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());
    TH1F h_SFR_BDT_wjets       ("h_SFR_BDT_wjets",       "", listBDTSignalRegions_MTtail.size(),0,listBDTSignalRegions_MTtail.size());

    varname=OBSERVABLE_FOR_FIT;

    float mean_SF1ltop = 0;
    float rms_SF1ltop = 0;
    float MaxStatUncert_SF1ltop = 0;

    float mean_SFwjets = 0;
    float rms_SFwjets = 0;
    float MaxStatUncert_SFwjets = 0;

    for(unsigned int i=0;i<listBDTSignalRegions_MTtail.size();i++)
    {
        cout<<"%%%%%%%%%%%%%%%%%% "<<listBDTSignalRegions_MTtail[i]<<endl;

        string label = listBDTSignalRegions_MTtail[i].substr(7);
        Figure SFR_1ltop;
        Figure SFR_wjets;

        //MT tail
        setup.Reset(); conditions="sigRegions_tail"; setup.region=listBDTSignalRegions_MTtail[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); 
        SFR_1ltop=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_wjets=Figure(res.SF_wjets.first,res.SF_wjets.second);
        h_SF_MTtail_BDT_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTtail_BDT_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTtail_BDT_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTtail_BDT_wjets.SetBinContent(i+1,res.SF_wjets.first);
        h_SF_MTtail_BDT_wjets.SetBinError(i+1,res.SF_wjets.second);
        h_SF_MTtail_BDT_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //MT peak
        setup.Reset(); conditions="sigRegions_peak"; setup.region=listBDTSignalRegions_MTpeak[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); 
        h_SF_MTpeak_BDT_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTpeak_BDT_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTpeak_BDT_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTpeak_BDT_wjets.SetBinContent(i+1,res.SF_wjets.first);
        h_SF_MTpeak_BDT_wjets.SetBinError(i+1,res.SF_wjets.second);
        h_SF_MTpeak_BDT_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //Now compute the ration : SF_tail/SF_peak
        SFR_1ltop/=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_wjets/=Figure(res.SF_wjets.first,res.SF_wjets.second);

        //-- do some additionnal test as function of the b-tag multiplicity

        //MT peak (no btag req)
        setup.Reset(); conditions="sigRegions_peak_NoBtag"; setup.region=listBDTSignalRegions_MTpeak_NoBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); 

        //MT peak (one btag req)
        setup.Reset(); conditions="sigRegions_peak_OneBtag"; setup.region=listBDTSignalRegions_MTpeak_OneBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); 

        //-- End of additionnal tests

        //Computation of mean/rms/ ..
        //It is based on  the ratio SF_tail/SF_peak
        //-- 1ltop
        mean_SF1ltop+=SFR_1ltop.value();
        rms_SF1ltop+=(SFR_1ltop.value()*SFR_1ltop.value());
        if(SFR_1ltop.error()>(MaxStatUncert_SF1ltop/SFR_1ltop.value()))  MaxStatUncert_SF1ltop=SFR_1ltop.error()/SFR_1ltop.value();
        //-- W+jets
        mean_SFwjets+=SFR_wjets.value();
        rms_SFwjets+=(SFR_wjets.value()*SFR_wjets.value());
        if(SFR_wjets.error()>(MaxStatUncert_SFwjets/SFR_wjets.value()))  MaxStatUncert_SFwjets=SFR_wjets.error()/SFR_wjets.value();
        //-----------------------------------



        //SFR
        h_SFR_BDT_1ltop.SetBinContent(i+1,SFR_1ltop.value());
        h_SFR_BDT_1ltop.SetBinError(i+1,SFR_1ltop.error());
        h_SFR_BDT_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SFR_BDT_wjets.SetBinContent(i+1,SFR_wjets.value());
        h_SFR_BDT_wjets.SetBinError(i+1,SFR_wjets.error());
        h_SFR_BDT_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());


        cout<<"signRegions: SFR_1ltop: "<<SFR_1ltop.Print()<<" SFR_wjets: "<<SFR_wjets.Print()<<endl;
    }
    //Save plots in roofile
    TFile fCR1_BDT((string(OUTPUT_FOLDER)+"/CR1_BDT.root").c_str(),"RECREATE");
    h_SF_MTpeak_BDT_1ltop.Write();
    h_SF_MTpeak_BDT_wjets.Write();
    h_SF_MTtail_BDT_1ltop.Write();
    h_SF_MTtail_BDT_wjets.Write();
    h_SFR_BDT_1ltop.Write();
    h_SFR_BDT_wjets.Write();


    //---------------------------------------//
    //We add quadratically 20% uncert. on the fit procedure (MC stat, JES, etc...
    float SystUncert = 0.2;
    mean_SF1ltop/=listBDTSignalRegions_MTtail.size();
    rms_SF1ltop/=listBDTSignalRegions_MTtail.size();
    rms_SF1ltop-=(mean_SF1ltop*mean_SF1ltop);
    Figure BDT_SF1ltop(mean_SF1ltop,sqrt(rms_SF1ltop+MaxStatUncert_SF1ltop*MaxStatUncert_SF1ltop*mean_SF1ltop*mean_SF1ltop+pow(SystUncert*mean_SF1ltop,2)));
    //---------------------------------------//
    mean_SFwjets/=listBDTSignalRegions_MTtail.size();
    rms_SFwjets/=listBDTSignalRegions_MTtail.size();
    rms_SFwjets-=(mean_SFwjets*mean_SFwjets);
    Figure BDT_SFwjets(mean_SFwjets,sqrt(rms_SFwjets+MaxStatUncert_SFwjets*MaxStatUncert_SFwjets*mean_SFwjets*mean_SFwjets+pow(SystUncert*mean_SF1ltop,2)));
    //---------------------------------------//


    vector<string> sigReglabels(listBDTSignalRegions_MTtail.size());
    for(unsigned int i=0;i<listBDTSignalRegions_MTtail.size();i++)
    {
        sigReglabels[i] = listBDTSignalRegions_MTtail[i].substr(3);
    } 
    vector<string> columns = {"SFR_1ltop","SFR_wjets"};
    Table SFR_BDT(columns,sigReglabels,columns,sigReglabels);


    for(unsigned int i=0;i<listBDTSignalRegions_MTtail.size();i++)
    {
        SFR_BDT.Set("SFR_1ltop",sigReglabels[i],BDT_SF1ltop);
        SFR_BDT.Set("SFR_wjets",sigReglabels[i],BDT_SFwjets);
    }
    SFR_BDT.Print(string(OUTPUT_FOLDER)+"/SFR_BDT.tab",4);
    //---------------------------------------//
    std::map<string,Figure> SFR_CC_1ltop_map;
    std::map<string,Figure> SFR_CC_wjets_map;

    //Create histos
    TH1F h_SF_MTpeak_CC_1ltop ("h_SF_MTpeak_CC_1ltop",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SF_MTtail_CC_1ltop ("h_SF_MTtail_CC_1ltop",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SFR_CC_1ltop       ("h_SFR_CC_1ltop",        "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SF_MTpeak_CC_wjets ("h_SF_MTpeak_CC_wjets",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SF_MTtail_CC_wjets ("h_SF_MTtail_CC_wjets",  "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());
    TH1F h_SFR_CC_wjets       ("h_SFR_CC_wjets",        "", listIndividualCuts_MTtail.size(), 0, listIndividualCuts_MTtail.size());

    for(unsigned int i=0;i<listIndividualCuts_MTtail.size();i++)
    {
        cout<<"%%%%%%%%%%%%%%%%%% "<<listIndividualCuts_MTtail[i]<<endl;

        string label = listIndividualCuts_MTtail[i].substr(15);
        Figure SFR_1ltop;
        Figure SFR_wjets;

        //MT tail
        setup.Reset(); conditions="sigRegions_CC_tail"; setup.region=listIndividualCuts_MTtail[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); 
        SFR_1ltop=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_wjets=Figure(res.SF_wjets.first,res.SF_wjets.second);
        h_SF_MTtail_CC_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTtail_CC_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTtail_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTtail_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
        h_SF_MTtail_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
        h_SF_MTtail_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //MT peak
        setup.Reset(); conditions="sigRegions_CC_peak"; setup.region=listIndividualCuts_MTpeak[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        res = doFit(setup,conditions); 
        h_SF_MTpeak_CC_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTpeak_CC_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTpeak_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTpeak_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
        h_SF_MTpeak_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
        h_SF_MTpeak_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //MT peak (no btag req)
        //setup.Reset(); conditions="sigRegions_peak_NoBtag"; setup.region=listBDTSignalRegions_MTpeak_NoBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        //res = doFit(setup,conditions); 

        //Now compute the ration : SF_tail/SF_peak
        SFR_1ltop/=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_wjets/=Figure(res.SF_wjets.first,res.SF_wjets.second);

        //It is based on the ratio SF_tail/SF_peak
        SFR_CC_1ltop_map[label] = SFR_1ltop;
        SFR_CC_wjets_map[label] = SFR_wjets;



        //SFR
        h_SFR_CC_1ltop.SetBinContent(i+1,SFR_1ltop.value());
        h_SFR_CC_1ltop.SetBinError(i+1,SFR_1ltop.error());
        h_SFR_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SFR_CC_wjets.SetBinContent(i+1,SFR_wjets.value());
        h_SFR_CC_wjets.SetBinError(i+1,SFR_wjets.error());
        h_SFR_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());


        cout<<"signRegions (CC): SFR_1ltop: "<<SFR_1ltop.Print()<<" SFR_wjets: "<<SFR_wjets.Print()<<endl;
    }

    //Save plots in roofile
    TFile fCR1_CC((string(OUTPUT_FOLDER)+"/CR1_CC.root").c_str(),"RECREATE");
    h_SF_MTpeak_CC_1ltop.Write();
    h_SF_MTpeak_CC_wjets.Write();
    h_SF_MTtail_CC_1ltop.Write();
    h_SF_MTtail_CC_wjets.Write();
    h_SFR_CC_1ltop.Write();
    h_SFR_CC_wjets.Write();

    //---------------------------------------------
    // Results for C&C
    initCutAndCountCuts();
    vector<string> cuts;

    Table SFR_CC(columns,listCutAndCounts,columns,listCutAndCounts);
    for(unsigned int r=0;r<listCutAndCounts.size();r++)
    {
        cuts = listCutAndCounts_cuts[listCutAndCounts[r]];    
        Figure SFR_CC_1ltop;
        Figure SFR_CC_wjets;
        for(unsigned i=0;i<cuts.size();i++)
        {
            cout<<cuts[i]<<" "<<SFR_CC_1ltop_map[cuts[i]].Print()<<endl;
            if(i == 0) SFR_CC_1ltop = SFR_CC_1ltop_map[cuts[i]];
            else SFR_CC_1ltop = Figure(SFR_CC_1ltop.value(),
                                       sqrt(pow(SFR_CC_1ltop.error(),2)
                                           +pow(SFR_CC_1ltop.value()
                                                -SFR_CC_1ltop_map[cuts[i].c_str()].value(),2)
                                           ));

            if(i == 0) SFR_CC_wjets = SFR_CC_wjets_map[cuts[i]];
            else SFR_CC_wjets = Figure(SFR_CC_wjets.value(),
                                       sqrt(pow(SFR_CC_wjets.error(),2)
                                           +pow(SFR_CC_wjets.value()
                                               -SFR_CC_wjets_map[cuts[i].c_str()].value(),2)
                                           ));
        }
        //Add quadratically uncert. of the fit itself (JES, MC stat. ...)
        SFR_CC_1ltop = Figure(SFR_CC_1ltop.value(),sqrt(pow(SFR_CC_1ltop.error(),2)+pow(SystUncert*mean_SF1ltop ,2)));
        SFR_CC_wjets = Figure(SFR_CC_wjets.value(),sqrt(pow(SFR_CC_wjets.error(),2)+pow(SystUncert*mean_SFwjets,2)));
        SFR_CC.Set("SFR_1ltop",listCutAndCounts[r],SFR_CC_1ltop);
        SFR_CC.Set("SFR_wjets",listCutAndCounts[r],SFR_CC_wjets);
        cout<<"TOO "<<listCutAndCounts[r]<<" "<<SFR_CC_1ltop.Print()<<endl;
    }


    SFR_CC.Print(string(OUTPUT_FOLDER)+"/SFR_CC.tab",4);

    //---------------------------------------------
    //  Perform an estimation for SFR C&C
    //---------------------------------------------

    /*

    //Create histos
    TH1F h_SF_MTpeakEstim_CC_1ltop("h_SF_MTpeakEstim_CC_1ltop","",listCutAndCountsRegions_MTtail.size(),0,listCutAndCountsRegions_MTtail.size());
    TH1F h_SF_MTtailEstim_CC_1ltop("h_SF_MTtailEstim_CC_1ltop","",listCutAndCountsRegions_MTtail.size(),0,listCutAndCountsRegions_MTtail.size());
    TH1F h_SFREstim_CC_1ltop("h_SFREstim_CC_1ltop","",listCutAndCountsRegions_MTtail.size(),0,listCutAndCountsRegions_MTtail.size());
    TH1F h_SF_MTpeakEstim_CC_wjets("h_SF_MTpeakEstim_CC_wjets","",listCutAndCountsRegions_MTtail.size(),0,listCutAndCountsRegions_MTtail.size());
    TH1F h_SF_MTtailEstim_CC_wjets("h_SF_MTtailEstim_CC_wjets","",listCutAndCountsRegions_MTtail.size(),0,listCutAndCountsRegions_MTtail.size());
    TH1F h_SFREstim_CC_wjets("h_SFREstim_CC_wjets","",listCutAndCountsRegions_MTtail.size(),0,listCutAndCountsRegions_MTtail.size());

    for(unsigned int i=0;i<listCutAndCountsRegions_MTtail.size();i++)
    {
        cout<<"%%%%%%%%%%%%%%%%%% "<<listCutAndCountsRegions_MTtail[i]<<endl;

        string label = listCutAndCountsRegions_MTtail[i].substr(15);
        Figure SFR_1ltop;
        Figure SFR_wjets;

        //MT tail
        setup.Reset(); conditions="sigRegionsEstim_CC_tail"; setup.region=listCutAndCountsRegions_MTtail[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        setup.varname=string(OBSERVABLE_FOR_FIT)+"_small";
        res = doFit(setup,conditions); 
        SFR_1ltop=Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_wjets=Figure(res.SF_wjets.first,res.SF_wjets.second);
        h_SF_MTtailEstim_CC_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTtailEstim_CC_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTtailEstim_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTtailEstim_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
        h_SF_MTtailEstim_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
        h_SF_MTtailEstim_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());

        //MT peak
        setup.Reset(); conditions="sigRegionsEstim_CC_peak"; setup.region=listCutAndCountsRegions_MTpeak[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        setup.varname=string(OBSERVABLE_FOR_FIT)+"_small";
        res = doFit(setup,conditions); 
        h_SF_MTpeakEstim_CC_1ltop.SetBinContent(i+1,res.SF_1ltop.first);
        h_SF_MTpeakEstim_CC_1ltop.SetBinError(i+1,res.SF_1ltop.second);
        h_SF_MTpeakEstim_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SF_MTpeakEstim_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
        h_SF_MTpeakEstim_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
        h_SF_MTpeakEstim_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
        SFR_1ltop  /= Figure(res.SF_1ltop.first,res.SF_1ltop.second);
        SFR_wjets /= Figure(res.SF_wjets.first,res.SF_1ltop.second);

        //MT peak (no btag req)
        //setup.Reset(); conditions="sigRegions_peak_NoBtag"; setup.region=listBDTSignalRegions_MTpeak_NoBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        //res = doFit(setup,conditions); 

        //SFR
        h_SFREstim_CC_1ltop.SetBinContent(i+1,SFR_1ltop.value());
        h_SFREstim_CC_1ltop.SetBinError(i+1,SFR_1ltop.error());
        h_SFREstim_CC_1ltop.GetXaxis()->SetBinLabel(i+1,label.c_str());
        h_SFREstim_CC_wjets.SetBinContent(i+1,SFR_wjets.value());
        h_SFREstim_CC_wjets.SetBinError(i+1,SFR_wjets.error());
        h_SFREstim_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());


        cout<<"signRegions (CC) - "<<listCutAndCountsRegions_MTpeak[i]<<": SFR_1ltop: "<<SFR_1ltop.Print()<<" SFR_wjets: "<<SFR_wjets.Print()<<endl;
    }
    //Save plots in roofile
    TFile fCR1Estim_CC((string(OUTPUT_FOLDER)+"/CR1Estim_CC.root").c_str(),"RECREATE");
    h_SF_MTpeakEstim_CC_1ltop.Write();
    h_SF_MTpeakEstim_CC_wjets.Write();
    h_SF_MTtailEstim_CC_1ltop.Write();
    h_SF_MTtailEstim_CC_wjets.Write();
    h_SFREstim_CC_1ltop.Write();
    h_SFREstim_CC_wjets.Write();

    */


    //---------------------------------------//
    //-- split region --//,   /* 
    /* 
       setup.Reset(); conditions = "split-region";
       setup.region = "0btagLD_MTtail";
       res = doFit(setup,conditions); 
       setup.region = "0btagHD_MTtail";
       res = doFit(setup,conditions); 



    //---  choice of MT test cut --//
    setup.Reset(); setup.region = "0btag_MT80"; conditions="MT80"; 
    res = doFit(setup,conditions); 
    setup.Reset(); setup.region = "0btag_MT90"; conditions="MT90"; 
    res = doFit(setup,conditions); 
    setup.Reset(); setup.region = "0btag_MT110"; conditions="M110"; 
    res = doFit(setup,conditions); 
    setup.Reset(); setup.region = "0btag_MT120"; conditions="MT120"; 
    res = doFit(setup,conditions); 
    setup.Reset(); setup.region = "0btag_MT130"; conditions="MT130"; 
    res = doFit(setup,conditions); 
    //setup.varname="M3b_modif";//test with that var ! -- does not work .. 
    res = doFit(setup,conditions); 
    setup.Reset(); setup.region = "0btag_MT140"; conditions="MT140"; setup.varname = "M3b_binningdown";
    setup.type="GSLMultiMin"; setup.algo="steepestdescent";
    setup.type="GSLMultiMin"; setup.algo="conjugatefr";
    res = doFit(setup,conditions); 
    setup.Reset(); setup.region = "0btag_MT160"; conditions="MT160"; setup.varname = "M3b_binningdown";
    setup.type="GSLMultiMin"; setup.algo="steepestdescent";
    res = doFit(setup,conditions); 
    */
    //---- choice of MT test cut with Mlb ---//
    /*
       setup.Reset(); setup.region = "0btag_MT80";   conditions="MT80";  setup.varname=OBSERVABLE_FOR_FIT; 
       res = doFit(setup,conditions); 
       setup.Reset(); setup.region = "0btag_MT90";   conditions="MT90";  setup.varname=OBSERVABLE_FOR_FIT;
       res = doFit(setup,conditions); 
       setup.Reset(); setup.region = "0btag_MTtail"; conditions="MT100"; setup.varname=OBSERVABLE_FOR_FIT;
       res = doFit(setup,conditions); 
       setup.Reset(); setup.region = "0btag_MT110";  conditions="MT110"; setup.varname=OBSERVABLE_FOR_FIT;
       res = doFit(setup,conditions); 
       setup.Reset(); setup.region = "0btag_MT120";  conditions="MT120"; setup.varname=OBSERVABLE_FOR_FIT;
       res = doFit(setup,conditions); 
       setup.Reset(); setup.region = "0btag_MT130";  conditions="MT130"; setup.varname=OBSERVABLE_FOR_FIT;
       res = doFit(setup,conditions); 
       */

    summary.Print();
}  
