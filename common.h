#ifndef _COMBINED1LEPTONSTOPANALYSIS_H_
#define _COMBINED1LEPTONSTOPANALYSIS_H_

// C/C++ headers

#include <fstream>
#include <cmath>
#include <iomanip>
#include <time.h>
#include <set>
#include <iostream>

using namespace std;

// ROOT headers

#include <TFile.h>
#include <TRandom.h>
#include <TMarker.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>
#include <TMVA/Reader.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>

// Sonic Screwdriver headers
/*
#include "interface/Table.h"
#include "interface/SonicScrewdriver.h"
#include "interface/tables/TableBackgroundSignal.h"
#include "interface/tables/TableDataMC.h"
using namespace theDoctor;
*/

// Debugging macro
#define DEBUG_MSG cerr << "DEBUG (" << __FILE__ << ", l." << __LINE__ << ") "

// Used to print info
#define INFO_MSG cout << "     > "

bool findSubstring(string input,string substring)
{
    if (input.find(substring) != string::npos) return true;
    else return false;
}

void replace(string& str, const string oldStr, const string newStr)
{
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != string::npos)
    {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

string intToString(int input)
{
    std::ostringstream ss;
    ss << input;
    return ss.str();
}

string floatToString(float input)
{
    std::ostringstream ss;
    ss << input;
    return ss.str();
}


void printProgressBar(int current, int max, string name = "Progress")
{
    std::string bar;
    int percent = 100 * (float) current / (float) max;

    for(int i = 0; i < 25; i++)
    {
        if( i < (percent/4))       bar.replace(i,1,"=");
        else if( i == (percent/4)) bar.replace(i,1,">");
        else                       bar.replace(i,1," ");
    }

    std::cout << "  [" << name << "]\t";
    std::cout <<   "[" << bar << "]  ";
    std::cout.width( 3 );
    std::cout << percent << "%     ";
    std::cout << "(" << current << " / " << max << ")" << "\r" << std::flush;
}

void printBoxedMessage(string message)
{
    cout << endl;

    cout << "   ┌──";
    for(unsigned int i = 0 ; i <= message.size() ; i++) cout << "─";
    cout << "─┐  " << endl;

    cout << "   │  " << message << "  │  " << endl;

    cout << "   └──";
    for(unsigned int i = 0 ; i <= message.size() ; i++) cout << "─";
    cout << "─┘  " << endl;

    cout << endl;

}

#endif
