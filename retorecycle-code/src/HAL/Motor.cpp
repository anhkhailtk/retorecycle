#include "HAL/Motor.h"
#include <avr/io.h>
Motor::Motor(int stepPin, int dirPin)
    : _stepPin(stepPin), _dirPin(dirPin), _lastStepTimeUs(0), _stepState(false), _pulseState(false), _stepsPerSec(0.0), totalTimerCounter(0)
{
}
void Motor::init()
{
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
    digitalWrite(_stepPin, LOW);
    digitalWrite(_dirPin, LOW);
    // Cấu hình Timer2, thạch anh 4MHz
    cli(); // Tắt ngắt toàn cục
    TCCR2 = 0;
    TCNT2 = 0;
    TCCR2 |= (1 << WGM21); // WGM21=1 (CTC Mode)
    // 4. Prescaler = 8
    TCCR2 |= (1 << CS21); // CS22=0, CS21=1, CS20=0 => Prescaler = 8
    // Tần số ngắt 10kHz (mỗi 100us)
    OCR2= 49;
    // 5. Kích hoạt Ngắt (Interrupt)
    TIMSK |= (1 << OCIE2); // Bật ngắt so sánh OCR2A
    TCNT2 = 0; // Đặt lại bộ đếm
    totalTimerCounter = 0;
    sei(); // Kích hoạt ngắt toàn cục
}
void Motor::setSpeedRpm(double speed)
{
    _currentSpeed = speed;
    _stepsPerSec = (speed * 200.0) / 60.0;                                                 // Chuyển RPM sang bước mỗi giây (200 bước/vòng)

    //Tần số ngắt 10khz, số lần ngắt cần để tạo ra một bước
    totalTimerCounter = static_cast<unsigned long>(10000.0 / (_stepsPerSec * 2)); // Số lần ngắt để tạo ra một bước

}

float Motor::getCurrentSpeedRpm()
{
    return _currentSpeed; // Chuyển bước mỗi giây sang RPM
}

unsigned long Motor::getTotalTimerCounter()
{
    return totalTimerCounter;
}

void Motor::pulse()
{
    if (_pulseState)
    {
        digitalWrite(_stepPin, HIGH);
        _pulseState = false;
    }
    else
    {
        digitalWrite(_stepPin, LOW);
        _pulseState = true;
    }
}