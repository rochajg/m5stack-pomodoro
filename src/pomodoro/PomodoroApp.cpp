#include "PomodoroApp.h"

#include <cmath>

#include "../core/Time.h"

PomodoroApp::PomodoroApp()
    : start_button_(20, 190, 140, 40, "Iniciar", TFT_DARKGREEN, TFT_WHITE),
      reset_button_(180, 190, 140, 40, "Reiniciar", kDarkRed, TFT_WHITE),
      gear_button_(276, 6, 38, 28, "CFG", TFT_DARKGREY, TFT_WHITE),
      back_button_(10, 6, 70, 28, "Voltar", TFT_DARKGREY, TFT_WHITE),
      minus_button_(40, 120, 90, 50, "-", TFT_DARKGREY, TFT_WHITE),
      plus_button_(190, 120, 90, 50, "+", TFT_DARKGREY, TFT_WHITE) {
  start_button_.setOnPress([this]() { toggleRunning(); });
  reset_button_.setOnPress([this]() { reset(); });
  gear_button_.setOnPress([this]() { openConfig(); });
  back_button_.setOnPress([this]() { closeConfig(); });
  minus_button_.setOnPress([this]() { adjustWorkMinutes(-1); });
  plus_button_.setOnPress([this]() { adjustWorkMinutes(1); });
}

void PomodoroApp::update(ui::UiContext& ctx) {
  updateOrientation(ctx);
  ensureLayout(ctx.display);

  if (screen_ == Screen::Config) {
    back_button_.update(ctx);
    minus_button_.update(ctx);
    plus_button_.update(ctx);
    return;
  }

  start_button_.update(ctx);
  reset_button_.update(ctx);
  gear_button_.update(ctx);

  if (!running_) {
    return;
  }

  if (ctx.now - last_tick_ >= 1000) {
    int32_t ticks = (ctx.now - last_tick_) / 1000;
    last_tick_ += ticks * 1000;
    remaining_ -= ticks;
    if (remaining_ <= 0) {
      switchMode();
    }
    requestRender();
  }
}

void PomodoroApp::render(ui::UiContext& ctx) {
  ensureLayout(ctx.display);
  const bool full_redraw = consumeFullRedraw();
  if (full_redraw) {
    ctx.display.clear(TFT_BLACK);
  }

  battery_indicator_.render(ctx);

  if (screen_ == Screen::Config) {
    config_view_.setMinutes(work_minutes_);
    config_view_.render(ctx);
    back_button_.render(ctx);
    minus_button_.render(ctx);
    plus_button_.render(ctx);
    return;
  }

  timer_face_.setState(mode_ == Mode::Work, remaining_);
  timer_face_.render(ctx);

  start_button_.render(ctx);
  reset_button_.render(ctx);
  gear_button_.render(ctx);
}

void PomodoroApp::toggleRunning() { setRunning(!running_); }

void PomodoroApp::setRunning(bool running) {
  if (running_ == running) {
    return;
  }
  running_ = running;
  start_button_.setLabel(running_ ? "Pausar" : "Iniciar");
  if (running_) {
    last_tick_ = millis();
  }
  requestRender();
}

void PomodoroApp::reset() {
  running_ = false;
  mode_ = Mode::Work;
  remaining_ = work_duration_;
  start_button_.setLabel("Iniciar");
  requestRender();
}

void PomodoroApp::setMode(Mode mode) {
  if (mode_ == mode) {
    return;
  }
  mode_ = mode;
  remaining_ = (mode_ == Mode::Work) ? work_duration_ : break_duration_;
  if (running_) {
    last_tick_ = millis();
  }
  full_redraw_ = true;
  requestRender();
}

void PomodoroApp::switchMode() {
  setMode((mode_ == Mode::Work) ? Mode::Break : Mode::Work);
}

void PomodoroApp::openConfig() {
  if (screen_ == Screen::Config) {
    return;
  }
  screen_ = Screen::Config;
  layout_dirty_ = true;
  full_redraw_ = true;
  if (running_) {
    paused_for_config_ = true;
    setRunning(false);
  }
  requestRender();
}

void PomodoroApp::closeConfig() {
  if (screen_ == Screen::Timer) {
    return;
  }
  screen_ = Screen::Timer;
  layout_dirty_ = true;
  full_redraw_ = true;
  if (paused_for_config_) {
    paused_for_config_ = false;
    setRunning(true);
  }
  requestRender();
}

void PomodoroApp::adjustWorkMinutes(int32_t delta) {
  int32_t next = work_minutes_ + delta;
  if (next < kMinWorkMinutes) {
    next = kMinWorkMinutes;
  } else if (next > kMaxWorkMinutes) {
    next = kMaxWorkMinutes;
  }
  if (next == work_minutes_) {
    return;
  }
  work_minutes_ = next;
  work_duration_ = work_minutes_ * app_time::MINUTE;
  if (mode_ == Mode::Work) {
    if (!running_) {
      remaining_ = work_duration_;
    } else if (remaining_ > work_duration_) {
      remaining_ = work_duration_;
    }
  }
  requestRender();
}

