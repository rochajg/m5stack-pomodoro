#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <functional>

#include "Component.h"

namespace ui {

class Button : public Component {
 public:
  Button(int16_t x, int16_t y, int16_t w, int16_t h, const String& label,
         uint32_t bg_color, uint32_t text_color)
      : x_(x),
        y_(y),
        w_(w),
        h_(h),
        label_(label),
        bg_color_(bg_color),
        text_color_(text_color) {}

  void setBounds(int16_t x, int16_t y, int16_t w, int16_t h) {
    x_ = x;
    y_ = y;
    w_ = w;
    h_ = h;
  }

  void setLabel(const String& label) { label_ = label; }
  void setOnPress(std::function<void()> on_press) { on_press_ = on_press; }
  void setEnabled(bool enabled) { enabled_ = enabled; }

  void update(UiContext& ctx) override {
    if (!enabled_) {
      return;
    }
    if (ctx.touch.wasPressed() && contains(ctx.touch.x, ctx.touch.y)) {
      if (on_press_) {
        on_press_();
      }
    }
  }

  void render(UiContext& ctx) override {
    ctx.display.fillRoundRect(x_, y_, w_, h_, 6, bg_color_);
    ctx.display.setTextColor(text_color_, bg_color_);
    ctx.display.setTextDatum(MC_DATUM);
    ctx.display.setTextFont(2);
    ctx.display.setTextSize(2);
    ctx.display.drawString(label_, x_ + w_ / 2, y_ + h_ / 2);
  }

 private:
  bool contains(int16_t px, int16_t py) const {
    return px >= x_ && px <= (x_ + w_) && py >= y_ && py <= (y_ + h_);
  }

  int16_t x_;
  int16_t y_;
  int16_t w_;
  int16_t h_;
  String label_;
  uint32_t bg_color_;
  uint32_t text_color_;
  bool enabled_ = true;
  std::function<void()> on_press_;
};

}  // namespace ui

#endif  // UI_BUTTON_H
