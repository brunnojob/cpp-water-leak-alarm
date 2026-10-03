#include <iostream>
#include <deque>
#include <string>
#include <cstdlib>

class LeakDetector {
    double threshold;
    int confirmations;
    std::deque<double> samples;
public:
    LeakDetector(double t, int n): threshold(t), confirmations(n) {}
    bool sample(double value) {
        samples.push_back(value);
        if (samples.size() > static_cast<size_t>(confirmations)) samples.pop_front();
        if (samples.size() < static_cast<size_t>(confirmations)) return false;
        for (double v : samples) if (v < threshold) return false;
        return true;
    }
};
int main(int argc, char **argv) {
    if (argc < 2) { std::cerr << "usage: leak_detector adc...\n"; return 2; }
    LeakDetector detector(650.0, 3);
    for (int i=1; i<argc; ++i) {
        char *end; double v=std::strtod(argv[i],&end);
        if (*end || v < 0 || v > 4095) { std::cerr<<"invalid ADC reading\n"; return 2; }
        if (detector.sample(v)) { std::cout<<"LEAK threshold sustained; close valve and inspect line\n"; return 1; }
    }
    std::cout<<"OK no sustained leak signal\n";
}