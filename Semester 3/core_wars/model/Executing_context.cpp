#include "Executing_context.h"

#include <cstddef> //size_t

size_t Executing_context::size() {
	return capacity;
}

std::unique_ptr<Instruction>& Executing_context::operator[](size_t idx) {
    return CORE[idx % capacity];
}
