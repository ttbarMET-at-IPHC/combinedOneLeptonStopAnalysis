#ifndef DataCardProducer_
#define DataCardProducer_


#include <vector>
#include <string>

#include "../../sonicScrewdriver/interface/Table.h"

using namespace std;
using namespace theDoctor;

class DataCardProducer{

  public:
    DataCardProducer(){;};
    ~DataCardProducer(){;};
  
    void ReadTable(const string& table_name);
    

    void LoadProcesses(const string& datasetName, const vector<string>& names, const vector<string>& table_names, const vector<float>& syst_uncert, const vector<bool>& stat_uncert);
    // datasetName is the name of the data
    // for a given process, we should have a name, the corresponding name in the table loaded, the total relative systematic uncertainty,
    // a boolean saying if we should add a stat. relative uncertainty
    
    void LoadBins(const vector<string>& names);
    void WriteBinCard(const string& name, int iBin);
    // iBin start from 1 not 0
    void WriteCards(const string& name);
    

  private:

    Table table;
    //processes
    vector<string> proc_names;
    vector<string> proc_table_names;
    //nuisances parameters
    vector<float> proc_sys_reluncert;
    vector<bool> proc_stat_buncert;
    //vector<float> proc_stat_reluncert;
    //bins
    vector<string> bins_table_names;
    //data
    string datasetName;

};

#endif

