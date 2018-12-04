#include "SUB.h"
#include "factory.h"
#include "Executing_context.h"

#include <cstddef>
#include <assert.h>

const char SUB::Immediate = '#';
const char SUB::Relative = '$';
const char SUB::Indirect = '@';

SUB::SUB() {
	A = {Relative, 0};
	B = {Relative, 0};
	nameInstruction = "SUB";
}

SUB::SUB(const std::pair<char, int> A1, const std::pair<char, int> B1) {
	A = A1;
	B = B1;
	nameInstruction = "SUB";
}

SUB* SUB::clone() {
	return new SUB(A, B);
}

bool SUB::run(Executing_context& program, size_t& idx) {
	Instruction* itB = *program[idx + B.second];
	Instruction* itB2 = *program[idx + itB->operandB() + B.second];
	Instruction* itA = *program[idx + A.second];
	Instruction* itA2 = *program[idx + itA->operandB() + A.second];

	if (A.first == Immediate) {
		switch (B.first) {
			case Immediate:
				return false;
			case Relative:
				itB->operandB() -= A.second;
				break;
			case Indirect:
				itB2->operandB() -= A.second;
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
				itB->operandB() -= itA->operandB();
				itB->operandA() -= itA->operandA();
				break;
			case Indirect:
				itB2->operandB() -= itA->operandB();
				itB2->operandA() -= itA->operandA();
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
				itB->operandB() -= itA2->operandB();
				itB->operandA() -= itA2->operandA();
				break;
			case Indirect:
				itB2->operandB() -= itA2->operandB();
				itB2->operandA() -= itA2->operandA();
				break;
			default:
				assert(false && "Undefined mode!");
				return false;
		}
	}

	idx = (idx + 1) % program.size();
	return true;
}


namespace {
	bool ok = Factory<std::string, Instruction, Instruction* (*)()>::getInstance()->
							registerProduct("SUB", []() -> Instruction* {return new SUB;});
}