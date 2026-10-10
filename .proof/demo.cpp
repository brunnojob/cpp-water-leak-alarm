#include "leak_monitor.hpp"
#include <cassert>
#include <iostream>
int main() {
    LeakMonitor monitor;
    auto first = monitor.ingest({0, 800, true});
    auto second = monitor.ingest({1000, 800, true});
    auto third = monitor.ingest({2000, 800, true});
    assert(first.state == LeakState::Suspected && second.state == LeakState::Suspected);
    assert(third.state == LeakState::Confirmed && third.alarm);
    auto stale = monitor.ingest({5001, 800, true});
    assert(stale.state == LeakState::SensorFault && stale.alarm);
    std::cout << "{\"sustained_leak_confirmed\":true,\"stale_sensor_alarm\":true,\"sequence\":" << stale.sequence << "}\n";
}
