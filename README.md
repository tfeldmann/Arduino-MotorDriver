# Arduino MotorDriver Library

There are many different ways to drive a DC motor.

This library gives you an unified way to control the following interfaces:

| Classname                  | Pins                        | Comment                                            |
| -------------------------- | --------------------------- | -------------------------------------------------- |
| `MotorDriver`              | \*                          | Generic base class, define your own interface      |
| `DirPwmMotor`              | `Dir`, `Pwm`                | A direction / pwm motor controller                 |
| `FwdBwdPwmMotor`           | `Dir FWD`, `Dir BWD`, `Pwm` | Uses two pins to set the direction                 |
| `HBridgeMotor`             | `H1`, `L1`, `H2`, `L2`      | A H-bridge motor controller                        |
| `HBridgeSoftDeadtimeMotor` | `H1`, `L1`, `H2`, `L2`      | A H-bridge motor controller with software deadtime |

This allows you to change the motor driver chip / circuitry with minimal changes to your code.

Use at your own risk.
