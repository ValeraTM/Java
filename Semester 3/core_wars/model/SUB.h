#pragma once
#include "instructions.h"

#include <cstddef>

class SUB : public Instruction {
public:
	SUB();
	SUB(const std::pair<char, int> A1, const std::pair<char, int> B1);
	SUB* clone() override;
	bool run(Executing_context& program, size_t& idx) override;
private:
	static const char Immediate;
	static const char Relative;
	static const char Indirect;
};