# Changelog

## WIP

- Removed `begin` methods, all parameters are now supplied into the constructor.
- Ensured calling the base class' constructor in derived classes.
- Added tests.

## 3.1.0 (2022-03-23)

- Add `reversed` parameter.
- Code cleanups.

## 3.0.0 (2021-12-15)

- Rename `HBridgeMotor` -> `HBridgeHighLowMotor`
- Add `PwmMotor` (single direction)
- Add `HBridgeSelectPwmMotor`
- Supports late initialization with `begin`. This is optional and can still be done
  in the constructor.
- replace `map_limit` function with `constrain`
- `brake` is now `false` by default

## 2.0.0 (2021-12-03)

- Changed interface!
  `applyElectric(int dir, int pwm)` -> `write(int dir, int pwm, bool brake)`
- Added examples.

## 1.0.0 (2021-11-03)

- Initial release
