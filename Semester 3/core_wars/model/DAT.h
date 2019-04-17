#pragma once
#include "instructions.h"

#include <cstddef>

class DAT : public Instruction {
public:
	DAT();
	DAT(const std::pair<char, int> A1, const std::pair<char, int> B1);
	DAT* clone() override;
	bool run(Executing_context& program, size_t& idx) override;
private:
	static const char Immediate;
	static const char Relative;
	static const char Indirect;
};