#include "MotorDriver.h"

int8_t sign(int x)
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
    int dir = sign(speed_);
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

// FwdBwdPwmMotor
// -------------------------------------------------------------------------------------

FwdBwdPwmMotor::FwdBwdPwmMotor()
{
}

FwdBwdPwmMotor::FwdBwdPwmMotor(
    int pin_dir_fwd,
    int pin_dir_bwd,
    int pin_pwm,
    bool reversed)
{
    begin(pin_dir_fwd, pin_dir_bwd, pin_pwm, reversed);
}

void FwdBwdPwmMotor::begin(
    int pin_dir_fwd,
    int pin_dir_bwd,
    int pin_pwm,
    bool reversed)
{
    pin_dir_fwd_ = pin_dir_fwd;
    pin_dir_bwd_ = pin_dir_bwd;
    pin_pwm_ = pin_pwm;
    pinMode(pin_dir_fwd_, OUTPUT);
    pinMode(pin_dir_bwd_, OUTPUT);
    pinMode(pin_pwm_, OUTPUT);
    setReversed(reversed);
    stop();
}

void FwdBwdPwmMotor::write(int dir, int pwm, bool brake)
{
    digitalWrite(pin_dir_fwd_, dir > 0);
    digitalWrite(pin_dir_bwd_, dir < 0);
    analogWrite(pin_pwm_, pwm);
};

// HBridgeHighLowMotor
// -------------------------------------------------------------------------------------

HBridgeHighLowMotor::HBridgeHighLowMotor()
{
}

HBridgeHighLowMotor::HBridgeHighLowMotor(
    int pin_a_high,
    int pin_a_low,
    int pin_b_high,
    int pin_b_low,
    int limit,
    bool reversed)
{
    begin(pin_a_high, pin_a_low, pin_b_high, pin_b_low, limit, reversed);
}

void HBridgeHighLowMotor::begin(
    int pin_a_high,
    int pin_a_low,
    int pin_b_high,
    int pin_b_low,
    int limit,
    bool reversed)
{
    pin_a_high_ = pin_a_high;
    pin_b_high_ = pin_b_high;
    pin_a_low_ = pin_a_low;
    pin_b_low_ = pin_b_low;
    pinMode(pin_a_high_, OUTPUT);
    pinMode(pin_b_high_, OUTPUT);
    pinMode(pin_a_low_, OUTPUT);
    pinMode(pin_b_low_, OUTPUT);
    setReversed(reversed);
    stop();

    limit_ = limit;
}

void HBridgeHighLowMotor::write(int dir, int pwm, bool brake)
{
    pwm = constrain(pwm, 0, limit_);
    switch (dir) {
    case 1:
        analogWrite(pin_a_high_, 0);
        analogWrite(pin_a_low_, pwm);
        analogWrite(pin_b_high_, pwm);
        analogWrite(pin_b_low_, 0);
        break;
    case 0:
        analogWrite(pin_a_high_, pwm);
        analogWrite(pin_a_low_, 0);
        analogWrite(pin_b_high_, pwm);
        analogWrite(pin_b_low_, 0);
        break;
    case -1:
        analogWrite(pin_a_high_, pwm);
        analogWrite(pin_a_low_, 0);
        analogWrite(pin_b_high_, 0);
        analogWrite(pin_b_low_, pwm);
        break;
    };
}

// HBridgeSelectPwmMotor
// -------------------------------------------------------------------------------------

HBridgeSelectPwmMotor::HBridgeSelectPwmMotor()
{
}

HBridgeSelectPwmMotor::HBridgeSelectPwmMotor(
    int pin_a_sel,
    int pin_a_pwm,
    int pin_b_sel,
    int pin_b_pwm,
    int limit,
    bool reversed)
{
    begin(pin_a_sel, pin_a_pwm, pin_b_sel, pin_b_pwm, limit, reversed);
}

