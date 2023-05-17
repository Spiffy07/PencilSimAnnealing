#include <iostream>
//#include <cmath>
#include "Objects.h"
#include "LogExtern.h"


#if PEN_DEBUG
const Log::LogLevel LOG_LEVEL = Log::Warning;		// set log level here
Log LOG;
#endif

// static member declarations
unsigned int Dealer::employeeNumberCounter;

Push PopulateTables(std::vector<Table>& tables, std::vector<Dealer>& dealers);
void CalculateFitness(Push& push, std::vector<Dealer>& dealers);
bool FindGameKnowledge(const Table::Games& gameName, Dealer* dealerPtr);
void SimulateAnnealing(std::vector<Table>& tablesIn, std::vector<Dealer>& dealersIn, Push& pushIn);

static const int SUCCESSFUL_CHANGE_LIMIT = 1000;	// limit of successful changes per tempurature iteration
static const int ATTEMPT_LIMIT = 15000;			// limit of attempts per tempurature iteration
static const double e = 2.718281828;
static double tempurature = 2000;					// starting temp, higher increases randomization
static int changeCount;
static int attemptCount;
static int attemptNoChange;
static double prob;			// random number between 0 and 1
static double eCalc;			// ? Boltzman's distribution?
static int bestFitness = 0;		// highest found fitness

int main()
{
#if PEN_DEBUG
	LOG.SetLogLevel(LOG_LEVEL);
#endif

	std::vector<Table> tables;
	std::vector<Dealer> dealers;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	srand(time(0));
	Push first = PopulateTables(tables, dealers);
	CalculateFitness(first, dealers);

	SimulateAnnealing(tables, dealers, first);

	std::cout << "End of Program! Final Fitness: " << first.fitness << std::endl;
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
			if (FindGameKnowledge(Table::BJ, a.aDealerPtr) 
				? push.fitness += 3 : push.fitness -= 25);
			break;
		case Table::Rou:
			if (FindGameKnowledge(Table::Rou, a.aDealerPtr)
				? push.fitness += 3 : push.fitness -= 25);
			break;
		case Table::MB:
			if (FindGameKnowledge(Table::MB, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 25);
			break;
		case Table::Poker:
			if (FindGameKnowledge(Table::Poker, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 25);
			break;
		default:
#if PEN_DEBUG
			LOG.LogError("Error: Invalid gameKnowledge fitness result!");		// runs literally every iteration
#endif
			break;
		}
	} 

	for (Dealer& d : dealers)
	{
		if (d.tablesAssigned > 1)
			push.fitness -= (d.tablesAssigned - 1) * 50;	// -50 per extra assigned table
	}

#if PEN_DEBUG
	LOG.LogInfo("Fitness: " + std::to_string(push.fitness));
#endif
}

bool FindGameKnowledge(const Table::Games& gameName, Dealer* dealerPtr)
{
	for (Table::Games g : dealerPtr->gameKnowledge)
	{
		if (gameName == g) return true;
	}
	return false;
}

void SimulateAnnealing(std::vector<Table>& tablesIn, std::vector<Dealer>& dealersIn, Push& pushIn)
{
	do
	{
		tempurature = tempurature * .95;
		attemptNoChange = 0;
		changeCount = 0;
		attemptCount = 0;

		while (changeCount < SUCCESSFUL_CHANGE_LIMIT
			&& attemptCount < ATTEMPT_LIMIT)
		{
			int randTable = rand() % tablesIn.size();		// select which table to change
			int newDealer = rand() % dealersIn.size();	// select a random dealer
			int oldFitness = pushIn.fitness;				// fitness before change
			Dealer* oldDealer = pushIn.push[randTable].aDealerPtr;

			if (pushIn.fitness > bestFitness) bestFitness = pushIn.fitness;
			pushIn.push[randTable].aDealerPtr = &dealersIn[newDealer];		// the change to be appraised

			CalculateFitness(pushIn, dealersIn);		// 'pushIn' object has post-change fitness

			prob = ((double)rand() / (RAND_MAX));		// random number between 0 and 1
			eCalc = pow(e, ((pushIn.fitness - oldFitness) / tempurature));

			if (prob < eCalc && pushIn.fitness != oldFitness)	// change accepted
				changeCount++;
			else
			{
				pushIn.fitness = oldFitness;						// reset fitness
				pushIn.push[randTable].aDealerPtr = oldDealer;	// reset dealer
				attemptNoChange++;
			}
			attemptCount++;
		}
#if PEN_DEBUG
		LOG.LogWarning(std::to_string(pushIn.fitness));
#endif
	} while (attemptNoChange != ATTEMPT_LIMIT);

#if PEN_DEBUG
	LOG.LogError("final Fitness: " + std::to_string(pushIn.fitness));
	LOG.LogWarning("Highest Fitness: " + std::to_string(bestFitness));

	for (auto& p : pushIn.push)
	{
		switch (p.aTable.gameName)
		{
		case Table::BJ:
			LOG.LogWarning(std::to_string(p.aTable.number) + "     BJ     " + p.aDealerPtr->name);
			break;
		case Table::Rou:
			LOG.LogWarning(std::to_string(p.aTable.number) + "     Rou    " + p.aDealerPtr->name);
			break;
		case Table::MB:
			LOG.LogWarning(std::to_string(p.aTable.number) + "     MB     " + p.aDealerPtr->name);
			break;
		case Table::Poker:
			LOG.LogWarning(std::to_string(p.aTable.number) + "     Poker  " + p.aDealerPtr->name);
			break;
		default:
			LOG.LogError("Error: Invalid message in final Log");
		}
	}
#endif
}