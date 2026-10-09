# Water Leak Monitor

Sensor reading analysis with filtering, hysteresis, temporal confirmation, sensor-failure detection, and an alarm latched until acknowledgement.

## Run

Requirements: C++20 and CMake.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
build/leak_monitor leituras.csv > result.json
```

## Behavior

CSV input: `timestamp_ms,adc`. The library exposes manual acknowledgement and data validation. Tests cover alarms, recovery, delays, and non-finite values. Physical actuation requires device integration and validation.

## Optional report archive

Use the [shared operations archive client](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/cloud) to queue `result.json` under project `cpp-water-leak-alarm`. The client uses `BRUNNODEV_ACCESS_TOKEN` and retains unacknowledged reports locally.

## License

Original source and documentation are MIT licensed; see [LICENSE](LICENSE). Third-party dependencies and media retain their respective terms. Maintained by [Brunno Dev](https://brunnodev.store).
