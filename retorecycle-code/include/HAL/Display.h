#pragma once
#include <Wire.h>
#include "MinimalI2CLCD.h"

class Display {
public:
    // Constructor: Chỉ cần địa chỉ I2C, giả định LCD 16x2
    Display(int address);

    // Khởi tạo LCD
    void init();

    // Hàm cập nhật toàn bộ màn hình (non-blocking)
    // Đây là hàm duy nhất cần gọi từ loop()
    void update(double currentT, double setT, double currentS, double setS);

private:
    MinimalI2CLCD _lcd;
    char _buffer[17]; // 16 ký tự + 1 null terminator (dùng chung để tiết kiệm RAM)
};