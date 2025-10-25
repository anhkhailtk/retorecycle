#pragma once
#include <max6675.h> // Bạn phải #include thư viện này

class TempSensor {
public:
    // Constructor: Nhận các chân SPI
    TempSensor(int sck, int cs, int so);

    // Hàm đọc nhiệt độ
    // Hàm readCelsius() của thư viện là blocking,
    // nhưng nó rất nhanh (vài mili giây) nên ta chấp nhận
    double readCelsius();

private:
    // Khởi tạo đối tượng thư viện ngay trong class
    MAX6675 _thermocouple;
};