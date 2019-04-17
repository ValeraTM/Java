#pragma once

#include <cstddef> //size_t
#include <string>
#include <utility> //std::pair

class Executing_context;

class Instruction {
public:
	void initialize(const std::pair<char, int>& A1, const std::pair<char, int>& B1);
	int& operandA();
	int& operandB();
    std::string info();

	virtual Instruction* clone() = 0;
	virtual bool run(Executing_context& program, size_t& idx) = 0;
	virtual ~Instruction() = default;
protected:
	Instruction() = default;
	Instruction(const Instruction& it) = delete;
	Instruction& operator=(const Instruction& it) = delete;

	std::pair<char, int> A;
	std::pair<char, int> B;
	std::string nameInstruction;
};
