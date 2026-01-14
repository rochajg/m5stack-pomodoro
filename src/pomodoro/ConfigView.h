#ifndef POMODORO_CONFIG_VIEW_H
#define POMODORO_CONFIG_VIEW_H

#include <cstdint>

#include "../ui/Component.h"

namespace pomodoro {

class ConfigView : public ui::Component {
 public:
  ConfigView() = default;

  void setLayout(int16_t origin_x, int16_t origin_y, int16_t size);
  void setMinutes(int32_t minutes);
  void update(ui::UiContext&) override {}
  void render(ui::UiContext& ctx) override;

 private:
  static int16_t scaleFrom240(int16_t size, int16_t value);
  int16_t scale(int16_t value) const;

  int16_t origin_x_ = 0;
  int16_t origin_y_ = 0;
  int16_t size_ = 0;
  int32_t minutes_ = 0;
};

}  // namespace pomodoro

#endif  // POMODORO_CONFIG_VIEW_H
