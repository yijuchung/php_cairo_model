#ifndef VEHICLES_CAR_H
#define VEHICLES_CAR_H

// A very simple car class
class Car {
public:
    Car();
    void shift(int gear);
    void accelerate();
    void brake();
    int getCurrentSpeed();
    int getCurrentGear();
    int maxGear;
    int currentGear;
    int speed;
    double startx;
    double tempmaxy;
};

#endif /* VEHICLES_CAR_H */
