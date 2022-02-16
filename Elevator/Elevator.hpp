/*
Лифт реализован в виде конечного автомата, который описан ниже. 
Таблица переходов описана в файле Elevator.cpp
Состояния State_Stopped_forUp и State_Stopped_forDown означают, 
что лифт остановился, но через секунду продолжит движение вверх или вниз соответственно


Этажи, на которых нужно останавливаться, записываются в "очереди" floorsForDownMoving и
floorsForUpMoving, для движения вниз и вверх соответственно. "Очереди" в кавычках, потому что
структура данных по сути является не очередью,
а множеством с сортировкой по убыванию и возрастанию соответственно.

Алгоритм наивный. Пока лифт движется вверх и в очереди на движение вверх(вниз) есть этажи - двигаемся
вверх(вниз). Как только в соответствующей очереди заканчиваются этажи, начинаем обходить этажи
из другой очереди, либо останавливаемся (если в другой очереди нет этажей).

Алгоритм движения организуется в методах callFromFloor и callFromCabin класса Elevator.
Существующий алгоритм описан в комментариях к этим методам
При данной реализации алгоритма эти методы содержат одинаковый код. Однако при создании программы
я предполагал еще один вариант алгоритма, когда лифт предполагает, что те, кто вызывают лифт с этажей,
а не из кабины (кроме первого этажа), по умолчанию хотят ехать вниз, а с первого этажа вверх. 
И обходит этажи в соотвествующем порядке (записывая в соотвествующую очередь). 
При такой реализации код в этих методах был бы разным, поэтому я оставил два метода, пусть и с одинаковым кодом

*/
#include <stdio.h>
#include <vector>
#include <set>
using namespace std;

//Состояния лифта
enum ElevatorStates
{
    State_goingUp,
    State_goingDown,
    State_Stopped_forUp, 
    State_Stopped_forDown,
    State_Stopped
};

//События переходов
enum events
{
    Event_goUp,
    Event_goDown,
    Event_goStopForUp,
    Event_goStopForDown,
    Event_goStop
};

//Структура, определяющая строку в таблице переходов
struct Transition
{
	ElevatorStates BaseState;
	events Event;
	ElevatorStates TargetState;
};

//Функция для добавления строки в таблицу переходов
void AddTransition(ElevatorStates fromState, events event, ElevatorStates toState);
//Таблица переходов
typedef vector<Transition> TransitionTable;

class Elevator
{
    int currentFloor;//Текущий этаж
    int countFloors;//Максимальное количество этажей

    set<int, greater<int>> floorsForDownMoving;//"очередь" остановки на этажах при езде вниз
    set<int> floorsForUpMoving;//"очередь" остановки на этажах при езде вверх

    public: 
        ElevatorStates state;//Текущее состояние лифта
        
        void callFromFloor(int floor);//обработка команды вызова из этажа
        void callFromCabin(int floor);//обработка команды вызова из кабины
        void fillTransitionTable();//Заполнение таблицы переходов конечного автомата
        void move();//функция, в которой происходит изменение состояния лифта
        void outState();//вывод текущего состояния в консоль
        void goToState(ElevatorStates fromState, events event);//поиск в таблице состояния, в которое нужно перейти
        Elevator(int countFloors);
        ~Elevator();
};


