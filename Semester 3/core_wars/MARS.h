#pragma once

#include "Executing_context.h"

#include <vector>
#include <string>
#include <utility> //std::pair
#include <cstddef> //size_t

class Instruction;

class MARS : public Executing_context {
public:
	MARS();
	MARS(size_t size);
	void fulfill_instructions();
	std::string* WAR(size_t limit);
	void add_warrior(std::vector<Instruction*>& arr, std::string& nameWarrior);
	~MARS();
private:
	void readyCore();

	std::vector<std::pair<size_t, std::string>> warrior;
	static const size_t STD_SIZE;
};