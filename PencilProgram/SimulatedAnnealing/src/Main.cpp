#include "Objects.h"

//#include <fstream>	// for file output stretch

// function declarations 
//void GenerateTables(std::vector<Table>& tablesIn);
//void GenerateDealers(std::vector<Dealer>& dealersIn);

// static member declarations
unsigned int Dealer::employeeNumberCounter;

int main()
{
	std::vector<Table> tables;	
	std::vector<Dealer> dealers;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	std::cout << "Program Successfully ran!" << std::endl;
	return 0;
}

