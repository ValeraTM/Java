#include "ADD.h"
#include "DAT.h"
#include "factory.h"
#include "instructions.h"
#include "JMP.h"
#include "Executing_context.h"
#include "MOV.h"
#include "MARS.h"
#include "SUB.h"

#include <assert.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

Instruction* createInstruction(std::string name, std::pair<char, int> A, std::pair<char, int> B) {
	auto f = Factory<std::string, Instruction, Instruction* (*)()>::getInstance();
	Instruction * In = f->createProductById(name);
	assert(In != nullptr && "This instruction doesn't exist!");
	In->initialize(A, B);
	return In;
}

std::vector<Instruction*> read_warrior(std::ifstream& fin) {
	std::pair<char, int> A;
	std::pair<char, int> B;
	std::string nameInstruction;
	
	std::vector<Instruction*> arr;

	while (!fin.eof()) {
		fin >> nameInstruction;
		fin >> A.first >> A.second;
		fin >> B.first >> B.second;
		arr.push_back(createInstruction(nameInstruction, A, B));
	}

	return arr;
}


int main(int argc, char ** argv) {
	srand(time(0));

	if (argc < 2) {
		std::cout << "No warrior specified!" << std::endl;
		return -1;
	}

	size_t size = 0;
	std::cout << "Enter size of field" << std::endl;
	std::cin >> size;

	MARS mars(size);
	for (int i = 1; i < argc; i++) {
		std::ifstream fin(argv[i]);
		if (!fin.is_open()) {
			std::cout << "File is not open!" << std::endl;
			return -1;
		}

		std::vector<Instruction*> arr = read_warrior(fin);
		std::cout << "Enter name of warrior" << std::endl;
		std::string nameWarrior;
		std::cin >> nameWarrior;
		mars.add_warrior(arr, nameWarrior);
	}

	std::string* result = mars.WAR(100);
	if (result == nullptr) {
		std::cout << "Draw!" << std::endl;
	}
	else {
		std::cout << "Winner: " << *result << std::endl;
	}

	return 0;
}