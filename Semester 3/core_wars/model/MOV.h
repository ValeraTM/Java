#pragma once
#include "instructions.h"

#include <cstddef>

class MOV : public Instruction {
public:
	MOV();
	MOV(const std::pair<char, int> A1, const std::pair<char, int> B1);
	MOV* clone() override;
	bool run(Executing_context& program, size_t& idx) override;
private:
	static const char Immediate;
	static const char Relative;
	static const char Indirect;
};