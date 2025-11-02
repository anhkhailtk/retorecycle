#include "MinimalI2CLCD.h"
#include <stdio.h> // Cần thư viện này cho sprintf()
#include <stdlib.h> // Cần thư viện này cho itoa()

// --- Hàm Tạo (Constructor) ---
MinimalI2CLCD::MinimalI2CLCD(uint8_t addr, uint8_t cols, uint8_t rows) {
  _addr = addr;
  _cols = cols;
  _rows = rows;
  _backlightStatus = LCD_BL; // Mặc định bật đèn nền
}

// --- Hàm Nội bộ: Gửi byte 8-bit đến PCF8574 qua I2C ---
void MinimalI2CLCD::i2c_write(uint8_t data) {
  Wire.beginTransmission(_addr);
  Wire.write(data);
  Wire.endTransmission();
}

// --- Hàm Nội bộ: Gửi 4-bit (nibble) dữ liệu/lệnh đến HD44780 ---
void MinimalI2CLCD::send_nibble(uint8_t nibble, uint8_t mode) {
  uint8_t data = nibble | mode | _backlightStatus;

  // Pulse EN High
  i2c_write(data | LCD_EN); 
  delayMicroseconds(1); 
  
  // Pulse EN Low
  i2c_write(data & ~LCD_EN);
  delayMicroseconds(50); 
}

// --- Hàm Nội bộ: Gửi 8-bit lệnh (RS = LOW) ---
void MinimalI2CLCD::command(uint8_t cmd) {
  // Gửi High Nibble (D7 D6 D5 D4)
  send_nibble(cmd & 0xF0, 0); 
  // Gửi Low Nibble (D3 D2 D1 D0)
  send_nibble((cmd << 4) & 0xF0, 0);
}

// --- Hàm Nội bộ: Gửi 8-bit dữ liệu (RS = HIGH) ---
void MinimalI2CLCD::data(uint8_t d) {
  // Gửi High Nibble (D7 D6 D5 D4)
  send_nibble(d & 0xF0, LCD_RS); 
  // Gửi Low Nibble (D3 D2 D1 D0)
  send_nibble((d << 4) & 0xF0, LCD_RS);
}

// --- Hàm Công cộng: Khởi tạo LCD ---
void MinimalI2CLCD::init() {
  Wire.begin(); 

  // Chuỗi khởi tạo 4-bit mode cho HD44780
  delay(50); 
  i2c_write(_backlightStatus); 
  
  send_nibble(0x30, 0);
  delayMicroseconds(4500); 
  send_nibble(0x30, 0);
  delayMicroseconds(4500);
  send_nibble(0x30, 0);
  delayMicroseconds(150);
  
  send_nibble(0x20, 0); // Set 4-bit mode
  
  command(0x28); // 4-bit mode, 2 dòng, font 5x8
  command(0x0C); // Display On, Cursor Off, Blink Off
  command(0x01); // Clear Display
  delayMicroseconds(2000); 
  command(0x06); // Entry Mode Set
}

// --- Hàm Công cộng: Hiển thị chuỗi ---
void MinimalI2CLCD::print(const char *str) {
  while (*str) {
    data(*str++);
  }
}

// --- Hàm Công cộng: In số nguyên (dùng itoa() và base 10 mặc định) ---
void MinimalI2CLCD::print(int num) {
    // Kích thước buffer đủ để chứa số nguyên 16-bit (+/- 32767) và dấu '\0'
    char buffer[7]; 
    
    // Chuyển số nguyên thành chuỗi với cơ số 10 (thập phân)
    itoa(num, buffer, 10); 
    
    // In chuỗi kết quả
    print(buffer);
}

// --- Hàm Công cộng: Đặt vị trí con trỏ ---
void MinimalI2CLCD::setCursor(uint8_t col, uint8_t row) {
  command(0x80 | (ROW_OFFSETS[row] + col));
}

// --- Hàm Công cộng: Xóa màn hình ---
void MinimalI2CLCD::clear() {
  command(0x01);
  delayMicroseconds(2000);
}

// --- Hàm Công cộng: Bật đèn nền ---
void MinimalI2CLCD::backlight() {
  _backlightStatus = LCD_BL;
  i2c_write(_backlightStatus);
}

// --- Hàm Công cộng: Tắt đèn nền ---
void MinimalI2CLCD::noBacklight() {
  _backlightStatus = 0;
  i2c_write(_backlightStatus);
}