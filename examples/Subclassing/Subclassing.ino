/**
 * This is an example on how to use the MotorDriver interface with an imaginary special
 * motor that is controlled via the serial port.
 **/
#include <MotorDriver.h>

class SerialMotor : public MotorDriver {
public:
    SerialMotor(int baudrate, bool reversed = false)
        : MotorDriver(reversed)
    {
        Serial.begin(baudrate);
    }

    void write(int dir, int pwm, bool brake) override
    {
        Serial.println(
            "Dir: " + String(dir)
            + ", PWM: " + String(pwm)
            + ", Brake: " + String(brake));
    }
};

SerialMotor motor(9600);

void setup()
{
}

void loop()
{
    motor.setSpeed(150);
    delay(1000);

    motor.stop();
    delay(1000);

    motor.setSpeed(-150);
    delay(1000);

    motor.stop();
    delay(1000);
}
