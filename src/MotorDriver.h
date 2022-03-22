#pragma once
#include <Arduino.h>

class MotorDriver {
public:
    MotorDriver(bool reversed = false);
    virtual int speed();
    virtual void setSpeed(int speed);
    virtual bool reversed();
    virtual void setReversed(bool reversed);
    virtual void stop(bool brake = false);

    // implement this function in your driver
    virtual void write(int dir, int pwm, bool brake = false) = 0;

private:
    int speed_ = 0;
    bool reversed_ = false;
};

class PwmMotor : public MotorDriver {
public:
    PwmMotor();
    PwmMotor(int pin_pwm, bool reversed = false);
    void begin(int pin_pwm, bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int pin_pwm_;
};

class DirPwmMotor : public MotorDriver {
public:
    DirPwmMotor();
    DirPwmMotor(int pin_dir, int pin_pwm, bool reversed = false);
    void begin(int pin_dir, int pin_pwm, bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int pin_dir_;
    int pin_pwm_;
};

class FwdBwdPwmMotor : public MotorDriver {
public:
    FwdBwdPwmMotor();
    FwdBwdPwmMotor(
        int pin_dir_fwd,
        int pin_dir_bwd,
        int pin_pwm,
        bool reversed = false)
    {
        begin(pin_dir_fwd, pin_dir_bwd, pin_pwm, reversed);
    }

    void begin(
        int pin_dir_fwd,
        int pin_dir_bwd,
        int pin_pwm,
        bool reversed = false)
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

    void write(int dir, int pwm, bool brake) override
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

class HBridgeHighLowMotor : public MotorDriver {
public:
    HBridgeHighLowMotor();
    HBridgeHighLowMotor(
        int pin_a_high,
        int pin_a_low,
        int pin_b_high,
        int pin_b_low,
        int limit = 255,
        bool reversed = false)
    {
        begin(pin_a_high, pin_a_low, pin_b_high, pin_b_low, limit, reversed);
    }

    void begin(
        int pin_a_high,
        int pin_a_low,
        int pin_b_high,
        int pin_b_low,
        int limit = 255,
        bool reversed = false)
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

    void write(int dir, int pwm, bool brake) override
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

private:
    int pin_a_high_;
    int pin_a_low_;
    int pin_b_high_;
    int pin_b_low_;

    int limit_;
};

class HBridgeSelectPwmMotor : public MotorDriver {
public:
    HBridgeSelectPwmMotor();
    HBridgeSelectPwmMotor(
        int pin_a_sel,
        int pin_a_pwm,
        int pin_b_sel,
        int pin_b_pwm,
        int limit = 255,
        bool reversed = false)
    {
        begin(pin_a_sel, pin_a_pwm, pin_b_sel, pin_b_pwm, limit, reversed);
    }

    void begin(
        int pin_a_sel,
        int pin_a_pwm,
        int pin_b_sel,
        int pin_b_pwm,
        int limit = 255,
        bool reversed = false)
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

    void write(int dir, int pwm, bool brake) override
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

private:
    int pin_a_sel_;
    int pin_a_pwm_;
    int pin_b_sel_;
    int pin_b_pwm_;

    int limit_;
};

class HBridgeSoftDeadtimeMotor : public MotorDriver {
public:
    HBridgeSoftDeadtimeMotor(
        int pin_m0_conn_vcc,
        int pin_m0_disc_gnd,
        int pin_m1_conn_vcc,
        int pin_m1_disc_gnd,
        bool reversed = false)
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

    void write(int dir, int pwm, bool brake) override
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

private:
    int prev_dir_;
    int pin_m0_conn_vcc_;
    int pin_m0_disc_gnd_;
    int pin_m1_conn_vcc_;
    int pin_m1_disc_gnd_;
};
