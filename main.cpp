#include<stdio.h>
#include<iostream>
#include<time.h>
#include <conio.h>
#include <windows.h>
#include "Elevator/Elevator.hpp"
using namespace std;

bool correctCinChek()
{
    if (cin.fail()) //проверка на корректность введенных данных
    {
        cin.clear(); 
        cin.ignore(32767,'\n'); 
        cout << "That iput is invalid. Try again" << endl;
        return false;
    } 
    return true;
}
//функция, которая в течении секунды ждет ввода команды
bool waintInput(int* targetFloor, bool* fromCabin)
{
    int numInput;

    clock_t start = clock();

    while ( ! _kbhit()) //Проверяем нажатие клавиши
    {
        if (((clock () - start)/ CLOCKS_PER_SEC ) >= 1) //Проверяем, не истек ли таймаут
        {
            return false;
        }
    }

    //Парсим команду
    char typeOfCall;

    cin >> typeOfCall >> *targetFloor;
    if(typeOfCall == 'F')
        *fromCabin = false;
    else if(typeOfCall == 'C')
        *fromCabin = true;
    else 
    {
        cout << "That iput is invalid. Try again" << endl;
        return false; //Если неверно введена команда - считаем что ничего не вводили
    }
    correctCinChek();
 
    return true;
}


int main(void) {
    //Вводим этажность дома, с проверкой на корректность
    int countFloors;
    do
    {
        cout << "Enter count of floors: ";
        cin >> countFloors;
    } while (!correctCinChek());
 
    Elevator elevator(countFloors);

    elevator.outState();
    bool stopped = true;
    while(1)
    {
        if(!stopped) //Нужно, чтобы выводить состояние Stopped только один раз
        {
            elevator.outState();
            if(elevator.state == State_Stopped)
                stopped = true;
        }
        int targetFloor = 0;
        bool fromCabin = false;
        if(waintInput(&targetFloor, &fromCabin))
        {
            fromCabin ? elevator.callFromCabin(targetFloor) : elevator.callFromFloor(targetFloor);
        }
        elevator.move();
        if(elevator.state != State_Stopped)//Нужно, чтобы выводить состояние Stopped только один раз
            stopped = false;
    }

  return 0;
}