#include "factory.h"
#include "instructions.h"

#include <cstddef> //size_t
#include <string>
#include <utility> //std::pair

void Instruction::initialize(const std::pair<char, int>& A1, const std::pair<char, int>& B1) {
	A = A1;
	B = B1;
}

int& Instruction::operandA() {
	return A.second;
}
int& Instruction::operandB() {
	return B.second;
}

//временно
std::string Instruction::name() {
	return nameInstruction;
}