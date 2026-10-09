#include "leak_monitor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "usage: leak-monitor readings.csv (timestamp_ms,adc)\n";
    return 2;
  }
  try {
    std::ifstream file(argv[1]);
    if (!file)
      throw std::runtime_error("cannot open readings");
    LeakMonitor monitor;
    std::string line;
    std::size_t count = 0;
    while (std::getline(file, line)) {
      std::istringstream row(line);
      std::uint64_t timestamp;
      double adc;
      char comma, extra;
      if (!(row >> timestamp >> comma >> adc) || comma != ',' || row >> extra)
        throw std::invalid_argument("invalid reading");
      monitor.ingest({timestamp, adc, true});
      count++;
    }
    auto s = monitor.snapshot();
    std::cout << "{\"state\":" << int(s.state)
              << ",\"alarm\":" << (s.alarm ? "true" : "false")
              << ",\"filtered\":" << s.filtered
              << ",\"transitions\":" << s.sequence << ",\"samples\":" << count
              << "}\n";
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 2;
  }
}
