#pragma once
#include <Arduino.h>

inline int8_t _sign(int x)
{
    if (x == 0)
        return 0;
    else if (x > 0)
        return 1;
    return -1;
}

inline long _mapLimit(long x, long in_min, long in_max, long out_min, long out_max)
{
    long lower_limit = min(out_min, out_max);
    long upper_limit = max(out_min, out_max);
    long value = map(x, in_min, in_max, out_min, out_max);
    return constrain(value, lower_limit, upper_limit);
}

class MotorDriver
{
public:
    virtual int speed()
    {
        return speed_;
    }

    virtual void setSpeed(int speed)
    {
        speed_ = constrain(speed, -255, 255);
        // splits the speed into a pwm (0...255) and a direction (-1...1)
        int dir = _sign(speed_);
        int pwm = abs(speed_);
        applyElectric(dir, pwm);
    }

    virtual void stop()
    {
        setSpeed(0);
    }

    // implement this function in your driver
    virtual void applyElectric(int dir, int pwm) = 0;

private:
    int speed_ = 0;
};

class DirPwmMotor : public MotorDriver
{
public:
    DirPwmMotor(int pin_dir, int pin_pwm, bool reverse_direction = false)
    {
        pin_dir_ = pin_dir;
        pin_pwm_ = pin_pwm;
        reverse_direction_ = reverse_direction;
        pinMode(pin_dir_, OUTPUT);
        pinMode(pin_pwm_, OUTPUT);
        stop();
    }

    void applyElectric(int dir, int pwm) override
    {
        digitalWrite(pin_dir_, (dir > 0) ^ reverse_direction_);
        analogWrite(pin_pwm_, pwm);
    };

private:
    int pin_dir_;
    int pin_pwm_;
    bool reverse_direction_;
};

class FwdBwdPwmMotor : public MotorDriver
{
public:
    FwdBwdPwmMotor(int pin_dir_fwd, int pin_dir_bwd, int pin_pwm)
    {
        pin_dir_fwd_ = pin_dir_fwd;
        pin_dir_bwd_ = pin_dir_bwd;
        pin_pwm_ = pin_pwm;
        pinMode(pin_dir_fwd_, OUTPUT);
        pinMode(pin_dir_bwd_, OUTPUT);
        pinMode(pin_pwm_, OUTPUT);
        stop();
    }

    void applyElectric(int dir, int pwm) override
    {
        digitalWrite(pin_dir_fwd_, dir > 0);
        digitalWrite(pin_dir_bwd_, dir < 0);
        analogWrite(pin_pwm_, pwm);
    };

private:
    int pin_dir_fwd_;
    int pin_dir_bwd_;
    int pin_pwm_;
};

class HBridgeMotor : public MotorDriver
{
public:
    HBridgeMotor(
        int pin_high1, int pin_low1, int pin_high2, int pin_low2, int limit = 255)
    {
        pin_high1_ = pin_high1;
        pin_high2_ = pin_high2;
        pin_low1_ = pin_low1;
        pin_low2_ = pin_low2;
        pinMode(pin_high1_, OUTPUT);
        pinMode(pin_high2_, OUTPUT);
        pinMode(pin_low1_, OUTPUT);
        pinMode(pin_low2_, OUTPUT);
        stop();

        limit_ = limit;
    }

    void applyElectric(int dir, int pwm) override
    {
        pwm = _mapLimit(pwm, 0, 255, 0, limit_);
        switch (dir)
        {
        case 1:
            analogWrite(pin_high1_, 0);
            analogWrite(pin_low1_, pwm);
            analogWrite(pin_high2_, pwm);
            analogWrite(pin_low2_, 0);
            break;
        case 0:
            analogWrite(pin_high1_, pwm);
            analogWrite(pin_low1_, 0);
            analogWrite(pin_high2_, pwm);
            analogWrite(pin_low2_, 0);
            break;
        case -1:
            analogWrite(pin_high1_, pwm);
            analogWrite(pin_low1_, 0);
            analogWrite(pin_high2_, 0);
            analogWrite(pin_low2_, pwm);
            break;
        };
    }

private:
    int pin_high1_;
    int pin_low1_;
    int pin_high2_;
    int pin_low2_;

    int limit_;
};

class HBridgeSoftDeadtimeMotor : public MotorDriver
{
public:
    HBridgeSoftDeadtimeMotor(
        int pin_m0_conn_vcc,
        int pin_m0_disc_gnd,
        int pin_m1_conn_vcc,
        int pin_m1_disc_gnd)
    {
        pin_m0_conn_vcc_ = pin_m0_conn_vcc;
        pin_m0_disc_gnd_ = pin_m0_disc_gnd;
        pin_m1_conn_vcc_ = pin_m1_conn_vcc;
        pin_m1_disc_gnd_ = pin_m1_disc_gnd;
        pinMode(pin_m0_conn_vcc, OUTPUT);
        pinMode(pin_m0_disc_gnd, OUTPUT);
        pinMode(pin_m1_conn_vcc, OUTPUT);
        pinMode(pin_m1_disc_gnd, OUTPUT);
        stop();
        prev_dir_ = 0;
    }

    void applyElectric(int dir, int pwm) override
    {
        // make electrical changes to change direction or brake
        bool dir_changed = (dir != prev_dir_);
        if (dir_changed)
        {
            switch (dir)
            {
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

private:
    int prev_dir_;
    int pin_m0_conn_vcc_;
    int pin_m0_disc_gnd_;
    int pin_m1_conn_vcc_;
    int pin_m1_disc_gnd_;
};
