

// Useful for debugging
#define DEBUG_MSG cout << "DEBUG (" << __FILE__ << ", l." << __LINE__ << ") "

// Used to print info
#define INFO1_MSG cout << "     > "

double pi=acos(-1.);



struct SortByPt
{
  bool operator()( TLorentzVector j1, TLorentzVector j2 ) const
  {
    return j1.Pt() > j2.Pt() ;
  }
};

void printProgressBar(int current, int max)
{
	std::string bar;
	int percent = 100 * (float) current / (float) max;

	for(int i = 0; i < 50; i++)
	{
		if( i < (percent/2))       bar.replace(i,1,"=");
		else if( i == (percent/2)) bar.replace(i,1,">");
		else					   bar.replace(i,1," ");
	}

	std::cout << "  [Progress]  ";
	std::cout << "[" << bar << "] ";
	std::cout.width( 3 );
	std::cout << percent << "%     ";
	std::cout << "(" << current << " / " << max << ")" << std::endl;
}

