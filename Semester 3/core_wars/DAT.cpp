#include "DAT.h"
#include "factory.h"

#include <cstddef>

const char DAT::Immediate = '#';
const char DAT::Relative = '$';
const char DAT::Indirect = '@';

DAT::DAT() {
	A = {Relative, 0};
	B = {Relative, 0};
	nameInstruction = "DAT";
}

DAT::DAT(const std::pair<char, int> A1, const std::pair<char, int> B1) {
	A = A1;
	B = B1;
	nameInstruction = "DAT";
}

DAT* DAT::clone() {
	return new DAT(A, B);
}

bool DAT::run(Executing_context& program, size_t& idx) {
	return false;
}

namespace {
	bool ok = Factory<std::string, Instruction, Instruction* (*)()>::getInstance()->
							registerProduct("DAT", []() -> Instruction* {return new DAT;});
}