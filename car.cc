#include "car.h"

Car::Car() {
    this->tempmaxy = 0;
    this->startx = 0;
}

void Car::shift(int gear) {
    if (gear < 1 || gear > maxGear) {
        return;
    }
    currentGear = gear;
}

void Car::accelerate() {
    speed += (5 * this->getCurrentGear());
}

void Car::brake() {
    speed -= (5 * this->getCurrentGear());
}

int Car::getCurrentSpeed() {
    return speed;
}

int Car::getCurrentGear() {
    return currentGear;
}
