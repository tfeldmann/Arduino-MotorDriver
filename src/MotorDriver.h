#pragma once
#include <Arduino.h>

inline int8_t sign(int x)
{
    if (x == 0)
        return 0;
    else if (x > 0)
        return 1;
    return -1;
}

class MotorDriver
{
public:
    Motor()
    {
        speed_ = 0;
    }

    int speed()
    {
        return speed_;
    }

    void set_speed(int speed)
    {
        speed_ = constrain(speed, -255, 255);
        update();
    }

    void update()
    {
        int dir = sign(speed_);
        int pwm = abs(speed_);
        apply_electric(dir, pwm);
    };

    virtual void apply_electric(int dir, int pwm) = 0;

private:
    int speed_;
};

class DirPwmMotor : public MotorDriver
{
public:
    DirPwmMotor(int pin_dir, int pin_pwm)
    {
        pin_dir_ = pin_dir_fwd;
        pin_pwm_ = pin_pwm;
        pinMode(pin_dir_, OUTPUT);
        pinMode(pin_pwm_, OUTPUT);
        set_speed(0);
    }

    void apply_electric(int dir, int pwm) override
    {
        digitalWrite(pin_dir_, dir > 0);
        analogWrite(pin_pwm_, pwm);
    };

private:
    int pin_dir_;
    int pin_pwm_;
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
        set_speed(0);
    }

    void apply_electric(int dir, int pwm) override
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
        set_speed(0);

        limit_ = limit;
    }

    void apply_electric(int dir, int pwm) override
    {
        pwm = mapLimit(pwm, 0, 255, 0, limit_);
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
        set_speed(0);
        prev_dir_ = 0;
    }

    void apply_electric(int dir, int pwm) override
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
