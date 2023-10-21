#pragma once

#include "pch.h"

#include "Objects.h"

namespace PencilSim
{
	static Push PopulateTables(std::array<Table, NUMBER_OF_TABLES>& tables, std::array<Dealer, NUMBER_OF_DEALERS>& dealers);
	//static void PopulateTables(std::array<Table, NUMBER_OF_TABLES>& tables, std::array<Dealer, NUMBER_OF_DEALERS>& dealers, Push& pIn);
	static void CalculateFitness(Push& push, std::array<Dealer, NUMBER_OF_DEALERS>& dealers);
	static bool FindGameKnowledge(const Table::Games& gameName, Dealer* dealer);
	static void SimulateAnnealing(std::array<Table, NUMBER_OF_TABLES>& tablesIn, std::array<Dealer, NUMBER_OF_DEALERS>& dealersIn, Push& pushIn);
	static void PrintPush(Push& pushIn);

	Push PenMain(std::array<Dealer, NUMBER_OF_DEALERS>& dealers);
	//void PenMain(Push&);

	static const int s_THREAD_COUNT = 8;
	static const int s_SUCCESSFUL_CHANGE_LIMIT = 2000;	// limit of successful changes per tempurature iteration
	static const int s_ATTEMPT_LIMIT = 50000;			// limit of attempts per tempurature iteration
	static const double s_STARTING_TEMPURATURE = 5000;	// starting temp, higher increases randomization
	static const double s_e = 2.718281828;
	const int TARGET_FITNESS_MIN = 72;				// TODO: subject to change with development
	static int s_bestFitness = 0;						// highest found fitness
	static bool s_calcFitnessLock = false;
	static std::mutex s_DealersMutex;


}