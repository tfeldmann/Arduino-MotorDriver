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
    FwdBwdPwmMotor(int pin_dir_fwd, int pin_dir_bwd, int pin_pwm, bool reversed);
    void begin(int pin_dir_fwd, int pin_dir_bwd, int pin_pwm, bool reversed);
    void write(int dir, int pwm, bool brake) override;

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
        bool reversed = false);
    void begin(
        int pin_a_high,
        int pin_a_low,
        int pin_b_high,
        int pin_b_low,
        int limit,
        bool reversed);
    void write(int dir, int pwm, bool brake) override;

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
        bool reversed = false);
    void begin(
        int pin_a_sel,
        int pin_a_pwm,
        int pin_b_sel,
        int pin_b_pwm,
        int limit = 255,
        bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

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
        bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int prev_dir_;
    int pin_m0_conn_vcc_;
    int pin_m0_disc_gnd_;
    int pin_m1_conn_vcc_;
    int pin_m1_disc_gnd_;
};
