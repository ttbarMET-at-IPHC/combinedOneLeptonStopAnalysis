#include "DataCardProducer.h"

#include <fstream>
#include <iomanip>
#include <sstream> 



void DataCardProducer::ReadTable(const string& table_name){
	table = Table(table_name);
}

void DataCardProducer::LoadProcesses(const string& datasetname, const vector<string>& names, const vector<string>& table_names, const vector<float>& syst_uncert, const vector<bool>& stat_uncert){
	this->datasetName = datasetname;
	this->proc_names = names;
	this->proc_table_names = table_names;
	this->proc_sys_reluncert = syst_uncert;
	this->proc_stat_buncert = stat_uncert;
}


void DataCardProducer::LoadBins(const vector<string>& names){
	bins_table_names = names;
}

void DataCardProducer::WriteBinCard(const string& ofilename, int iBin){
	ostringstream convert;
	convert << iBin;
	string oname = ofilename+convert.str()+".txt";
	ofstream ofile(oname.c_str());
        ofile << std::fixed;
	ofile << std::setprecision(2);
	
	ofile<<"## datacard with 1 MET bins"<<endl;
	
	// Global parameters
	ofile<<"---"<<endl;
	ofile<<"imax 1 number of channels"<<endl;
	//processes
	ofile<<"jmax "<<proc_names.size()<<" number of backgrounds (";
	for(unsigned int i=0;i<proc_names.size()-1;i++){
		if(proc_names[i]!="signal") ofile<<proc_names[i]<<", ";
	}
	if(proc_names.size()>0) ofile<<proc_names[proc_names.size()-1]<<")"<<endl;
	//---------
	// systematics
	int nof_nparam = proc_sys_reluncert.size();
	for(unsigned int i=0;i<proc_stat_buncert.size();i++){
		if(proc_stat_buncert[i]) nof_nparam++;
	}
	ofile<<"kmax "<<nof_nparam<<" number of nuisance parameters (sources of systematical uncertainties)"<<endl;
	
	// Nof in data
	ofile<<"---"<<endl;
	ofile<<"## data yields"<<endl;
	ofile<<"bin\t\t"<<iBin<<endl;
	float n_data = 0;
	//retrieve n_data
	n_data = table.Get(datasetName,bins_table_names[iBin]).value();
	ofile<<"observation\t\t"<<n_data<<endl;
	
	// Processes and rate
	ofile<<"---"<<endl;
	ofile<<"## we list the expected events for signal and all backgrounds in the bins"<<endl;
	ofile<<"## the second process line must have a positive number for backgrounds, and 0 for signal"<<endl;
	ofile<<"---"<<endl;
	ofile<<"bin\t";
	for(unsigned int i=0;i<proc_names.size();i++) ofile<<"\t"<<iBin;
	ofile<<endl;
	ofile<<"process\t";
	for(unsigned int i=0;i<proc_names.size();i++) ofile<<"\t"<<proc_names[i];
	ofile<<endl;
	ofile<<"process\t";
	for(unsigned int i=0;i<proc_names.size();i++) ofile<<"\t"<<i;
	ofile<<endl;
	ofile<<"rate\t";
	for(unsigned int i=0;i<proc_names.size();i++){
		float nevents = 0;
		if(proc_names[i] != "signal")
			//retrieve nevents
			nevents = table.Get(proc_table_names[i],bins_table_names[iBin]).value();
		ofile<<"\t"<<nevents;
	}
  	ofile<<endl;

	// Nuisance parameters
	ofile<<"---"<<endl;
	ofile<<"## then we list the independent sources of uncertainties, and give their effect (syst. error)"<<endl;
	ofile<<"## on each process and bin"<<endl;
	ofile<<"---"<<endl;
	for(unsigned int i=0;i<proc_names.size();i++){
		//systematic uncertainty
		string name;
		name = proc_names[i]+"Syst"+convert.str();
		ofile<<name<<"\t";
		ofile<<"lnN\t";
		for(int j=0;j<nof_nparam;j++){
			if((int)i==j) ofile<<proc_sys_reluncert[i]<<"\t";
			else ofile<<"-\t";
		}
		ofile<<endl;

		//statistic uncertainty
		if(proc_stat_buncert[i]){
			name = proc_names[i]+"Stat"+convert.str();
			ofile<<name<<"\t";
			ofile<<"lnN\t";
			float proc_stat_reluncert = 0;
			//should be computed from the table 
			Figure Npred = table.Get(proc_table_names[i],bins_table_names[iBin]);
			if(Npred.value()>0) proc_stat_reluncert = Npred.error()/Npred.value();
			for(int j=0;j<nof_nparam;j++){
				if((int)i==j) ofile<<proc_stat_reluncert<<"\t";
				else ofile<<"-\t";
			}
			ofile<<endl;
		}
	}
	ofile<<"---"<<endl;

}

void DataCardProducer::WriteCards(const string& name){
	for(unsigned int iBin = 0;iBin<bins_table_names.size();iBin++){
		WriteBinCard(name,(int)iBin);
	}
}
