#include <MotorDriver.h>

DirPwmMotor motor(13, 14);

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
