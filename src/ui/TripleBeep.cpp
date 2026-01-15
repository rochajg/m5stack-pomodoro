#include "TripleBeep.h"

#include <M5Unified.h>

namespace ui {

const TripleBeep::BeepStep TripleBeep::kPattern[3] = {
    {80, 200},
    {80, 120},
    {80, 0},
};

void TripleBeep::trigger() {
  active_ = true;
  step_ = 0;
  next_time_ = 0;
  vibration_on_ = false;
  vibration_until_ = 0;
}

void TripleBeep::update(UiContext& ctx) {
  if (vibration_on_ && ctx.now >= vibration_until_) {
    M5.Power.setVibration(0);
    vibration_on_ = false;
  }
  if (!active_) {
    return;
  }
  if (ctx.now < next_time_) {
    return;
  }

  if (step_ >= (sizeof(kPattern) / sizeof(kPattern[0]))) {
    active_ = false;
    if (vibration_on_) {
      M5.Power.setVibration(0);
      vibration_on_ = false;
    }
    return;
  }

  const BeepStep& step = kPattern[step_];
  M5.Speaker.tone(kBeepFrequency, step.tone_ms);
  M5.Power.setVibration(100);
  vibration_on_ = true;
  vibration_until_ = ctx.now + step.tone_ms;
  next_time_ = ctx.now + step.tone_ms + step.pause_ms;
  ++step_;
}

}  // namespace ui
