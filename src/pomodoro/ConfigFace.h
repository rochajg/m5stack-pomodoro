#ifndef POMODORO_CONFIG_FACE_H
#define POMODORO_CONFIG_FACE_H

#include <cstdint>
#include <functional>
#include <utility>

#include "../ui/Button.h"
#include "../ui/Component.h"

namespace pomodoro {

class ConfigFace : public ui::Component {
 public:
  ConfigFace();

  void setLayout(int16_t origin_x, int16_t origin_y, int16_t size,
                 int16_t left_inset);
  void setValues(int32_t work_minutes, int32_t break_minutes);
  void setOnBack(std::function<void()> on_back);
  void setOnWorkDelta(std::function<void(int32_t)> on_work_delta);
  void setOnBreakDelta(std::function<void(int32_t)> on_break_delta);

  void update(ui::UiContext& ctx) override;
  void render(ui::UiContext& ctx) override;

 private:
  static int16_t scaleFrom240(int16_t size, int16_t value);
  int16_t scale(int16_t value) const;

  void handleWorkDelta(int32_t delta);
  void handleBreakDelta(int32_t delta);

  int16_t origin_x_ = 0;
  int16_t origin_y_ = 0;
  int16_t size_ = 0;
  int16_t left_inset_ = 0;
  int32_t work_minutes_ = 0;
  int32_t break_minutes_ = 0;

  ui::Button back_button_;
  ui::Button work_minus_button_;
  ui::Button work_plus_button_;
  ui::Button break_minus_button_;
  ui::Button break_plus_button_;

  std::function<void()> on_back_;
  std::function<void(int32_t)> on_work_delta_;
  std::function<void(int32_t)> on_break_delta_;
};

}  // namespace pomodoro

#endif  // POMODORO_CONFIG_FACE_H
