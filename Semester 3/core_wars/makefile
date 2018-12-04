core_wars: Executing_context.o main.o MARS.o instructions.o DAT.o ADD.o MOV.o SUB.o JMP.o
	g++ Executing_context.o main.o MARS.o instructions.o DAT.o ADD.o MOV.o SUB.o JMP.o -o core_wars -std=c++14
Executing_context.o: Executing_context.cpp Executing_context.h
	g++ -c Executing_context.cpp
main.o: main.cpp
	g++ -c main.cpp
MARS.o: MARS.cpp MARS.h
	g++ -c MARS.cpp
instructions.o: instructions.cpp instructions.h
	g++ -c instructions.cpp
DAT.o: DAT.cpp DAT.h
	g++ -c DAT.cpp
MOV.o: MOV.cpp MOV.h
	g++ -c MOV.cpp
ADD.o: ADD.cpp ADD.h
	g++ -c ADD.cpp
SUB.o: SUB.cpp SUB.h
	g++ -c SUB.cpp
JMP.o: JMP.cpp JMP.h
	g++ -c JMP.cpp