// $(SolutionDir)bin\intermediates\$(Platform)\$(Configuration)
#include "PenMain.h"

#define SINGLE_THREAD

#if PEN_DEBUG
const Log::LogLevel LOG_LEVEL = Log::Warning;		// set log level here
Log LOG;
#else
#define RELEASE_CONSOLE_OUTPUT 1
#endif

#if RELEASE_CONSOLE_OUTPUT
const Log::LogLevel LOG_LEVEL = Log::Error;		// set log level here
Log LOG;
#endif

static bool GeneratedDealers = false;			// TODO: find a better way to generate only once

//void* operator new(size_t size)
//{
//	std::cout << "------------ Allocating " << size << " bytes -------------\n";
//
//	return malloc(size);
//}

namespace PencilSim 
{


	Push PenMain(std::array<Dealer, NUMBER_OF_DEALERS>& dealers)
	//void PenMain(Push& p)
	{
#if PROFILING	// turn on/off profiling in pch.h
		Instrumentor::Get().BeginSession("Profile");
		InstrumentationTimer timer("Main");
#endif

#if PEN_DEBUG
		LOG.SetLogLevel(LOG_LEVEL);
#endif

		std::array<Table, NUMBER_OF_TABLES> tables;
		//std::array<Dealer, NUMBER_OF_DEALERS> dealers;
		std::array<Push, s_THREAD_COUNT> results;
		std::array<std::future<void>, s_THREAD_COUNT> futures;

		Table::GenerateTables(tables);
		if (!GeneratedDealers)
		{
			Dealer::GenerateDealers(dealers);
			GeneratedDealers = true;
		}

		srand(time(0));

		Push first = PopulateTables(tables, dealers);

		CalculateFitness(first, dealers);

#ifdef SINGLE_THREAD
		SimulateAnnealing(tables, dealers, first);

		CalculateFitness(first, dealers);

		for (Dealer& d : dealers)
			d.ChangePushMinutes();

	#if PEN_DEBUG
		PrintPush(first);
	#endif
	#if RELEASE_CONSOLE_OUTPUT
		PrintPush(first);
	#endif
#else
		for (int i = 0; i < s_THREAD_COUNT; i++)
		{
			results[i] =first;					// copy original Push to prevent scoping issue
		}
		for (int i = 0; i < s_THREAD_COUNT - 1; i++)
		{
			std::cout << "\n" << "thread: " << i << "\n";
			futures[i] = std::async(std::launch::async, 
				SimulateAnnealing, tables, dealers, std::ref(results[i]));
		}
		futures[s_THREAD_COUNT - 1] = std::async(std::launch::async, 
			SimulateAnnealing, tables, dealers, std::ref(results[s_THREAD_COUNT - 1]));

		for (auto& f : futures)
			f.wait();

		for (auto& p : results)
		{
			PrintPush(p);
		}
#endif

#if PEN_DEBUG
		LOG.LogError("Final Fitness: " + std::to_string(first.fitness));
#else
		std::cout << " Final fitness: " << std::to_string(first.fitness) + "\n";
#endif
	
#if PROFILING
		timer.Stop();
		Instrumentor::Get().EndSession();
#endif

		// dealer name going out of scope

		std::cout << "End of Simulated Annealing\n";
		return first;
	}


	static Push PopulateTables(std::array<Table, NUMBER_OF_TABLES>& tables, std::array<Dealer, NUMBER_OF_DEALERS>& dealers)
	{
		PROFILE_FUNCTION();
		Push pIn;
		for (int i = 0; i < NUMBER_OF_TABLES; i++)
		{
			pIn.push[i] = { tables[i], dealers[rand() % NUMBER_OF_DEALERS] };
		}   

		return pIn;
	}


