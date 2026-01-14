#include "BatteryIndicator.h"

#include <M5Unified.h>

namespace ui {

void BatteryIndicator::setLayout(int16_t origin_x, int16_t origin_y,
                                 int16_t size) {
  origin_x_ = origin_x;
  origin_y_ = origin_y;
  size_ = size;
}

int16_t BatteryIndicator::scaleFrom240(int16_t size, int16_t value) {
  return static_cast<int16_t>((value * size) / 240);
}

int16_t BatteryIndicator::batteryWidth(int16_t size) {
  return scaleFrom240(size, 36);
}

int16_t BatteryIndicator::batteryHeight(int16_t size) {
  return scaleFrom240(size, 16);
}

int16_t BatteryIndicator::batteryMargin(int16_t size) {
  return scaleFrom240(size, 6);
}

int16_t BatteryIndicator::WidthFor(int16_t size) {
  return batteryWidth(size);
}

int16_t BatteryIndicator::HeightFor(int16_t size) {
  return batteryHeight(size);
}

int16_t BatteryIndicator::MarginFor(int16_t size) {
  return batteryMargin(size);
}

bool BatteryIndicator::isCharging() { return chargingFromPower(M5.Power); }

void BatteryIndicator::render(UiContext& ctx) {
  if (size_ <= 0) {
    return;
  }

  int level = M5.Power.getBatteryLevel();
  if (level < 0) {
    level = 0;
  } else if (level > 100) {
    level = 100;
  }
  int bars = (level + 19) / 20;
  if (level == 0) {
    bars = 0;
  }

  const int16_t margin = batteryMargin(size_);
  const int16_t battery_w = batteryWidth(size_);
  const int16_t battery_h = batteryHeight(size_);
  const int16_t tip_w = scaleFrom240(size_, 4);
  const int16_t tip_h = scaleFrom240(size_, 8);
  const int16_t padding = scaleFrom240(size_, 2);
  const int16_t gap = scaleFrom240(size_, 1);

  const int16_t x = origin_x_ + margin;
  const int16_t y = origin_y_ + margin;

  ctx.display.drawRect(x, y, battery_w, battery_h, TFT_WHITE);
  ctx.display.fillRect(x + battery_w, y + (battery_h - tip_h) / 2, tip_w,
                       tip_h, TFT_WHITE);
  if (battery_w > 2 && battery_h > 2) {
    ctx.display.fillRect(x + 1, y + 1, battery_w - 2, battery_h - 2, TFT_BLACK);
  }

  const int16_t inner_w = battery_w - padding * 2;
  const int16_t inner_h = battery_h - padding * 2;
  if (inner_w <= 0 || inner_h <= 0) {
    return;
  }

  const int16_t bar_w = (inner_w - gap * 4) / 5;
  for (int i = 0; i < bars; ++i) {
    int16_t bx = x + padding + i * (bar_w + gap);
    ctx.display.fillRect(bx, y + padding, bar_w, inner_h, TFT_WHITE);
  }

  if (isCharging()) {
    const int16_t bolt_w = scaleFrom240(size_, 10);
    const int16_t bolt_h = scaleFrom240(size_, 12);
    const int16_t bx = x + (battery_w - bolt_w) / 2;
    const int16_t by = y + (battery_h - bolt_h) / 2;
    const uint32_t bolt_color = TFT_YELLOW;

    ctx.display.drawLine(bx + bolt_w * 2 / 3, by, bx + bolt_w / 3,
                         by + bolt_h / 2, bolt_color);
    ctx.display.drawLine(bx + bolt_w / 3, by + bolt_h / 2,
                         bx + bolt_w * 2 / 3, by + bolt_h / 2, bolt_color);
    ctx.display.drawLine(bx + bolt_w * 2 / 3, by + bolt_h / 2,
                         bx + bolt_w / 3, by + bolt_h, bolt_color);
    ctx.display.drawLine(bx + bolt_w * 2 / 3 + 1, by, bx + bolt_w / 3 + 1,
                         by + bolt_h / 2, bolt_color);
    ctx.display.drawLine(bx + bolt_w / 3 + 1, by + bolt_h / 2,
                         bx + bolt_w * 2 / 3 + 1, by + bolt_h / 2,
                         bolt_color);
    ctx.display.drawLine(bx + bolt_w * 2 / 3 + 1, by + bolt_h / 2,
                         bx + bolt_w / 3 + 1, by + bolt_h, bolt_color);
  }
}

}  // namespace ui
