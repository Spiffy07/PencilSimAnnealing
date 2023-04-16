#include "Objects.h"

const unsigned int STARTING_EMPLOYEE_NUMBER = 100000;


// constructors
Table::Table(int numIn, Table::Games gameName)
	:number(numIn), gameName(gameName)
{
}

Table::Table(const Table& table)
	:number(table.number), gameName(table.gameName)
{
	std::cout << "    Table object copied!" << std::endl;
}

void Table::GenerateTables(std::vector<Table>& tablesIn)
{
	tablesIn.reserve(NUMBER_OF_TABLES);

	tablesIn.emplace_back(101, BJ);
	tablesIn.emplace_back(102, BJ);
	tablesIn.emplace_back(103, BJ);
	tablesIn.emplace_back(104, BJ);
	tablesIn.emplace_back(105, BJ);
	tablesIn.emplace_back(106, Poker);
	tablesIn.emplace_back(107, Poker);
	tablesIn.emplace_back(108, Poker);
	tablesIn.emplace_back(109, MB);
	tablesIn.emplace_back(110, Rou);
}


	// constructors
Dealer::Dealer()
	: pushMinutes(0), tablesAssigned(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;
	name = std::to_string(employeeNumber);

	gameKnowledge.reserve(NUMBER_OF_GAMES);
	gameKnowledge.emplace_back(Table::BJ);
	gameKnowledge.emplace_back(Table::Poker);
}

Dealer::Dealer(std::string nameIn)
	: name(nameIn), pushMinutes(0), tablesAssigned(0)
{
	employeeNumberCounter++;
	employeeNumber = STARTING_EMPLOYEE_NUMBER + employeeNumberCounter;

	gameKnowledge.reserve(NUMBER_OF_GAMES);
	gameKnowledge.emplace_back(Table::BJ);
	gameKnowledge.emplace_back(Table::Poker);
}

Dealer::Dealer(std::string nameIn, std::vector<Table::Games> gameKnowledgeIn)
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
	std::cout << "    Dealer object copied!" << std::endl;
}

void Dealer::GenerateDealers(std::vector<Dealer>& dealersIn)
{
	dealersIn.reserve(NUMBER_OF_DEALERS);

	dealersIn.emplace_back("Thomas", std::vector<Table::Games>{Table::BJ, Table::Rou, Table::MB, Table::Poker});
	dealersIn.emplace_back("Clarissa", std::vector<Table::Games>{Table::BJ, Table::Rou, Table::MB, Table::Poker});
	dealersIn.emplace_back("Tiffany", std::vector<Table::Games>{Table::BJ, Table::Rou, Table::MB, Table::Poker});
	dealersIn.emplace_back("Chris", std::vector<Table::Games>{Table::BJ, Table::Poker});
	dealersIn.emplace_back("Alex", std::vector<Table::Games>{Table::BJ, Table::Rou, Table::MB, Table::Poker});
	dealersIn.emplace_back();
	dealersIn.emplace_back();
	dealersIn.emplace_back();
	dealersIn.emplace_back();
	dealersIn.emplace_back();
}

Assignment::Assignment(Table& t, Dealer& d) 
	: aTable(t), aDealer(d)
{
}

