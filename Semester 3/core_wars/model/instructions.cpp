#include "factory.h"
#include "instructions.h"

#include <cstddef> //size_t
#include <string>
#include <utility> //std::pair
#include <sstream>

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

std::string Instruction::info() {
    std::stringstream ss;
    ss << nameInstruction << " " << A.first << A.second << " " << B.first << B.second;
    std::string it;
    std::string result;
    while (!ss.eof()) {
        ss >> it;
        result += it + " ";
    }
    return result;
}
