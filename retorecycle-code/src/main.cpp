#include "config.h"
#include <Arduino.h>
#include <PID_v1.h>
#include <max6675.h>
#include "HAL/Button.h"
#include "HAL/Display.h"
#include "HAL/Heater.h"
#include "HAL/TempSensor.h"
#include "HAL/Motor.h"

// // === Biến trạng thái toàn cục ===
double setpointTemp = 0.0;
double currentTemp = 0.0;
double pidOutput = 0.0; // Công suất 0-255

double setpointSpeed = 0.0; // steps/sec
double currentSpeed = 0.0;

// === Khởi tạo các đối tượng ===
// (Lưu ý: ATmega8 không có I2C_SDA, I2C_SCL.
// Nó dùng chân A4 (SDA) và A5 (SCL) cố định)
Display display(LCD_ADDRESS);
Heater heater(HEATER_PIN);
TempSensor sensor(MAX_SCK_PIN, MAX_CS_PIN, MAX_SO_PIN);
Motor motor(MOTOR_STEP_PIN, MOTOR_DIR_PIN);

// // Các nút nhấn
Button btnTempUp(BTN_TEMP_UP_PIN);
Button btnTempDw(BTN_TEMP_DOWN_PIN);
Button btnSpeedUp(BTN_SPEED_UP_PIN);
Button btnSpeedDw(BTN_SPEED_DOWN_PIN);

// PID
PID tempPID(&currentTemp, &pidOutput, &setpointTemp, PID_KP, PID_KI, PID_KD, DIRECT);

// // === Biến hẹn giờ (Timer variables) ===
unsigned long lastPidTime = 0;
unsigned long lastLcdTime = 0;

void setup()
{
    display.init();
    heater.init();
    motor.init(); // Quan trọng: Cấu hình Timer1 ở đây

    tempPID.SetMode(AUTOMATIC);
    tempPID.SetOutputLimits(0, 255); // PWM 8-bit (0-255)
    tempPID.SetSampleTime(PID_INTERVAL_MS);
}

// === HÀM NGẮT TIMER CỦA ĐỘNG CƠ ===
// Đây là "Task" ưu tiên cao nhất, chạy ngầm
ISR(TIMER2_COMP_vect)
{
    if (motor.getTotalTimerCounter() == 0)
        return;
    static unsigned long timerCounter = 0;
    timerCounter++; // Tăng bộ đếm mỗi khi vào ISR

    if (timerCounter >= motor.getTotalTimerCounter())
    {
        motor.pulse();
        timerCounter = 0;
    }
}

// === VÒNG LẶP CHÍNH (SUPER-LOOP) ===
void loop()
{
    unsigned long now = millis();

    // 1. Xử lý nút nhấn (Chạy mỗi vòng lặp)
    btnTempUp.update();
    btnTempDw.update();
    btnSpeedUp.update();
    btnSpeedDw.update();
    // 1. Kiểm tra nhấn giữ lâu trước (tăng +5)
    int tempUpInc = btnTempUp.getLongPressIncrement(); // BT1
    if (tempUpInc != 0)
    {
        if (setpointTemp + tempUpInc <= TEMP_MAX)
        {
            setpointTemp += tempUpInc; // Tăng +5
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointTemp = TEMP_MAX;
        }
    }
    // 2. Kiểm tra click ngắn (tăng +1)
    else if (btnTempUp.isClicked())
    {
        // Kiểm tra xem việc cộng +1 có vượt quá giới hạn không
        if (setpointTemp + 1.0 <= TEMP_MAX)
        {
            setpointTemp += 1.0; // Tăng +1
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointTemp = TEMP_MAX;
        }
    }

    int tempDwInc = btnTempDw.getLongPressIncrement(); // BT2
    if (tempDwInc != 0)
    {
        if (setpointTemp - tempDwInc >= TEMP_MIN)
        {
            setpointTemp -= tempDwInc; // Giảm 5
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointTemp = TEMP_MIN;
        }
    }
    else if (btnTempDw.isClicked())
    {
        // Kiểm tra xem việc cộng +1 có vượt quá giới hạn không
        if (setpointTemp - 1.0 >= TEMP_MIN)
        {
            setpointTemp -= 1.0; // Giảm 1
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointTemp = TEMP_MIN;
        }
    }

    int speedUpInc = btnSpeedUp.getLongPressIncrement(); // BT3
    if (speedUpInc != 0)
    {
        if (setpointSpeed + speedUpInc <= SPEED_MAX)
        {
            setpointSpeed += speedUpInc; // Tăng +5
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointSpeed = SPEED_MAX;
        }
    }
    else if (btnSpeedUp.isClicked())
    {
        // Kiểm tra xem việc cộng +1 có vượt quá giới hạn không
        if (setpointSpeed + 1.0 <= SPEED_MAX)
        {
            setpointSpeed += 1.0; // Tăng +1
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointSpeed = SPEED_MAX;
        }
    }

    int speedDwInc = btnSpeedDw.getLongPressIncrement(); // BT4
    if (speedDwInc != 0)
    {
        if (setpointSpeed - speedDwInc >= SPEED_MIN)
        {
            setpointSpeed -= speedDwInc; // Giảm 5
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointSpeed = SPEED_MIN;
        }
    }
    else if (btnSpeedDw.isClicked())
    {
        // Kiểm tra xem việc cộng +1 có vượt quá giới hạn không
        if (setpointSpeed - 1.0 >= SPEED_MIN)
        {
            setpointSpeed -= 1.0; // Giảm 1
        }
        else
        {
            // Nếu vượt quá, chỉ đặt bằng giá trị tối đa
            setpointSpeed = SPEED_MIN;
        }
    }

    // 2. Cập nhật tốc độ motor (Chạy mỗi vòng lặp)
    // (Vì hàm motor.setSpeed() rất nhẹ, chỉ tính toán và ghi 1 thanh ghi)
    motor.setSpeedRpm(setpointSpeed);          // Sẽ cập nhật OCR1A nếu cần
    currentSpeed = motor.getCurrentSpeedRpm(); // Giả sử tốc độ đạt ngay lập tức

    // 3. Tác vụ PID & Nhiệt độ (Chạy định kỳ)
    if (now - lastPidTime >= PID_INTERVAL_MS)
    {
        lastPidTime = now;

        currentTemp = sensor.readCelsius();

        tempPID.Compute();
        heater.setPower((int)pidOutput);
    }

    // 4. Tác vụ LCD (Chạy định kỳ)
    if (now - lastLcdTime >= LCD_INTERVAL_MS)
    {
        lastLcdTime = now;
        // Truyền trạng thái hiện tại cho LCD
        display.update(currentTemp, setpointTemp, currentSpeed, setpointSpeed);
    }
}