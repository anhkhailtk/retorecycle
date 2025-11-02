#include "HAL/Heater.h"
Heater::Heater(int heaterPin)
    : _heaterPin(heaterPin), _currentPower(0)
{
}
void Heater::init()
{
    pinMode(_heaterPin, OUTPUT);
    analogWrite(_heaterPin, 0); // Khởi tạo với công suất 0
}
void Heater::setPower(int power)
{
    analogWrite(_heaterPin, power);
}