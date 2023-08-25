#pragma once 

#include "pch.h"
//#include <string>
//#include <vector>
//#include <chrono>
//#define PEN_DEBUG 1

const int NUMBER_OF_TABLES = 20;
const int NUMBER_OF_DEALERS = 20;
const int NUMBER_OF_GAMES = 4;		// BJ, Rou, MB, Poker


class Table
{
public:
	enum Games
	{
		BJ, Rou, MB, Poker
	};
	int number;
	Games gameName;

	// constructors
	//Table() = delete;			// do not allow blank Table object
	Table();
	Table(int numIn, Games gameName);
	Table(const Table& table);

	static void GenerateTables(std::array<Table, NUMBER_OF_TABLES>& tablesIn);
};


class Dealer
{
public:
	static unsigned int employeeNumberCounter;
	std::string name;
	int employeeNumber;
	std::array<Table::Games, NUMBER_OF_GAMES> gameKnowledge;				// Determine list of games and set vector.resize
	int pushMinutes;
	int tablesAssigned;

		// constructors
	Dealer();
	Dealer(std::string nameIn);
	Dealer(std::string nameIn, std::array<Table::Games, NUMBER_OF_GAMES> gameKnowledgeIn);
	Dealer(const Dealer& dealer);

	static void GenerateDealers(std::array<Dealer, NUMBER_OF_DEALERS>& dealersIn);
};



class Assignment
{
public:
	Table aTable;
	Dealer* aDealerPtr;

	Assignment();
	Assignment(Table& t, Dealer& d);
	Assignment(Table& t, Dealer* dPtr);
};


class Push
{
public:
	std::array<Assignment, NUMBER_OF_TABLES> push;
	int fitness = 0;

	//Push() = delete;
};



//struct Timer
//{
//	std::chrono::time_point<std::chrono::steady_clock> start, end;
//	std::chrono::duration<float> duration;
//
//	Timer();
//	~Timer();
//
//	void endTimer();
//};