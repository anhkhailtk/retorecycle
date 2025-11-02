#ifndef MINIMAL_I2C_LCD_H
#define MINIMAL_I2C_LCD_H

#include <Arduino.h>
#include <Wire.h>

// --- Định nghĩa Hằng số ---
const uint8_t LCD_RS = 0b00000001; // Register Select (P0)
const uint8_t LCD_EN = 0b00000100; // Enable (P2)
const uint8_t LCD_BL = 0b00001000; // Backlight (P3)

// Địa chỉ của các hàng trên LCD 16x2
const uint8_t ROW_OFFSETS[] = { 0x00, 0x40 };

class MinimalI2CLCD {
private:
  uint8_t _addr;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightStatus;

  // Các hàm nội bộ
  void i2c_write(uint8_t data);
  void send_nibble(uint8_t nibble, uint8_t mode);
  void command(uint8_t cmd);
  void data(uint8_t d);

public:
  // Hàm tạo (Constructor)
  MinimalI2CLCD(uint8_t addr, uint8_t cols, uint8_t rows);

  // Các hàm công cộng
  void init();
  void print(const char *str);
  void print(int num);
  void setCursor(uint8_t col, uint8_t row);
  void clear();
  void backlight();
  void noBacklight();
};

#endif