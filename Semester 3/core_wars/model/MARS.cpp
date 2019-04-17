#include "MARS.h"
#include "DAT.h"
#include "factory.h"

#include <string>
#include <vector>
#include <memory>
#include <sstream>

const size_t MARS::STD_SIZE = 8000u;

MARS::MARS() {
	capacity = STD_SIZE;
    CORE = new std::unique_ptr<Instruction>[capacity];
}
MARS::MARS(size_t size) {
	capacity = size;
    CORE = new std::unique_ptr<Instruction>[capacity];
}

std::string MARS::getInfo(int idx) {
    if (CORE[idx % capacity]) {
        return CORE[idx % capacity]->info();
    }
    else {
        return "";
    }
}

bool MARS::toInstruction(std::vector<Instruction*>& arr, std::string code) {
    std::stringstream ss;
    ss << code;

    std::string nameInstruction;
    std::pair<char, int> A;
    std::pair<char, int> B;
    auto f = Factory<std::string, Instruction, Instruction* (*)()>::getInstance();

    while (!ss.eof()) {
        ss >> nameInstruction;
        ss >> A.first >> A.second;
        ss >> B.first >> B.second;

        Instruction * In = f->createProductById(nameInstruction);
        if (In == nullptr) {
            return false;
        }
        In->initialize(A, B);
        arr.push_back(In);
    }

    return true;
}

int MARS::add_warrior(std::string code) {
    std::vector<Instruction*> arr;
    if (!toInstruction(arr, code)) {
        return capacity;
    }
    if (arr.size() > capacity) {
        return -1;
    }

    char * temp = new char[capacity];
    size_t count = 0;
    std::fill(temp, temp + capacity, 0);
    for (uint i = 0; i < capacity; i++) {
        if (CORE[i]) {
            temp[i] = 1;
            count++;
        }
    }
    if (count == capacity) {
        return -1;
    }

    size_t idx = rand() % capacity;
    do {
        while (temp[idx] == 1) {
            idx = rand() % capacity;
        }
        for (uint i = 1; i < arr.size(); i++) {
            if (temp[(idx + i) % capacity] == 1) {
                for (uint j = 0; j <= i; j++) {
                    temp[(idx + j) % capacity] = 1;
                }
                count += i + 1;
                break;
            }
        }
        if (count >= capacity*4/5) {
            return -1;
        }
    } while (temp[idx] != 0);

	for (size_t i = 0u; i < arr.size(); i++) {
        CORE[(idx + i) % capacity].reset(arr[i]);
	}
    return idx;
}

bool MARS::fulfill_instructions(size_t& idx) {
    return CORE[idx]->run(*this, idx);
}

void MARS::readyCore() {
    for (size_t i = 0u; i < capacity; i++) {
        if (!CORE[i]) {
            CORE[i].reset(new DAT);
        }
    }
}

MARS::~MARS() {
	delete[] CORE;
}

