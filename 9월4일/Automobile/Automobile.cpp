#include <stdio.h>
#include <string.h>

#include "Automobile.h"

Automobile::Automobile()
{
    fuel = 50.F;
    speed = 0.F;
    strcpy(licensePlate, "UNNAMED");
}

void Automobile::Display()
{
    printf("license plate = %s, speed = %.1f, fuel = %.1f\n", licensePlate, speed, fuel);
}

void Automobile::Accelerate(int amount)
{
    speed += amount;
}

void Automobile::Decelerate(int amount)
{
    speed -= amount;
    if(speed < 0.F)
        speed = 0.F;
}

void Automobile::Stop()
{
    speed = 0;
}