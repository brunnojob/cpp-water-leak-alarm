#include "leak_monitor.hpp"
#include <cassert>
#include <limits>
int main() {
  LeakMonitor m;
  m.ingest({1000, 800});
  m.ingest({2000, 800});
  assert(m.ingest({3000, 800}).alarm);
  assert(m.ingest({4000, 0}).alarm);
  bool blocked = false;
  try {
    m.acknowledge(4000);
  } catch (const std::logic_error &) {
    blocked = true;
  }
  assert(blocked);
  for (std::uint64_t t = 5000; t <= 10000; t += 1000)
    m.ingest({t, 0});
  m.acknowledge(10000);
  assert(!m.snapshot().alarm);
  assert(m.ingest({9000, 0}).state == LeakState::SensorFault);
  m.reset_fault({11000, 0});
  assert(!m.snapshot().alarm);
  assert(m.ingest({12000, std::numeric_limits<double>::quiet_NaN()}).alarm);
}
