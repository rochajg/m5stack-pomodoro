#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <M5Unified.h>

namespace ui {

struct UiContext {
  M5GFX& display;
  m5::touch_detail_t touch;
  uint32_t now;
};

class Component {
 public:
  virtual void update(UiContext& ctx) = 0;
  virtual void render(UiContext& ctx) = 0;
  virtual ~Component() {}
};

}  // namespace ui

#endif  // UI_COMPONENT_H
