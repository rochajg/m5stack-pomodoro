#include <M5Unified.h>

#include "pomodoro/PomodoroApp.h"

PomodoroApp app;
bool standby = false;
uint8_t saved_brightness = 128;

void setup() {
  M5.begin();
  M5.Power.begin();

  M5.Display.setRotation(1);
  saved_brightness = M5.Display.getBrightness();
  app.requestRender();
}

void loop() {
  M5.update();

  bool power_event = M5.BtnPWR.wasPressed();
  if (M5.Power.getKeyState() != 0) {
    power_event = true;
  }
  if (power_event) {
    standby = !standby;
    if (standby) {
      saved_brightness = M5.Display.getBrightness();
      M5.Display.setBrightness(0);
      M5.Display.sleep();
    } else {
      M5.Display.wakeup();
      M5.Display.setBrightness(saved_brightness);
      app.requestRender();
    }
  }

  m5::touch_detail_t touch = standby ? m5::touch_detail_t{} : M5.Touch.getDetail();
  ui::UiContext ctx{M5.Display, touch, millis()};
  app.update(ctx);
  if (!standby && app.consumeRender()) {
    app.render(ctx);
  }
}

