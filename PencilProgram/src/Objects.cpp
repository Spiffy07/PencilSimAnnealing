#include "pch.h"
#include "Objects.h"
#include "LogExtern.h"

#if PEN_DEBUG
#else
#include <iostream>
#endif

const unsigned int STARTING_EMPLOYEE_NUMBER = 100000 - NUMBER_OF_DEALERS;

// static member declarations
unsigned int Dealer::employeeNumberCounter;

	// Table
Table::Table()
	: number(0), gameName(BJ)
{

}

Table::Table(int numIn, Table::Games gameName)
	:number(numIn), gameName(gameName)
{
}

Table::Table(const Table& table)
	:number(table.number), gameName(table.gameName)
{
#if PEN_DEBUG
	LOG.LogInfo("Table object copied!");
#endif
}

void Table::GenerateTables(std::array<Table, NUMBER_OF_TABLES>& tablesIn)
{
	PROFILE_FUNCTION();
	//tablesIn.reserve(NUMBER_OF_TABLES);

	tablesIn[0] = Table(101, BJ);
	tablesIn[1] = Table(102, BJ);
	tablesIn[2] = Table(103, BJ);
	tablesIn[3] = Table(104, BJ);
	tablesIn[4] = Table(105, BJ);
	tablesIn[5] = Table(106, Poker);
	tablesIn[6] = Table(107, Poker);
	tablesIn[7] = Table(108, Poker);
	tablesIn[8] = Table(109, MB);
	tablesIn[9] = Table(110, MB);
	tablesIn[10] = Table(111, MB);
	tablesIn[11] = Table(112, MB);
	tablesIn[12] = Table(113, Rou);
	tablesIn[13] = Table(114, Rou);
	tablesIn[14] = Table(115, BJ);
	tablesIn[15] = Table(116, BJ);
	tablesIn[16] = Table(117, BJ);
	tablesIn[17] = Table(118, BJ);
	tablesIn[18] = Table(119, Poker);
	tablesIn[19] = Table(120, Poker);
}

bool Table::operator==(Table& other)
{
	if (number != other.number)
		return false;

	if (gameName != other.gameName)
		return false;

	return true;
}

bool Table::operator!=(Table& other)
{
	if (number != other.number)
		return true;

	if (gameName != other.gameName)
		return true;

	return false;
}

	// Dealer
Dealer::Dealer()
	: pushMinutes(0), tablesAssigned(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;
	name = std::to_string(employeeNumber);

	gameKnowledge[Table::BJ]	= true;
	gameKnowledge[Table::Poker] = true;
	gameKnowledge[Table::Rou]	= false;
	gameKnowledge[Table::MB]	= false;
}

Dealer::Dealer(std::string nameIn)
	: name(nameIn), pushMinutes(0), tablesAssigned(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;

	gameKnowledge[Table::BJ] = true;
	gameKnowledge[Table::Poker] = true;
	gameKnowledge[Table::Rou] = false;
	gameKnowledge[Table::MB] = false;
}

Dealer::Dealer(std::string nameIn, std::array<bool, NUMBER_OF_GAMES> gameKnowledgeIn)
	: name(nameIn), gameKnowledge(gameKnowledgeIn), pushMinutes(0), tablesAssigned(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;

}

Dealer::Dealer(const Dealer& dealer)
	: name(dealer.name), employeeNumber(dealer.employeeNumber), 
	gameKnowledge(dealer.gameKnowledge), pushMinutes(dealer.pushMinutes),
	tablesAssigned(dealer.tablesAssigned)
{
#if PEN_DEBUG
	LOG.LogInfo("Dealer object copied!");
#endif
}

void Dealer::GenerateDealers(std::array<Dealer, NUMBER_OF_DEALERS>& dealersIn)
{
	PROFILE_FUNCTION();
	//dealersIn.reserve(NUMBER_OF_DEALERS);

	dealersIn[0] = Dealer("Thomas", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[1] = Dealer("Clarissa", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[2] = Dealer("Tiffany", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[3] = Dealer("Alex", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[4] = Dealer("Chris");
	dealersIn[5] = Dealer("John", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[6] = Dealer("Ryan", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[7] = Dealer("Anthony", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[8] = Dealer("Chad", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[9] = Dealer("Kim", std::array<bool, NUMBER_OF_GAMES>{true, true, true, true});
	dealersIn[10] = Dealer();
	dealersIn[11] = Dealer();
	dealersIn[12] = Dealer();
	dealersIn[13] = Dealer();
	dealersIn[14] = Dealer();
	dealersIn[15] = Dealer();
	dealersIn[16] = Dealer();
	dealersIn[17] = Dealer();
	dealersIn[18] = Dealer();
	dealersIn[19] = Dealer();
	dealersIn[20] = Dealer();
	dealersIn[21] = Dealer();
	dealersIn[22] = Dealer();
	dealersIn[23] = Dealer();
	dealersIn[24] = Dealer();
}

void Dealer::ChangePushMinutes()
{
	if (tablesAssigned > 0)
		pushMinutes += 20;
	else
		pushMinutes = 0;
}

	// Assignment
Assignment::Assignment()
	: aTable(Table::Table()), aDealerPtr(nullptr)
{
}

Assignment::Assignment(Table& t, Dealer& d) 
	: aTable(t), aDealerPtr(&d)
{
}

bool Assignment::operator==(Assignment& other)
{
	if (aDealerPtr != other.aDealerPtr)
		return false;

	if (aTable != other.aTable)
		return false;

	return true;
}

bool Assignment::operator!=(Assignment& other)
{
	if (aDealerPtr != other.aDealerPtr)
		return true;

	if (aTable != other.aTable)
		return true;

	return false;
}

int Assignment::GetTableNumber()
{
	return this->aTable.number;
}

char* Assignment::GetTableGameName()
{

	switch (this->aTable.gameName)
	{
	case Table::BJ:
		return "BJ";
	case Table::Rou:
		return "Rou";
	case Table::MB:
		return "MB";
	case Table::Poker:
		return "Poker";
	default:
#if PEN_DEBUG
		LOG.LogError("Error: Invalid message in final Log");
#endif
		return "Error!";
	}
}

std::string Assignment::GetDealerName()
{
	if (this->aDealerPtr->name.empty())
		return "Invalid Dealer name";
	else
		return this->aDealerPtr->name;
}


	// Push
bool Push::operator==(Push& other)
{
	if (fitness != other.fitness)
		return false;

	for (int i = 0; i < NUMBER_OF_TABLES; i++)
	{
		if (push[i] != other.push[i])
			return false;
	}

	return true;
}

bool Push::operator!=(Push& other)
{
	if (fitness != other.fitness)
		return true;

	for (int i = 0; i < NUMBER_OF_TABLES; i++)
	{
		if (push[i] != other.push[i])
			return true;
	}

	return false;
}

//Timer::Timer()
//{
//	start = std::chrono::high_resolution_clock::now();
//	duration = start - start;
//}
//
//Timer::~Timer()
//{
//
//}
//
//void Timer::endTimer()
//{
//	end = std::chrono::high_resolution_clock::now();
//	duration = end - start;
//	float s = duration.count() * 1000;
//
//#if PEN_DEBUG
//	LOG.LogError("Timer: " + std::to_string(s) + "ms");
//#else
//	std::cout << "Timer: " + std::to_string(s) + "ms\n";
//#endif
//}