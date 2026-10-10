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

Use the [native C operations archive client](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/clients/c) to queue `result.json` under project `cpp-water-leak-alarm`. The client uses `BRUNNODEV_ACCESS_TOKEN` and retains unacknowledged reports locally.

## License

Original source and documentation are MIT licensed; see [LICENSE](LICENSE). Third-party dependencies and media retain their respective terms. Maintained by [Brunno Dev](https://brunnodev.store).

## Implementation update

Fault resets require a timestamp newer than the last accepted reading. A delayed or replayed reading cannot clear a clock fault. Native regression checks cover stale reset rejection and valid recovery.

Contribution trailer: `Co-authored-by: nyctophile <329826984+ineedfoundmyway@users.noreply.github.com>`.

## Execution proof

[![Executable proof](https://github.com/brunnojob/cpp-water-leak-alarm/actions/workflows/proof.yml/badge.svg)](https://github.com/brunnojob/cpp-water-leak-alarm/actions/workflows/proof.yml)

[Recorded execution and downloadable evidence](https://github.com/brunnojob/cpp-water-leak-alarm/actions/workflows/proof.yml)

Run `python .proof/record.py` after installing the prerequisites above. The scenarios execute repository code and verify exit codes and expected output. CI publishes `execution-proof` with the transcript, input fingerprints and source commit. The downloadable report identifies the exact tested version; the workflow badge tracks the latest run.
