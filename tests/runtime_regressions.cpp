#include "leak_monitor.hpp"
#include <cassert>
int main() {
 LeakMonitor monitor;
 monitor.ingest({100,10,true});
 assert(monitor.ingest({99,10,true}).state==LeakState::SensorFault);
 bool rejected=false;
 try {monitor.reset_fault({99,10,true});} catch(...) {rejected=true;}
 assert(rejected);
 monitor.reset_fault({101,10,true});
 assert(monitor.snapshot().state==LeakState::Dry);
}
