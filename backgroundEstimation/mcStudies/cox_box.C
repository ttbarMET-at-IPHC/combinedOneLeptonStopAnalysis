// Macro used to determine the parameter to be choosen in a Cox-Box method
// to transfom a landau variable into a more gaussiam variable

{
	#include <vector>
	#include "TH1F.h"
	#include "TF1.h"
	#include "TRandom3.h"

	using namespace std;

	TF1* f_landau = new TF1("landau","TMath::Landau(x,[0],[1])",0,2000);
	f_landau->SetParameters(650,190);
	f_landau->Draw();
	
	//Randomize following a Landau
	TRandom3 rand;
	double lambda = 0.05;
	double step = 0.05;
	int nstep = 20;
	vector<TH1F*> histos;
	cout<<"coucou"<<endl;
	//for(int i=0;i<nstep;i++){
	for(int i=0;i<nstep;i++){
		cout<<i<<endl;
		char title[100];
		sprintf(title,"h_%d",i);
		//TH1F* h = new TH1F(title,"",2000,0,1000000);
		TH1F* h = new TH1F(title,"",100,0,3000);
		histos.push_back(h);
		//histos.push_back(new TH1F(title,"",50,0,2000));
	}
	double c = 5; //shift
	double g = 1.05; //spread
	TH1F* htest = new TH1F("htest","",50,0,2000);	
	for(int i=0;i<10000;i++){

		double x = rand.Landau(650,190);
		//double x = rand.Landau(100,37);
		if(x>650) a = x*(1-sqrt(x)/80);
		else a = x;
		double y = rand.Landau(650,70);
		htest->Fill(y);
		cout<<x<<" "<<a<<endl;
		for(int z=0;z<nstep;z++){
			//c = pow(650,lambda);
			c = 0;
			//double xp = (pow(x+c,lambda)-1)/(lambda);
			//double xp = (pow(x+c,lambda)-1)/(lambda*g);
			lambda= (z+1)*step;
			//double xp = (pow(x,lambda)-1)/(lambda);
			double xp = (pow(x+c,lambda)-1)/(100*pow(lambda,4));
		//	xp/=(100*lambda);
			histos[z]->Fill(xp);
			
		}

	}
	histos[0]->Draw();
	htest->Draw("same");
	return;
	TH1F* h_Kurtosis = new TH1F("kurtosis", "",nstep,0,nstep);
	TH1F* h_Skewness = new TH1F("skewness", "",nstep,0,nstep);
	for(int i=0;i<nstep;i++){
		//h_Kurtosis->SetBinContent(z,histos[i]->GetKurtosis());
		//h_Skewness->SetBinContent(z,histos[i]->GetSkewness());
		cout<<(i+1)*step<<" "<<histos[i]->GetKurtosis()<<" "<<histos[i]->GetSkewness()<<endl;
	}
	h_Kurtosis->Draw();
	h_Kurtosis->SetLineColor(kRed);
	h_Kurtosis->Draw();
	h_Skewness->Draw("same");
	
	histos[10]->Draw();
	//htest->Draw();
}

