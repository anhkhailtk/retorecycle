#include <Arduino.h>
// Button.h
class Button {
public:
    Button(int pin);
    void update(); // Gọi liên tục trong ui_task
    
    bool isClicked();     // Trả về true nếu vừa nhấn thả (click)
    int getLongPressIncrement(); // Trả về 0, +5, hoặc -5

private:
    int _pin;
    unsigned long _lastPressTime;
    uint8_t _state;
    // ... các biến khác để xử lý debounce và long press
};