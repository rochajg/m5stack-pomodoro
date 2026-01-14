#ifndef POMODORO_APP_H
#define POMODORO_APP_H

#include <cstdint>

#include <M5Unified.h>

#include "../core/Time.h"
#include "../ui/App.h"
#include "../ui/BatteryIndicator.h"
#include "../ui/Button.h"
#include "ConfigView.h"
#include "TimerFace.h"

class PomodoroApp : public ui::App {
 public:
  PomodoroApp();

  void update(ui::UiContext& ctx) override;
  void render(ui::UiContext& ctx) override;

 private:
  static constexpr uint32_t kDarkRed = 0x8000;
  static constexpr int32_t kMinWorkMinutes = 5;
  static constexpr int32_t kMaxWorkMinutes = 90;
  static constexpr float kOrientationThreshold = 0.6f;
  static constexpr uint32_t kOrientationHoldMs = 900;
  static constexpr uint32_t kOrientationSampleMs = 200;

  enum class Mode { Work, Break };
  enum class Screen { Timer, Config };
  enum class Orientation { Unknown, XAxis, YAxis };

  struct SquareLayout {
    int16_t x;
    int16_t y;
    int16_t size;
  };

  void toggleRunning();
  void setRunning(bool running);
  void reset();
  void setMode(Mode mode);
  void switchMode();
  void openConfig();
  void closeConfig();
  void adjustWorkMinutes(int32_t delta);
  void updateOrientation(ui::UiContext& ctx);
  void applyOrientation(Orientation orientation);
  void ensureLayout(M5GFX& display);
  void applyButtonLayout();
  bool consumeFullRedraw();

  static SquareLayout makeSquareLayout(M5GFX& display);
  static int16_t scaleFrom240(int16_t size, int16_t value);
  int16_t scale(int16_t value) const;

  int32_t work_minutes_ = 25;
  int32_t work_duration_ = work_minutes_ * app_time::MINUTE;
  const int32_t break_duration_ = 5 * app_time::MINUTE;
  int32_t remaining_ = work_duration_;
  bool running_ = false;
  bool paused_for_config_ = false;
  uint32_t last_tick_ = 0;
  Mode mode_ = Mode::Work;
  Screen screen_ = Screen::Timer;
  Orientation last_orientation_ = Orientation::Unknown;
  Orientation pending_orientation_ = Orientation::Unknown;
  uint32_t pending_since_ = 0;
  uint32_t last_orientation_sample_ = 0;
  int current_rotation_ = 1;
  SquareLayout layout_{0, 0, 0};
  int16_t layout_width_ = 0;
  int16_t layout_height_ = 0;
  bool layout_dirty_ = true;
  bool full_redraw_ = true;

  ui::BatteryIndicator battery_indicator_;
  pomodoro::TimerFace timer_face_;
  pomodoro::ConfigView config_view_;

  ui::Button start_button_;
  ui::Button reset_button_;
  ui::Button gear_button_;
  ui::Button back_button_;
  ui::Button minus_button_;
  ui::Button plus_button_;
};

#endif  // POMODORO_APP_H
