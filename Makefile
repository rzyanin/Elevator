all: main.o Elevator/Elevator.o
	g++ main.cpp Elevator/Elevator.cpp -o Elevator

main.o: main.cpp
	g++ -c main.cpp

Elevator/Elevator.o: Elevator/Elevator.cpp Elevator/Elevator.hpp
	g++ -c Elevator/Elevator.cpp
