#include <stdio.h>

#include "Automobile.h"

int main()
{
    Automobile my_car;
    Automobile *pCar = &my_car;

    my_car.Display();

    my_car.Accelerate(10);
    my_car.Display();

    my_car.Stop();
    my_car.Display();

    my_car.Accelerate(40);
    my_car.Display();

    pCar->Decelerate(50);
    pCar->Display();

    return 0;
}
