#include "WelcomeFace.h"

namespace pomodoro {

void WelcomeFace::setLayout(int16_t origin_x, int16_t origin_y, int16_t size) {
  origin_x_ = origin_x;
  origin_y_ = origin_y;
  size_ = size;
}

int16_t WelcomeFace::scaleFrom240(int16_t size, int16_t value) {
  return static_cast<int16_t>((value * size) / 240);
}

int16_t WelcomeFace::scale(int16_t value) const {
  return scaleFrom240(size_, value);
}

void WelcomeFace::render(ui::UiContext& ctx) {
  if (size_ <= 0) {
    return;
  }

  static constexpr uint32_t kBordeaux = 0x8000;

  ctx.display.setTextColor(kBordeaux, TFT_BLACK);
  ctx.display.setTextDatum(MC_DATUM);
  ctx.display.setTextFont(2);
  ctx.display.setTextSize(2);
  ctx.display.drawString("M5Stack\nPomodoro", origin_x_ + size_ / 2,
                         origin_y_ + scale(120));
}

}  // namespace pomodoro
