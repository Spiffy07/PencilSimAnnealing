#pragma once
#include <string>
#include <vector>
#include <iostream>

const int NUMBER_OF_TABLES = 10;
const int NUMBER_OF_DEALERS = 10;
const int NUMBER_OF_GAMES = 4;		// BJ, Rou, MB, Poker


class Dealer
{
public:
	static unsigned int employeeNumberCounter;
	std::string name;
	int employeeNumber;
	std::vector<bool> gameKnowledge;				// Determine list of games and set vector.resize
	int pushMinutes;
	int tablesAssigned;

		// constructors
	Dealer();
	Dealer(std::string nameIn);
	Dealer(std::string nameIn, std::vector<bool> gameKnowledgeIn);
	Dealer(const Dealer& dealer);

	static void GenerateDealers(std::vector<Dealer>& dealersIn);
};



class Table
{
public:
	int number;
	std::string gameName;

		// constructors
	Table() = delete;			// do not allow blank Table object
	Table(int numIn, std::string nameIn);
	Table(const Table& table);

	static void GenerateTables(std::vector<Table>& tablesIn);
};


class Assignment
{
public:
	Table aTable;
	Dealer aDealer;

	Assignment() = delete;
	Assignment(Table& t, Dealer& d);
};


class Push
{
public:
	std::vector<Assignment> push;
	int fitness;

	//Push() = delete;
};