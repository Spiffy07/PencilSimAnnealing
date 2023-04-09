#include "Objects.h"

//#include <fstream>	// for file output stretch

// function declarations 
//void GenerateTables(std::vector<Table>& tablesIn);
//void GenerateDealers(std::vector<Dealer>& dealersIn);


int main()
{
	std::vector<Table> tables;	
	std::vector<Dealer> dealers;
	GenerateTables(tables);
	GenerateDealers(dealers);

	std::cout << "Program Successfully ran!" << std::endl;
	return 0;
}

