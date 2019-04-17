#pragma once

#include <cstddef> //size_t
#include <memory> //unique_ptr

class Instruction;

class Executing_context {
public:
	size_t size();
    std::unique_ptr<Instruction>& operator[](size_t idx);
protected:
	Executing_context() = default;
	size_t capacity = 0;
    std::unique_ptr<Instruction> * CORE = nullptr;
};
