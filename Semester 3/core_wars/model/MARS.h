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
    bool fulfill_instructions(size_t& idx);
    int add_warrior(std::string code);
    void readyCore();
    std::string getInfo(int idx);
	~MARS();
private:
    bool toInstruction(std::vector<Instruction*>& arr, std::string code);
	static const size_t STD_SIZE;
};
