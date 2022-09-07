#include "MotorDriver.h"
#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

class SPIMotor : public DirPwmMotor {
public:
    SPIMotor(int pin_cs, int pin_enable, int pin_dir, int pin_pwm, bool reversed = false)
        : DirPwmMotor(pin_dir, pin_pwm, reversed)
    {
        pin_cs_ = pin_cs;
        pin_enable_ = pin_enable;
        pinMode(pin_cs_, OUTPUT);
        pinMode(pin_enable_, OUTPUT);
        digitalWrite(pin_cs_, LOW);
        digitalWrite(pin_enable_, HIGH);
    }

    int pin_cs_;
    int pin_enable_;
};

void setUp(void)
{
    ArduinoFakeReset();
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
}

void test_pwm_motor()
{
    PwmMotor motor(10);
    motor.stop();
    motor.setSpeed(200);
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 200));
}

void test_dir_pwm_motor()
{
    DirPwmMotor motor(10, 11);
    motor.stop();
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 0));

    motor.setSpeed(200);
    Verify(Method(ArduinoFake(), digitalWrite).Using(10, HIGH));
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 200));

    motor.setSpeed(-200);
    Verify(Method(ArduinoFake(), digitalWrite).Using(10, LOW));
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 200));
}

void test_dir_pwm_motor_reversed()
{
    DirPwmMotor motor(10, 11);
    motor.stop();
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 0));

    motor.setSpeed(200);
    Verify(Method(ArduinoFake(), digitalWrite).Using(10, LOW));
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 200));

    motor.setSpeed(-200);
    Verify(Method(ArduinoFake(), digitalWrite).Using(10, HIGH));
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 200));
}

void test_subclassed_motor()
{
    SPIMotor motor(8, 9, 11, 10);
    motor.stop();
    motor.setSpeed(200);
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 200));
    Verify(Method(ArduinoFake(), digitalWrite).Using(8, LOW));
    Verify(Method(ArduinoFake(), digitalWrite).Using(9, HIGH));
}

void test_subclassed_motor_new()
{
    MotorDriver* motor;
    motor = new SPIMotor(8, 9, 11, 10);
    Verify(Method(ArduinoFake(), pinMode).Using(11, OUTPUT));
    Verify(Method(ArduinoFake(), pinMode).Using(10, OUTPUT));
    Verify(Method(ArduinoFake(), digitalWrite).Using(8, LOW));
    Verify(Method(ArduinoFake(), digitalWrite).Using(9, HIGH));
    motor->stop();
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 0));
    motor->setSpeed(200);
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 200));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_pwm_motor);
    RUN_TEST(test_dir_pwm_motor);
    RUN_TEST(test_dir_pwm_motor_reversed);
    RUN_TEST(test_subclassed_motor);
    RUN_TEST(test_subclassed_motor_new);
    return UNITY_END();
}
