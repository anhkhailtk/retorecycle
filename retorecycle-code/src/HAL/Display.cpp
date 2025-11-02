#include "HAL/Display.h"
#include <Arduino.h>

Display::Display(int address):_lcd(address, 16, 2) {}

void Display::init()
{
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
    _lcd.setCursor(0,0);
    _lcd.setCursor(0, 0);
    _lcd.print("T: ");  // in thêm khoảng trắng để xóa phần dư
    _lcd.setCursor(0, 1);
    _lcd.print("S: ");  // xóa phần dư
}

void Display::update(double currentT, double setT, double currentS, double setS)
{
    // ====== Dòng 1: Nhiệt độ ======    
    _lcd.setCursor(6, 0);
    _lcd.print(currentT);     // hiển thị 1 chữ số thập phân
    _lcd.setCursor(9, 0);
    _lcd.print("/");
    _lcd.setCursor(10, 0);
    _lcd.print(setT);

    // ====== Dòng 2: Tốc độ ======
    _lcd.setCursor(6, 1);
    _lcd.print(currentS);     // tốc độ thường là số nguyên
    _lcd.setCursor(9, 1);
    _lcd.print("/");
    _lcd.setCursor(10, 1);
    _lcd.print(setS);
}
