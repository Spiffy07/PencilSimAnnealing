#include "Objects.h"

const unsigned int STARTING_EMPLOYEE_NUMBER = 100000;

	// constructors
Dealer::Dealer()
	: pushMinutes(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;
	name = std::to_string(employeeNumber);

	gameKnowledge.reserve(NUMBER_OF_GAMES);
	gameKnowledge.emplace_back(true);
	gameKnowledge.emplace_back(false);
	gameKnowledge.emplace_back(false);
	gameKnowledge.emplace_back(false);
}
Dealer::Dealer(std::string nameIn)
	: name(nameIn), pushMinutes(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;

	gameKnowledge.reserve(NUMBER_OF_GAMES);
	gameKnowledge.emplace_back(true);
	gameKnowledge.emplace_back(false);
	gameKnowledge.emplace_back(false);
	gameKnowledge.emplace_back(false);
}
Dealer::Dealer(const Dealer& dealer)
	: name(dealer.name), employeeNumber(dealer.employeeNumber), 
	gameKnowledge(dealer.gameKnowledge), pushMinutes(dealer.pushMinutes)
{
	std::cout << "Dealer object copied!" << std::endl;
}

void Dealer::GenerateDealers(std::vector<Dealer>& dealersIn)
{
	dealersIn.reserve(NUMBER_OF_DEALERS);

	dealersIn.emplace_back("Thomas");
	dealersIn.emplace_back("Clarissa");
	dealersIn.emplace_back("Tiffany");
	dealersIn.emplace_back("Chris");
	dealersIn.emplace_back("alex");
	dealersIn.emplace_back();
	dealersIn.emplace_back();
	dealersIn.emplace_back();
	dealersIn.emplace_back();
	dealersIn.emplace_back();
}

	// constructors
Table::Table(int numIn, std::string nameIn)
	:number(numIn), gameName(nameIn)
{
}
Table::Table(const Table& table)
	:number(table.number), gameName(table.gameName)
{
	std::cout << "   Table object copied!" << std::endl;
}

void Table::GenerateTables(std::vector<Table>& tablesIn)
{
	tablesIn.reserve(NUMBER_OF_TABLES);

	tablesIn.emplace_back(101, "BJ");
	tablesIn.emplace_back(102, "BJ");
	tablesIn.emplace_back(103, "BJ");
	tablesIn.emplace_back(104, "Poker");
	tablesIn.emplace_back(105, "Poker");
	tablesIn.emplace_back(106, "Poker");
	tablesIn.emplace_back(107, "BJ");
	tablesIn.emplace_back(108, "BJ");
	tablesIn.emplace_back(109, "MB");
	tablesIn.emplace_back(110, "Rou");
}
