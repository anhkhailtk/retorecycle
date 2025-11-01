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
    unsigned long _lastDebounceTime;
    bool _lastReading;
    bool _isPressed;
    bool _clickFlag;
    int _longPressIncrement;
    unsigned long _lastIncrementTime;
    // ... các biến khác để xử lý debounce và long press
};