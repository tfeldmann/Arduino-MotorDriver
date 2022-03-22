#include "MotorDriver.h"

int8_t _sign(int x)
{
    if (x == 0)
        return 0;
    else if (x > 0)
        return 1;
    return -1;
}

// MotorDriver
// -------------------------------------------------------------------------------------

MotorDriver::MotorDriver(bool reversed)
    : reversed_(reversed)
{
}

int MotorDriver::speed()
{
    return speed_;
}

void MotorDriver::setSpeed(int speed)
{
    speed_ = speed;
    // splits the speed into a pwm (0...255) and a direction (-1...1)
    int dir = _sign(speed_);
    int pwm = abs(speed_);
    write(reversed_ ? -dir : dir, pwm, false);
}

bool MotorDriver::reversed()
{
    return reversed_;
}

void MotorDriver::setReversed(bool reversed)
{
    reversed_ = reversed;
}

void MotorDriver::stop(bool brake)
{
    speed_ = 0;
    write(0, 0, brake);
}

// PwmMotor
// -------------------------------------------------------------------------------------

PwmMotor::PwmMotor()
{
}

PwmMotor::PwmMotor(int pin_pwm, bool reversed)
{
    begin(pin_pwm, reversed);
}

void PwmMotor::begin(int pin_pwm, bool reversed)
{
    pin_pwm_ = pin_pwm;
    pinMode(pin_pwm_, OUTPUT);
    setReversed(reversed);
    stop();
}

void PwmMotor::write(int dir, int pwm, bool brake)
{
    analogWrite(pin_pwm_, pwm > 0 ? pwm : 0);
};

// DirPwmMotor
// -------------------------------------------------------------------------------------

DirPwmMotor::DirPwmMotor()
{
}

DirPwmMotor::DirPwmMotor(int pin_dir, int pin_pwm, bool reversed)
{
    begin(pin_dir, pin_pwm, reversed);
}

void DirPwmMotor::begin(int pin_dir, int pin_pwm, bool reversed)
{
    pin_dir_ = pin_dir;
    pin_pwm_ = pin_pwm;
    pinMode(pin_dir_, OUTPUT);
    pinMode(pin_pwm_, OUTPUT);
    setReversed(reversed);
    stop();
}

void DirPwmMotor::write(int dir, int pwm, bool brake)
{
    digitalWrite(pin_dir_, dir > 0);
    analogWrite(pin_pwm_, pwm);
};
