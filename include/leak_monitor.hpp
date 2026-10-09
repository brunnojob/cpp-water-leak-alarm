#pragma once
#include <cmath>
#include <cstdint>
#include <stdexcept>
enum class LeakState { Dry, Suspected, Confirmed, SensorFault };
struct LeakConfig {
  double trip = 650, reset = 500;
  std::uint64_t confirmMs = 2000, clearMs = 5000, maxGapMs = 2000;
};
struct LeakSample {
  std::uint64_t timestamp;
  double adc;
  bool quality = true;
};
struct LeakStatus {
  LeakState state;
  bool alarm;
  double filtered;
  std::uint64_t sequence;
};
class LeakMonitor {
  LeakConfig config_;
  LeakState state_ = LeakState::Dry;
  std::uint64_t previous_ = 0, since_ = 0, sequence_ = 0, drySince_ = 0;
  bool initialized_ = false, drying_ = false;
  double filtered_ = 0;
  void transition(LeakState next, std::uint64_t now) {
    if (state_ != next) {
      state_ = next;
      since_ = now;
      sequence_++;
    }
  }

public:
  explicit LeakMonitor(LeakConfig c = {}) : config_(c) {
    if (!std::isfinite(c.trip) || !std::isfinite(c.reset) || c.reset < 0 ||
        c.trip > 4095 || c.reset >= c.trip || !c.confirmMs || !c.clearMs ||
        !c.maxGapMs)
      throw std::invalid_argument("invalid thresholds");
  }
  LeakStatus ingest(LeakSample s) {
    bool stale = initialized_ && (s.timestamp <= previous_ ||
                                  s.timestamp - previous_ > config_.maxGapMs);
    if (!s.quality || !std::isfinite(s.adc) || s.adc < 0 || s.adc > 4095 ||
        stale) {
      transition(LeakState::SensorFault, s.timestamp);
      drying_ = false;
      return snapshot();
    }
    filtered_ = initialized_ ? filtered_ + 0.5 * (s.adc - filtered_) : s.adc;
    previous_ = s.timestamp;
    initialized_ = true;
    if (state_ == LeakState::SensorFault)
      return snapshot();
    if (filtered_ >= config_.trip) {
      drying_ = false;
      if (state_ == LeakState::Dry)
        transition(LeakState::Suspected, s.timestamp);
      if (state_ == LeakState::Suspected &&
          s.timestamp - since_ >= config_.confirmMs)
        transition(LeakState::Confirmed, s.timestamp);
    } else if (filtered_ <= config_.reset) {
      if (!drying_) {
        drySince_ = s.timestamp;
        drying_ = true;
      }
      if (state_ == LeakState::Suspected)
        transition(LeakState::Dry, s.timestamp);
    } else
      drying_ = false;
    return snapshot();
  }
  void acknowledge(std::uint64_t now) {
    if (!initialized_ || !drying_ || now < previous_ ||
        now - previous_ > config_.maxGapMs || now < drySince_ ||
        now - drySince_ < config_.clearMs)
      throw std::logic_error("stable dry signal required");
    transition(LeakState::Dry, now);
    drying_ = false;
  }
  void reset_fault(LeakSample s) {
    if (state_ != LeakState::SensorFault || !s.quality ||
        !std::isfinite(s.adc) || s.adc < 0 || s.adc > config_.reset)
      throw std::logic_error("valid dry sample required");
    initialized_ = false;
    transition(LeakState::Dry, s.timestamp);
    ingest(s);
  }
  LeakStatus snapshot() const {
    return {state_,
            state_ == LeakState::Confirmed || state_ == LeakState::SensorFault,
            filtered_, sequence_};
  }
};
