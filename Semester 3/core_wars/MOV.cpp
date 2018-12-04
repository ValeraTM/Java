#include "MOV.h"
#include "factory.h"
#include "Executing_context.h"

#include <cstddef>
#include <assert.h>

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
	Instruction** itB = program[idx + B.second];
	Instruction** itB2 = program[idx + (*itB)->operandB() + B.second];
	Instruction** itA = program[idx + A.second];
	Instruction** itA2 = program[idx + (*itA)->operandB() + A.second];

	Instruction* temp = nullptr;

	if (A.first == Immediate) {
		switch (B.first) {
			case Immediate:
				return false;
			case Relative:
				(*itB)->operandB() = A.second;
				break;
			case Indirect:
				(*itB2)->operandB() = A.second;
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
				temp = *itB;
				*itB = (*itA)->clone();
				delete temp;
				break;
			case Indirect:
				temp = *itB2;
				*itB2 = (*itA)->clone();
				delete temp;
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
				temp = *itB;
				*itB = (*itA2)->clone();
				delete temp;
				break;
			case Indirect:
				temp = *itB2;
				*itB2 = (*itA2)->clone();
				delete temp;
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