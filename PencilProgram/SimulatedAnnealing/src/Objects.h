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

	Dealer();

	Dealer(std::string nameIn);

	static void GenerateDealers(std::vector<Dealer>& dealersIn);
};



class Table
{
public:
	int number;
	std::string gameName;

	Table(int numIn, std::string nameIn)		// constructor
		: number(numIn), gameName(nameIn)
	{
	}

	Table(const Table& table)					// copy constructor
		:number(table.number), gameName(table.gameName)
	{
		std::cout << "Table object copied!" << std::endl;
	}

	static void GenerateTables(std::vector<Table>& tablesIn);
};

