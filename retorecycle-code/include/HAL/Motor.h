// Motor.h
#pragma once
#include <Arduino.h>

class Motor
{
public:
    Motor(int stepPin, int dirPin);
    void init();
    // Tốc độ (rounds per minute)
    void setSpeedRpm(double speed);
    // Lấy tốc độ hiện tại (rounds per minute)
    float getCurrentSpeedRpm();
    // Lấy tổng bộ đếm Timer để xử lý bước chậm hơn
    unsigned long getTotalTimerCounter();
    // Hàm này được gọi bởi ISR
    void pulse();

private:
    int _stepPin;
    int _dirPin;
    unsigned long _lastStepTimeUs = 0;
    bool _stepState = false; // Trạng thái hiện tại của chân STEP
    bool _pulseState;        // Trạng thái xung STEP hiện tại
    float _stepsPerSec;     // Tốc độ hiện tại (bước/giây)
    float _currentSpeed = 0.0f;
    unsigned long totalTimerCounter = 0;
};