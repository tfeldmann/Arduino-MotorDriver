#pragma once
#include <Arduino.h>

/**
 * The MotorDriver base class.
 * Subclass this for your own motor drivers.
 */
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
    int speed_;
    bool reversed_;
};

/**
 * A motor controlled with a single PWM signal
 */
class PwmMotor : public MotorDriver {
public:
    PwmMotor(int pin_pwm, bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int pin_pwm_;
};

/**
 * A motor controlled with a direction (H/L) and PWM signal.
 */
class DirPwmMotor : public MotorDriver {
public:
    DirPwmMotor(int pin_dir, int pin_pwm, bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int pin_dir_;
    int pin_pwm_;
};

/**
 * A motor controlled via two separate PWM signals for forward and backward
 */
class FwdBwdPwmMotor : public MotorDriver {
public:
    FwdBwdPwmMotor(int pin_dir_fwd, int pin_dir_bwd, int pin_pwm, bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int pin_dir_fwd_;
    int pin_dir_bwd_;
    int pin_pwm_;
};

/**
 * A motor controlled via a H-bridge, setting the individual current paths
 */
class HBridgeHighLowMotor : public MotorDriver {
public:
    HBridgeHighLowMotor(
        int pin_a_high,
        int pin_a_low,
        int pin_b_high,
        int pin_b_low,
        int limit = 255,
        bool reversed = false);
    void write(int dir, int pwm, bool brake) override;

private:
    int pin_a_high_;
    int pin_a_low_;
    int pin_b_high_;
    int pin_b_low_;

    int limit_;
};

/**
 * A motor controlled via an H-bridge with a "select + pwm" interface
 */
class HBridgeSelectPwmMotor : public MotorDriver {
public:
    HBridgeSelectPwmMotor(
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

/**
 * A motor controlled via an H-bridge with delays to prevent shorts
 */
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
