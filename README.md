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

## Result synchronization

The [operations archive](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=cpp-water-leak-alarm) stores execution results. Supabase migrations are in the [API repository](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue result.json --project cpp-water-leak-alarm
python cloud/sync.py sync
```

Set `BRUNNODEV_ACCESS_TOKEN` to your session token. The SQLite outbox retains reports until the server confirms persistence; identical content does not create duplicate records. Tokens are not stored in source code. To run the synchronization tests:

```sh
python -m unittest discover -s cloud
```
