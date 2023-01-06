# Arduino MotorDriver Library

[![tests](https://github.com/tfeldmann/Arduino-MotorDriver/actions/workflows/tests.yaml/badge.svg)](https://github.com/tfeldmann/Arduino-MotorDriver/actions/workflows/tests.yaml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/tfeldmann/library/MotorDriver.svg)](https://registry.platformio.org/libraries/tfeldmann/MotorDriver)

This library let's you easily switch between different methods of controlling motors.

Assume you have the following code which controls a DC motor via an PWM and direction pin:

```c
// code without MotorDriver library

const int MOTOR_DIR_PIN = 5;
const int MOTOR_PWM_PIN = 6;

void setup() {
    pinMode(MOTOR_DIR_PIN, OUTPUT);
    pinMode(MOTOR_PWM_PIN, OUTPUT);
}

void loop() {
    // forward
    digitalWrite(MOTOR_DIR_PIN, HIGH);
    analogWrite(MOTOR_PWM_PIN, 255);
    delay(1000);

    // stop
    analogWrite(MOTOR_PWM_PIN, 0);
    delay(1000);

    // backward
    digitalWrite(MOTOR_DIR_PIN, LOW);
    analogWrite(MOTOR_PWM_PIN, 255);
    delay(1000);

    // stop
    analogWrite(MOTOR_PWM_PIN, 0);
    delay(1000);
}
```

With this library this can be simplified to:

```c
#include <MotorDriver.h>

DirPwmMotor motor(5, 6);

void setup() {}

void loop() {
    motor.setSpeed(255);
    delay(1000);

    motor.stop();
    delay(1000);

    motor.setSpeed(-255);
    delay(1000);

    motor.stop();
    delay(1000);
}
```

Now imagine you change the motor control circuit and now have two
separate direction pins FWD and BWD and a PWM pin for speed.
With this library you only need to change the `DirPwmMotor` to `FwdBwdPwmMotor`:

```diff
-DirPwmMotor motor(5, 6);
+FwdBwdPwmMotor motor(4, 5, 6);
```

A `MotorDriver` has a super simple api:

```c
int speed();
void setSpeed(int speed);
bool reversed();
void setReversed(bool reversed);
void stop(bool brake = false);
```

The following implementations are available:

| Classname                  | Pins                           | Comment                                                  |
| -------------------------- | ------------------------------ | -------------------------------------------------------- |
| `MotorDriver`              | \*                             | Generic base class, define your own interface            |
| `PwmMotor`                 | `Pwm`                          | A single direction pwm motor controller                  |
| `DirPwmMotor`              | `Dir`, `Pwm`                   | A direction / pwm motor controller                       |
| `FwdBwdPwmMotor`           | `Dir FWD`, `Dir BWD`, `Pwm`    | Uses two pins to set the direction                       |
| `HBridgeHighLowMotor`      | `H1`, `L1`, `H2`, `L2`         | A H-bridge motor controller with high / low pwm controls |
| `HBridgeSelectPwmMotor`    | `Sel1`, `Pwm1`, `Sel2`, `Pwm2` | A H-bridge controller with select / pwm controls         |
| `HBridgeSoftDeadtimeMotor` | `H1`, `L1`, `H2`, `L2`         | A H-bridge motor controller with software deadtime       |

PRs / issues welcome!
