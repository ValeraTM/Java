#pragma once

#include <cstddef> //size_t

class Instruction;

class Executing_context {
public:
	size_t size();
	Instruction** operator[](size_t idx);
protected:
	Executing_context() = default;
	size_t capacity = 0;
	Instruction ** CORE = nullptr;
};