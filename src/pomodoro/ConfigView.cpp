#include "ConfigView.h"

#include <cstdio>

namespace pomodoro {

void ConfigView::setLayout(int16_t origin_x, int16_t origin_y, int16_t size) {
  origin_x_ = origin_x;
  origin_y_ = origin_y;
  size_ = size;
}

void ConfigView::setMinutes(int32_t minutes) { minutes_ = minutes; }

int16_t ConfigView::scaleFrom240(int16_t size, int16_t value) {
  return static_cast<int16_t>((value * size) / 240);
}

int16_t ConfigView::scale(int16_t value) const {
  return scaleFrom240(size_, value);
}

void ConfigView::render(ui::UiContext& ctx) {
  if (size_ <= 0) {
    return;
  }

  ctx.display.setTextColor(TFT_WHITE, TFT_BLACK);
  ctx.display.setTextDatum(TC_DATUM);
  ctx.display.setTextFont(2);
  ctx.display.setTextSize(2);
  ctx.display.drawString("Configurar Foco", origin_x_ + size_ / 2,
                         origin_y_ + scale(40));

  char minutes_text[8] = {0};
  std::snprintf(minutes_text, sizeof(minutes_text), "%d min", minutes_);
  ctx.display.setTextDatum(MC_DATUM);
  ctx.display.setTextFont(4);
  ctx.display.setTextSize(1);
  ctx.display.drawString(minutes_text, origin_x_ + size_ / 2,
                         origin_y_ + scale(90));
}

}  // namespace pomodoro
