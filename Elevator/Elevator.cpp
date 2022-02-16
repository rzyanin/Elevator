#include "Elevator.hpp"
#include <iostream>
using namespace std;

//Заводим таблицу состояний
TransitionTable transitions;

Elevator::Elevator(int countFloors)
{
    state = State_Stopped;
    currentFloor = 1;
    this->countFloors = countFloors;
    fillTransitionTable();//Заполняем таблицу состояний при создании объекта "лифт"
}

Elevator::~Elevator()
{

}

//Заполняем таблицу переходов
void Elevator::fillTransitionTable()
{
    AddTransition(State_Stopped, Event_goUp, State_goingUp);
    AddTransition(State_Stopped, Event_goDown, State_goingDown);
    AddTransition(State_Stopped, Event_goStop, State_Stopped);

    AddTransition(State_Stopped_forDown, Event_goDown, State_goingDown);
    AddTransition(State_Stopped_forDown, Event_goStop, State_Stopped);

    AddTransition(State_Stopped_forUp, Event_goUp, State_goingUp);
    AddTransition(State_Stopped_forUp, Event_goStop, State_Stopped);

    AddTransition(State_goingDown, Event_goDown, State_goingDown);
    AddTransition(State_goingDown, Event_goStop, State_Stopped);
    AddTransition(State_goingDown, Event_goStopForDown, State_Stopped_forDown);
    AddTransition(State_goingDown, Event_goStopForUp, State_Stopped_forUp);

    AddTransition(State_goingUp, Event_goUp, State_goingUp);
    AddTransition(State_goingUp, Event_goStop, State_Stopped);
    AddTransition(State_goingUp, Event_goStopForDown, State_Stopped_forDown);
    AddTransition(State_goingUp, Event_goStopForUp, State_Stopped_forUp);   

}
/*
обработка вызова лифта с этажа. 
Если кабина выше целевого этажа - добавляем ее в очередь для езды вниз
Если кабина ниже целевого этажа - добавляем ее в очередь для езды вверх
Если кабина находится на целевом этаже и сейчас движется - добавляем в очередь на движение в обратную сторону
Если кабина на целевом этаже и не движется - ничего не делаем
*/

 void Elevator::callFromFloor(int floor)
{
    if(floor > this->countFloors || floor < 1)//проверка на корректность
    {
        cout << "incorrect floor" << endl;
        return;
    }
    if(this->currentFloor > floor)
    {
        this->floorsForDownMoving.insert(floor);
    }
    if(this->currentFloor < floor)
    {
        this->floorsForUpMoving.insert(floor); 
    }
    if(this->currentFloor == floor)
    {
        if(this->state == State_goingUp)
        {
            this->floorsForDownMoving.insert(floor);
        } else if (this->state == State_goingDown)
        {
            this->floorsForUpMoving.insert(floor);
        }
    }
}
/*
Здесь все тоже самое, что и в обработке вызова с этажа. 
Функция нужна, если нужно будет поменять алгоритм движения
*/
void Elevator::callFromCabin(int floor)
{
    if(floor > this->countFloors || floor < 1)//проверка на корректность
    {
        cout << "incorrect floor" << endl;
        return;
    }
    if(this->currentFloor > floor)
    {
        this->floorsForDownMoving.insert(floor);
    }
    if(this->currentFloor < floor)
    {
        this->floorsForUpMoving.insert(floor); 
        
    }
    if(this->currentFloor == floor)
    {
        if(this->state == State_goingUp)
        {
            this->floorsForDownMoving.insert(floor);
        } else if (this->state == State_goingDown)
        {
            this->floorsForUpMoving.insert(floor);
        }
    }
}

