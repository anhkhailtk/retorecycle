// Motor.h
#pragma once
#include <Arduino.h>

class Motor {
public:
    Motor(int stepPin, int dirPin);
    void init();
    // Tốc độ (steps per second)
    void setSpeed(double speed);
    // Hàm này được gọi bởi ISR
    void pulse(); 

private:
    int _stepPin;
    int _dirPin;
    bool _pulseState;
};