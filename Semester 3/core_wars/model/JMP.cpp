#include "JMP.h"
#include "factory.h"
#include "Executing_context.h"

#include <cstddef>
#include <assert.h>

const char JMP::Immediate = '#';
const char JMP::Relative = '$';
const char JMP::Indirect = '@';

JMP::JMP() {
	A = {Relative, 0};
	B = {Relative, 0};
	nameInstruction = "JMP";
}

JMP::JMP(const std::pair<char, int> A1, const std::pair<char, int> B1) {
	A = A1;
	B = B1;
	nameInstruction = "JMP";
}

JMP* JMP::clone() {
	return new JMP(A, B);
}

bool JMP::run(Executing_context& program, size_t& idx) {
    std::unique_ptr<Instruction>& itA = program[idx + A.second];

	switch (A.first) {
		case Immediate:
			return false;
		case Relative:
			idx = (idx + A.second) % program.size();
			return true;
		case Indirect:
			idx = (idx + A.second + itA->operandA()) % program.size();
			return true;
		default:
			assert(false && "Undefined mode!");
			return false;
	}
}

namespace {
	bool ok = Factory<std::string, Instruction, Instruction* (*)()>::getInstance()->
							registerProduct("JMP", []() -> Instruction* {return new JMP;});
}
