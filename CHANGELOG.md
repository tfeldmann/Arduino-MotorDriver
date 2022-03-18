# Changelog

## WIP

- Add `reversed` parameter.

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