void Elevator::move()
{
    set<int>::iterator Downit = floorsForDownMoving.begin();
    set<int>::iterator Upit = floorsForUpMoving.begin();

    //Если мы ехали вниз - убавим этаж, если ехали вверх - прибавим этаж
    if(this->state == State_goingDown)
    {
        this->currentFloor--;
    } else if(this->state == State_goingUp)
    {
        this->currentFloor++;
    }  

    events currentEvent;//определим текущее событие
    switch (this->state)
    {
    case State_Stopped:
        if(!floorsForDownMoving.size() && !floorsForUpMoving.size())
        //Если лифт остановлен и никуда ехать не надо - продолжаем стоять
            currentEvent = Event_goStop;
        else
        {
            //Есди куда-то надо ехать - определим куда
            bool goUpFromStop = false;
            if(!floorsForDownMoving.size() || !floorsForUpMoving.size())
            {
                //Если нужно ехать только в одну сторону - туда и поедем
                goUpFromStop = (floorsForDownMoving.size() ? false : true);
            } else if((*Upit - this->currentFloor) < (this->currentFloor - *Downit))
            //Иначе определим, в какую сторону поедем, исходя из того, какой этаж ближе
                goUpFromStop = true; 
            currentEvent = (goUpFromStop ? Event_goUp : Event_goDown);           
        }
        break;
    case State_goingDown:
    //Если едем вниз и еще не доехали до ближайшего этажа - продолжаем ехать вниз
        if(this->currentFloor > *Downit)
        {
            currentEvent =  Event_goDown;
        } else if(this->currentFloor == *Downit)
        {
            /*
            Если доахели до ближайшего целевого этажа, удаляем его из очереди
            А дальше, либо продолжаем ехать вниз, либо останавливаемся, либо готовимся ехать вверх
            в зависимости от того, есть ли элементы в соответствующих очередях
            По умолчанию мы хотим двигаться в направлении, в котором двигались, если есть этажи 
            в соответствующей очереди
            */
            floorsForDownMoving.erase(Downit);
            if(floorsForDownMoving.size())
            {
                currentEvent = Event_goStopForDown;
            }
            else
            {
                currentEvent = (floorsForUpMoving.size() ? Event_goStopForUp : Event_goStop);
            }
        }
        break;
    case State_goingUp:
    //Если едем вверх и еще не доехали до ближайшего этажа - продолжаем ехать вверх
   
        if(this->currentFloor < *Upit)
        {
            currentEvent = Event_goUp;
        } else if(this->currentFloor == *Upit)
        {
            /*
            Если доахели до ближайшего целевого этажа, удаляем его из очереди
            А дальше, либо продолжаем ехать вниз, либо останавливаемся, либо готовимся ехать вверх
            в зависимости от того, есть ли элементы в соответствующих очередях
            По умолчанию мы хотим двигаться в направлении, в котором двигались, если есть этажи 
            в соответствующей очереди
            */
            floorsForUpMoving.erase(Upit);
            if(floorsForUpMoving.size())
            {
                currentEvent = Event_goStopForUp;
            }
            else
            {
                currentEvent = (floorsForDownMoving.size() ? Event_goStopForDown : Event_goStop);
            }
        }      
        break;
    case State_Stopped_forDown:
    /*
    Если мы при движении вниз остановились по запросу из кабины или с этажа, 
    продолжаем ехать вниз после остановки
    */        
        currentEvent = Event_goDown;
        break;  
    case State_Stopped_forUp:
    /*
    Если мы при движении вверх остановились по запросу из кабины или с этажа, 
    продолжаем ехать вверх после остановки
    */  
        if(this->currentFloor < *Upit)
        {
            currentEvent = Event_goUp;
        }        
        break;   
    }
    //Перейдем в новое состояние в зависимости от текущего события
    goToState(this->state, currentEvent);
  
}
//Вывод в консоль текущего состояния лифта
void Elevator::outState()
{
    switch (this->state)
    {
    case State_goingDown:
        cout << "Cabin on floor " << this->currentFloor << ". Going down.";
        break;
    case State_goingUp:
        cout << "Cabin on floor " << this->currentFloor << ". Going up.";
        break;
    default:
        cout << "Cabin on floor " << this->currentFloor << ". Stopped. Doors opened";   
        break;
    }
    cout << "<Press any key to enter new command>" << endl;
}

//добавляем в таблицу переходов новую строку
void AddTransition(ElevatorStates fromState, events event, ElevatorStates toState)
{
    Transition transition;
    transition.BaseState = fromState;
    transition.Event = event;
    transition.TargetState = toState;
    transitions.push_back(transition);
}

//Поиск состояния в таблице переходов, в которое нужно перейти и переход в это состояние
void Elevator::goToState(ElevatorStates fromState, events event)
{
    for (int i = 0; i < transitions.size(); i++)
    {
        if(transitions[i].BaseState == fromState && transitions[i].Event == event)
            this->state = transitions[i].TargetState;
    }

}