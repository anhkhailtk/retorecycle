
#include "HAL/Button.h"
#define DEBOUNCE_TIME 50 // thời gian trống dội phím (ms)
#define LONG_PRESS_TIME 1000 // thời gian nhấn giữ lâu (ms)
Button::Button(int pin) : _pin(pin), _lastPressTime(0), _lastDebounceTime(0),
_lastReading(0),_isPressed(0), _clickFlag(0), _longPressIncrement(0),_lastIncrementTime(0) 
{
    pinMode(_pin,INPUT);
}

void Button::update() {
    // Đọc trạng thái hiện tại của nút nhấn (giả định HIGH = nhấn, LOW = nhả)
    bool reading = digitalRead(_pin);

    // --- 1. Xử lý Debounce ---
    // Nếu trạng thái thay đổi, reset bộ đếm debounce
    if (reading != _lastReading) {
        _lastDebounceTime = millis();
    }

    // Kiểm tra nếu đã qua thời gian DEBOUNCE_TIME
    if ((millis() - _lastDebounceTime) > DEBOUNCE_TIME) {

        // --- 2. Xử lý nhấn xuống (Press Down) ---
        // Nếu nút đang nhấn (HIGH) và chưa được ghi nhận là đang nhấn (_isPressed = 0)
        if (reading == HIGH && !_isPressed) {
            _isPressed = 1;
            _lastPressTime = millis(); // Ghi lại thời điểm bắt đầu nhấn
            _clickFlag = 0;
            _longPressIncrement = 0;
            _lastIncrementTime = millis(); // Khởi tạo thời điểm tăng
        }

        // --- 3. Xử lý Nhấn giữ lâu (Long Press Auto-Repeat) ---
        // Chỉ xử lý nếu nút đang được nhấn
        if (_isPressed) {
            unsigned long pressDuration = millis() - _lastPressTime;

            // Nếu vượt qua ngưỡng LONG_PRESS_TIME, bắt đầu chế độ tự động tăng
            if (pressDuration >= LONG_PRESS_TIME) {
                // Kiểm tra xem đã qua 500ms kể từ lần tăng cuối cùng chưa
                if ((millis() - _lastIncrementTime) >= 500) {
                    _longPressIncrement = 5;       // Gán giá trị tăng
                    _lastIncrementTime = millis(); // Cập nhật thời điểm tăng
                }
            }
        }

        // --- 4. Xử lý Nhả nút (Release) ---
        // Nếu nút đang nhả (LOW) và trước đó đã được ghi nhận là đang nhấn (_isPressed = 1)
        if (reading == LOW && _isPressed) {
            unsigned long pressDuration = millis() - _lastPressTime;
            _isPressed = 0; // Đặt lại trạng thái nhả

            // Phân biệt Click ngắn và Nhấn giữ lâu
            if (pressDuration < LONG_PRESS_TIME) {
                _clickFlag = 1; // Nhấn click ngắn
            } else {
                _clickFlag = 0; // Nhấn giữ lâu (sau khi nhả thì không tính là click ngắn nữa)
            }
            
            // Đặt lại các cờ và biến đếm sau khi nhả nút
            _longPressIncrement = 0;
            _lastIncrementTime = 0;
        }
    }
    
    // Ghi lại trạng thái đọc hiện tại cho lần lặp tiếp theo
    _lastReading = reading;
}

bool Button::isClicked() // Trả về true nếu vừa nhấn thả (click)
{
   if(_clickFlag)
   {
    _clickFlag = 0;
    return true;
   }
   return false;
}


int  Button::getLongPressIncrement() // Trả về 0, +5, hoặc -5
{
    int val = _longPressIncrement;
    _longPressIncrement = 0;
    return val;
}