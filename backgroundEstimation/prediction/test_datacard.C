#include "DataCardProducer.h"

using namespace std;
using namespace theDoctor;

int main(){

	//Create a dummy table
	vector<string> colTags = {"SingleMu","T2tt_850_100", "ttbar_1l", "ttbar_2l", "Wjets", "SingleTop", "ttV"};
	vector<string> rowTags = {"MET_100_150", "MET_150_inf"};
	theDoctor::Table table(colTags,rowTags);
	// fill the table
	table.Set("SingleMu", "MET_100_150", Figure(3));
	table.Set("T2tt_850_100","MET_100_150", Figure(2.09183063824,2));
	table.Set("ttbar_1l","MET_100_150", Figure(0,0));
	table.Set("ttbar_2l","MET_100_150", Figure(0.740050598979,1.71*0.740050598979));
	table.Set("Wjets","MET_100_150", Figure(0.1,0.05));
	table.Set("SingleTop","MET_100_150", Figure(0.35678666085,1.71*0.35678666085));
	table.Set("ttV","MET_100_150", Figure(1.18165183812,1.11*1.18165183812));
	
	table.Set("SingleMu", "MET_150_inf", Figure(1));
	table.Set("T2tt_850_100","MET_150_inf", Figure(1.09183063824,2));
	table.Set("ttbar_1l","MET_150_inf", Figure(0,0));
	table.Set("ttbar_2l","MET_150_inf", Figure(0.240050598979,1.71*0.740050598979));
	table.Set("Wjets","MET_150_inf", Figure(0.01,0.008));
	table.Set("SingleTop","MET_150_inf", Figure(0.15678666085,1.71*0.15678666085));
	table.Set("ttV","MET_150_inf", Figure(0.18165183812,1.11*0.18165183812));

	table.Print("card_test.dat");

	// Now test the dataCardProducer
	
	DataCardProducer prod;
	prod.ReadTable("card_test.dat");
    	
	string datasetName = "SingleMu";
	vector<string> names = {"signal", "TTbar2l", "TTbar1l", "WJets", "SingleTop", "TTV"};
	vector<string> table_names = {"T2tt_850_100", "ttbar_2l", "ttbar_1l", "Wjets", "SingleTop", "ttV"};
	vector<float> syst_uncert = {1.3, 1.3, 1.3, 1.3, 1.3, 1.3};
	vector<bool> stat_uncert = {false, true, false, false, true, true};
	prod.LoadProcesses(datasetName,  names, table_names, syst_uncert, stat_uncert);
	
	vector<string> bin_names = {"MET_100_150", "MET_150_inf"};
	prod.LoadBins(bin_names);

	prod.WriteBinCard(string("card"),0);
}
