#ifndef UI_TRIPLE_BEEP_H
#define UI_TRIPLE_BEEP_H

#include <cstdint>

#include "Component.h"

namespace ui {

class TripleBeep : public Component {
 public:
  TripleBeep() = default;

  void trigger();
  void update(UiContext& ctx) override;
  void render(UiContext&) override {}

 private:
  struct BeepStep {
    uint16_t tone_ms;
    uint16_t pause_ms;
  };

  static constexpr float kBeepFrequency = 370.0f;  // F#4
  static const BeepStep kPattern[3];

  bool active_ = false;
  uint8_t step_ = 0;
  uint32_t next_time_ = 0;
  bool vibration_on_ = false;
  uint32_t vibration_until_ = 0;
};

}  // namespace ui

#endif  // UI_TRIPLE_BEEP_H