void HBridgeSelectPwmMotor::begin(
    int pin_a_sel,
    int pin_a_pwm,
    int pin_b_sel,
    int pin_b_pwm,
    int limit,
    bool reversed)
{
    pin_a_sel_ = pin_a_sel;
    pin_b_sel_ = pin_b_sel;
    pin_a_pwm_ = pin_a_pwm;
    pin_b_pwm_ = pin_b_pwm;
    pinMode(pin_a_sel_, OUTPUT);
    pinMode(pin_a_pwm_, OUTPUT);
    pinMode(pin_b_sel_, OUTPUT);
    pinMode(pin_b_pwm_, OUTPUT);
    setReversed(reversed);
    stop();

    limit_ = limit;
}

void HBridgeSelectPwmMotor::write(int dir, int pwm, bool brake)
{
    pwm = constrain(pwm, 0, limit_);
    switch (dir) {
    case 1:
        digitalWrite(pin_a_sel_, HIGH);
        digitalWrite(pin_b_sel_, LOW);
        analogWrite(pin_a_pwm_, pwm);
        analogWrite(pin_b_pwm_, pwm);
        break;
    case 0:
        digitalWrite(pin_a_sel_, LOW);
        digitalWrite(pin_b_sel_, LOW);
        analogWrite(pin_a_pwm_, 0);
        analogWrite(pin_b_pwm_, 0);
        break;
    case -1:
        digitalWrite(pin_a_sel_, LOW);
        digitalWrite(pin_b_sel_, HIGH);
        analogWrite(pin_a_pwm_, pwm);
        analogWrite(pin_b_pwm_, pwm);
        break;
    };
}

// HBridgeSoftDeadtimeMotor
// -------------------------------------------------------------------------------------

HBridgeSoftDeadtimeMotor::HBridgeSoftDeadtimeMotor(
    int pin_m0_conn_vcc,
    int pin_m0_disc_gnd,
    int pin_m1_conn_vcc,
    int pin_m1_disc_gnd,
    bool reversed)
{
    pin_m0_conn_vcc_ = pin_m0_conn_vcc;
    pin_m0_disc_gnd_ = pin_m0_disc_gnd;
    pin_m1_conn_vcc_ = pin_m1_conn_vcc;
    pin_m1_disc_gnd_ = pin_m1_disc_gnd;
    pinMode(pin_m0_conn_vcc, OUTPUT);
    pinMode(pin_m0_disc_gnd, OUTPUT);
    pinMode(pin_m1_conn_vcc, OUTPUT);
    pinMode(pin_m1_disc_gnd, OUTPUT);
    setReversed(reversed);
    stop();
    prev_dir_ = 0;
}

void HBridgeSoftDeadtimeMotor::write(int dir, int pwm, bool brake)
{
    // make electrical changes to change direction or brake
    bool dir_changed = (dir != prev_dir_);
    if (dir_changed) {
        switch (dir) {
        case 1: // FORWARD
            analogWrite(pin_m1_disc_gnd_, 255);
            delay(1);
            analogWrite(pin_m0_conn_vcc_, 0);
            analogWrite(pin_m1_conn_vcc_, pwm);
            analogWrite(pin_m0_disc_gnd_, 0);
            delay(1);
            break;
        case 0: // BRAKING
            analogWrite(pin_m0_conn_vcc_, 0);
            analogWrite(pin_m0_disc_gnd_, 0);
            delay(1);
            analogWrite(pin_m1_conn_vcc_, 0);
            analogWrite(pin_m1_disc_gnd_, 0);
            delay(1);
            break;
        case -1: // BACKWARD
            analogWrite(pin_m0_disc_gnd_, 255);
            delay(1);
            analogWrite(pin_m1_conn_vcc_, 0);
            analogWrite(pin_m0_conn_vcc_, pwm);
            analogWrite(pin_m1_disc_gnd_, 0);
            delay(1);
            break;
        }
    }

    if (dir == 1)
        analogWrite(pin_m1_conn_vcc_, pwm); // FORWARD
    else if (dir == -1)
        analogWrite(pin_m0_conn_vcc_, pwm); // BACKWARD

    prev_dir_ = dir;
}
