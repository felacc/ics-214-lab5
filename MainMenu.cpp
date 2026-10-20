// See comments in MainMenu.h for general details on what each functin does.
#include <iostream>
#include "MainMenu.h"
#include "MemoryDebug.h"


// clear any characters from the (keyboard) input buffer
void ignoreLine() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// read and return an int from the console.  Keep trying if invalid until an int is returned.
int getIntFromUser() {
	while (true) {
		int guess;
		std::cin >> guess;

		if (std::cin.fail()) {
			std::cin.clear();
			ignoreLine();
			std::cerr << "Invalid input. Input must be a valid int.  Please try again.\n";
		}
		else
		{
			ignoreLine();
			return guess;
		}
	}
}

// output the menu choices to the console.
void displayMainMenu()
{
	std::cout << "\n";
	std::cout << "--- MENU ---\n";
	std::cout << "1) View Employees\n";
	std::cout << "2) Add Employee\n";
	std::cout << "3) Remove Employee\n";
	std::cout << "4) Remove All Employees\n";
	std::cout << "0) Exit\n";
	std::cout << "-------------\n";
	std::cout << "Select:";
}

// Attempt to read an int from the keyboard.  
// If there was a failure, clear the buffer and return -1.
Command getMenuCommand() {
	// read user input
	int input{ -1 };  // default 
	std::cin >> input;
	if (std::cin.fail()) {
		std::cin.clear();	// clear the failure
		input = -1;			// indicate an error
	}
	ignoreLine();	// ignore any extra input

	Command command = Command::invalid;	// default
	// match input with specific commands
	switch (input) {
	case 1: command = Command::viewEmployees; break;
	case 2: command = Command::addEmployee; break;
	case 3: command = Command::removeEmployee; break;
	case 4: command = Command::removeAllEmployees; break;
	case 0: command = Command::exit; break;
	}
	return command;
}

// This function is called when a user picks a selection from the menu.
// It determines what action to take depending on the menuItemSelected, 
// and calls the appropriate function.
// Returns true the selection was a request to exit menu, false otherwise.
void handleMenuInput(Employee*& pHead, Command command)
{
	switch (command)
	{
	case Command::viewEmployees:
		std::cout << ">> View Employees:\n";
		viewEmployees(pHead);
		break;
	case Command::addEmployee: {
		std::cout << ">> Add Employee:\n";
		std::cout << "Enter name:";
		std::string name;
		std::getline(std::cin, name);
		addNewEmployee(pHead, name);
	}
							 break;
	case Command::removeEmployee:
		std::cout << ">> Remove Employee:\n";
		std::cout << "Enter id:";
		removeEmployee(pHead, getIntFromUser());
		break;
	case Command::removeAllEmployees:
		std::cout << ">> Removing all employees...";
		removeAllEmployees(pHead);
		break;
	case Command::exit:
		std::cout << "Exiting\n";
		break;
	default:
		std::cout << "Invalid input.\n";
		break;
	}
}

void viewEmployees(const Employee* const pHead)
{
	if (pHead == nullptr)
	{
		std::cout << "empty list.\n";
	}
	else
	{
		const Employee* current = pHead;
		while (current != nullptr)
		{
			std::cout << "[id: " << current->id << ", name: " << current->name << "]\n";
			current = current->pNext;
		}
	}

}

Employee* createEmployee(const std::string& name)
{
	static int eid{0}; // create employee eid static var - this line only runs once
	Employee* emp = DBG_NEW Employee{ eid, name, nullptr }; // note - new keyword tells the computer to create the obj on the heap not the stack
	eid++; // increment eid so it is unique for each employee
	return emp;
}

void addNewEmployee(Employee*& pHead, const std::string& name, bool alphabetical)
{
	Employee* pEmp = createEmployee(name);
	
	// If not alphabetical insert at front of list
	if (!alphabetical)
	{
		pEmp->pNext = pHead;
		pHead = pEmp;
	}
	else
	{
		// find nodes to place new employee between
		NodeInfo parentAndChild = findAlphabeticalSpot(pHead, name);
		
		// if parent null, insert at front of list
		if (parentAndChild.pParent == nullptr)
		{
			pEmp->pNext = pHead;
			pHead = pEmp;
			return;
		}
		
		parentAndChild.pParent->pNext = pEmp;
		pEmp->pNext = parentAndChild.pNode;
	}

}

NodeInfo getNodeInfo(Employee* const pHead, int eId)
{
	Employee* pCurrent = pHead;
	Employee* pParent = nullptr;
	
	while (pCurrent != nullptr)
	{
		if (pCurrent->id == eId)
		{
			return NodeInfo{ pCurrent, pParent };
		}

		pParent = pCurrent;
		pCurrent = pCurrent->pNext;
	}

	// Node was not found
	return NodeInfo{ nullptr, nullptr };
}

NodeInfo findAlphabeticalSpot(Employee* const pHead, const std::string& name)
{
	// check for empty list
	if (pHead == nullptr)
	{
		return NodeInfo{ nullptr, nullptr };
	}

	Employee* pCurrent = pHead;
	Employee* pParent = nullptr;

	while (pCurrent != nullptr && pCurrent->name <= name)
	{
		pParent = pCurrent;
		pCurrent = pCurrent->pNext;
	}

	return NodeInfo{ pCurrent, pParent };
}

void removeEmployee(Employee*& pHead, int eid)
{
	NodeInfo target = getNodeInfo(pHead, eid);
	
	// if node not found
	if (target.pNode == nullptr && target.pParent == nullptr)
	{
		std::cout << "Error: employee id: " << eid << " not found\n";
		return;
	}
	
	if (target.pParent == nullptr)
	{
		pHead = pHead->pNext;
	}

	if (target.pNode != nullptr && target.pParent != nullptr)
	{
		target.pParent->pNext = target.pNode->pNext;
	}

	delete target.pNode;
	std::cout << "removed id: " << eid << '\n';

}

void removeAllEmployees(Employee*& pHead)
{
	while (pHead != nullptr)
	{
		Employee* pEmpToDelete = pHead;
		pHead = pHead->pNext;
		delete pEmpToDelete;
	}

	std::cout << "all employees removed\n";
}



