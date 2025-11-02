#include "HAL/TempSensor.h"
TempSensor::TempSensor(int sck, int cs, int so)
    : _thermocouple(sck, cs, so)
{
}  
double TempSensor::readCelsius()
{
    return static_cast<double>(_thermocouple.readCelsius());
}
