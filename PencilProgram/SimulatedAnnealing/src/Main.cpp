#include "Objects.h"

//#include <fstream>	// for file output stretch

// function declarations 
//void GenerateTables(std::vector<Table>& tablesIn);
//void GenerateDealers(std::vector<Dealer>& dealersIn);

// static member declarations
unsigned int Dealer::employeeNumberCounter;

Push PopulateTables(const std::vector<Table>& tables, std::vector<Dealer> dealers);

int main()
{
	std::vector<Table> tables;	
	std::vector<Dealer> dealers;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	srand(time(0));
	PopulateTables(tables, dealers);

	std::cout << "Program Successfully ran!" << std::endl;
	std::cin.get();
	return 0;
}

Push PopulateTables(const std::vector<Table>& tables, std::vector<Dealer> dealers)
{
	Push p;
	p.push.reserve(NUMBER_OF_TABLES);
	for (Table t : tables)
	{
		p.push.emplace_back(t, dealers[rand() % 10]);
	}

	return p;
}