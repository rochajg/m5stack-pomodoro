#include "TimerFace.h"

#include <cstdio>

#include "../core/Time.h"

namespace pomodoro {

void TimerFace::setLayout(int16_t origin_x, int16_t origin_y, int16_t size) {
  origin_x_ = origin_x;
  origin_y_ = origin_y;
  size_ = size;
}

void TimerFace::setState(bool is_work_mode, int32_t remaining_seconds) {
  is_work_mode_ = is_work_mode;
  remaining_seconds_ = remaining_seconds;
}

int16_t TimerFace::scaleFrom240(int16_t size, int16_t value) {
  return static_cast<int16_t>((value * size) / 240);
}

int16_t TimerFace::scale(int16_t value) const {
  return scaleFrom240(size_, value);
}

void TimerFace::render(ui::UiContext& ctx) {
  if (size_ <= 0) {
    return;
  }

  ctx.display.setTextColor(TFT_WHITE, TFT_BLACK);
  ctx.display.setTextDatum(TC_DATUM);
  ctx.display.setTextFont(2);
  ctx.display.setTextSize(2);
  ctx.display.drawString("Pomodoro", origin_x_ + size_ / 2,
                         origin_y_ + scale(10));

  const char* mode_label = is_work_mode_ ? "Foco" : "Pausa";
  ctx.display.setTextDatum(TC_DATUM);
  ctx.display.setTextFont(2);
  ctx.display.setTextSize(2);
  ctx.display.drawString(mode_label, origin_x_ + size_ / 2,
                         origin_y_ + scale(40));

  char timer_text[6] = {0};
  formatTime(timer_text, sizeof(timer_text), remaining_seconds_);
  ctx.display.setTextDatum(MC_DATUM);
  ctx.display.setTextFont(7);
  ctx.display.setTextSize(1);
  ctx.display.drawString(timer_text, origin_x_ + size_ / 2,
                         origin_y_ + scale(120));
}

void TimerFace::formatTime(char* out, size_t out_len, int32_t seconds) {
  if (seconds < 0) {
    seconds = 0;
  }
  int32_t minutes = seconds / app_time::MINUTE;
  int32_t secs = seconds % app_time::MINUTE;
  std::snprintf(out, out_len, "%02d:%02d", minutes, secs);
}

}  // namespace pomodoro
