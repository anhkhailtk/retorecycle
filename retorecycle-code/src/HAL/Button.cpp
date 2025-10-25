#include "HAL/Button.h"

Button::Button(int pin) : _pin(pin), _lastPressTime(0), _state(0) {
}

void Button::update() {
    // Cập nhật trạng thái nút nhấn, xử lý debounce và long press
    // (Cần triển khai chi tiết ở đây)
}