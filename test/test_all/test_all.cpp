#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

#include "MotorDriver.h"

class SPIMotor : public DirPwmMotor {
public:
    SPIMotor(int pin_cs, int pin_enable, int pin_dir, int pin_pwm)
        : DirPwmMotor(pin_dir, pin_pwm)
    {
        pin_cs_ = pin_cs;
        pin_enable_ = pin_enable;
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
    motor.setSpeed(200);
    motor.setSpeed(-200);
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 200));
    Verify(Method(ArduinoFake(), analogWrite).Using(11, 200));
}

void test_subclassed_motor()
{
    SPIMotor motor(8, 9, 11, 10);
    motor.stop();
    motor.setSpeed(200);
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(10, 200));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_pwm_motor);
    RUN_TEST(test_dir_pwm_motor);
    RUN_TEST(test_subclassed_motor);
    return UNITY_END();
}
