#pragma once
#include <Arduino.h>

class Heater {
public:
    // Constructor: nhận chân pin Mosfet
    Heater(int pin);

    // Hàm khởi tạo (set pinMode)
    void init();

    // Đặt công suất (0-255)
    // ATmega8 dùng 8-bit PWM, nên giá trị từ 0 (0%) đến 255 (100%)
    void setPower(int power);

private:
    int _heaterPin; // Chân pin điều khiển Heater
    int _currentPower; // Lưu trữ công suất hiện tại
};