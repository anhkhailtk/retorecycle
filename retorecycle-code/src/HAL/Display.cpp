#include "HAL/Display.h"
#include <Arduino.h>

Display::Display(int address):_lcd(address,16,2) {}

void Display::init()
{
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
    _lcd.setCursor(0,0);
    _lcd.print("System ready");
}

void Display::printMessage(const char* line1, const char* line2)
{
    _lcd.clear();
    _lcd.setCursor(0,0);
    _lcd.print(line1);
    _lcd.setCursor(1,0);
    _lcd.print(line2);
}

void Display::update(double currentT, double setT, double currentS, double setS)
{
    // ====== Dòng 1: Nhiệt độ ======
    _lcd.setCursor(0, 0);
    _lcd.print("Temp:       ");  // in thêm khoảng trắng để xóa phần dư
    _lcd.setCursor(6, 0);
    _lcd.print(currentT, 1);     // hiển thị 1 chữ số thập phân
    _lcd.print("/");
    _lcd.print(setT, 1);

    // ====== Dòng 2: Tốc độ ======
    _lcd.setCursor(0, 1);
    _lcd.print("Speed:         ");  // xóa phần dư
    _lcd.setCursor(7, 1);
    _lcd.print(currentS, 0);     // tốc độ thường là số nguyên
    _lcd.print("/");
    _lcd.print(setS, 0);
}
