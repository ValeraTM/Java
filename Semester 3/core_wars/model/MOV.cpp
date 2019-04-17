#include "MOV.h"
#include "factory.h"
#include "Executing_context.h"

#include <assert.h>
#include <cstddef>
#include <memory>

const char MOV::Immediate = '#';
const char MOV::Relative = '$';
const char MOV::Indirect = '@';

MOV::MOV() {
	A = {Relative, 0};
	B = {Relative, 0};
	nameInstruction = "MOV";
}

MOV::MOV(const std::pair<char, int> A1, const std::pair<char, int> B1) {
	A = A1;
	B = B1;
	nameInstruction = "MOV";
}

MOV* MOV::clone() {
	return new MOV(A, B);
}

bool MOV::run(Executing_context& program, size_t& idx) {
    std::unique_ptr<Instruction>& itB = program[idx + B.second];
    std::unique_ptr<Instruction>& itB2 = program[idx + itB->operandB() + B.second];
    std::unique_ptr<Instruction>& itA = program[idx + A.second];
    std::unique_ptr<Instruction>& itA2 = program[idx + itA->operandB() + A.second];

	if (A.first == Immediate) {
		switch (B.first) {
			case Immediate:
				return false;
			case Relative:
                itB->operandB() = A.second;
				break;
			case Indirect:
                itB2->operandB() = A.second;
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
                itB.reset(itA->clone());
				break;
			case Indirect:
                itB2.reset(itA->clone());
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
                itB.reset(itA2->clone());
				break;
			case Indirect:
                itB2.reset(itA2->clone());
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
							registerProduct("MOV", []() -> Instruction* {return new MOV;});
};