	static void CalculateFitness(Push& push, std::array<Dealer, NUMBER_OF_DEALERS>& dealers)
	{
		//PROFILE_FUNCTION();	// severely slows down due to calling every iteration

				/* due to std::async passing by value,
					the std::vector<Dealer>& needed for the function call was
					actually passed by value (copied) 
					meaning no lock or mutex is needed*/
		//std::lock_guard<std::mutex> lock(s_DealersMutex);	
	
		//using namespace std::chrono_literals;
		//Timer timer;													// Runs everytime
		//while (s_calcFitnessLock)
		//{
		//	std::this_thread::sleep_for(5ms);
		//}
		//s_calcFitnessLock = true;

		push.fitness = 0;			// initialize and/or reset to zero
		for (Dealer& d : dealers) 
			d.tablesAssigned = 0;		// reset assigned tables to zero ***********

		for (Assignment& a : push.push)
		{
			/* next step is to include logic to target dealers with highest pushMinutes first.
				- subtable and calculate first?
				- calculate number of each duration of pushMinutes (20, 40, 60, 80) and include in fitness calculation?
			*/


			// count number of assigned tables MAX = 1
			a.aDealerPtr->tablesAssigned += 1;	
			//LOG.LogInfo(a.aDealerPtr->name + ": " + std::to_string(a.aDealerPtr->tablesAssigned));
		
			// Game Knowledge check
			switch (a.aTable.gameName){
			case Table::BJ:
				if (FindGameKnowledge(Table::BJ, a.aDealerPtr)
					? push.fitness += 3 : push.fitness -= 35);
				break;
			case Table::Rou:
				if (FindGameKnowledge(Table::Rou, a.aDealerPtr)
					? push.fitness += 5 : push.fitness -= 35);
				break;
			case Table::MB:
				if (FindGameKnowledge(Table::MB, a.aDealerPtr)
					? push.fitness += 5 : push.fitness -= 35);
				break;
			case Table::Poker:
				if (FindGameKnowledge(Table::Poker, a.aDealerPtr)
					? push.fitness += 3 : push.fitness -= 35);
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
				push.fitness -= (d.tablesAssigned - 1) * 75;	// -75 per extra assigned table

			if (d.pushMinutes == 80 ? push.fitness -= 10 : push.fitness += 1);	// -10 for leaving someone in too long

			//switch (d.pushMinutes)
			//{
			//case 80:
			//	push.fitness -= 3;
			//	break;
			//case 60:
			//	push.fitness -= 2;
			//	break;
			//default:
			//	break;
			//}
		}
		//s_calcFitnessLock = false;

#if PEN_DEBUG
		//LOG.LogInfo("Fitness: " + std::to_string(push.fitness));		// runs literally every iteration
#endif
	}


	static bool FindGameKnowledge(const Table::Games& gameName, Dealer* dealer)
	{
		return dealer->gameKnowledge[gameName];

		//for (bool g : dealer->gameKnowledge)
		//{
		//	if (gameName == g) return true;
		//}
		//return false;
	}


	static void SimulateAnnealing(std::array<Table, NUMBER_OF_TABLES>& tablesIn, 
		std::array<Dealer, NUMBER_OF_DEALERS>& dealersIn, Push& pushIn)
	{
		PROFILE_FUNCTION();

		int changeCount;
		int attemptCount;
		int attemptNoChange;
		double prob;			// random number between 0 and 1
		double eCalc;			// ? Boltzman's distribution?
		double tempurature = s_STARTING_TEMPURATURE;

		do
		{
			tempurature = tempurature * .99;		// TODO: refactor to static const variable
			changeCount = 0;
			attemptCount = 0;
			attemptNoChange = 0;

			while (changeCount < s_SUCCESSFUL_CHANGE_LIMIT
				&& attemptCount < s_ATTEMPT_LIMIT)
			{
				int randTable = rand() % tablesIn.size();			// select which table to change
				int newDealerInt = rand() % dealersIn.size();			// select a random dealer
				int oldFitness = pushIn.fitness;					// fitness before change
				Dealer* oldDealer = pushIn.push[randTable].aDealerPtr;

				if (pushIn.fitness > s_bestFitness) 
					s_bestFitness = pushIn.fitness;

				Dealer* newDealer = &dealersIn[newDealerInt];
				int oldPushMinutes = newDealer->pushMinutes;
				pushIn.push[randTable].aDealerPtr = newDealer;		// the change to be appraised

				CalculateFitness(pushIn, dealersIn);		// 'pushIn' object has post-change fitness

				prob = ((double)rand() / (RAND_MAX));		// random number between 0 and 1
				eCalc = pow(s_e, ((pushIn.fitness - oldFitness) / tempurature));

				if (prob < eCalc && pushIn.fitness != oldFitness)		// change accepted
					changeCount++;
				else
				{
					pushIn.fitness = oldFitness;						// reset fitness
					newDealer->pushMinutes = oldPushMinutes;			// reset pushMinutes
					pushIn.push[randTable].aDealerPtr = oldDealer;		// reset dealer
					attemptNoChange++;
				}
				attemptCount++;
			}
#if PEN_DEBUG
			if (LOG_LEVEL > Log::Warning)
				std::cout << std::this_thread::get_id() << ": ";
			LOG.LogInfo(std::to_string(pushIn.fitness));
#endif
		} while (attemptNoChange < s_ATTEMPT_LIMIT);
	}


	static void PrintPush(Push& p)
	{
		PROFILE_FUNCTION();
#if PEN_DEBUG
#elif RELEASE_CONSOLE_OUTPUT
		LOG.SetLogLevel(LOG_LEVEL);
#endif

		LOG.LogWarning("final Fitness: " + std::to_string(p.fitness));

		for (auto& p : p.push)
		{
			switch (p.aTable.gameName)
			{
			case Table::BJ:
				LOG.LogError(std::to_string(p.aTable.number) + "     BJ     " + "pushMinutes: " 
					+ std::to_string(p.aDealerPtr->pushMinutes) + "    " + p.aDealerPtr->name);
				break;																	 
			case Table::Rou:															 
				LOG.LogError(std::to_string(p.aTable.number) + "     Rou    " + "pushMinutes: "
					+ std::to_string(p.aDealerPtr->pushMinutes) + "    " + p.aDealerPtr->name);
				break;																	 
			case Table::MB:																 
				LOG.LogError(std::to_string(p.aTable.number) + "     MB     " + "pushMinutes: "
					+ std::to_string(p.aDealerPtr->pushMinutes) + "    " + p.aDealerPtr->name);
				break;																	 
			case Table::Poker:															 
				LOG.LogError(std::to_string(p.aTable.number) + "     Poker  " + "pushMinutes: "
					+ std::to_string(p.aDealerPtr->pushMinutes) + "    " + p.aDealerPtr->name);
				break;
			default:
				LOG.LogError("Error: Invalid message in final Log");
			}
		}
	}


}