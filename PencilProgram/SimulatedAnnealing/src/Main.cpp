#include "Objects.h"

//#include <fstream>	// for file output stretch

// function declarations 
//void GenerateTables(std::vector<Table>& tablesIn);
//void GenerateDealers(std::vector<Dealer>& dealersIn);

// static member declarations
unsigned int Dealer::employeeNumberCounter;

Push PopulateTables(const std::vector<Table>& tables, std::vector<Dealer>& dealers);
void CalculateFitness(Push& push, std::vector<Dealer>& dealers);
bool findGameKnowledge(const Table::Games& gameName, Dealer& dealer);


int main()
{
	std::vector<Table> tables;	
	std::vector<Dealer> dealers;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	srand(time(0));
	Push first = PopulateTables(tables, dealers);

	CalculateFitness(first, dealers);

	std::cout << "   Program Successfully ran!" << std::endl;
	std::cin.get();
	return 0;
}

Push PopulateTables(const std::vector<Table>& tables, std::vector<Dealer>& dealers)
{
	Push p;
	p.push.reserve(NUMBER_OF_TABLES);
	for (Table t : tables)
	{
		p.push.emplace_back(t, dealers[rand() % 10]);
	}

	return p;
}

void CalculateFitness(Push& push, std::vector<Dealer>& dealers)
{
	push.fitness = 0;			// initialize and/or reset to zero
	
	for (Assignment a : push.push)
	{
		a.aDealer.tablesAssigned += 1;	// count number of assigned tables MAX = 1
		
		switch (a.aTable.gameName){
		case Table::BJ:
			if (findGameKnowledge(Table::BJ, a.aDealer) 
				? push.fitness += 3 : push.fitness -= 10);
			break;
		case Table::Rou:
			if (findGameKnowledge(Table::Rou, a.aDealer)
				? push.fitness += 5 : push.fitness -= 10);
			break;
		case Table::MB:
			if (findGameKnowledge(Table::MB, a.aDealer)
				? push.fitness += 5 : push.fitness -= 10);
			break;
		case Table::Poker:
			if (findGameKnowledge(Table::Poker, a.aDealer)
				? push.fitness += 5 : push.fitness -= 10);
			break;
		default:
			std::cout << "Error: Invalid gameKnowledge fitness result!" << std::endl;
		}
	}

	std::cout << "  Fitness: " << push.fitness << std::endl;


	for (Dealer d : dealers)
	{
		d.tablesAssigned = 0;	// reset assigned tables to zero **reference**
	}
}

bool findGameKnowledge(const Table::Games& gameName, Dealer& dealer)
{
	for (Table::Games g : dealer.gameKnowledge)
	{
		if (gameName == g) return true;
	}
	return false;
}