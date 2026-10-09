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

Export a JSON report from the command above, then run `python cloud/sync.py enqueue result.json --project cpp-water-leak-alarm` and `python cloud/sync.py sync`. Synchronization requires `BRUNNODEV_ACCESS_TOKEN` and the external operations API; the local outbox retains unacknowledged reports.

## License

Original source and documentation are MIT licensed; see [LICENSE](LICENSE). Third-party dependencies and media retain their respective terms. Maintained by [Brunno Dev](https://brunnodev.store).
