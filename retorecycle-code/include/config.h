#define BTN_TEMP_UP_PIN A0 // BT1
#define BTN_TEMP_DOWN_PIN A1 // BT2
#define BTN_SPEED_UP_PIN A2 // BT3
#define BTN_SPEED_DOWN_PIN A3 //BT4
#define HEATER_PIN 9 // Chân PWM điều khiển Heater
#define MAX_SCK_PIN 13 // Chân SCK của cảm biến nhiệt độ MAX6675
#define MAX_CS_PIN 10  // Chân CS của cảm biến nhiệt độ MAX6675
#define MAX_SO_PIN 12  // Chân SO của cảm biến nhiệt độ MAX6675
#define MOTOR_STEP_PIN 3 // Chân STEP của động cơ bước
#define MOTOR_DIR_PIN 2  // Chân DIR của động cơ bước

#define LCD_ADDRESS 0x27
#define TEMP_MAX 400
#define TEMP_MIN 0
#define SPEED_MAX 200
#define SPEED_MIN 0

#define PID_INTERVAL_MS 200
#define LCD_INTERVAL_MS 1000

#define PID_KP 5
#define PID_KI 0
#define PID_KD 0