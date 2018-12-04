#include "MARS.h"
#include "DAT.h"

#include <string>
#include <vector>

#include <iostream>	

const size_t MARS::STD_SIZE = 8000u;

MARS::MARS() {
	capacity = STD_SIZE;
	CORE = new Instruction*[capacity];
}
MARS::MARS(size_t size) {
	capacity = size;
	CORE = new Instruction*[capacity];
}

void MARS::add_warrior(std::vector<Instruction*>& arr, std::string& nameWarrior) {
	size_t idx = rand() % capacity;
	while (CORE[idx] != nullptr) {
		idx = rand() % capacity;
	}

	warrior.push_back({idx, nameWarrior});
	for (size_t i = 0u; i < arr.size(); i++) {
		CORE[(idx + i) % capacity] = arr[i];
	}
}

void fulfill_instructions() {
	for (auto it = warrior.begin(); it != warrior.end(); ++it) {
		if (!CORE[it->first]->run(*this, it->first)) {
			warrior.erase(it);
			--it;
		}
	}
}

std::string* MARS::WAR(size_t limit) {
	readyCore();
	size_t count = 0u;
	while (warrior.size() && count < limit) {
		if (warrior.size() == 1) {
			return &(warrior[0].second);
		}
		count++;
	}

	return nullptr;
}

MARS::~MARS() {
	for (size_t i = 0u; i < capacity; i++) {
		delete CORE[i]; 
	}
	delete[] CORE;
}

void MARS::readyCore() {
	for (size_t i = 0u; i < capacity; i++) {
		if (CORE[i] == nullptr) {
			CORE[i] = new DAT;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
/*void MARS::print() {
	std::cin.get();
	std::system("clear");
	int field[capacity] = {};
	int count = 1;
	for (auto it = warrior.begin(); it != warrior.end(); it++) {
		field[(*it).first] = count;
		count++;
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (field[i*10 + j] == 0) {
				std::cout << ". ";
			}
			else {
				std::cout << field[i*10 + j] << " ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (auto it = warrior.begin(); it != warrior.end(); it++) {
		std::cout << CORE[(*it).first]->name();
		std::cout << " " << CORE[(*it).first]->operandA() << " " << CORE[(*it).first]->operandB();
		std::cout << std::endl;
	}

	//std::cin.get();
}*/