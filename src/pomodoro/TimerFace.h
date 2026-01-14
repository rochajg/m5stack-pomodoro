#ifndef POMODORO_TIMER_FACE_H
#define POMODORO_TIMER_FACE_H

#include <cstddef>
#include <cstdint>

#include "../ui/Component.h"

namespace pomodoro {

class TimerFace : public ui::Component {
 public:
  TimerFace() = default;

  void setLayout(int16_t origin_x, int16_t origin_y, int16_t size);
  void setState(bool is_work_mode, int32_t remaining_seconds);
  void update(ui::UiContext&) override {}
  void render(ui::UiContext& ctx) override;

 private:
  static int16_t scaleFrom240(int16_t size, int16_t value);
  int16_t scale(int16_t value) const;
  static void formatTime(char* out, size_t out_len, int32_t seconds);

  int16_t origin_x_ = 0;
  int16_t origin_y_ = 0;
  int16_t size_ = 0;
  bool is_work_mode_ = true;
  int32_t remaining_seconds_ = 0;
};

}  // namespace pomodoro

#endif  // POMODORO_TIMER_FACE_H
