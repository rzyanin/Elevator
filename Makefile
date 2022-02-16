all: main.o Elevator.o
	g++ main.cpp Elevator/Elevator.cpp -o bin/Elevator

main.o: main.cpp
	g++ -c main.cpp -o bin/main.o

Elevator.o: Elevator/Elevator.cpp Elevator/Elevator.hpp
	g++ -c Elevator/Elevator.cpp -o bin/Elevator.o
