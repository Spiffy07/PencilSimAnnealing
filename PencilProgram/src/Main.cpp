#include <iostream>
#include <thread>
//#include <cmath>
#include "Objects.h"
#include "LogExtern.h"


#if PEN_DEBUG
const Log::LogLevel LOG_LEVEL = Log::Warning;		// set log level here
Log LOG;
#endif

// static member declarations
unsigned int Dealer::employeeNumberCounter;

static Push PopulateTables(std::vector<Table>& tables, std::vector<Dealer>& dealers);
static void CalculateFitness(Push& push, std::vector<Dealer>& dealers);
static bool FindGameKnowledge(const Table::Games& gameName, Dealer* dealerPtr);
static void SimulateAnnealing(std::vector<Table>& tablesIn, std::vector<Dealer>& dealersIn, Push& pushIn);
static void PrintPush(Push& pushIn);

static const int s_THREAD_COUNT = 8;
static const int s_SUCCESSFUL_CHANGE_LIMIT = 2000;	// limit of successful changes per tempurature iteration
static const int s_ATTEMPT_LIMIT = 50000;			// limit of attempts per tempurature iteration
static const double s_STARTING_TEMPURATURE = 5000;	// starting temp, higher increases randomization
static const double s_e = 2.718281828;
static int s_bestFitness = 0;						// highest found fitness
static bool s_calcFitnessLock = false;


int main()
{
	Timer t;
#if PEN_DEBUG
	LOG.SetLogLevel(LOG_LEVEL);
#endif

	std::vector<Table> tables;
	std::vector<Dealer> dealers;
	std::vector<Push> results;
	std::vector<std::thread> threads;
	Table::GenerateTables(tables);
	Dealer::GenerateDealers(dealers);

	srand(time(0));
	results.reserve(s_THREAD_COUNT);					
	threads.reserve(s_THREAD_COUNT);
	Push first = PopulateTables(tables, dealers);

	CalculateFitness(first, dealers);
	for (int i = 0; i < s_THREAD_COUNT; i++)
	{
		results.push_back(first);					// copy original Push to prevent scoping issue
		threads.emplace_back([&tables, &dealers, &results, i]() {SimulateAnnealing(tables, dealers, results[i]); });
	}

	for (auto& t : threads) t.join();

#if PEN_DEBUG
	for (auto& p : results)
	{
		PrintPush(p);
	}
	LOG.LogWarning("Highest Fitness: " + std::to_string(s_bestFitness));
#else
	for (auto& p : results)
		std::cout << std::to_string(p.fitness) + "\n";
	std::cout << "    Best fitness: " << std::to_string(bestFitness) + "\n";
#endif

	t.~Timer();
	std::cout << "End of Program\n";
	std::cin.get();
	return 0;
}


static Push PopulateTables(std::vector<Table>& tables, std::vector<Dealer>& dealers)
{
	Push p;
	p.push.reserve(NUMBER_OF_TABLES);
	for (Table& t : tables)
	{
		p.push.emplace_back(t, dealers[rand() % 10]);
	}   

	return p;
}

static void CalculateFitness(Push& push, std::vector<Dealer>& dealers)
{
	using namespace std::chrono_literals;
	//Timer timer;													// Runs everytime
	while (s_calcFitnessLock)
	{
		std::this_thread::sleep_for(5ms);
	}
	s_calcFitnessLock = true;

	push.fitness = 0;			// initialize and/or reset to zero
	for (Dealer& d : dealers) d.tablesAssigned = 0;		// reset assigned tables to zero ***********

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
				? push.fitness += 5 : push.fitness -= 25);
			break;
		case Table::MB:
			if (FindGameKnowledge(Table::MB, a.aDealerPtr)
				? push.fitness += 5 : push.fitness -= 25);
			break;
		case Table::Poker:
			if (FindGameKnowledge(Table::Poker, a.aDealerPtr)
				? push.fitness += 3 : push.fitness -= 25);
			break;
		default:
#if PEN_DEBUG
			LOG.LogError("Error: Invalid gameKnowledge fitness result!");
#endif
			break;
		}
	} 

	for (Dealer& d : dealers)
	{
		if (d.tablesAssigned > 1)
			push.fitness -= (d.tablesAssigned - 1) * 50;	// -50 per extra assigned table
	}
	s_calcFitnessLock = false;

#if PEN_DEBUG
	LOG.LogInfo("Fitness: " + std::to_string(push.fitness));		// runs literally every iteration
#endif
}

static bool FindGameKnowledge(const Table::Games& gameName, Dealer* dealerPtr)
{
	for (Table::Games g : dealerPtr->gameKnowledge)
	{
		if (gameName == g) return true;
	}
	return false;
}

static void SimulateAnnealing(std::vector<Table>& tablesIn, std::vector<Dealer>& dealersIn, Push& pushIn)
{
	int changeCount;
	int attemptCount;
	int attemptNoChange;
	double prob;			// random number between 0 and 1
	double eCalc;			// ? Boltzman's distribution?
	double tempurature = s_STARTING_TEMPURATURE;

	do
	{
		tempurature = tempurature * .95;
		changeCount = 0;
		attemptCount = 0;
		attemptNoChange = 0;

		while (changeCount < s_SUCCESSFUL_CHANGE_LIMIT
			&& attemptCount < s_ATTEMPT_LIMIT)
		{
			int randTable = rand() % tablesIn.size();			// select which table to change
			int newDealer = rand() % dealersIn.size();			// select a random dealer
			int oldFitness = pushIn.fitness;					// fitness before change
			Dealer* oldDealer = pushIn.push[randTable].aDealerPtr;

			if (pushIn.fitness > s_bestFitness) s_bestFitness = pushIn.fitness;
			pushIn.push[randTable].aDealerPtr = &dealersIn[newDealer];		// the change to be appraised

			CalculateFitness(pushIn, dealersIn);		// 'pushIn' object has post-change fitness

			prob = ((double)rand() / (RAND_MAX));		// random number between 0 and 1
			eCalc = pow(s_e, ((pushIn.fitness - oldFitness) / tempurature));

			if (prob < eCalc && pushIn.fitness != oldFitness)		// change accepted
				changeCount++;
			else
			{
				pushIn.fitness = oldFitness;						// reset fitness
				pushIn.push[randTable].aDealerPtr = oldDealer;		// reset dealer
				attemptNoChange++;
			}
			attemptCount++;
		}
#if PEN_DEBUG
		if (LOG_LEVEL > Log::Warning)
			std::cout << std::this_thread::get_id() << ": ";
		LOG.LogWarning(std::to_string(pushIn.fitness));
#endif
	} while (attemptNoChange < s_ATTEMPT_LIMIT);
}

static void PrintPush(Push& p)
{
#if PEN_DEBUG
	LOG.LogError("final Fitness: " + std::to_string(p.fitness));
	//LOG.LogWarning("Highest Fitness: " + std::to_string(bestFitness));

	for (auto& p : p.push)
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