#include "ConfigFace.h"

#include <cstdio>

namespace pomodoro {

ConfigFace::ConfigFace()
    : back_button_(10, 6, 70, 28, "Voltar", TFT_DARKGREY, TFT_WHITE),
      work_minus_button_(40, 120, 70, 44, "-", TFT_DARKGREY, TFT_WHITE),
      work_plus_button_(190, 120, 70, 44, "+", TFT_DARKGREY, TFT_WHITE),
      break_minus_button_(40, 120, 70, 44, "-", TFT_DARKGREY, TFT_WHITE),
      break_plus_button_(190, 120, 70, 44, "+", TFT_DARKGREY, TFT_WHITE) {
  back_button_.setOnPress([this]() {
    if (on_back_) {
      on_back_();
    }
  });
  work_minus_button_.setOnPress([this]() { handleWorkDelta(-1); });
  work_plus_button_.setOnPress([this]() { handleWorkDelta(1); });
  break_minus_button_.setOnPress([this]() { handleBreakDelta(-1); });
  break_plus_button_.setOnPress([this]() { handleBreakDelta(1); });
}

void ConfigFace::setLayout(int16_t origin_x, int16_t origin_y, int16_t size,
                           int16_t left_inset) {
  origin_x_ = origin_x;
  origin_y_ = origin_y;
  size_ = size;
  left_inset_ = left_inset;

  const int16_t margin = scaleFrom240(size, 10);
  const int16_t button_w = scaleFrom240(size, 60);
  const int16_t button_h = scaleFrom240(size, 36);
  const int16_t row_gap = scaleFrom240(size, 18);
  const int16_t row1_y = scaleFrom240(size, 80);
  const int16_t row2_y = row1_y + button_h + row_gap;

  const int16_t left_x = origin_x + margin;
  const int16_t right_x = origin_x + size - margin - button_w;

  back_button_.setBounds(origin_x + margin + left_inset,
                         origin_y + margin / 2, scaleFrom240(size, 70),
                         scaleFrom240(size, 24));

  work_minus_button_.setBounds(left_x, origin_y + row1_y, button_w, button_h);
  work_plus_button_.setBounds(right_x, origin_y + row1_y, button_w, button_h);

  break_minus_button_.setBounds(left_x, origin_y + row2_y, button_w, button_h);
  break_plus_button_.setBounds(right_x, origin_y + row2_y, button_w, button_h);
}

void ConfigFace::setValues(int32_t work_minutes, int32_t break_minutes) {
  work_minutes_ = work_minutes;
  break_minutes_ = break_minutes;
}

void ConfigFace::setOnBack(std::function<void()> on_back) {
  on_back_ = std::move(on_back);
}

void ConfigFace::setOnWorkDelta(std::function<void(int32_t)> on_work_delta) {
  on_work_delta_ = std::move(on_work_delta);
}

void ConfigFace::setOnBreakDelta(std::function<void(int32_t)> on_break_delta) {
  on_break_delta_ = std::move(on_break_delta);
}

void ConfigFace::update(ui::UiContext& ctx) {
  if (ctx.touch.wasPressed()) {
    const int16_t hot_w = scaleFrom240(size_, 100);
    const int16_t hot_h = scaleFrom240(size_, 40);
    const int16_t hot_x = origin_x_ + left_inset_;
    const int16_t hot_y = origin_y_;
    if (ctx.touch.x >= hot_x && ctx.touch.x <= (hot_x + hot_w) &&
        ctx.touch.y >= hot_y && ctx.touch.y <= (hot_y + hot_h)) {
      if (on_back_) {
        on_back_();
      }
      return;
    }
  }

  back_button_.update(ctx);
  work_minus_button_.update(ctx);
  work_plus_button_.update(ctx);
  break_minus_button_.update(ctx);
  break_plus_button_.update(ctx);
}

void ConfigFace::render(ui::UiContext& ctx) {
  if (size_ <= 0) {
    return;
  }

  ctx.display.setTextColor(TFT_WHITE, TFT_BLACK);
  ctx.display.setTextDatum(TC_DATUM);
  ctx.display.setTextFont(2);
  ctx.display.setTextSize(1);
  ctx.display.drawString("Configurar Tempos", origin_x_ + size_ / 2,
                         origin_y_ + scale(24));

  ctx.display.setTextDatum(TC_DATUM);
  ctx.display.setTextFont(2);
  ctx.display.setTextSize(1);
  ctx.display.drawString("Foco", origin_x_ + size_ / 2,
                         origin_y_ + scale(86));
  char work_text[12] = {0};
  std::snprintf(work_text, sizeof(work_text), "%d min", work_minutes_);
  ctx.display.drawString(work_text, origin_x_ + size_ / 2,
                         origin_y_ + scale(106));

  ctx.display.drawString("Pausa", origin_x_ + size_ / 2,
                         origin_y_ + scale(144));
  char break_text[12] = {0};
  std::snprintf(break_text, sizeof(break_text), "%d min", break_minutes_);
  ctx.display.drawString(break_text, origin_x_ + size_ / 2,
                         origin_y_ + scale(164));

  back_button_.render(ctx);
  work_minus_button_.render(ctx);
  work_plus_button_.render(ctx);
  break_minus_button_.render(ctx);
  break_plus_button_.render(ctx);
}

int16_t ConfigFace::scaleFrom240(int16_t size, int16_t value) {
  return static_cast<int16_t>((value * size) / 240);
}

int16_t ConfigFace::scale(int16_t value) const {
  return scaleFrom240(size_, value);
}

void ConfigFace::handleWorkDelta(int32_t delta) {
  if (on_work_delta_) {
    on_work_delta_(delta);
  }
}

void ConfigFace::handleBreakDelta(int32_t delta) {
  if (on_break_delta_) {
    on_break_delta_(delta);
  }
}

}  // namespace pomodoro
