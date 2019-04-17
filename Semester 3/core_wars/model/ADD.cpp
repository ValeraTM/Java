#include "ADD.h"
#include "factory.h"
#include "Executing_context.h"

#include <cstddef>
#include <assert.h>

const char ADD::Immediate = '#';
const char ADD::Relative = '$';
const char ADD::Indirect = '@';

ADD::ADD() {
	A = {Relative, 0};
	B = {Relative, 0};
	nameInstruction = "ADD";
}

ADD::ADD(const std::pair<char, int> A1, const std::pair<char, int> B1) {
	A = A1;
	B = B1;
	nameInstruction = "ADD";
}

ADD* ADD::clone() {
	return new ADD(A, B);
}

bool ADD::run(Executing_context& program, size_t& idx) {
    std::unique_ptr<Instruction>& itB = program[idx + B.second];
    std::unique_ptr<Instruction>& itB2 = program[idx + itB->operandB() + B.second];
    std::unique_ptr<Instruction>& itA = program[idx + A.second];
    std::unique_ptr<Instruction>& itA2 = program[idx + itA->operandB() + A.second];

	if (A.first == Immediate) {
		switch (B.first) {
			case Immediate:
				return false;
			case Relative:
				itB->operandB() += A.second;
				break;
			case Indirect:
				itB2->operandB() += A.second;
				break;
			default:
				assert(false && "Undefined mode!");
				return false;
		}
	}

	if (A.first == Relative) {
		switch (B.first) {
			case Immediate:
				return false;
			case Relative:
				itB->operandB() += itA->operandB();
				itB->operandA() += itA->operandA();
				break;
			case Indirect:
				itB2->operandB() += itA->operandB();
				itB2->operandA() += itA->operandA();
				break;
			default:
				assert(false && "Undefined mode!");
				return false;
		}
	}

	if (A.first == Indirect) {
		switch (B.first) {
			case Immediate:
				return false;
			case Relative:
				itB->operandB() += itA2->operandB();
				itB->operandA() += itA2->operandA();
				break;
			case Indirect:
				itB2->operandB() += itA2->operandB();
				itB2->operandA() += itA2->operandA();
				break;
			default:
				assert(false && "Undefined mode!");
				return false;
		}
	}

	idx =  (idx + 1) % program.size();
	return true;
}


namespace {
	bool ok = Factory<std::string, Instruction, Instruction* (*)()>::getInstance()->
							registerProduct("ADD", []() -> Instruction* {return new ADD;});
}

