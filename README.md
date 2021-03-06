# Arduino MotorDriver Library

[![tests](https://github.com/tfeldmann/Arduino-MotorDriver/actions/workflows/tests.yaml/badge.svg)](https://github.com/tfeldmann/Arduino-MotorDriver/actions/workflows/tests.yaml)

There are many different ways to drive a DC motor.

This library gives you an unified interface to control the following setups:

| Classname                  | Pins                           | Comment                                                  |
| -------------------------- | ------------------------------ | -------------------------------------------------------- |
| `MotorDriver`              | \*                             | Generic base class, define your own interface            |
| `PwmMotor`                 | `Pwm`                          | A single direction pwm motor controller                  |
| `DirPwmMotor`              | `Dir`, `Pwm`                   | A direction / pwm motor controller                       |
| `FwdBwdPwmMotor`           | `Dir FWD`, `Dir BWD`, `Pwm`    | Uses two pins to set the direction                       |
| `HBridgeHighLowMotor`      | `H1`, `L1`, `H2`, `L2`         | A H-bridge motor controller with high / low pwm controls |
| `HBridgeSelectPwmMotor`    | `Sel1`, `Pwm1`, `Sel2`, `Pwm2` | A H-bridge controller with select / pwm controls         |
| `HBridgeSoftDeadtimeMotor` | `H1`, `L1`, `H2`, `L2`         | A H-bridge motor controller with software deadtime       |

This allows you to change the motor driver chip / circuitry with minimal changes to your code.
