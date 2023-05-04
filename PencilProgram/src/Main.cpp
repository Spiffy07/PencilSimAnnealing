#include <iostream>
#include "Objects.h"
#include "LogExtern.h"

const Log::LogLevel LOG_LEVEL = Log::Info;		// set log level here
Log LOG;

// static member declarations
unsigned int Dealer::employeeNumberCounter;

Push PopulateTables(std::vector<Table>& tables, std::vector<Dealer>& dealers);
void CalculateFitness(Push& push, std::vector<Dealer>& dealers);
bool findGameKnowledge(const Table::Games& gameName, Dealer* dealerPtr);


int main()
{
	LOG.SetLogLevel(LOG_LEVEL);

	std::vector<Table> tables;	
	std::vector<Dealer> dealers;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	srand(time(0));
	Push first = PopulateTables(tables, dealers);

	CalculateFitness(first, dealers);

	LOG.LogError("Program Successfully ran!");
	LOG.LogWarning("Warning test");
	LOG.LogInfo("Info test");
	std::cin.get();
	return 0;
}

Push PopulateTables(std::vector<Table>& tables, std::vector<Dealer>& dealers)
{
	Push p;
	p.push.reserve(NUMBER_OF_TABLES);
	for (Table& t : tables)
	{
		p.push.emplace_back(t, dealers[rand() % 10]);
	}

	return p;
}

void CalculateFitness(Push& push, std::vector<Dealer>& dealers)
{
	push.fitness = 0;			// initialize and/or reset to zero
	
	for (Assignment& a : push.push)
	{
		// count number of assigned tables MAX = 1
		a.aDealerPtr->tablesAssigned += 1;	
		LOG.LogInfo(a.aDealerPtr->name + ": " + std::to_string(a.aDealerPtr->tablesAssigned));
		
		// Game Knowledge check
		switch (a.aTable.gameName){
		case Table::BJ:
			if (findGameKnowledge(Table::BJ, a.aDealerPtr) 
				? push.fitness += 3 : push.fitness -= 10);
			break;
		case Table::Rou:
			if (findGameKnowledge(Table::Rou, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 10);
			break;
		case Table::MB:
			if (findGameKnowledge(Table::MB, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 10);
			break;
		case Table::Poker:
			if (findGameKnowledge(Table::Poker, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 10);
			break;
		default:
			LOG.LogError("Error: Invalid gameKnowledge fitness result!");
		}
	} 


	for (Dealer& d : dealers)
	{
		// reset assigned tables to zero **reference**

		if (d.tablesAssigned > 1)
			push.fitness -= (d.tablesAssigned - 1) * 50;	// -50 per extra assigned table

		d.tablesAssigned = 0;	
	}

	LOG.LogInfo("Fitness: " + std::to_string(push.fitness));
}

bool findGameKnowledge(const Table::Games& gameName, Dealer* dealerPtr)
{
	for (Table::Games g : dealerPtr->gameKnowledge)
	{
		if (gameName == g) return true;
	}
	return false;
}