void PomodoroApp::updateOrientation(ui::UiContext& ctx) {
  if (ctx.now - last_orientation_sample_ < kOrientationSampleMs) {
    return;
  }
  last_orientation_sample_ = ctx.now;

  float ax = 0.0f;
  float ay = 0.0f;
  float az = 0.0f;
  if (!M5.Imu.getAccel(&ax, &ay, &az)) {
    return;
  }

  Orientation detected = Orientation::Unknown;
  float abs_x = std::fabs(ax);
  float abs_y = std::fabs(ay);
  if (abs_x > abs_y && abs_x >= kOrientationThreshold) {
    detected = Orientation::XAxis;
  } else if (abs_y >= kOrientationThreshold) {
    detected = Orientation::YAxis;
  }

  if (detected == Orientation::Unknown || detected == last_orientation_) {
    pending_orientation_ = Orientation::Unknown;
    return;
  }

  if (pending_orientation_ != detected) {
    pending_orientation_ = detected;
    pending_since_ = ctx.now;
    return;
  }

  if (ctx.now - pending_since_ < kOrientationHoldMs) {
    return;
  }

  last_orientation_ = detected;
  pending_orientation_ = Orientation::Unknown;
  if (last_orientation_ == Orientation::XAxis) {
    setMode(Mode::Break);
    applyOrientation(Orientation::XAxis);
  } else if (last_orientation_ == Orientation::YAxis) {
    setMode(Mode::Work);
    applyOrientation(Orientation::YAxis);
  }
}

void PomodoroApp::applyOrientation(Orientation orientation) {
  int rotation = 1;
  if (orientation == Orientation::XAxis) {
    rotation = 0;
  }
  if (rotation == current_rotation_) {
    return;
  }
  current_rotation_ = rotation;
  M5.Display.setRotation(current_rotation_);
  layout_dirty_ = true;
  requestRender();
}

PomodoroApp::SquareLayout PomodoroApp::makeSquareLayout(M5GFX& display) {
  int16_t w = display.width();
  int16_t h = display.height();
  int16_t size = (w < h) ? w : h;
  int16_t x = (w - size) / 2;
  int16_t y = (h - size) / 2;
  return SquareLayout{x, y, size};
}

int16_t PomodoroApp::scaleFrom240(int16_t size, int16_t value) {
  return static_cast<int16_t>((value * size) / 240);
}

int16_t PomodoroApp::scale(int16_t value) const {
  return scaleFrom240(layout_.size, value);
}

bool PomodoroApp::consumeFullRedraw() {
  bool value = full_redraw_;
  full_redraw_ = false;
  return value;
}

void PomodoroApp::ensureLayout(M5GFX& display) {
  int16_t w = display.width();
  int16_t h = display.height();
  if (!layout_dirty_ && w == layout_width_ && h == layout_height_) {
    return;
  }
  layout_width_ = w;
  layout_height_ = h;
  layout_ = makeSquareLayout(display);
  battery_indicator_.setLayout(layout_.x, layout_.y, layout_.size);
  timer_face_.setLayout(layout_.x, layout_.y, layout_.size);
  config_view_.setLayout(layout_.x, layout_.y, layout_.size);
  applyButtonLayout();
  full_redraw_ = true;
  layout_dirty_ = false;
}

void PomodoroApp::applyButtonLayout() {
  const int16_t size = layout_.size;
  const int16_t margin = scaleFrom240(size, 10);
  const int16_t gear_w = scaleFrom240(size, 36);
  const int16_t gear_h = scaleFrom240(size, 24);
  const int16_t button_h = scaleFrom240(size, 38);
  const int16_t button_w = (size - margin * 3) / 2;
  const int16_t button_y = size - margin - button_h;
  const int16_t battery_w = ui::BatteryIndicator::WidthFor(size);
  const int16_t back_gap = scaleFrom240(size, 4);

  const int16_t gear_x = size - margin - gear_w;
  const int16_t gear_y = margin / 2;

  start_button_.setBounds(layout_.x + margin, layout_.y + button_y, button_w,
                          button_h);
  reset_button_.setBounds(layout_.x + margin * 2 + button_w,
                          layout_.y + button_y, button_w, button_h);
  gear_button_.setBounds(layout_.x + gear_x, layout_.y + gear_y, gear_w,
                         gear_h);
  back_button_.setBounds(layout_.x + margin + battery_w + back_gap,
                         layout_.y + gear_y, scaleFrom240(size, 70), gear_h);

  const int16_t adjust_w = scaleFrom240(size, 70);
  const int16_t adjust_h = scaleFrom240(size, 44);
  const int16_t adjust_y = scaleFrom240(size, 130);
  const int16_t adjust_x_left = scaleFrom240(size, 30);
  const int16_t adjust_x_right = size - adjust_x_left - adjust_w;

  minus_button_.setBounds(layout_.x + adjust_x_left, layout_.y + adjust_y,
                          adjust_w, adjust_h);
  plus_button_.setBounds(layout_.x + adjust_x_right, layout_.y + adjust_y,
                         adjust_w, adjust_h);
}
