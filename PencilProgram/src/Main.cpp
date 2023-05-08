#include <iostream>
//#include <cmath>
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

	const int SUCCESSFUL_CHANGE_LIMIT = 400;	// limit of successful changes per tempurature iteration
	const int ATTEMPT_LIMIT = 10000;			// limit of attempts per tempurature iteration
	const double e = 2.718281828;
	double tempurature = 2000;					// starting temp, higher increases randomization
	int changeCount;
	int attemptCount;
	int attemptNoChange;	
	double prob;			// random number between 0 and 1
	double eCalc;			// ? Boltzman's distribution?
	int bestFitness;		// highest found fitness

	std::vector<Table> tables;
	std::vector<Dealer> dealers;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	srand(time(0));
	Push first = PopulateTables(tables, dealers);
	CalculateFitness(first, dealers);

	do
	{
		tempurature = tempurature * .95;
		attemptNoChange = 0;
		changeCount = 0;
		attemptCount = 0;

		while (changeCount < SUCCESSFUL_CHANGE_LIMIT
			&& attemptCount < ATTEMPT_LIMIT)
		{			
			int randTable = rand() % tables.size();		// select which table to change
			int newDealer = rand() % dealers.size();	// select a random dealer
			int oldFitness = first.fitness;				// fitness before change
			Dealer* oldDealer = first.push[randTable].aDealerPtr;

			first.push[randTable].aDealerPtr = &dealers[newDealer];		// the change to be appraised

			CalculateFitness(first, dealers);		// 'first' object has post-change fitness

			prob = ((double)rand() / (RAND_MAX));		// random number between 0 and 1
			eCalc = pow(e, ((first.fitness - oldFitness) / tempurature));

			if (prob < eCalc && first.fitness != oldFitness)	// change accepted
				changeCount++;
			else
			{
				first.fitness = oldFitness;						// reset fitness
				first.push[randTable].aDealerPtr = oldDealer;	// reset dealer
				attemptNoChange++;
			}
			attemptCount++;
		}
	} while (attemptNoChange != ATTEMPT_LIMIT);

	LOG.LogError(std::to_string(first.fitness));


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
	for (Dealer& d : dealers) d.tablesAssigned = 0;		// reset assigned tables to zero

	for (Assignment& a : push.push)
	{
		// count number of assigned tables MAX = 1
		a.aDealerPtr->tablesAssigned += 1;	
		//LOG.LogInfo(a.aDealerPtr->name + ": " + std::to_string(a.aDealerPtr->tablesAssigned));
		
		// Game Knowledge check
		switch (a.aTable.gameName){
		case Table::BJ:
			if (findGameKnowledge(Table::BJ, a.aDealerPtr) 
				? push.fitness += 3 : push.fitness -= 15);
			break;
		case Table::Rou:
			if (findGameKnowledge(Table::Rou, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 15);
			break;
		case Table::MB:
			if (findGameKnowledge(Table::MB, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 15);
			break;
		case Table::Poker:
			if (findGameKnowledge(Table::Poker, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 15);
			break;
		default:
			LOG.LogError("Error: Invalid gameKnowledge fitness result!");
		}
	} 

	for (Dealer& d : dealers)
	{
		if (d.tablesAssigned > 1)
			push.fitness -= (d.tablesAssigned - 1) * 50;	// -50 per extra assigned table
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
