#pragma once 

#include "pch.h"
//#include <string>
//#include <vector>
//#include <chrono>
//#define PEN_DEBUG 1

const int NUMBER_OF_TABLES = 20;
const int NUMBER_OF_DEALERS = 25;
const int NUMBER_OF_GAMES = 4;		// BJ, Rou, MB, Poker


class Table
{
public:
	enum Games
	{
		BJ, Poker, Rou, MB
	};
	int number;
	Games gameName;

	// constructors
	//Table() = delete;			// do not allow blank Table object
	Table();
	Table(int numIn, Games gameName);
	Table(const Table& table);

	bool operator==(Table& other);
	bool operator!=(Table& other);

	static void GenerateTables(std::array<Table, NUMBER_OF_TABLES>& tablesIn);
};


class Dealer
{
public:
	static unsigned int employeeNumberCounter;
	std::string name;
	int employeeNumber;
	std::array<bool, NUMBER_OF_GAMES> gameKnowledge;	// Determine list of games
	int pushMinutes;
	int tablesAssigned;

		// constructors
	Dealer();
	Dealer(std::string nameIn);
	Dealer(std::string nameIn, std::array<bool, NUMBER_OF_GAMES> gameKnowledgeIn);
	Dealer(const Dealer& dealer);
	Dealer(Dealer&&) = default;			// move constructor
	Dealer& operator=(Dealer&&) = default;

	static void GenerateDealers(std::array<Dealer, NUMBER_OF_DEALERS>& dealersIn);
};



class Assignment
{
public:
	Table aTable;
	Dealer* aDealerPtr;

	Assignment();
	Assignment(Table& t, Dealer& d);

	bool operator==(Assignment& other);
	bool operator!=(Assignment& other);

	int GetTableNumber();
	char* GetTableGameName();
	std::string GetDealerName();
};


class Push
{
public:
	std::array<Assignment, NUMBER_OF_TABLES> push;
	int fitness = 0;

	Push() = default;
	Push(Push&) = default;
	Push& operator=(Push&) = default;

	Push(Push&&) = default;				// move constructor
	Push& operator=(Push&&) = default;	// move assignment operator

	bool operator==(Push& other);
	bool operator!=(Push& other);

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